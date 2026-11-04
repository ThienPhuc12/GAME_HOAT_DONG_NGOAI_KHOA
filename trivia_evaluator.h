#pragma once

#include <stdint.h>

#if defined(ARDUINO_ARCH_ESP32)
#include <esp_system.h>
#else
#include <cstdlib>
#endif

enum Team : uint8_t { TEAM_FR = 0, TEAM_VN = 1 };

struct TriviaResult {
  float pctFR = 0.0f;
  float pctVN = 0.0f;
  uint8_t activeLanes = 3;
  int8_t blockedLane = -1;

  bool isLaneActive(uint8_t lane) const {
    return lane < 3 && lane != blockedLane;
  }
};

// Team-local member slots are supplied by the lobby's stable member registry.
// Capture the total member counts at the start of each round; unanswered
// members stay in the denominator. One answer per slot, including spectators.
class TriviaEvaluator {
 public:
  static constexpr uint32_t DURATION_MS = 5000;
  static constexpr uint8_t MAX_MEMBERS_PER_TEAM = 64;
  using LaneRng = uint8_t (*)();

  explicit TriviaEvaluator(LaneRng rng = defaultLane) : rng_(rng ? rng : defaultLane) {}

  // Returns false if a team exceeds the fixed capacity; the old round is then
  // left intact. Callers should reject excess lobby joins before begin().
  bool begin(uint8_t membersFR, uint8_t membersVN, uint8_t correctIdx, uint32_t now) {
    if (membersFR > MAX_MEMBERS_PER_TEAM || membersVN > MAX_MEMBERS_PER_TEAM || correctIdx > 3)
      return false;
    total_[0] = membersFR;
    total_[1] = membersVN;
    correct_[0] = correct_[1] = 0;
    answered_[0] = answered_[1] = 0;
    correctIdx_ = correctIdx;
    startMs_ = now;
    active_ = true;
    finished_ = false;
    result_ = TriviaResult{};
    return true;
  }

  // The first valid answer counts. Unsigned subtraction handles millis wrap.
  bool recordAnswer(Team team, uint8_t memberSlot, uint8_t answerIdx, uint32_t now) {
    const uint8_t t = static_cast<uint8_t>(team);
    if (!active_ || finished_ || t > 1 || answerIdx > 3 ||
        static_cast<uint32_t>(now - startMs_) >= DURATION_MS || memberSlot >= total_[t])
      return false;
    const uint64_t bit = uint64_t{1} << memberSlot;
    if (answered_[t] & bit) return false;
    answered_[t] |= bit;
    if (answerIdx == correctIdx_) ++correct_[t];
    return true;
  }

  // Finalizes once, after exactly five seconds. The shooting team wins ties.
  bool finish(uint32_t now, Team shootingTeam) {
    if (!active_ || finished_ || static_cast<uint8_t>(shootingTeam) > 1 ||
        static_cast<uint32_t>(now - startMs_) < DURATION_MS)
      return false;
    result_.pctFR = total_[0] ? float(correct_[0]) * 100.0f / float(total_[0]) : 0.0f;
    result_.pctVN = total_[1] ? float(correct_[1]) * 100.0f / float(total_[1]) : 0.0f;
    const float pct[2] = {result_.pctFR, result_.pctVN};
    const uint8_t shooter = static_cast<uint8_t>(shootingTeam);
    if (pct[1 - shooter] > pct[shooter]) {
      result_.activeLanes = 2;
      result_.blockedLane = static_cast<int8_t>(rng_() % 3);
    }
    finished_ = true;
    return true;
  }

  TriviaResult result() const { return result_; }
  bool finished() const { return finished_; }
  uint8_t correctCount(Team team) const { return correct_[static_cast<uint8_t>(team)]; }
  uint8_t answeredCount(Team team) const {
    return static_cast<uint8_t>(__builtin_popcountll(answered_[static_cast<uint8_t>(team)]));
  }

 private:
  static uint8_t defaultLane() {
#if defined(ARDUINO_ARCH_ESP32)
    return static_cast<uint8_t>(esp_random() % 3);
#else
    return static_cast<uint8_t>(std::rand() % 3);
#endif
  }

  LaneRng rng_;
  uint64_t answered_[2] = {0, 0};
  uint32_t startMs_ = 0;
  uint8_t total_[2] = {0, 0};
  uint8_t correct_[2] = {0, 0};
  uint8_t correctIdx_ = 0;
  bool active_ = false;
  bool finished_ = false;
  TriviaResult result_{};
};
