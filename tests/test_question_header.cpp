#include <cassert>
#include <cstring>
#include "../questions.h"

int main() {
  assert(QUESTION_COUNT == 20);
  Question q{};
  assert(loadQuestion(0, q));
  assert(q.id == 1);
  assert(std::strcmp(q.q_vi, "Thủ đô của Việt Nam là gì?") == 0);
  assert(std::strcmp(q.q_fr, "Quelle est la capitale du Viêt Nam ?") == 0);
  assert(q.correctIdx == 1);
  assert(std::strcmp(q.optB, "Hà Nội") == 0);
  assert(loadQuestion(19, q) && q.id == 20 && q.correctIdx == 0);
  assert(!loadQuestion(20, q));
}
