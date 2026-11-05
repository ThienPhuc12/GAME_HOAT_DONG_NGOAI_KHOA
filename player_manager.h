#pragma once
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "trivia_evaluator.h"

enum PlayerRole : uint8_t {
  ROLE_MAIN_PLAYER = 0, ROLE_SPECTATOR = 1, ROLE_HOST_DISPLAY = 2
};

struct Player {
  char name[32] = {};
  char playerId[25] = {};
  uint32_t clientId = 0;
  Team team = TEAM_FR;
  PlayerRole role = ROLE_SPECTATOR;
  uint8_t teamSlot = 0;
  int8_t prediction = -1;
  uint16_t correctAnswers = 0;
  bool used = false;
  bool connected = false;
  bool bot = false;
};

class PlayerManager {
 public:
  Player* join(uint32_t clientId, const char* name, Team team, const char* playerId = nullptr) {
    if (!clientId || !name || !name[0] || strlen(name) >= sizeof(Player::name) ||
        static_cast<uint8_t>(team) > 1) return nullptr;
    const bool hasId = playerId && playerId[0];
    if (hasId && strlen(playerId) >= sizeof(Player::playerId)) return nullptr;
    if (Player* bound = findByClient(clientId))
      if (strcmp(bound->name, name) != 0 || bound->team != team ||
          (hasId && strcmp(bound->playerId, playerId) != 0)) return nullptr;
    for (auto& player : players_) {
      if (player.used && !player.bot &&
          ((hasId && strcmp(player.playerId, playerId) == 0) ||
           (hasId && !player.playerId[0] && strcmp(player.name, name) == 0) ||
           (!hasId && !player.playerId[0] && strcmp(player.name, name) == 0))) {
        if (player.team != team || strcmp(player.name, name) != 0) return nullptr;
        if (hasId && !player.playerId[0]) strcpy(player.playerId, playerId);
        player.clientId = clientId;
        player.connected = true;
        return &player;
      }
    }
    for (const auto& player : players_)
      if (player.used && !player.bot && strcmp(player.name, name) == 0) return nullptr;
    int8_t botSlot = -1;
    for (auto& player : players_)
      if (player.used && player.bot && player.role == ROLE_MAIN_PLAYER && player.team == team) {
        botSlot = player.teamSlot;
        player = Player{};
        hasMainPlayer_[team] = false;
        break;
      }
    if (count(team) >= TriviaEvaluator::MAX_MEMBERS_PER_TEAM ||
        totalCount() >= MAX_CLIENTS - 1) return nullptr; // One Wi-Fi slot is reserved for Host.
    for (auto& player : players_) {
      if (!player.used) {
        player = Player{};
        strcpy(player.name, name);
        if (hasId) strcpy(player.playerId, playerId);
        player.clientId = clientId;
        player.team = team;
        player.role = hasMainPlayer_[team] ? ROLE_SPECTATOR : ROLE_MAIN_PLAYER;
        player.teamSlot = botSlot >= 0 ? botSlot : count(team);
        player.used = player.connected = true;
        if (player.role == ROLE_MAIN_PLAYER) hasMainPlayer_[team] = true;
        return &player;
      }
    }
    return nullptr;
  }

  void disconnect(uint32_t clientId) {
    if (Player* player = findByClient(clientId)) {
      player->connected = false;
      player->clientId = 0;
    }
  }

  bool removeById(const char* playerId) {
    if (!playerId || !playerId[0]) return false;
    for (auto& player : players_) {
      if (player.used && !player.bot && strcmp(player.playerId, playerId) == 0) {
        const Team team = player.team;
        player = Player{};
        reindexTeam(team);
        return true;
      }
    }
    return false;
  }

  Player* reconnectById(uint32_t clientId, const char* playerId) {
    if (!clientId || !playerId || !playerId[0]) return nullptr;
    for (auto& player : players_) if (player.used && !player.bot &&
                                     strcmp(player.playerId, playerId) == 0) {
      player.clientId = clientId;
      player.connected = true;
      return &player;
    }
    return nullptr;
  }

  bool removeByClient(uint32_t clientId) {
    if (Player* player = findByClient(clientId)) {
      const Team team = player->team;
      *player = Player{};
      reindexTeam(team);
      return true;
    }
    return false;
  }

  void clearAll() {
    for (auto& player : players_) player = Player{};
    hasMainPlayer_[TEAM_FR] = hasMainPlayer_[TEAM_VN] = false;
  }

  Player* findByClient(uint32_t clientId) {
    if (!clientId) return nullptr;
    for (auto& player : players_)
      if (player.used && player.connected && player.clientId == clientId) return &player;
    return nullptr;
  }

  bool predict(uint32_t clientId, Team winner) {
    Player* player = findByClient(clientId);
    if (!player || player->role != ROLE_SPECTATOR || static_cast<uint8_t>(winner) > 1)
      return false;
    player->prediction = static_cast<int8_t>(winner);
    return true;
  }

  uint8_t count(Team team) const {
    uint8_t n = 0;
    for (const auto& player : players_) if (player.used && player.team == team) ++n;
    return n;
  }

  uint8_t totalCount() const { return count(TEAM_FR) + count(TEAM_VN); }

  uint8_t connectedCount(Team team) const {
    uint8_t n = 0;
    for (const auto& player : players_)
      if (player.used && player.connected && player.team == team) ++n;
    return n;
  }

  bool hasMain(Team team) const { return hasMainPlayer_[team]; }
  bool hasRealMain(Team team) const {
    for (const auto& player : players_)
      if (player.used && !player.bot && player.team == team && player.role == ROLE_MAIN_PLAYER) return true;
    return false;
  }
  bool addBot(const char* name, Team team, bool main) {
    if (!name || !name[0] || strlen(name) >= sizeof(Player::name) ||
        static_cast<uint8_t>(team) > 1 || (main && hasMainPlayer_[team]) ||
        totalCount() >= MAX_CLIENTS - 1) return false;
    for (const auto& p : players_) if (p.used && strcmp(p.name, name) == 0) return false;
    for (auto& p : players_) if (!p.used) {
      p = Player{};
      strcpy(p.name, name);
      p.team = team;
      p.role = main ? ROLE_MAIN_PLAYER : ROLE_SPECTATOR;
      p.teamSlot = count(team);
      p.used = p.bot = true;
      if (main) hasMainPlayer_[team] = true;
      return true;
    }
    return false;
  }
  void removeBots() {
    for (auto& p : players_) if (p.used && p.bot) p = Player{};
    for (uint8_t t = 0; t < 2; ++t) {
      uint8_t slot = 0;
      hasMainPlayer_[t] = false;
      for (auto& p : players_) if (p.used && static_cast<uint8_t>(p.team) == t) {
        p.teamSlot = slot++;
        if (p.role == ROLE_MAIN_PLAYER) hasMainPlayer_[t] = true;
      }
    }
  }
  const Player* players() const { return players_; }
  void clearPredictions() {
    for (auto& player : players_) if (player.used) player.prediction = -1;
  }
  void recordSpectatorAnswers(const TriviaEvaluator& quiz) {
    if (!quiz.finished()) return;
    for (auto& player : players_) if (player.used && !player.bot && player.role == ROLE_SPECTATOR &&
                                      quiz.choiceCorrect(player.team, player.teamSlot))
      ++player.correctAnswers;
  }

 private:
  void reindexTeam(Team team) {
    uint8_t slot = 0;
    hasMainPlayer_[team] = false;
    for (auto& player : players_) if (player.used && player.team == team) {
      player.teamSlot = slot++;
      if (player.role == ROLE_MAIN_PLAYER) hasMainPlayer_[team] = true;
    }
  }
  Player players_[MAX_CLIENTS]{};
  bool hasMainPlayer_[2] = {false, false};
};
