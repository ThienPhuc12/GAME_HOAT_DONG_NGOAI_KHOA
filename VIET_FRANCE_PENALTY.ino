#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <DNSServer.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "config.h"
#include "questions.h"
#include "trivia_evaluator.h"
#include "player_manager.h"
#include "web_player.h"
#include "display_oled.h"

static PenaltyOled oled;

enum CommandType : uint8_t {
  CMD_CONNECT, CMD_DISCONNECT, CMD_HELLO, CMD_JOIN, CMD_PREDICT,
  CMD_HOST_START, CMD_TUTORIAL_NEXT, CMD_HOST_SETTINGS, CMD_HOST_REPLAY,
  CMD_SELECT_ANS, CMD_SELECT_CHAR, CMD_SHOOT, CMD_SAVE, CMD_PING,
  CMD_HOST_BOTS, CMD_HOST_RESET, CMD_TEST_BLOCK, CMD_TEST_GOAL, CMD_TEST_VICTORY
};

struct Command {
  CommandType type;
  uint32_t clientId;
  uint32_t receivedAt;
  char name[32];
  char playerId[25];
  int8_t value;
  int8_t team;
  uint32_t durationMs;
};

static QueueHandle_t commandQueue;
static bool backendReady = false;
static DNSServer dnsServer;
static AsyncWebServer server(80);
static AsyncWebSocket ws("/ws");
static PlayerManager players;
static TriviaEvaluator trivia;
struct HostSession {
  char token[25] = {};
  uint32_t clientId = 0;
};
static HostSession host;
static GameState state = STATE_LOBBY;
static uint32_t stateStarted = 0;
static uint8_t tutorialSlide = 0;
static Team shootingTeam = TEAM_FR;
static uint8_t score[2] = {0, 0};
static uint8_t selectedChar[2] = {255, 255};
static int8_t shootDir = -1, saveDir = -1;
static int8_t blockedLane = -1;
static uint8_t activeLanes = 3;
static bool goal = false;
static bool coinResultSent = false;
static bool soloMode = false;
static uint16_t botAnswerDelay[MAX_CLIENTS]{};
static uint8_t questionOrder[QUESTION_COUNT];
static uint8_t questionCursor = QUESTION_COUNT;
static Question currentQuestion{};
static uint8_t currentQuestionId = 0;
static uint32_t lastOledFrame = 0, lastWsCleanup = 0;
static uint32_t lastNetworkLog = 0;

static const char* stateName(GameState s) {
  switch (s) {
    case STATE_LOBBY: return "LOBBY";
    case STATE_TUTORIAL_SLIDES: return "TUTORIAL_SLIDES";
    case STATE_COIN_TOSS: return "COIN_TOSS";
    case STATE_COUNTDOWN: return "COUNTDOWN";
    case STATE_TRIVIA_8S: return "TRIVIA_8S";
    case STATE_CHAR_SELECT_7S: return "CHAR_SELECT_7S";
    case STATE_AIM_STANDOFF: return "AIM_STANDOFF";
    case STATE_PENALTY_ANIM: return "PENALTY_ANIM";
    case STATE_MONUMENT_BUILD: return "MONUMENT_BUILD";
    default: return "VICTORY_CEREMONY";
  }
}

static uint32_t stateDuration(GameState s) {
  switch (s) {
    case STATE_TUTORIAL_SLIDES: return TUTORIAL_SLIDE_MS;
    case STATE_COIN_TOSS: return COIN_TOSS_MS;
    case STATE_COUNTDOWN: return COUNTDOWN_MS;
    case STATE_TRIVIA_8S: return TRIVIA_MS;
    case STATE_CHAR_SELECT_7S: return CHAR_SELECT_MS;
    case STATE_AIM_STANDOFF: return AIM_MS;
    case STATE_PENALTY_ANIM: return SHOT_ANIM_MS;
    case STATE_MONUMENT_BUILD: return BUILD_ANIM_MS;
    default: return 0;
  }
}

static uint32_t elapsed(uint32_t now) { return static_cast<uint32_t>(now - stateStarted); }
static uint32_t remaining(uint32_t now) {
  const uint32_t duration = stateDuration(state);
  return elapsed(now) >= duration ? 0 : duration - elapsed(now);
}

static char jsonOut[3072];
template <size_t N> static void sendJson(uint32_t clientId, StaticJsonDocument<N>& doc) {
  if (doc.overflowed()) { Serial.println("JSON document overflow"); return; }
  const size_t n = measureJson(doc);
  if (n >= sizeof(jsonOut)) return;
  serializeJson(doc, jsonOut, sizeof(jsonOut));
  if (clientId) {
    if (AsyncWebSocketClient* client = ws.client(clientId)) client->text(jsonOut, n);
  } else {
    ws.textAll(jsonOut, n);
  }
}

static void sendError(uint32_t clientId, const char* message) {
  StaticJsonDocument<160> doc;
  doc["t"] = "error";
  doc["message"] = message;
  sendJson(clientId, doc);
}

static void appendVictoryLists(JsonDocument& doc) {
  const Team winner = score[TEAM_FR] >= GOALS_TO_WIN ? TEAM_FR : TEAM_VN;
  doc["winnerTeam"] = static_cast<uint8_t>(winner);
  JsonArray winners = doc.createNestedArray("winners");
  JsonArray runnersUp = doc.createNestedArray("runnersUp");
  JsonArray predictors = doc.createNestedArray("correctPredictors");
  for (uint8_t i = 0; i < MAX_CLIENTS; ++i) {
    const Player& p = players.players()[i];
    if (!p.used) continue;
    (p.team == winner ? winners : runnersUp).add(p.name);
    if (p.role == ROLE_SPECTATOR && p.prediction == winner) predictors.add(p.name);
  }
}

static void sendState(uint32_t clientId = 0) {
  static StaticJsonDocument<3072> doc;
  doc.clear();
  doc["t"] = "state";
  doc["state"] = stateName(state);
  doc["scoreFR"] = score[TEAM_FR];
  doc["scoreVN"] = score[TEAM_VN];
  doc["shootingTeam"] = static_cast<uint8_t>(shootingTeam);
  doc["remainingMs"] = remaining(millis());
  doc["membersFR"] = players.count(TEAM_FR);
  doc["membersVN"] = players.count(TEAM_VN);
  doc["mainFR"] = players.hasMain(TEAM_FR);
  doc["mainVN"] = players.hasMain(TEAM_VN);
  doc["triviaMs"] = TRIVIA_MS;
  doc["soloMode"] = soloMode;
  if (state == STATE_TUTORIAL_SLIDES) doc["tutorialSlide"] = tutorialSlide;
  JsonArray roster = doc.createNestedArray("roster");
  for (uint8_t i = 0; i < MAX_CLIENTS; ++i) {
    const Player& p = players.players()[i];
    if (!p.used) continue;
    JsonObject member = roster.createNestedObject();
    member["name"] = p.name;
    member["team"] = static_cast<uint8_t>(p.team);
    member["role"] = static_cast<uint8_t>(p.role);
    member["bot"] = p.bot;
  }
  if (state == STATE_COIN_TOSS) doc["winnerTeam"] = static_cast<uint8_t>(shootingTeam);
  if (state == STATE_TRIVIA_8S) {
    doc["answeredFR"] = trivia.answeredCount(TEAM_FR);
    doc["answeredVN"] = trivia.answeredCount(TEAM_VN);
    doc["questionId"] = currentQuestionId;
    doc["qVi"] = currentQuestion.q_vi;
    doc["qFr"] = currentQuestion.q_fr;
    JsonArray opts = doc.createNestedArray("options");
    opts.add(currentQuestion.optA);
    opts.add(currentQuestion.optB);
    opts.add(currentQuestion.optC);
    opts.add(currentQuestion.optD);
  }
  if (state >= STATE_CHAR_SELECT_7S && state <= STATE_MONUMENT_BUILD) {
    doc["activeLanes"] = activeLanes;
    doc["blockedLane"] = blockedLane;
    doc["pctFR"] = trivia.result().pctFR;
    doc["pctVN"] = trivia.result().pctVN;
    doc["charFR"] = selectedChar[TEAM_FR];
    doc["charVN"] = selectedChar[TEAM_VN];
  }
  if (state == STATE_PENALTY_ANIM || state == STATE_MONUMENT_BUILD) {
    doc["shootDir"] = shootDir;
    doc["saveDir"] = saveDir;
    doc["goal"] = goal;
  }
  if (state == STATE_VICTORY_CEREMONY) appendVictoryLists(doc);
  sendJson(clientId, doc);
}

static void setState(GameState next, uint32_t now) {
  state = next;
  stateStarted = now;
  sendState();
}

static void shuffleQuestions() {
  for (uint8_t i = 0; i < QUESTION_COUNT; ++i) questionOrder[i] = i;
  for (int i = QUESTION_COUNT - 1; i > 0; --i) {
    int j = random(i + 1);
    uint8_t tmp = questionOrder[i];
    questionOrder[i] = questionOrder[j];
    questionOrder[j] = tmp;
  }
  questionCursor = 0;
}

static void startTrivia(uint32_t now) {
  if (questionCursor >= QUESTION_COUNT) shuffleQuestions();
  if (!loadQuestion(questionOrder[questionCursor++], currentQuestion)) return;
  currentQuestionId = currentQuestion.id;
  if (!trivia.begin(players.count(TEAM_FR), players.count(TEAM_VN),
                    currentQuestion.correctIdx, now, TRIVIA_MS)) return;
  for (uint8_t i = 0; i < MAX_CLIENTS; ++i)
    botAnswerDelay[i] = players.players()[i].used && players.players()[i].bot
        ? static_cast<uint16_t>(random(350, TRIVIA_MS - 350)) : 0;
  setState(STATE_TRIVIA_8S, now);
}

static uint8_t randomActiveLane() {
  uint8_t lane;
  do { lane = static_cast<uint8_t>(random(3)); } while (lane == blockedLane);
  return lane;
}

static void sendJoined(const Player& player) {
  StaticJsonDocument<256> doc;
  doc["t"] = "joined";
  doc["name"] = player.name;
  doc["team"] = static_cast<uint8_t>(player.team);
  doc["role"] = static_cast<uint8_t>(player.role);
  doc["prediction"] = player.prediction;
  doc["playerId"] = player.playerId;
  doc["host"] = false;
  sendJson(player.clientId, doc);
  if (player.role == ROLE_SPECTATOR && player.prediction < 0) {
    StaticJsonDocument<96> prediction;
    prediction["t"] = "predict_prompt";
    sendJson(player.clientId, prediction);
  }
  sendState(player.clientId);
}

static bool hostAuthorized(const Command& cmd) { return host.clientId == cmd.clientId; }

static void sendIdentity(uint32_t clientId, bool owner) {
  StaticJsonDocument<160> doc;
  doc["t"] = "identity";
  doc["role"] = owner ? "HOST" : "GUEST";
  doc["host"] = owner;
  if (owner) doc["playerId"] = host.token;
  sendJson(clientId, doc);
  sendState(clientId);
}

static void startCoin(uint32_t now) {
  shootingTeam = static_cast<Team>(random(0, 2));
  coinResultSent = false;
  StaticJsonDocument<128> doc;
  doc["t"] = "coin_toss";
  doc["winnerTeam"] = static_cast<uint8_t>(shootingTeam);
  doc["durationMs"] = COIN_TOSS_MS;
  sendJson(0, doc);
  setState(STATE_COIN_TOSS, now);
}

static void resetMatch(uint32_t now) {
  players.clearPredictions();
  score[TEAM_FR] = score[TEAM_VN] = 0;
  selectedChar[TEAM_FR] = selectedChar[TEAM_VN] = 255;
  shootDir = saveDir = blockedLane = -1;
  activeLanes = 3;
  goal = coinResultSent = false;
  questionCursor = QUESTION_COUNT;
  tutorialSlide = 0;
  StaticJsonDocument<64> replay;
  replay["t"] = "replay";
  sendJson(0, replay);
  setState(STATE_LOBBY, now);
}

static bool activateSoloBots() {
  if (soloMode) return true;
  const uint8_t required = (!players.hasMain(TEAM_FR) ? 1 : 0) +
                           (!players.hasMain(TEAM_VN) ? 1 : 0) + 4;
  if (players.totalCount() + required > MAX_CLIENTS - 1) return false;
  bool ok = true;
  if (!players.hasMain(TEAM_FR)) ok = players.addBot("BOT PHAP", TEAM_FR, true);
  if (ok && !players.hasMain(TEAM_VN)) ok = players.addBot("BOT VIET NAM", TEAM_VN, true);
  if (ok) ok = players.addBot("FAN PHAP 1", TEAM_FR, false);
  if (ok) ok = players.addBot("FAN PHAP 2", TEAM_FR, false);
  if (ok) ok = players.addBot("FAN VN 1", TEAM_VN, false);
  if (ok) ok = players.addBot("FAN VN 2", TEAM_VN, false);
  if (!ok) { players.removeBots(); return false; }
  soloMode = true;
  return true;
}

static void processCommand(const Command& cmd, uint32_t now) {
  if (cmd.type == CMD_CONNECT) { sendState(cmd.clientId); return; }
  if (cmd.type == CMD_DISCONNECT) {
    if (host.clientId == cmd.clientId) host.clientId = 0;
    players.disconnect(cmd.clientId);
    return;
  }
  if (cmd.type == CMD_HELLO) {
    if (!cmd.playerId[0]) { sendError(cmd.clientId, "Thiếu mã thiết bị / Identifiant absent"); return; }
    if (players.findByClient(cmd.clientId)) {
      sendIdentity(cmd.clientId, false); return;
    }
    if (!host.token[0]) strcpy(host.token, cmd.playerId);
    const bool owner = strcmp(host.token, cmd.playerId) == 0;
    if (owner) host.clientId = cmd.clientId;
    sendIdentity(cmd.clientId, owner);
    return;
  }
  if (cmd.type == CMD_PING) {
    StaticJsonDocument<64> pong;
    pong["t"] = "pong";
    sendJson(cmd.clientId, pong);
    return;
  }
  if (cmd.type == CMD_JOIN) {
    if (!host.token[0]) {
      sendError(cmd.clientId, "Đang xác định Máy Chủ / Attribution de l’hôte en cours"); return;
    }
    if (host.clientId == cmd.clientId) {
      sendError(cmd.clientId, "Máy chủ không thể vào đội / L'hôte ne joue pas"); return;
    }
    Player* p = players.join(cmd.clientId, cmd.name, static_cast<Team>(cmd.team), cmd.playerId);
    if (!p) { sendError(cmd.clientId, "Tên/đội không hợp lệ hoặc phòng đầy / Nom, équipe invalides ou salle pleine"); return; }
    sendJoined(*p);
    sendState();
    return;
  }
  if (cmd.type == CMD_HOST_RESET || cmd.type == CMD_HOST_REPLAY ||
      cmd.type == CMD_HOST_BOTS || cmd.type == CMD_TEST_BLOCK ||
      cmd.type == CMD_TEST_GOAL || cmd.type == CMD_TEST_VICTORY ||
      cmd.type == CMD_HOST_START || cmd.type == CMD_TUTORIAL_NEXT ||
      cmd.type == CMD_HOST_SETTINGS) {
    if (!hostAuthorized(cmd)) { sendError(cmd.clientId, "Chỉ Máy Chủ được điều khiển / Hôte uniquement"); return; }
  }
  if (cmd.type == CMD_HOST_RESET || cmd.type == CMD_HOST_REPLAY) {
    resetMatch(now);
    return;
  }
  if (cmd.type == CMD_HOST_SETTINGS) {
    sendError(cmd.clientId, "Quiz fixé à 8 secondes / Câu hỏi cố định 8 giây");
    return;
  }
  if (cmd.type == CMD_HOST_BOTS) {
    if (state != STATE_LOBBY) { sendError(cmd.clientId, "Chỉ đổi Bot ở sảnh chờ / Bots modifiables dans le hall uniquement"); return; }
    if (cmd.value == 0) { players.removeBots(); soloMode = false; }
    else {
      if (!activateSoloBots()) {
        sendError(cmd.clientId, "Phòng không đủ chỗ cho Bot / Salle pleine pour les bots"); return;
      }
    }
    sendState();
    return;
  }
  if (cmd.type == CMD_TEST_BLOCK) {
    activeLanes = 2;
    blockedLane = static_cast<int8_t>(random(3));
    shootDir = saveDir = -1;
    goal = false;
    for (uint8_t t = 0; t < 2; ++t)
      if (selectedChar[t] == 255) selectedChar[t] = static_cast<uint8_t>(random(2));
    setState(STATE_AIM_STANDOFF, now);
    return;
  }
  if (cmd.type == CMD_TEST_GOAL) {
    if (cmd.team < 0 || cmd.team > 1) return;
    shootingTeam = static_cast<Team>(cmd.team);
    if (score[shootingTeam] < GOALS_TO_WIN) ++score[shootingTeam];
    goal = true;
    setState(STATE_MONUMENT_BUILD, now);
    return;
  }
  if (cmd.type == CMD_TEST_VICTORY) {
    Team winner = TEAM_VN;
    if (score[TEAM_FR] > score[TEAM_VN]) winner = TEAM_FR;
    if (score[TEAM_VN] > score[TEAM_FR]) winner = TEAM_VN;
    score[winner] = GOALS_TO_WIN;
    if (score[1 - winner] >= GOALS_TO_WIN) score[1 - winner] = GOALS_TO_WIN - 1;
    shootingTeam = winner;
    setState(STATE_VICTORY_CEREMONY, now);
    return;
  }
  if (cmd.type == CMD_HOST_START) {
    if (state != STATE_LOBBY) { sendError(cmd.clientId, "Trận đấu đã bắt đầu / Match déjà commencé"); return; }
    if (!players.hasMain(TEAM_FR) || !players.hasMain(TEAM_VN)) {
      sendError(cmd.clientId, "Mỗi đội cần một người chơi chính / Un joueur principal par équipe"); return;
    }
    tutorialSlide = 0;
    setState(STATE_TUTORIAL_SLIDES, now);
    return;
  }
  if (cmd.type == CMD_TUTORIAL_NEXT) {
    if (state != STATE_TUTORIAL_SLIDES) return;
    if (++tutorialSlide >= TUTORIAL_SLIDE_COUNT) startCoin(now);
    else setState(STATE_TUTORIAL_SLIDES, now);
    return;
  }
  Player* p = players.findByClient(cmd.clientId);
  if (!p) { sendError(cmd.clientId, "Hãy vào đội trước / Rejoignez une équipe d’abord"); return; }
  if (cmd.type == CMD_PREDICT) {
    if (!players.predict(cmd.clientId, static_cast<Team>(cmd.team))) {
      sendError(cmd.clientId, "Chỉ khán giả được dự đoán / Pronostics réservés aux spectateurs"); return;
    }
    StaticJsonDocument<96> doc;
    doc["t"] = "predict_ack";
    doc["team"] = cmd.team;
    sendJson(cmd.clientId, doc);
    return;
  }
  if (cmd.type == CMD_SELECT_ANS) {
    if (state != STATE_TRIVIA_8S ||
        !trivia.recordChoice(p->team, p->teamSlot, static_cast<uint8_t>(cmd.value), cmd.receivedAt)) {
      sendError(cmd.clientId, "Đáp án sai định dạng hoặc hết giờ / Réponse invalide ou temps écoulé"); return;
    }
    StaticJsonDocument<96> doc;
    doc["t"] = "choice_ack";
    doc["a"] = cmd.value;
    sendJson(cmd.clientId, doc);
    StaticJsonDocument<96> progress;
    progress["t"] = "trivia_progress";
    progress["fr"] = trivia.answeredCount(TEAM_FR);
    progress["vn"] = trivia.answeredCount(TEAM_VN);
    sendJson(0, progress);
    return;
  }
  if (p->role != ROLE_MAIN_PLAYER) { sendError(cmd.clientId, "Chỉ người chơi chính được điều khiển / Joueur principal uniquement"); return; }
  if (cmd.type == CMD_SELECT_CHAR) {
    if (state != STATE_CHAR_SELECT_7S || elapsed(cmd.receivedAt) >= CHAR_SELECT_MS ||
        cmd.value < 0 || cmd.value > (p->team == shootingTeam ? 1 : 0)) {
      sendError(cmd.clientId, "Lựa chọn nhân vật không hợp lệ / Personnage invalide"); return;
    }
    selectedChar[p->team] = static_cast<uint8_t>(cmd.value);
    return;
  }
  if (cmd.type == CMD_SHOOT || cmd.type == CMD_SAVE) {
    const bool isShooter = p->team == shootingTeam;
    if (state != STATE_AIM_STANDOFF || elapsed(cmd.receivedAt) >= AIM_MS ||
        (cmd.type == CMD_SHOOT) != isShooter || cmd.value < 0 || cmd.value > 2 ||
        cmd.value == blockedLane) {
      sendError(cmd.clientId, "Hướng không hợp lệ / Direction invalide"); return;
    }
    int8_t& choice = isShooter ? shootDir : saveDir;
    choice = cmd.value;
  }
}

static void tickGame(uint32_t now) {
  if (soloMode && state == STATE_TRIVIA_8S) {
    for (uint8_t i = 0; i < MAX_CLIENTS; ++i) {
      const Player& p = players.players()[i];
      if (p.used && p.bot && botAnswerDelay[i] && elapsed(now) >= botAnswerDelay[i]) {
        trivia.recordChoice(p.team, p.teamSlot, static_cast<uint8_t>(random(4)), now);
        botAnswerDelay[i] = 0;
      }
    }
  }
  if (soloMode && state == STATE_CHAR_SELECT_7S && elapsed(now) >= 650) {
    for (uint8_t i = 0; i < MAX_CLIENTS; ++i) {
      const Player& p = players.players()[i];
      if (p.used && p.bot && p.role == ROLE_MAIN_PLAYER && selectedChar[p.team] == 255)
        selectedChar[p.team] = p.team == shootingTeam ? static_cast<uint8_t>(random(2)) : 0;
    }
  }
  if (soloMode && state == STATE_AIM_STANDOFF && elapsed(now) >= 650) {
    for (uint8_t i = 0; i < MAX_CLIENTS; ++i) {
      const Player& p = players.players()[i];
      if (!p.used || !p.bot || p.role != ROLE_MAIN_PLAYER) continue;
      if (p.team == shootingTeam && shootDir < 0) shootDir = randomActiveLane();
      if (p.team != shootingTeam && saveDir < 0) saveDir = randomActiveLane();
    }
  }
  switch (state) {
    case STATE_LOBBY: break;
    case STATE_TUTORIAL_SLIDES:
      if (elapsed(now) >= TUTORIAL_SLIDE_MS) {
        if (++tutorialSlide >= TUTORIAL_SLIDE_COUNT) startCoin(now);
        else setState(STATE_TUTORIAL_SLIDES, now);
      }
      break;
    case STATE_COIN_TOSS:
      if (elapsed(now) >= COIN_TOSS_MS && !coinResultSent) {
        StaticJsonDocument<96> doc;
        doc["t"] = "coin_result";
        doc["winnerTeam"] = static_cast<uint8_t>(shootingTeam);
        sendJson(0, doc);
        coinResultSent = true;
      }
      if (elapsed(now) >= COIN_TOSS_MS) setState(STATE_COUNTDOWN, now);
      break;
    case STATE_COUNTDOWN:
      if (elapsed(now) >= COUNTDOWN_MS) startTrivia(now);
      break;
    case STATE_TRIVIA_8S:
      if (elapsed(now) >= TRIVIA_MS && trivia.finish(now, shootingTeam)) {
        TriviaResult result = trivia.result();
        activeLanes = result.activeLanes;
        blockedLane = result.blockedLane;
        selectedChar[0] = selectedChar[1] = 255;
        StaticJsonDocument<192> doc;
        doc["t"] = "trivia_result";
        doc["pctFR"] = result.pctFR;
        doc["pctVN"] = result.pctVN;
        doc["activeLanes"] = activeLanes;
        doc["blockedLane"] = blockedLane;
        doc["correctIdx"] = currentQuestion.correctIdx;
        sendJson(0, doc);
        setState(STATE_CHAR_SELECT_7S, now);
      }
      break;
    case STATE_CHAR_SELECT_7S:
      if (elapsed(now) >= CHAR_SELECT_MS) {
        for (uint8_t t = 0; t < 2; ++t)
          if (selectedChar[t] == 255) selectedChar[t] = t == shootingTeam ? static_cast<uint8_t>(random(2)) : 0;
        shootDir = saveDir = -1;
        StaticJsonDocument<96> doc;
        doc["t"] = "characters";
        doc["fr"] = selectedChar[TEAM_FR];
        doc["vn"] = selectedChar[TEAM_VN];
        sendJson(0, doc);
        setState(STATE_AIM_STANDOFF, now);
      }
      break;
    case STATE_AIM_STANDOFF:
      if (elapsed(now) >= AIM_MS) {
        if (shootDir < 0) shootDir = randomActiveLane();
        if (saveDir < 0) saveDir = randomActiveLane();
        goal = shootDir != saveDir;
        StaticJsonDocument<160> doc;
        doc["t"] = "shot";
        doc["shootDir"] = shootDir;
        doc["saveDir"] = saveDir;
        doc["goal"] = goal;
        doc["shootingTeam"] = static_cast<uint8_t>(shootingTeam);
        sendJson(0, doc);
        setState(STATE_PENALTY_ANIM, now);
      }
      break;
    case STATE_PENALTY_ANIM:
      if (elapsed(now) >= SHOT_ANIM_MS) {
        if (goal) {
          ++score[shootingTeam];
          setState(STATE_MONUMENT_BUILD, now);
        } else {
          shootingTeam = static_cast<Team>(1 - shootingTeam);
          startTrivia(now);
        }
      }
      break;
    case STATE_MONUMENT_BUILD:
      if (elapsed(now) >= BUILD_ANIM_MS) {
        if (score[TEAM_FR] >= GOALS_TO_WIN || score[TEAM_VN] >= GOALS_TO_WIN)
          setState(STATE_VICTORY_CEREMONY, now);
        else {
          shootingTeam = static_cast<Team>(1 - shootingTeam);
          startTrivia(now);
        }
      }
      break;
    case STATE_VICTORY_CEREMONY: break;
  }
}

// AsyncWebSocket callback only parses bounded packets and queues commands.
// Game data is exclusively changed from loop(), avoiding task races.
static void onWsEvent(AsyncWebSocket*, AsyncWebSocketClient* client,
                      AwsEventType type, void* arg, uint8_t* data, size_t len) {
  Command cmd{};
  cmd.clientId = client->id();
  cmd.receivedAt = millis();
  if (type == WS_EVT_CONNECT) cmd.type = CMD_CONNECT;
  else if (type == WS_EVT_DISCONNECT) cmd.type = CMD_DISCONNECT;
  else if (type == WS_EVT_DATA) {
    auto* info = static_cast<AwsFrameInfo*>(arg);
    if (!info->final || info->index != 0 || info->len != len ||
        info->opcode != WS_TEXT || len > 383) return;
    StaticJsonDocument<384> doc;
    if (deserializeJson(doc, data, len)) return;
    const char* t = doc["t"] | "";
    if (!strcmp(t, "hello")) {
      cmd.type = CMD_HELLO;
      const char* deviceId = doc["deviceId"] | "";
      if (!deviceId[0]) deviceId = doc["playerId"] | "";
      if (strlen(deviceId) >= sizeof(cmd.playerId)) return;
      strncpy(cmd.playerId, deviceId, sizeof(cmd.playerId) - 1);
    } else if (!strcmp(t, "join")) {
      cmd.type = CMD_JOIN;
      const char* name = doc["name"] | "";
      const char* playerId = doc["playerId"] | "";
      if (strlen(name) >= sizeof(cmd.name)) return;
      if (strlen(playerId) >= sizeof(cmd.playerId)) return;
      strncpy(cmd.name, name, sizeof(cmd.name) - 1);
      strncpy(cmd.playerId, playerId, sizeof(cmd.playerId) - 1);
      cmd.team = doc["team"] | -1;
    } else if (!strcmp(t, "predict")) { cmd.type = CMD_PREDICT; cmd.team = doc["team"] | -1; }
    else if (!strcmp(t, "ping")) cmd.type = CMD_PING;
    else if (!strcmp(t, "host_start") || !strcmp(t, "tutorial_next") ||
             !strcmp(t, "host_settings") || !strcmp(t, "host_replay") ||
             !strcmp(t, "host_reset") || !strcmp(t, "host_bots") ||
             !strcmp(t, "test_block") || !strcmp(t, "test_goal") ||
             !strcmp(t, "test_victory")) {
      if (!strcmp(t, "host_start")) cmd.type = CMD_HOST_START;
      else if (!strcmp(t, "tutorial_next")) cmd.type = CMD_TUTORIAL_NEXT;
      else if (!strcmp(t, "host_settings")) cmd.type = CMD_HOST_SETTINGS;
      else if (!strcmp(t, "host_replay")) cmd.type = CMD_HOST_REPLAY;
      else if (!strcmp(t, "host_reset")) cmd.type = CMD_HOST_RESET;
      else if (!strcmp(t, "host_bots")) cmd.type = CMD_HOST_BOTS;
      else if (!strcmp(t, "test_block")) cmd.type = CMD_TEST_BLOCK;
      else if (!strcmp(t, "test_goal")) cmd.type = CMD_TEST_GOAL;
      else cmd.type = CMD_TEST_VICTORY;
      cmd.value = doc["enabled"] | 0;
      cmd.team = doc["team"] | -1;
      cmd.durationMs = doc["triviaMs"] | 0;
    } else if (!strcmp(t, "select_ans")) { cmd.type = CMD_SELECT_ANS; cmd.value = doc["a"] | -1; }
    else if (!strcmp(t, "answer")) { cmd.type = CMD_SELECT_ANS; cmd.value = doc["idx"] | -1; }
    else if (!strcmp(t, "select_char")) { cmd.type = CMD_SELECT_CHAR; cmd.value = doc["charId"] | -1; }
    else if (!strcmp(t, "shoot")) { cmd.type = CMD_SHOOT; cmd.value = doc["dir"] | -1; }
    else if (!strcmp(t, "save")) { cmd.type = CMD_SAVE; cmd.value = doc["dir"] | -1; }
    else return;
  } else return;
  if (xQueueSend(commandQueue, &cmd, 0) != pdTRUE) client->text("{\"t\":\"error\",\"message\":\"Máy chủ đang bận\"}");
}

static void drawOled(uint32_t now) {
  oled.render(state, score[TEAM_FR], score[TEAM_VN], shootingTeam, now);
}

static void redirectPortal(AsyncWebServerRequest* request) {
  request->redirect("http://192.168.4.1/");
}

void setup() {
  Serial.begin(115200);
  oled.begin();
  drawOled(millis());
  commandQueue = xQueueCreate(32, sizeof(Command));
  if (!commandQueue) { Serial.println("Command queue allocation failed"); return; }
  randomSeed(esp_random());
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  if (!WiFi.softAP(AP_SSID, nullptr, 1, false, MAX_CLIENTS)) {
    Serial.println("SoftAP start failed; check core max_connection limit");
    return;
  }
  dnsServer.start(53, "*", IPAddress(192,168,4,1));
  const bool audioFsReady = SPIFFS.begin(false);
  Serial.printf("SPIFFS audio: %s\n", audioFsReady && SPIFFS.exists("/wavin_flag.mp3") ? "ready" : "missing (host can choose local MP3)");
  server.on("/audio/wavin_flag.mp3", HTTP_GET, [audioFsReady](AsyncWebServerRequest* r) {
    if (!audioFsReady || !SPIFFS.exists("/wavin_flag.mp3")) {
      r->send(404, "text/plain; charset=utf-8", "Audio unavailable");
      return;
    }
    r->send(SPIFFS, "/wavin_flag.mp3", "audio/mpeg");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* r) {
    Serial.printf("HTTP / heap=%u\n", ESP.getFreeHeap());
    r->send(200, "text/html; charset=utf-8",
            reinterpret_cast<const uint8_t*>(WEB_PLAYER_HTML), sizeof(WEB_PLAYER_HTML) - 1);
  });
  server.on("/host", HTTP_GET, [](AsyncWebServerRequest* r) {
    Serial.printf("HTTP /host heap=%u\n", ESP.getFreeHeap());
    r->send(200, "text/html; charset=utf-8",
            reinterpret_cast<const uint8_t*>(WEB_PLAYER_HTML), sizeof(WEB_PLAYER_HTML) - 1);
  });
  server.on("/generate_204", HTTP_ANY, redirectPortal);
  server.on("/hotspot-detect.html", HTTP_ANY, redirectPortal);
  server.on("/connecttest.txt", HTTP_ANY, redirectPortal);
  server.on("/ncsi.txt", HTTP_ANY, redirectPortal);
  server.onNotFound(redirectPortal);
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
  backendReady = true;
  stateStarted = millis();
  Serial.println("AP ready: http://192.168.4.1/  Host: /host");
}

void loop() {
  if (!backendReady) return;
  const uint32_t now = millis();
  dnsServer.processNextRequest();
  Command cmd;
  for (uint8_t i = 0; i < 32 && xQueueReceive(commandQueue, &cmd, 0) == pdTRUE; ++i)
    processCommand(cmd, millis());
  tickGame(millis());
  if (static_cast<uint32_t>(now - lastOledFrame) >= 200) {
    drawOled(now);
    lastOledFrame = now;
  }
  if (static_cast<uint32_t>(now - lastWsCleanup) >= 1000) {
    ws.cleanupClients();
    lastWsCleanup = now;
  }
  if (static_cast<uint32_t>(now - lastNetworkLog) >= 5000) {
    Serial.printf("AP clients=%u heap=%u\n", WiFi.softAPgetStationNum(), ESP.getFreeHeap());
    lastNetworkLog = now;
  }
}
