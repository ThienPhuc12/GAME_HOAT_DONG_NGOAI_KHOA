#pragma once

#include <stdint.h>
#include "config.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <esp_system.h>
#else
#include <cstdlib>
#endif

enum Team : uint8_t { TEAM_FR = 0, TEAM_VN = 1 };

struct TriviaResult {
  float pctFR = 0.0f;
  float pctVN = 0.0f;
  uint8_t activeZones = 6;
  int8_t blockedZones[2] = {-1, -1};

  bool isZoneActive(uint8_t zone) const {
    return zone < 6 && zone != blockedZones[0] && zone != blockedZones[1];
  }
};

// Team-local member slots are supplied by the lobby's stable member registry.
// Capture the total member counts at the start of each round; unanswered
// members stay in the denominator. One answer per slot, including spectators.
class TriviaEvaluator {
 public:
  static constexpr uint32_t DURATION_MS = TRIVIA_MS;
  static constexpr uint8_t MAX_MEMBERS_PER_TEAM = 64;
  using ZoneRng = uint8_t (*)();

  explicit TriviaEvaluator(ZoneRng rng = defaultZone) : rng_(rng ? rng : defaultZone) {}

  // One mutable answer per member; scoring happens only at the deadline.
  bool begin(uint8_t membersFR, uint8_t membersVN, uint8_t correctIdx,
             uint32_t now, uint32_t durationMs = TRIVIA_MS) {
    if (membersFR > MAX_MEMBERS_PER_TEAM || membersVN > MAX_MEMBERS_PER_TEAM ||
        correctIdx > 3 || durationMs < 1000)
      return false;
    total_[0] = membersFR;
    total_[1] = membersVN;
    correct_[0] = correct_[1] = 0;
    answered_[0] = answered_[1] = 0;
    correctIdx_ = correctIdx;
    startMs_ = now;
    durationMs_ = durationMs;
    active_ = true;
    finished_ = false;
    result_ = TriviaResult{};
    memsetChoices();
    return true;
  }

  // Every valid tap replaces the previous choice until the deadline.
  bool recordChoice(Team team, uint8_t memberSlot, uint8_t answerIdx, uint32_t now) {
    const uint8_t t = static_cast<uint8_t>(team);
    if (!active_ || finished_ || t > 1 || answerIdx > 3 ||
        static_cast<uint32_t>(now - startMs_) >= durationMs_ || memberSlot >= total_[t])
      return false;
    const uint64_t bit = uint64_t{1} << memberSlot;
    answered_[t] |= bit;
    choices_[t][memberSlot] = answerIdx;
    choiceElapsedMs_[t][memberSlot] = static_cast<uint32_t>(now - startMs_);
    return true;
  }

  // Compatibility for existing callers; identical mutable behavior.
  bool recordAnswer(Team team, uint8_t slot, uint8_t answer, uint32_t now) {
    return recordChoice(team, slot, answer, now);
  }

  // Finalizes once, after the configured duration. The shooting team wins ties.
  bool finish(uint32_t now, Team shootingTeam) {
    if (!active_ || finished_ || static_cast<uint8_t>(shootingTeam) > 1 ||
        static_cast<uint32_t>(now - startMs_) < durationMs_)
      return false;
    for (uint8_t t = 0; t < 2; ++t)
      for (uint8_t slot = 0; slot < total_[t]; ++slot)
        if (choices_[t][slot] == correctIdx_) ++correct_[t];
    result_.pctFR = total_[0] ? float(correct_[0]) * 100.0f / float(total_[0]) : 0.0f;
    result_.pctVN = total_[1] ? float(correct_[1]) * 100.0f / float(total_[1]) : 0.0f;
    const float pct[2] = {result_.pctFR, result_.pctVN};
    const uint8_t shooter = static_cast<uint8_t>(shootingTeam);
    if (pct[1 - shooter] > pct[shooter]) {
      const uint8_t first = static_cast<uint8_t>(rng_() % 6);
      const uint8_t offset = static_cast<uint8_t>(1 + rng_() % 5);
      result_.activeZones = 4;
      result_.blockedZones[0] = static_cast<int8_t>(first);
      result_.blockedZones[1] = static_cast<int8_t>((first + offset) % 6);
    }
    finished_ = true;
    return true;
  }

  TriviaResult result() const { return result_; }
  bool finished() const { return finished_; }
  bool choiceCorrect(Team team, uint8_t memberSlot) const {
    const uint8_t t = static_cast<uint8_t>(team);
    return finished_ && t < 2 && memberSlot < total_[t] &&
           choices_[t][memberSlot] == correctIdx_;
  }
  uint32_t choiceTimeMs(Team team, uint8_t memberSlot) const {
    const uint8_t t = static_cast<uint8_t>(team);
    return finished_ && t < 2 && memberSlot < total_[t]
        ? choiceElapsedMs_[t][memberSlot] : 0;
  }
  uint8_t correctCount(Team team) const { return correct_[static_cast<uint8_t>(team)]; }
  uint8_t totalCount(Team team) const {
    const uint8_t t = static_cast<uint8_t>(team);
    return t < 2 ? total_[t] : 0;
  }
  uint8_t choiceFor(Team team, uint8_t memberSlot) const {
    const uint8_t t = static_cast<uint8_t>(team);
    return t < 2 && memberSlot < total_[t] ? choices_[t][memberSlot] : 255;
  }
  uint8_t answeredCount(Team team) const {
    return static_cast<uint8_t>(__builtin_popcountll(answered_[static_cast<uint8_t>(team)]));
  }

 private:
  static uint8_t defaultZone() {
#if defined(ARDUINO_ARCH_ESP32)
    return static_cast<uint8_t>(esp_random());
#else
    return static_cast<uint8_t>(std::rand());
#endif
  }

  ZoneRng rng_;
  uint64_t answered_[2] = {0, 0};
  uint8_t choices_[2][MAX_MEMBERS_PER_TEAM]{};
  uint32_t choiceElapsedMs_[2][MAX_MEMBERS_PER_TEAM]{};
  uint32_t startMs_ = 0;
  uint32_t durationMs_ = TRIVIA_MS;
  uint8_t total_[2] = {0, 0};
  uint8_t correct_[2] = {0, 0};
  uint8_t correctIdx_ = 0;
  bool active_ = false;
  bool finished_ = false;
  TriviaResult result_{};

  void memsetChoices() {
    for (uint8_t team = 0; team < 2; ++team)
      for (uint8_t slot = 0; slot < MAX_MEMBERS_PER_TEAM; ++slot) {
        choices_[team][slot] = 255;
        choiceElapsedMs_[team][slot] = 0;
      }
  }
};
