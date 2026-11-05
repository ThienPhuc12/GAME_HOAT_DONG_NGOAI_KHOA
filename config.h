#pragma once
#include <stdint.h>

static constexpr int PIN_OLED_SDA = 21;
static constexpr int PIN_OLED_SCL = 22;

static constexpr uint32_t TUTORIAL_SLIDE_MS = 10000;
static constexpr uint8_t TUTORIAL_SLIDE_COUNT = 3;
static constexpr uint32_t COIN_TOSS_MS = 3500;
static constexpr uint32_t COUNTDOWN_MS = 3000;
static constexpr uint32_t TRIVIA_MS = 10000;
static constexpr uint32_t CHAR_SELECT_MS = 8000;
static constexpr uint32_t AIM_MS = 7000;
static constexpr uint32_t SHOT_ANIM_MS = 3500;
static constexpr uint32_t BUILD_ANIM_MS = 3000;
static constexpr uint8_t GOALS_TO_WIN = 5;
static constexpr uint8_t MAX_CLIENTS = 12;

static constexpr char AP_SSID[] = "VIET-FRANCE-PENALTY";
static constexpr uint8_t OLED_ADDRESS = 0x3C;

enum GameState : uint8_t {
  STATE_LOBBY, STATE_TUTORIAL_SLIDES, STATE_COIN_TOSS, STATE_COUNTDOWN,
  STATE_TRIVIA_10S, STATE_CHAR_SELECT_8S, STATE_AIM_STANDOFF,
  STATE_PENALTY_ANIM, STATE_MONUMENT_BUILD, STATE_VICTORY_CEREMONY
};
