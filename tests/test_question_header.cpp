#include <cassert>
#include <cstring>
#include "../questions.h"

int main() {
  assert(QUESTION_COUNT == 50);
  Question q{};
  assert(loadQuestion(0, q));
  assert(q.id == 1);
  assert(std::strstr(q.q_vi, "biệt danh phổ biến") != nullptr);
  assert(std::strstr(q.q_fr, "surnom populaire") != nullptr);
  assert(q.correctIdx == 1);
  assert(std::strstr(q.optB, "Les Guerriers de l'Étoile d'Or | Những chiến binh Sao Vàng") != nullptr);
  assert(loadQuestion(49, q) && q.id == 50 && q.correctIdx == 1);
  assert(!loadQuestion(50, q));
}
