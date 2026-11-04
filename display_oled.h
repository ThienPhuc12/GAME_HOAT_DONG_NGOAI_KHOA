#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "config.h"

// U8g2's 128x64 1-bit framebuffer is exactly 1024 bytes.
class PenaltyOled {
 public:
  PenaltyOled()
#if defined(USE_SSD1306)
      : display_(U8G2_R0, U8X8_PIN_NONE)
#else
      : display_(U8G2_R0, U8X8_PIN_NONE)
#endif
  {}

  void begin() {
    Wire.begin(PIN_OLED_SDA, PIN_OLED_SCL);
    display_.setI2CAddress(OLED_ADDRESS << 1);
    display_.begin();
    display_.setContrast(255);
  }

  void render(GameState state, uint8_t scoreFR, uint8_t scoreVN,
              uint8_t shootingTeam, uint32_t now) {
    display_.clearBuffer();
    display_.setDrawColor(1);
    display_.setFont(u8g2_font_6x10_tf);
    display_.drawStr(3, 9, "PHAP");
    display_.drawStr(77, 9, "VIET NAM");
    // Tiny trophy in the center of the team labels.
    display_.drawFrame(59, 1, 10, 6);
    display_.drawLine(61, 7, 66, 7);
    display_.drawLine(63, 7, 63, 10);
    display_.drawLine(60, 10, 67, 10);

    char scoreText[12];
    snprintf(scoreText, sizeof(scoreText), "%u-%u", scoreFR, scoreVN);
    display_.setFont(u8g2_font_logisoso38_tn);
    int16_t scoreWidth = display_.getStrWidth(scoreText);
    display_.drawStr((128 - scoreWidth) / 2, 50, scoreText);

    display_.setFont(u8g2_font_6x10_tf);
    if (state == STATE_VICTORY_CEREMONY) {
      if ((now / 500) & 1)
        display_.drawStr(scoreFR >= GOALS_TO_WIN ? 24 : 20, 63,
                         scoreFR >= GOALS_TO_WIN ? "PHAP VO DICH" : "VN VO DICH");
      else
        display_.drawStr(28, 63, "CHUC MUNG!");
    } else if (state == STATE_MONUMENT_BUILD) {
      char progress[16] = "[.....]-[.....]";
      for (uint8_t i = 0; i < scoreFR && i < 5; ++i) progress[1 + i] = '*';
      for (uint8_t i = 0; i < scoreVN && i < 5; ++i) progress[9 + i] = '*';
      display_.drawStr(10, 63, progress);
    } else if (state == STATE_LOBBY) {
      display_.drawStr(30, 63, "CHO BAT DAU");
    } else {
      display_.drawStr(4, 63, shootingTeam == 0 ? "PHAP SUT | VN BAT" : "VN SUT | PHAP BAT");
    }
    display_.sendBuffer();
  }

 private:
#if defined(USE_SSD1306)
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C display_;
#else
  U8G2_SH1106_128X64_NONAME_F_HW_I2C display_;
#endif
};
