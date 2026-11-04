#include <cassert>
#include <cstring>
#include "../player_manager.h"

int main() {
  PlayerManager players;
  auto first = players.join(101, "Alice", TEAM_FR);
  assert(first && first->role == ROLE_MAIN_PLAYER);
  auto second = players.join(102, "Bob", TEAM_FR);
  assert(second && second->role == ROLE_SPECTATOR);
  assert(players.count(TEAM_FR) == 2);
  assert(players.connectedCount(TEAM_FR) == 2);
  players.disconnect(101);
  assert(players.connectedCount(TEAM_FR) == 1);
  auto again = players.join(103, "Alice", TEAM_FR);
  assert(again == first && again->role == ROLE_MAIN_PLAYER);
  assert(players.findByClient(101) == nullptr);
  assert(players.findByClient(103) == first);
  assert(players.count(TEAM_FR) == 2);
  assert(!players.join(103, "Eve", TEAM_FR));
  assert(!players.join(104, "Alice", TEAM_VN));
  assert(players.join(105, "Chloé", TEAM_VN)->role == ROLE_MAIN_PLAYER);
  assert(players.findByClient(102)->teamSlot == 1);
  assert(players.findByClient(103)->teamSlot == 0);
  assert(players.predict(102, TEAM_VN));
  players.clearPredictions();
  assert(players.findByClient(102)->prediction == -1);
  assert(!players.predict(103, TEAM_VN));

  const char* extras[] = {"P4", "P5", "P6", "P7", "P8", "P9", "P10", "P11"};
  for (int i = 0; i < 8; ++i)
    assert(players.join(200 + i, extras[i], TEAM_VN));
  assert(players.totalCount() == MAX_CLIENTS - 1);
  assert(!players.join(300, "Overflow", TEAM_VN));

  PlayerManager solo;
  Player* owner = solo.join(1, "Lan", TEAM_VN, "abc123");
  assert(owner && owner->role == ROLE_MAIN_PLAYER);
  solo.disconnect(1);
  assert(solo.join(2, "Lan", TEAM_VN, "abc123") == owner);
  assert(!solo.join(3, "Lan", TEAM_VN, "different"));
  assert(!solo.join(3, "Mai", TEAM_VN, "abc123"));
  assert(solo.addBot("Bot Phap", TEAM_FR, true));
  assert(solo.addBot("Fan A", TEAM_FR, false));
  assert(solo.addBot("Fan B", TEAM_VN, false));
  assert(solo.count(TEAM_FR) == 2 && solo.count(TEAM_VN) == 2);
  Player* opponent = solo.join(4, "Pierre", TEAM_FR, "real456");
  assert(opponent && opponent->role == ROLE_MAIN_PLAYER && !opponent->bot);
  assert(opponent->teamSlot == 0 && solo.count(TEAM_FR) == 2);
  solo.removeBots();
  assert(solo.count(TEAM_FR) == 1 && solo.count(TEAM_VN) == 1);
  assert(solo.hasMain(TEAM_FR) && solo.hasMain(TEAM_VN));
}
