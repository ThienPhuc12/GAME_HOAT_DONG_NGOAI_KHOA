#include <cassert>
#include "../trivia_evaluator.h"

static uint8_t fixedLane() { return 1; }

int main() {
  TriviaEvaluator trivia(fixedLane);
  trivia.begin(2, 3, 2, 1000);
  assert(trivia.recordAnswer(TEAM_FR, 0, 2, 1000));
  assert(!trivia.recordAnswer(TEAM_FR, 0, 2, 1001));
  assert(trivia.recordAnswer(TEAM_FR, 1, 0, 1001));
  assert(trivia.recordAnswer(TEAM_VN, 0, 2, 1002));
  assert(trivia.recordAnswer(TEAM_VN, 1, 2, 1003));
  assert(trivia.answeredCount(TEAM_FR) == 2);
  assert(trivia.answeredCount(TEAM_VN) == 2);
  assert(!trivia.recordAnswer(TEAM_VN, 3, 2, 1003));
  assert(trivia.recordAnswer(TEAM_VN, 2, 0, 8999));
  assert(!trivia.recordAnswer(TEAM_FR, 0, 2, 9000));
  assert(!trivia.finish(8999, TEAM_FR));
  assert(trivia.finish(9000, TEAM_FR));
  auto result = trivia.result();
  assert(result.pctFR == 50.0f);
  assert(result.pctVN > 66.6f && result.pctVN < 66.7f);
  assert(result.activeLanes == 2 && result.blockedLane == 1);
  assert(!result.isLaneActive(1) && result.isLaneActive(0) && result.isLaneActive(2));
  assert(!trivia.finish(6001, TEAM_FR));

  trivia.begin(2, 2, 0, 7000);
  trivia.recordAnswer(TEAM_FR, 0, 0, 7001);
  trivia.recordAnswer(TEAM_VN, 0, 0, 7001);
  assert(trivia.finish(15000, TEAM_FR));
  result = trivia.result();
  assert(result.pctFR == 50.0f && result.pctVN == 50.0f);
  assert(result.activeLanes == 3 && result.blockedLane == -1);

  trivia.begin(0, 0, 3, 0xFFFFFF00u);
  assert(trivia.finish(0x00001E40u, TEAM_VN));
  result = trivia.result();
  assert(result.pctFR == 0.0f && result.pctVN == 0.0f);
  assert(result.activeLanes == 3);
  assert(!trivia.recordAnswer(TEAM_VN, 0, 3, 0x00001E41u));

  assert(trivia.begin(1, 1, 1, 20000));
  assert(trivia.recordAnswer(TEAM_FR, 0, 1, 20001));
  assert(trivia.finish(28000, TEAM_VN));
  result = trivia.result();
  assert(result.activeLanes == 2 && result.blockedLane == 1);

  assert(trivia.begin(64, 1, 0, 30000));
  assert(trivia.recordAnswer(TEAM_FR, 63, 0, 30001));
  assert(!trivia.begin(65, 1, 0, 30002));
  assert(trivia.finish(38000, TEAM_VN));
  result = trivia.result();
  assert(result.pctFR > 1.56f && result.pctFR < 1.57f);
}
