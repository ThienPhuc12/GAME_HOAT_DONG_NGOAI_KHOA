#pragma once
#include <stdint.h>

static constexpr int PIN_TFT_SCLK = 18;
static constexpr int PIN_TFT_MOSI = 23;
static constexpr int PIN_TFT_CS = 5;
static constexpr int PIN_TFT_DC = 2;
static constexpr int PIN_TFT_RST = 4;
static constexpr int PIN_TFT_BL = 15;
static constexpr int PIN_OLED_SDA = 21;
static constexpr int PIN_OLED_SCL = 22;

static constexpr uint32_t COIN_TOSS_MS = 3500;
static constexpr uint32_t COUNTDOWN_MS = 3000;
static constexpr uint32_t TRIVIA_MS = 5000;
static constexpr uint32_t CHAR_SELECT_MS = 3000;
static constexpr uint32_t AIM_MS = 8000;
static constexpr uint32_t SHOT_ANIM_MS = 3500;
static constexpr uint32_t BUILD_ANIM_MS = 3000;
static constexpr uint8_t GOALS_TO_WIN = 5;
static constexpr uint8_t MAX_CLIENTS = 12;

static constexpr char AP_SSID[] = "VIET-FRANCE-PENALTY";
// Change before public use. The host page requires this PIN for START.
static constexpr char HOST_PIN[] = "8426";
static constexpr uint8_t OLED_ADDRESS = 0x3C;

enum GameState : uint8_t {
  STATE_LOBBY, STATE_COIN_TOSS, STATE_COUNTDOWN, STATE_TRIVIA_5S,
  STATE_CHAR_SELECT_3S, STATE_AIM_AND_SHOOT, STATE_PENALTY_ANIM,
  STATE_MONUMENT_BUILD, STATE_VICTORY_CEREMONY
};
