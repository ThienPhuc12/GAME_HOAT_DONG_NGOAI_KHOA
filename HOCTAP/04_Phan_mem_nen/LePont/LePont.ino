/*
  =============================================================================
   LE PONT · CẦU — jeu de questions France–Vietnam
                   trò chơi đố vui Pháp–Việt Nam
   ESP32-S3 + smartphones des joueurs (hotspot Wi-Fi) + écran rond en option
   ESP32-S3 + điện thoại của người chơi (điểm phát Wi-Fi) + màn hình tròn (tùy chọn)
   Atelier PFIEV × DUT Maker Innovation Space — Da Nang, oct.–nov. 2026
   Licence libre (MIT) : réutilise, adapte, partage, dis d'où ça vient.
   VI: Giấy phép mở (MIT): dùng lại, chỉnh sửa, chia sẻ, ghi rõ nguồn gốc.
   Chaque commentaire français est suivi de sa traduction « VI: ».
   VI: Mỗi chú thích tiếng Pháp được theo sau bởi bản dịch « VI: ».
  =============================================================================

  COMMENT ÇA MARCHE · CÁCH HOẠT ĐỘNG
   1. L'objet crée un Wi-Fi « LePont-N » (N = ton groupe) avec un portail captif.
      VI: Vật thể tạo một mạng Wi-Fi « LePont-N » (N = số nhóm của bạn), có cổng tự mở.
   2. Les téléphones s'y connectent : la page du jeu s'ouvre (http://192.168.4.1).
      VI: Điện thoại kết nối vào: trang trò chơi tự mở (http://192.168.4.1).
   3. Un ordinateur sur le même Wi-Fi ouvre http://192.168.4.1/host : c'est le
      grand écran (projecteur) et la télécommande (Lancer / Suivante / Reset).
      VI: Một máy tính trong cùng mạng Wi-Fi mở http://192.168.4.1/host: đó là màn hình
      lớn (máy chiếu) và bộ điều khiển (Lancer / Suivante / Reset).
   4. Deux rives (France, Vietnam) construisent chacune leur moitié du pont :
      à chaque question, une rive dont la majorité a juste pose une travée de
      son côté. Quand les deux rives ont 4 travées, une question « double »
      pose la travée centrale si les deux rives réussissent ensemble.
      Le pont se ferme TOUJOURS : une mauvaise réponse laisse la travée
      suspendue à la grue, la question revient plus tard avec moins d'options,
      et le chantier s'adapte au niveau de chaque rive (questions plus
      faciles, deux options, indice du chef de chantier, 30 s).
      Le résultat est une fiche de chantier : nombre de questions, durée,
      indices utilisés, vitesse de chaque rive.
      VI: Hai bờ (Pháp, Việt Nam) mỗi bờ xây một nửa cây cầu: sau mỗi câu hỏi, bờ nào
      có đa số trả lời đúng sẽ đặt thêm một nhịp cầu ở phía mình. Khi cả hai bờ có
      đủ 4 nhịp, một câu hỏi « đôi » đặt nhịp giữa nếu cả hai bờ cùng trả lời đúng.
      Cây cầu LUÔN khép lại: câu trả lời sai để nhịp cầu treo trên cần cẩu, câu hỏi
      quay lại sau với ít lựa chọn hơn, và công trường tự điều chỉnh theo trình độ của
      từng bờ (câu hỏi dễ hơn, hai lựa chọn, gợi ý của đốc công, 30 giây).
      Kết quả là một phiếu công trường: số câu hỏi, thời gian, số gợi ý đã dùng,
      tốc độ của mỗi bờ.
   5. http://192.168.4.1/admin : changer les questions sans reflasher.
      VI: http://192.168.4.1/admin: thay câu hỏi mà không cần nạp lại chương trình.

  FICHIERS · CÁC TỆP
   config.h      numéro de groupe, écran, broches, équipes
                 VI: số nhóm, màn hình, chân nối, các đội
   questions.h   la base de questions (JSON)
                 VI: bộ câu hỏi (JSON)
   web_player.h  page des joueurs (téléphones)
                 VI: trang của người chơi (điện thoại)
   web_host.h    page grand écran + télécommande
                 VI: trang màn hình lớn + bộ điều khiển
   web_admin.h   page de modification des questions
                 VI: trang sửa câu hỏi
   web_assets.h  images et polices du téléphone ; les données sont dans web_assets.cpp
   web_assets.cpp  (générés par gen_web_h.py, ne pas modifier ; compilés une fois puis réutilisés)
                 VI: hình ảnh và phông chữ của điện thoại; dữ liệu nằm trong web_assets.cpp
                 (do gen_web_h.py tạo, không sửa; chỉ biên dịch một lần rồi dùng lại)
   display.h     l'écran de l'objet — À PERSONNALISER par ton groupe
                 VI: màn hình của vật thể — NHÓM BẠN TỰ TÙY BIẾN
   touch.h       le tactile (Niveau 2, TOUCH_ENABLE 1) : tap = suite, appui long = nouvelle partie
                 VI: cảm ứng (mức 2, TOUCH_ENABLE 1): chạm = tiếp tục, nhấn giữ = ván mới

  BIBLIOTHÈQUES · THƯ VIỆN (Croquis → Inclure une bibliothèque → Gérer les bibliothèques)
   ESP Async WebServer (ESP32Async) + Async TCP · ArduinoJson 7 · GFX Library for Arduino

  CARTE · BO MẠCH (menu Outils / Tools) : ESP32S3 Dev Module ·
   USB CDC On Boot: Disabled si la carte est sur sa prise COM (Enabled seulement sur la prise USB) ·
   Flash Size: 16MB · PSRAM: OPI PSRAM · Partition: Huge APP (3MB No OTA/1MB SPIFFS)
   VI: USB CDC On Boot: Disabled nếu bo mạch cắm cổng COM (chỉ chọn Enabled khi cắm cổng USB).

  PROTOCOLE WEBSOCKET (JSON) — utile pour vibe coder de nouvelles manches
  VI: GIAO THỨC WEBSOCKET (JSON) — hữu ích để vibe code thêm vòng chơi mới
   téléphone → objet : {"t":"join","name":"Linh","team":0}   {"t":"answer","a":2}   {"t":"answer","v":350000} (estimation)
   VI: điện thoại → vật thể
   grand écran → objet : {"t":"hello","role":"host"}   {"t":"host","cmd":"start|next|reveal|reset"}
   VI: màn hình lớn → vật thể
   objet → tous : config · lobby · question (avec hide[rive], hint, bridge) · tick · reveal (res[rive], bridge) · end (fiche) · reset
   VI: vật thể → tất cả
   objet → un joueur : joined · you · full
   VI: vật thể → một người chơi
*/

#include <WiFi.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "questions.h"
#include "web_player.h"
#include "web_host.h"
#include "web_admin.h"
#include "web_assets.h"
#include "display.h"
#include "touch.h"

void hostCommand(const char* cmd);

// ---------------------------------------------------------- réseau · mạng ----
DNSServer dns;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
char ssid[16];

// --------------------------------------------------- joueurs · người chơi ----
struct Player {
  uint32_t id;        // identifiant de la connexion WebSocket
                      // VI: mã định danh của kết nối WebSocket
  char name[20];
  uint8_t team;       // 0 = rive France, 1 = rive Vietnam
                      // VI: 0 = bờ Pháp, 1 = bờ Việt Nam
  int score;          // points de vitesse (pour la fiche de chantier)
                      // VI: điểm tốc độ (cho phiếu công trường)
  int answer;         // -1 = pas encore répondu ; index affiché (QCM)
                      // VI: -1 = chưa trả lời; chỉ số lựa chọn đang hiển thị (trắc nghiệm)
  long value;         // réponse numérique (estimation)
                      // VI: câu trả lời bằng số (câu hỏi ước lượng)
  uint32_t answerMs;
  bool active;        // false = déconnecté (la place et le score sont gardés)
                      // VI: false = mất kết nối (vẫn giữ chỗ và điểm)
};
Player players[MAX_PLAYERS];

// --------------------------------------------------------- jeu · trò chơi ----
enum State { LOBBY, QUESTION, REVEAL, END };
State state = LOBBY;
JsonDocument qdoc;              // toutes les questions jouables (parsées une fois)
                                // VI: tất cả câu hỏi chơi được (chỉ phân tích JSON một lần)
int qCount = 0;
bool played[64];                // question déjà posée dans cette partie
                                // VI: câu hỏi đã được hỏi trong ván này
int retryQ[16]; int retryAt[16]; int retryN = 0;   // questions ratées, à reposer plus tard (après 3 autres)
                                                   // VI: câu hỏi bị trả lời sai, sẽ hỏi lại sau
                                                   //     (sau 3 câu khác)
int curQ = -1;                  // index dans qdoc de la question en cours
                                // VI: chỉ số trong qdoc của câu hỏi đang chơi
int qPlayed = 0;                // nombre de questions posées
                                // VI: số câu hỏi đã hỏi
uint32_t qStart = 0, gameStart = 0;
int qSec = QUESTION_SECONDS;
int lastTick = -1;
uint32_t revealAt = 0;
bool chantierShown = false;

// Le chantier : deux rives, 4 travées chacune, puis la travée centrale (la rencontre)
// VI: Công trường: hai bờ, mỗi bờ 4 nhịp cầu, sau đó là nhịp giữa (cuộc gặp gỡ)
const int PER_SIDE = 4;
int side[2] = {0, 0};           // travées posées par rive
                                // VI: số nhịp cầu đã đặt của mỗi bờ
bool pending[2] = {false, false}; // travée suspendue à la grue (dernier essai raté)
                                  // VI: nhịp cầu đang treo trên cần cẩu (lần thử gần nhất bị sai)
int fails[2] = {0, 0};          // échecs consécutifs sur la travée en cours
                                // VI: số lần sai liên tiếp với nhịp cầu hiện tại
int level[2] = {2, 2};          // niveau des questions proposées (1 facile … 3 difficile)
                                // VI: cấp độ câu hỏi được đưa ra (1 dễ … 3 khó)
int streak[2] = {0, 0};         // série (+ réussites / - échecs) pour adapter le niveau
                                // VI: chuỗi (+ đúng / - sai) để điều chỉnh cấp độ
bool center = false;            // travée centrale posée = pont fermé
                                // VI: nhịp giữa đã đặt = cây cầu đã nối liền
int dblFails = 0;               // tentatives ratées de la rencontre (chaque échec enlève une option)
                                // VI: số lần thất bại ở câu hỏi gặp gỡ (mỗi lần sai bớt đi một lựa
                                //     chọn)
int hintsUsed = 0;
int sideDoneAt[2] = {0, 0};     // question à laquelle chaque rive a fini ses 4 travées
                                // VI: câu hỏi mà tại đó mỗi bờ đã hoàn thành 4 nhịp cầu
int teamScore[2] = {0, 0};

// Question en cours, telle qu'affichée (options mélangées à chaque question)
// VI: Câu hỏi hiện tại, đúng như đang hiển thị (các lựa chọn được xáo trộn ở mỗi câu)
int curO[4]; int curN = 0; int curR = -1; bool curNum = false; long curTarget = 0; bool curDbl = false;
int hideN[2] = {0, 0}; int hideIdx[2][3];   // options cachées pour chaque rive (assistance)
                                            // VI: các lựa chọn bị ẩn cho từng bờ (chế độ trợ giúp)
bool assist[2] = {false, false};

BridgeView view() { BridgeView v; v.side[0] = side[0]; v.side[1] = side[1]; v.pending[0] = pending[0]; v.pending[1] = pending[1]; v.center = center; v.q = qPlayed; v.dbl = curDbl; v.hint = assist[0] || assist[1] || curDbl; return v; }

// ---------------------------------------------- utilitaires · hàm tiện ích ----
Player* findPlayer(uint32_t id) {
  for (auto& p : players) if (p.active && p.id == id) return &p;
  return nullptr;
}
int activePlayers() { int n = 0; for (auto& p : players) if (p.active) n++; return n; }
int activeOn(int r)  { int n = 0; for (auto& p : players) if (p.active && p.team == r) n++; return n; }
int answersCount()  { int n = 0; for (auto& p : players) if (p.active && p.answer >= 0) n++; return n; }
JsonObject currentQ() { return qdoc[curQ].as<JsonObject>(); }

void loadQuestions(const char* json) {
  JsonDocument d;
  DeserializationError e = deserializeJson(d, json);
  if (e) { Serial.printf("[questions] JSON invalide : %s\n", e.c_str()); return; }
  qdoc.clear();
  JsonArray out = qdoc.to<JsonArray>();
  for (JsonObject q : d.as<JsonArray>()) {
    JsonArray o = q["o"].as<JsonArray>();
    int r = q["r"] | -1;
    bool qcm = o.size() == 4 && r >= 0 && r <= 3;
    bool est = q["num"].is<long>();
    if (qcm || est) out.add(q);
  }
  qCount = out.size(); if (qCount > 64) qCount = 64;
  Serial.printf("[questions] %d questions chargees\n", qCount);
}

bool isNum(int i) { return qdoc[i]["num"].is<long>(); }
int  lvOf(int i)  { int l = qdoc[i]["lv"] | 2; return constrain(l, 1, 3); }

// Choisit la prochaine question : d'abord une question ratée qui revient, sinon une question
// inédite au niveau de la rive la moins avancée, sinon la plus proche en niveau. -1 = plus rien.
// VI: Chọn câu hỏi tiếp theo: trước tiên là một câu đã trả lời sai được hỏi lại, nếu không thì một
//     câu mới ở cấp độ của bờ đang chậm hơn, nếu không nữa thì câu có cấp độ gần nhất. -1 = hết câu
//     hỏi.
int pickQuestion() {
  for (int k = 0; k < retryN; k++) if (qPlayed >= retryAt[k]) { int q = retryQ[k]; for (int j = k; j < retryN - 1; j++) { retryQ[j] = retryQ[j + 1]; retryAt[j] = retryAt[j + 1]; } retryN--; return q; }
  int want = min(level[0], level[1]);
  for (int dist = 0; dist <= 2; dist++) {
    int cands[64], n = 0;
    for (int i = 0; i < qCount; i++) if (!played[i] && abs(lvOf(i) - want) == dist && !(curDbl && isNum(i))) cands[n++] = i;
    if (n) return cands[random(n)];
  }
  return -1;
}

void sendTo(AsyncWebSocketClient* c, JsonDocument& d) { String s; serializeJson(d, s); c->text(s); }
void broadcast(JsonDocument& d) { String s; serializeJson(d, s); ws.textAll(s); }
void addBridge(JsonDocument& d) {
  JsonObject b = d["bridge"].to<JsonObject>();
  JsonArray s = b["side"].to<JsonArray>(); s.add(side[0]); s.add(side[1]);
  JsonArray pq = b["pending"].to<JsonArray>(); pq.add(pending[0]); pq.add(pending[1]);
  b["center"] = center; b["per"] = PER_SIDE;
  JsonArray lv = b["level"].to<JsonArray>(); lv.add(level[0]); lv.add(level[1]);
}
void addScores(JsonDocument& d) { JsonArray a = d["scores"].to<JsonArray>(); a.add(teamScore[0]); a.add(teamScore[1]); }

void sendConfig(AsyncWebSocketClient* c) {
  JsonDocument d;
  d["t"] = "config"; d["ssid"] = ssid; d["sec"] = QUESTION_SECONDS; d["per"] = PER_SIDE; d["mode"] = "chantier";
  JsonArray teams = d["teams"].to<JsonArray>();
  for (int t = 0; t < 2; t++) { JsonObject o = teams.add<JsonObject>(); o["n"] = TEAM_NAMES[t]; o["c"] = TEAM_COLORS[t]; }
  sendTo(c, d);
}
void lobbyDoc(JsonDocument& d) {
  d["t"] = "lobby";
  JsonArray a = d["players"].to<JsonArray>();
  for (auto& p : players) if (p.active) { JsonObject o = a.add<JsonObject>(); o["n"] = p.name; o["team"] = p.team; o["s"] = p.score; }
  addBridge(d); addScores(d);
}
void broadcastLobby() { JsonDocument d; lobbyDoc(d); broadcast(d); displayLobby(ssid, activeOn(0), activeOn(1)); }

// Indice du chef de chantier : le champ "hfr" de la question s'il existe, sinon la première phrase de la révélation
// VI: Gợi ý của đốc công: trường "hfr" của câu hỏi nếu có, nếu không thì câu đầu tiên của phần giải
//     thích đáp án
String hintOf(JsonObject q) {
  if (q["hfr"].is<const char*>()) return String((const char*)q["hfr"]);
  const char* s = q["pfr"] | "";
  String h = s; int cut = h.indexOf(". "); if (cut < 0) cut = h.indexOf(" : "); if (cut < 0) cut = h.indexOf(", ");
  if (cut > 20) h = h.substring(0, cut + 1);
  if (h.length() > 110) h = h.substring(0, 108) + "…";
  return h;
}

void questionDoc(JsonDocument& d) {
  JsonObject q = currentQ();
  d["t"] = "question"; d["i"] = qPlayed; d["sec"] = qSec; d["lv"] = lvOf(curQ); d["dbl"] = curDbl;
  d["cat"] = q["cat"]; d["fr"] = q["fr"]; d["vi"] = q["vi"];
  if (curNum) { d["num"] = true; }
  else { JsonArray o = d["o"].to<JsonArray>(); JsonArray src = q["o"].as<JsonArray>(); for (int k = 0; k < curN; k++) o.add(src[curO[k]]); }
  JsonArray hide = d["hide"].to<JsonArray>();
  for (int r = 0; r < 2; r++) { JsonArray h = hide.add<JsonArray>(); for (int k = 0; k < hideN[r]; k++) h.add(hideIdx[r][k]); }
  if (assist[0] || assist[1] || curDbl) d["hint"] = hintOf(q);
  JsonArray as = d["assist"].to<JsonArray>(); as.add(assist[0]); as.add(assist[1]);
  addBridge(d);
}

// Options cachées pour une rive : n options fausses retirées, la bonne reste toujours
// VI: Các lựa chọn bị ẩn cho một bờ: bỏ đi n lựa chọn sai, đáp án đúng luôn được giữ lại
void hideWrong(int r, int n) {
  hideN[r] = 0;
  if (curNum || n <= 0) return;
  int wrong[4], w = 0;
  for (int k = 0; k < curN; k++) if (k != curR) wrong[w++] = k;
  for (int i = w - 1; i > 0; i--) { int j = random(i + 1); int t = wrong[i]; wrong[i] = wrong[j]; wrong[j] = t; }
  for (int k = 0; k < n && k < w && hideN[r] < 3; k++) hideIdx[r][hideN[r]++] = wrong[k];
}

// ---------------------------------- déroulé du jeu · diễn biến trò chơi ----
void endGame();

void startQuestion() {
  if (center) { endGame(); return; }
  curDbl = (side[0] >= PER_SIDE && side[1] >= PER_SIDE);
  int q = pickQuestion();
  if (q < 0) {                     // plus de questions : « le dernier coup de main » ferme le pont
                                   // VI: hết câu hỏi: « cú hỗ trợ cuối cùng » khép cây cầu lại
    side[0] = side[1] = PER_SIDE; center = true; pending[0] = pending[1] = false;
    Serial.println("[jeu] plus de questions : dernier coup de main, le pont est ferme");
    endGame(); return;
  }
  curQ = q; played[q] = true; qPlayed++;
  JsonObject qq = currentQ();
  curNum = isNum(q); curTarget = curNum ? (long)qq["num"] : 0;
  if (curNum) { curN = 0; curR = -1; }
  else {
    curN = 4; for (int k = 0; k < 4; k++) curO[k] = k;
    for (int i = 3; i > 0; i--) { int j = random(i + 1); int t = curO[i]; curO[i] = curO[j]; curO[j] = t; }
    int r = qq["r"] | 0; for (int k = 0; k < 4; k++) if (curO[k] == r) curR = k;
  }
  // assistance : niveau 1, ou trois échecs sur la même travée -> deux options + indice + 30 s
  // VI: trợ giúp: cấp độ 1, hoặc sai ba lần với cùng một nhịp cầu -> còn hai lựa chọn + gợi ý + 30
  //     giây
  for (int r = 0; r < 2; r++) { assist[r] = (level[r] <= 1) || (fails[r] >= 3); hideWrong(r, assist[r] ? 2 : 0); }
  if (curDbl) { int n = min(dblFails, 2); if (n) { hideWrong(0, n); hideWrong(1, n); hideIdx[1][0] = hideIdx[0][0]; hideIdx[1][1] = hideIdx[0][1]; hideN[1] = hideN[0]; } }
  if (assist[0] || assist[1] || curDbl) hintsUsed++;
  qSec = (assist[0] || assist[1]) ? QUESTION_SECONDS + 10 : QUESTION_SECONDS;
  for (auto& p : players) { p.answer = -1; p.value = 0; p.answerMs = 0; }
  state = QUESTION; qStart = millis(); lastTick = -1;
  if (qPlayed == 1) gameStart = millis();
  JsonDocument d; questionDoc(d); broadcast(d);
  displayQuestion(view(), qSec, qSec);
  Serial.printf("[jeu] question %d (niveau %d%s%s) rives %d/%d  %d/%d\n", qPlayed, lvOf(q), curDbl ? ", rencontre" : "", curNum ? ", estimation" : "", side[0], PER_SIDE, side[1], PER_SIDE);
}

// Réussite d'une rive : majorité de bonnes réponses parmi ceux qui ont répondu (estimation : médiane à ±40 % de la cible)
// VI: Một bờ thành công khi: đa số người đã trả lời chọn đúng (câu ước lượng: trung vị nằm trong
//     khoảng ±40 % so với giá trị đích)
bool sideSucceeds(int r, int& nc, int& na, long& med) {
  nc = 0; na = 0; med = 0;
  if (curNum) {
    long vals[MAX_PLAYERS]; int n = 0;
    for (auto& p : players) if (p.active && p.team == r && p.answer >= 0) vals[n++] = p.value;
    na = n; if (!n) return false;
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && vals[j - 1] > vals[j]; j--) { long t = vals[j]; vals[j] = vals[j - 1]; vals[j - 1] = t; }
    med = vals[n / 2];
    // Calcul en nombre à virgule : l'ancienne formule curTarget / 100 * 40 (division entière)
    // donnait ±1 seulement pour une cible inférieure à 100 (54 / 100 = 0).
    // VI: Tính bằng số thực: công thức cũ curTarget / 100 * 40 (phép chia lấy phần nguyên) chỉ cho
    //     ±1 với giá trị đích nhỏ hơn 100 (54 / 100 = 0).
    long tol = (curTarget >= 1800 && curTarget <= 2100) ? 5 : (long)(labs(curTarget) * 0.4);   // une année : ±5 ans ; sinon ±40 %
                                                                                      // VI: nếu là một năm: ±5 năm; nếu không: ±40 %
    if (tol < 1) tol = 1;
    bool ok = labs(med - curTarget) <= tol; if (ok) nc = n;
    return ok;
  }
  for (auto& p : players) if (p.active && p.team == r && p.answer >= 0) { na++; if (p.answer == curR) nc++; }
  return na > 0 && nc * 2 >= na;
}

void reveal() {
  if (state != QUESTION) return;
  state = REVEAL; revealAt = millis(); chantierShown = false;
  JsonObject q = currentQ();
  // points de vitesse individuels (QCM) : 100 + jusqu'à 50 selon la rapidité
  // VI: điểm tốc độ cá nhân (trắc nghiệm): 100 + tối đa 50 tùy theo độ nhanh
  for (auto& p : players) {
    if (!p.active) continue;
    int pts = 0; bool ok = false;
    if (p.answer >= 0 && !curNum && p.answer == curR) {
      ok = true; float speed = 1.0f - (float)(p.answerMs - qStart) / (qSec * 1000.0f); if (speed < 0) speed = 0;
      pts = 100 + (int)(50 * speed); p.score += pts; teamScore[p.team] += pts;
    }
    JsonDocument y; y["t"] = "you"; y["ok"] = ok; y["pts"] = pts; y["answered"] = (p.answer >= 0);
    AsyncWebSocketClient* c = ws.client(p.id); if (c) sendTo(c, y);
  }
  int nc[2], na[2]; long med[2]; bool ok[2];
  for (int r = 0; r < 2; r++) ok[r] = sideSucceeds(r, nc[r], na[r], med[r]);
  for (int r = 0; r < 2; r++) if (activeOn(r) == 0) ok[r] = ok[1 - r];   // rive vide : elle suit l'autre
                                                                         // VI: bờ không có người chơi: đi theo kết quả
                                                                         //     của bờ kia
  bool res[2] = {false, false};
  if (curDbl) {
    if (ok[0] && ok[1]) { center = true; res[0] = res[1] = true; pending[0] = pending[1] = false; }
    else { dblFails++; pending[0] = !ok[0]; pending[1] = !ok[1]; }
  } else {
    for (int r = 0; r < 2; r++) {
      if (side[r] >= PER_SIDE) { res[r] = true; continue; }      // rive déjà arrivée : elle attend l'autre
                                                                 // VI: bờ đã xây xong: chờ bờ kia
      if (ok[r]) { side[r]++; pending[r] = false; fails[r] = 0; res[r] = true; if (side[r] == PER_SIDE) sideDoneAt[r] = qPlayed;
                   if (++streak[r] >= 2 && level[r] < 3) { level[r]++; streak[r] = 0; } if (streak[r] < 0) streak[r] = 0; }
      else { pending[r] = true; fails[r]++; res[r] = false;
             if (--streak[r] <= -2 && level[r] > 1) { level[r]--; streak[r] = 0; } if (streak[r] > 0) streak[r] = 0; }
    }
    if ((!ok[0] || !ok[1]) && retryN < 16) { retryQ[retryN] = curQ; retryAt[retryN] = qPlayed + 3; retryN++; }
  }
  JsonDocument d;
  d["t"] = "reveal"; d["r"] = curR; d["pfr"] = q["pfr"]; d["pvi"] = q["pvi"]; d["dbl"] = curDbl;
  JsonArray rs = d["res"].to<JsonArray>(); rs.add(res[0]); rs.add(res[1]);
  JsonArray a1 = d["nc"].to<JsonArray>(); a1.add(nc[0]); a1.add(nc[1]);
  JsonArray a2 = d["na"].to<JsonArray>(); a2.add(na[0]); a2.add(na[1]);
  if (curNum) { d["target"] = curTarget; JsonArray a3 = d["med"].to<JsonArray>(); a3.add(med[0]); a3.add(med[1]); }
  addBridge(d); addScores(d);
  broadcast(d);
  displayReveal(view(), res[0], res[1]);
  Serial.printf("[jeu] rive F %s (%d/%d)  rive V %s (%d/%d)  pont %d+%d%s\n", res[0] ? "pose" : "suspend", nc[0], na[0], res[1] ? "pose" : "suspend", nc[1], na[1], side[0], side[1], center ? " FERME" : "");
}

void endGame() {
  state = END;
  int dur = gameStart ? (millis() - gameStart) / 1000 : 0;
  JsonDocument d; d["t"] = "end"; d["qs"] = qPlayed; d["dur"] = dur; d["hints"] = hintsUsed;
  JsonArray da = d["doneAt"].to<JsonArray>(); da.add(sideDoneAt[0]); da.add(sideDoneAt[1]);
  addBridge(d); addScores(d); broadcast(d);
  displayEnd(view(), qPlayed, dur);
  Serial.printf("[jeu] PONT FERME en %d questions, %d s, %d indices\n", qPlayed, dur, hintsUsed);
}

void resetGame(bool keepPlayers) {
  state = LOBBY; curQ = -1; qPlayed = 0; gameStart = 0; retryN = 0; center = false; dblFails = 0; hintsUsed = 0; curDbl = false;
  for (int i = 0; i < 64; i++) played[i] = false;
  for (int r = 0; r < 2; r++) { side[r] = 0; pending[r] = false; fails[r] = 0; level[r] = 2; streak[r] = 0; sideDoneAt[r] = 0; teamScore[r] = 0; }
  for (auto& p : players) { p.score = 0; p.answer = -1; if (!keepPlayers) { p.active = false; p.name[0] = 0; } }
  JsonDocument d; d["t"] = "reset"; broadcast(d);
  broadcastLobby();
  Serial.println("[jeu] reset");
}

// ------------------------------------------------------------ WebSocket ----
void onWsMessage(AsyncWebSocketClient* c, uint8_t* data, size_t len) {
  JsonDocument m;
  if (deserializeJson(m, data, len)) return;
  const char* t = m["t"] | "";

  if (!strcmp(t, "hello")) {                       // grand écran / télécommande
                                                   // VI: màn hình lớn / bộ điều khiển
    sendConfig(c);
    if (state == LOBBY) { JsonDocument d; lobbyDoc(d); sendTo(c, d); }
    else if (state == QUESTION || state == REVEAL) { JsonDocument d; questionDoc(d); sendTo(c, d); }
  }
  else if (!strcmp(t, "join")) {
    const char* name = m["name"] | "?";
    Player* p = findPlayer(c->id());
    if (!p) for (auto& x : players) if (!x.active && x.name[0] && !strcmp(x.name, name)) { p = &x; break; }   // reconnexion : même prénom
                                                                                                              // VI: kết nối lại: cùng tên
    if (!p) for (auto& x : players) if (!x.active) { p = &x; p->score = 0; break; }
    if (!p) { JsonDocument d; d["t"] = "full"; sendTo(c, d); return; }
    p->active = true; p->id = c->id();
    strlcpy(p->name, name, sizeof(p->name));
    int team = m["team"] | -1;
    if (team < 0 || team > 1) team = activeOn(0) <= activeOn(1) ? 0 : 1;     // équilibrage automatique des rives
                                                                             // VI: tự động cân bằng hai bờ
    p->team = team;
    if (state != QUESTION) p->answer = -1;
    JsonDocument d; d["t"] = "joined"; d["id"] = c->id(); d["team"] = p->team; sendTo(c, d);
    if (state == LOBBY) broadcastLobby();
    else if (state == QUESTION) { JsonDocument q; questionDoc(q); sendTo(c, q); }
    Serial.printf("[joueur] %s rejoint la rive %d (%d connectes)\n", p->name, p->team + 1, activePlayers());
  }
  else if (!strcmp(t, "answer")) {
    if (state != QUESTION) return;
    Player* p = findPlayer(c->id());
    if (!p || p->answer >= 0) return;
    if (curNum) { if (!m["v"].is<long>()) return; p->value = m["v"]; p->answer = 0; }
    else { int a = m["a"] | -1; if (a < 0 || a >= curN) return; p->answer = a; }
    p->answerMs = millis();
    if (answersCount() >= activePlayers()) reveal();   // tout le monde a répondu : on révèle sans attendre
                                                       // VI: mọi người đã trả lời: công bố đáp án
                                                       //     ngay, không chờ
  }
  else if (!strcmp(t, "host")) {
    hostCommand(m["cmd"] | "");
  }
}

// Lancer depuis le salon. Deux protections pour les petits groupes :
// VI: Bắt đầu từ phòng chờ. Hai cơ chế bảo vệ cho nhóm nhỏ:
//  - aucun joueur connecté : on ne lance pas (le grand écran grise le bouton) ;
// VI: - không có người chơi nào kết nối: không bắt đầu (màn hình lớn làm mờ nút);
//  - une rive vide alors que l'autre a 2 joueurs ou plus : un joueur change de rive,
//    pour que chaque rive ait au moins un humain. Avec 1 seul joueur, la rive vide suit l'autre.
// VI: - một bờ trống trong khi bờ kia có từ 2 người chơi: một người chuyển sang bờ trống, để mỗi bờ
//     có ít nhất một người. Nếu chỉ có 1 người chơi, bờ trống đi theo bờ kia.
void lobbyStart() {
  if (activePlayers() == 0) { Serial.println("[jeu] aucun joueur connecte : on attend"); return; }
  for (int r = 0; r < 2; r++) {
    if (activeOn(r) > 0 || activeOn(1 - r) < 2) continue;
    Player* m = nullptr;
    for (auto& p : players) if (p.active && p.team == 1 - r) m = &p;
    if (!m) continue;
    m->team = r;
    AsyncWebSocketClient* c = ws.client(m->id);
    if (c) { JsonDocument d; d["t"] = "joined"; d["id"] = m->id; d["team"] = r; d["moved"] = true; sendTo(c, d); }
    Serial.printf("[jeu] %s passe sur la rive %d pour equilibrer\n", m->name, r + 1);
  }
  startQuestion();
}

// Commandes de l'animateur : viennent de la page /host, ou de l'écran tactile (touch.h)
// VI: Lệnh của người điều hành: đến từ trang /host, hoặc từ màn hình cảm ứng (touch.h)
void hostCommand(const char* cmd) {
  if (!strcmp(cmd, "start"))       { if (state == LOBBY) lobbyStart(); }
  else if (!strcmp(cmd, "next"))   { if (state == LOBBY) lobbyStart(); else if (state == QUESTION) reveal(); else if (state == REVEAL) startQuestion(); else if (state == END) resetGame(true); }
  else if (!strcmp(cmd, "reveal")) { reveal(); }
  else if (!strcmp(cmd, "reset"))  { resetGame(false); }
}

void onWsEvent(AsyncWebSocket* s, AsyncWebSocketClient* c, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    sendConfig(c);
  } else if (type == WS_EVT_DISCONNECT) {
    Player* p = findPlayer(c->id());
    if (p) { p->active = false; if (state == LOBBY) broadcastLobby(); }
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) onWsMessage(c, data, len);
  }
}

// ------------------------ serveur web + captif · máy chủ web + cổng tự mở ----
void redirectHome(AsyncWebServerRequest* r) { r->redirect("http://192.168.4.1/"); }

void setupServer() {
  server.on("/",      HTTP_GET, [](AsyncWebServerRequest* r) { r->send(200, "text/html; charset=utf-8", PLAYER_HTML); });
  server.on("/host",  HTTP_GET, [](AsyncWebServerRequest* r) { r->send(200, "text/html; charset=utf-8", HOST_HTML); });
  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest* r) { r->send(200, "text/html; charset=utf-8", ADMIN_HTML); });

  // Images et polices du téléphone (/a/...) : gardées en cache par le navigateur, chargées une seule fois
  // VI: Hình ảnh và phông chữ của điện thoại (/a/...): trình duyệt giữ trong bộ nhớ đệm, chỉ tải một lần
  for (int i = 0; i < WEB_ASSET_COUNT; i++) {
    const WebAsset* a = &WEB_ASSETS[i];
    server.on(a->path, HTTP_GET, [a](AsyncWebServerRequest* r) {
      AsyncWebServerResponse* res = r->beginResponse(200, a->type, a->data, a->len);
      res->addHeader("Cache-Control", "public, max-age=31536000, immutable");
      r->send(res);
    });
  }

  server.on("/api/questions", HTTP_GET, [](AsyncWebServerRequest* r) {
    String s; serializeJson(qdoc, s);
    r->send(200, "application/json; charset=utf-8", s);
  });
  server.on("/api/questions", HTTP_POST,
    [](AsyncWebServerRequest* r) {},
    NULL,
    [](AsyncWebServerRequest* r, uint8_t* data, size_t len, size_t index, size_t total) {
      static String body;
      if (index == 0) body = "";
      body.concat((const char*)data, len);
      if (index + len == total) {
        loadQuestions(body.c_str());
        body = "";
        for (int i = 0; i < 64; i++) played[i] = false;
        r->send(200, "text/plain; charset=utf-8", String(qCount) + " questions chargees dans l'objet");
      }
    });
  server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest* r) {
    JsonDocument d;
    d["ssid"] = ssid; d["state"] = (int)state; d["players"] = activePlayers(); d["question"] = qPlayed;
    d["questions"] = qCount; d["heap"] = ESP.getFreeHeap(); d["clients"] = ws.count(); addBridge(d);
    String s; serializeJson(d, s);
    r->send(200, "application/json", s);
  });

  // Portail captif : les sondes des téléphones sont renvoyées vers la page du jeu
  // VI: Cổng tự mở: các yêu cầu kiểm tra Internet của điện thoại được chuyển hướng về trang trò
  //     chơi
  server.on("/generate_204", HTTP_GET, redirectHome);          // Android
  server.on("/gen_204", HTTP_GET, redirectHome);
  server.on("/hotspot-detect.html", HTTP_GET, redirectHome);   // iPhone / Mac
  server.on("/library/test/success.html", HTTP_GET, redirectHome);
  server.on("/connecttest.txt", HTTP_GET, redirectHome);       // Windows
  server.on("/ncsi.txt", HTTP_GET, redirectHome);
  server.on("/fwlink", HTTP_GET, redirectHome);
  server.onNotFound(redirectHome);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

// ------------------------------------------------------ setup · khởi động ----
void setup() {
  Serial.begin(115200);
  uint32_t t0 = millis();
  while (!Serial && millis() - t0 < 3000) delay(10);   // USB natif : on laisse au moniteur série le temps de se reconnecter
                                                       // VI: USB gốc: chờ Serial Monitor kết nối lại
  delay(200);
  Serial.println("\n=== LE PONT · CAU ===");

  snprintf(ssid, sizeof(ssid), "LePont-%d", GROUP_ID);
  for (auto& p : players) { p.active = false; p.name[0] = 0; p.score = 0; p.answer = -1; }
  loadQuestions(QUESTIONS_JSON);
  displayBegin();
  touchBegin();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  uint8_t ch = WIFI_CHANNELS[(GROUP_ID - 1) % 7];
  int maxConn = MAX_PLAYERS + 2; if (maxConn > 10) maxConn = 10;
  bool ok = WiFi.softAP(ssid, strlen(WIFI_PASSWORD) >= 8 ? WIFI_PASSWORD : NULL, ch, 0, maxConn);
  Serial.printf("[wifi] %s  canal %d  %s  (%s)\n", ssid, ch, WiFi.softAPIP().toString().c_str(), ok ? "OK" : "ECHEC");

  dns.start(53, "*", WiFi.softAPIP());
  setupServer();
  displayLobby(ssid, 0, 0);
  Serial.println("[http] joueurs : http://192.168.4.1/   grand ecran : /host   questions : /admin   etat : /api/state");
}

// -------------------------------------------------------- loop · vòng lặp ----
void loop() {
  dns.processNextRequest();
  displayTick();   // animations de l'écran (travée qui se pose, dragon, feux d'artifice)
                   // VI: hoạt hình trên màn hình (nhịp cầu hạ xuống, con rồng, pháo hoa)

  // Tactile (Niveau 2) : un tap fait avancer la partie, un appui long la remet à zéro
  // VI: Cảm ứng (mức 2): chạm một lần để chuyển bước tiếp theo, nhấn giữ để chơi lại từ đầu
  TouchEvent te = touchPoll();
  if (te == TOUCH_TAP)       { Serial.println("[tactile] tap -> suite"); hostCommand("next"); }
  else if (te == TOUCH_LONG) { Serial.println("[tactile] appui long -> nouvelle partie"); hostCommand("reset"); }

  static uint32_t lastClean = 0;
  if (millis() - lastClean > 1000) { ws.cleanupClients(); lastClean = millis(); }

  // Bilan toutes les 10 s dans le moniteur série : utile pour tester le Wi-Fi avec plusieurs téléphones
  // VI: Tổng kết mỗi 10 giây trên Serial Monitor: hữu ích để thử Wi-Fi với nhiều điện thoại
  static uint32_t lastStat = 0;
  if (millis() - lastStat > 10000) {
    lastStat = millis();
    Serial.printf("[etat] %s  wifi %d appareils  pages ouvertes %d  joueurs %d  memoire libre %u\n",
                  ssid, WiFi.softAPgetStationNum(), (int)ws.count(), activePlayers(), (unsigned)ESP.getFreeHeap());
  }

  if (state == QUESTION) {
    int left = qSec - (int)((millis() - qStart) / 1000);
    if (left < 0) left = 0;
    if (left != lastTick) {
      lastTick = left;
      JsonDocument d; d["t"] = "tick"; d["left"] = left;
      JsonArray a = d["answers"].to<JsonArray>(); int n0 = 0, n1 = 0;
      for (auto& p : players) if (p.active && p.answer >= 0) { if (p.team == 0) n0++; else n1++; }
      a.add(n0); a.add(n1); broadcast(d);
      displayQuestion(view(), left, qSec);
    }
    if (left == 0) reveal();
  }
  else if (state == REVEAL && !chantierShown && millis() - revealAt > 4000) {
    chantierShown = true;
    displayChantier(view());
  }
}
