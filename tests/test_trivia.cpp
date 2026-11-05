#include <cassert>
#include "../trivia_evaluator.h"

static uint8_t fixedLane() { return 1; }

static_assert(TUTORIAL_SLIDE_MS == 10000);
static_assert(TRIVIA_MS == 10000);
static_assert(CHAR_SELECT_MS == 8000);

int main() {
  TriviaEvaluator trivia(fixedLane);
  assert(trivia.begin(2, 3, 2, 1000));
  assert(trivia.recordChoice(TEAM_FR, 0, 0, 1000));
  assert(trivia.recordChoice(TEAM_FR, 0, 2, 10999));
  assert(trivia.recordChoice(TEAM_FR, 1, 2, 1001));
  assert(trivia.recordChoice(TEAM_VN, 0, 2, 1002));
  assert(trivia.recordChoice(TEAM_VN, 1, 2, 1003));
  assert(trivia.recordChoice(TEAM_VN, 1, 0, 10999));
  assert(trivia.answeredCount(TEAM_FR) == 2);
  assert(trivia.answeredCount(TEAM_VN) == 2);
  assert(!trivia.recordChoice(TEAM_VN, 3, 2, 1003));
  assert(!trivia.recordChoice(TEAM_VN, 2, 2, 11000));
  assert(!trivia.finish(10999, TEAM_FR));
  assert(trivia.finish(11000, TEAM_FR));
  assert(trivia.choiceCorrect(TEAM_FR, 0));
  assert(!trivia.choiceCorrect(TEAM_VN, 1));
  auto result = trivia.result();
  assert(result.pctFR == 100.0f);
  assert(result.pctVN > 33.3f && result.pctVN < 33.4f);
  assert(result.activeLanes == 3 && result.blockedLane == -1);

  assert(trivia.begin(2, 2, 0, 7000));
  assert(trivia.recordChoice(TEAM_FR, 0, 0, 7001));
  assert(trivia.recordChoice(TEAM_VN, 0, 0, 7001));
  assert(trivia.finish(17000, TEAM_FR));
  result = trivia.result();
  assert(result.pctFR == 50.0f && result.pctVN == 50.0f);
  assert(result.activeLanes == 3);

  assert(trivia.begin(1, 1, 1, 20000));
  assert(trivia.recordChoice(TEAM_FR, 0, 1, 20001));
  assert(trivia.finish(30000, TEAM_VN));
  result = trivia.result();
  assert(result.activeLanes == 2 && result.blockedLane == 1);

  assert(trivia.begin(0, 0, 3, 0xFFFFFF00u));
  assert(!trivia.finish(0x0000260Fu, TEAM_VN));
  assert(trivia.finish(0x00002610u, TEAM_VN));
  assert(!trivia.recordChoice(TEAM_VN, 0, 3, 0x00002611u));

  assert(trivia.begin(1, 1, 1, 30000, 8000));
  assert(trivia.recordChoice(TEAM_FR, 0, 1, 37999));
  assert(!trivia.recordChoice(TEAM_FR, 0, 0, 38000));
  assert(trivia.finish(38000, TEAM_VN));
  assert(trivia.result().pctFR == 100.0f);

  assert(trivia.begin(64, 1, 0, 40000));
  assert(trivia.recordChoice(TEAM_FR, 63, 0, 40001));
  assert(!trivia.begin(65, 1, 0, 40002));
  assert(trivia.finish(50000, TEAM_VN));
  assert(trivia.result().pctFR > 1.56f && trivia.result().pctFR < 1.57f);
}
