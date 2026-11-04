#pragma once
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <algorithm>
#include <math.h>
#include <pgmspace.h>
#include "config.h"
#include "questions.h"
#include "player_manager.h"
#include "pixel_assets.h"
#include "tft_font_vi.h"

struct TftFrame {
  GameState state = STATE_LOBBY;
  uint32_t nowMs = 0;
  uint32_t elapsedMs = 0;
  Team shootingTeam = TEAM_FR;
  uint8_t scoreFR = 0, scoreVN = 0;
  uint8_t membersFR = 0, membersVN = 0;
  uint8_t answeredFR = 0, answeredVN = 0;
  float pctFR = 0, pctVN = 0;
  uint8_t questionId = 0;
  const Question* question = nullptr;
  const Player* roster = nullptr;
  int8_t blockedLane = -1;
  uint8_t selectedFR = 0, selectedVN = 0;
  int8_t shootDir = -1, saveDir = -1;
  bool goal = false;
};

class PenaltyTft {
  class Device : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01 panel_;
    lgfx::Bus_SPI bus_;
   public:
    Device() {
      auto b = bus_.config();
      b.spi_host = SPI3_HOST;
      b.spi_mode = 0;
      b.freq_write = 10000000;
      b.spi_3wire = false;
      b.dma_channel = 0;
      b.pin_sclk = PIN_TFT_SCLK;
      b.pin_mosi = PIN_TFT_MOSI;
      b.pin_miso = -1;
      b.pin_dc = PIN_TFT_DC;
      bus_.config(b);
      panel_.setBus(&bus_);
      auto p = panel_.config();
      p.pin_cs = PIN_TFT_CS;
      p.pin_rst = PIN_TFT_RST;
      p.pin_busy = -1;
      p.panel_width = 240;
      p.panel_height = 240;
      p.readable = false;
      p.invert = true;
      panel_.config(p);
      setPanel(&panel_);
    }
  };

 public:
  PenaltyTft() : strip_(&device_) {}

  bool begin() {
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, HIGH);
    if (!device_.init()) return false;
    device_.setRotation(0);
    strip_.setColorDepth(16);
    ready_ = strip_.createSprite(240, STRIP_H) != nullptr;
    if (ready_) device_.fillScreen(TFT_BLACK);
    return ready_;
  }

  void setDiagnostic(bool enabled) {
    diagnostic_ = enabled;
    if (!ready_ || !enabled) return;
    device_.waitDMA();
    device_.fillScreen(TFT_BLACK);
    device_.fillRect(0, 0, 120, 120, TFT_RED);
    device_.fillRect(120, 0, 120, 120, TFT_GREEN);
    device_.fillRect(0, 120, 120, 120, TFT_BLUE);
    device_.fillRect(120, 120, 120, 120, TFT_YELLOW);
    device_.drawRect(10, 10, 220, 220, TFT_WHITE);
    device_.setTextColor(TFT_WHITE, TFT_BLACK);
    device_.drawString("TFT SPI TEST", 65, 112);
  }

  void render(const TftFrame& frame) {
    if (!ready_ || diagnostic_) return;
    frame_ = &frame;
    for (uint8_t band = 0; band < 6; ++band) {
      y0_ = band * STRIP_H;
      shakeX_ = frame.state == STATE_PENALTY_ANIM &&
                frame.elapsedMs >= SHOT_CONTACT_MS && frame.elapsedMs < SHOT_CONTACT_MS + 180
                  ? ((frame.elapsedMs / 35) & 1 ? 2 : -2) : 0;
      strip_.fillSprite(TFT_BLACK);
      switch (frame.state) {
        case STATE_LOBBY: drawLobbySplit(); break;
        case STATE_COIN_TOSS: drawCoinTossAnim(); break;
        case STATE_COUNTDOWN: drawCountdown(); break;
        case STATE_TRIVIA_5S:
        case STATE_CHAR_SELECT_3S: drawTriviaSplit(); break;
        case STATE_AIM_AND_SHOOT:
        case STATE_PENALTY_ANIM: drawPenaltyShootout(); break;
        case STATE_MONUMENT_BUILD: drawMonumentAssembly(); break;
        case STATE_VICTORY_CEREMONY: drawVictoryScreen(); break;
      }
      // Physical panel is round. The safe text regions are inside radius 112.
      strip_.pushSprite(0, y0_);
      device_.waitDMA(); // The next band reuses this same 19.2 KB buffer.
    }
  }

  static constexpr size_t STRIP_BYTES = 240 * 40 * 2;

 private:
  static constexpr int STRIP_H = 40;
  Device device_;
  LGFX_Sprite strip_;
  const TftFrame* frame_ = nullptr;
  int16_t y0_ = 0, shakeX_ = 0;
  bool ready_ = false;
  bool diagnostic_ = false;

  void rect(int x, int y, int w, int h, uint16_t color) {
    strip_.fillRect(x + shakeX_, y - y0_, w, h, color);
  }
  void line(int x1, int y1, int x2, int y2, uint16_t color) {
    strip_.drawLine(x1 + shakeX_, y1 - y0_, x2 + shakeX_, y2 - y0_, color);
  }
  void circle(int x, int y, int r, uint16_t color) {
    strip_.drawCircle(x + shakeX_, y - y0_, r, color);
  }
  void text(const char* value, int x, int y, uint16_t color, uint8_t scale = 1) {
    strip_.setTextDatum(textdatum_t::middle_center);
    strip_.setTextSize(scale);
    strip_.setTextColor(color);
    strip_.drawString(value, x + shakeX_, y - y0_);
  }
  void star(int x, int y, int r, uint16_t color) {
    for (int i = 0; i < 5; ++i) {
      float a = -1.5707963f + i * 1.2566371f;
      int px = x + int(cosf(a) * r), py = y + int(sinf(a) * r);
      line(x, y, px, py, color);
      rect(px - 1, py - 1, 3, 3, color);
    }
  }
  void crown(int x, int y, uint16_t color) {
    rect(x - 8, y + 4, 16, 4, color);
    rect(x - 8, y, 3, 5, color);
    rect(x - 1, y - 3, 3, 8, color);
    rect(x + 6, y, 3, 5, color);
  }

  // Maps Vietnamese UTF-8 to legible ASCII for the small built-in TFT font.
  static char latin(uint32_t cp) {
    if (cp < 128) return static_cast<char>(cp);
    if (cp >= 0x1EA0 && cp <= 0x1EB7) return cp & 1 ? 'a' : 'A';
    if (cp >= 0x1EB8 && cp <= 0x1EC7) return cp & 1 ? 'e' : 'E';
    if (cp >= 0x1ECC && cp <= 0x1EE3) return cp & 1 ? 'o' : 'O';
    if (cp >= 0x1EE4 && cp <= 0x1EF1) return cp & 1 ? 'u' : 'U';
    if (cp >= 0x1EF2 && cp <= 0x1EF9) return cp & 1 ? 'y' : 'Y';
    switch (cp) {
      case 0x00C0: case 0x00C1: case 0x00C2: case 0x00C3: case 0x0102: return 'A';
      case 0x00E0: case 0x00E1: case 0x00E2: case 0x00E3: case 0x0103: return 'a';
      case 0x00C8: case 0x00C9: case 0x00CA: return 'E';
      case 0x00E8: case 0x00E9: case 0x00EA: return 'e';
      case 0x00CC: case 0x00CD: case 0x0128: case 0x1EC8: case 0x1ECA: return 'I';
      case 0x00EC: case 0x00ED: case 0x0129: case 0x1EC9: case 0x1ECB: return 'i';
      case 0x00D2: case 0x00D3: case 0x00D4: case 0x00D5: case 0x01A0: return 'O';
      case 0x00F2: case 0x00F3: case 0x00F4: case 0x00F5: case 0x01A1: return 'o';
      case 0x00D9: case 0x00DA: case 0x0168: case 0x01AF: return 'U';
      case 0x00F9: case 0x00FA: case 0x0169: case 0x01B0: return 'u';
      case 0x00DD: return 'Y';
      case 0x00FD: return 'y';
      case 0x0110: return 'D';
      case 0x0111: return 'd';
      case 0x2013: case 0x2014: return '-';
      default: return '?';
    }
  }
  static void ascii(const char* src, char* dst, size_t capacity) {
    if (!capacity) return;
    size_t out = 0;
    while (*src && out + 1 < capacity) {
      uint8_t a = static_cast<uint8_t>(*src++);
      uint32_t cp = a;
      if ((a & 0xE0) == 0xC0 && *src) {
        cp = (a & 0x1F) << 6;
        cp |= static_cast<uint8_t>(*src++) & 0x3F;
      } else if ((a & 0xF0) == 0xE0 && src[0] && src[1]) {
        cp = (a & 0x0F) << 12;
        cp |= (static_cast<uint8_t>(*src++) & 0x3F) << 6;
        cp |= static_cast<uint8_t>(*src++) & 0x3F;
      }
      dst[out++] = latin(cp);
    }
    dst[out] = 0;
  }
  void shortName(const char* src, int x, int y, uint16_t color) {
    char name[14];
    ascii(src, name, sizeof(name));
    name[11] = 0;
    text(name, x, y, color);
  }
  void wrappedQuestion(const char* source) {
    if (!source) return;
    char flat[270];
    ascii(source, flat, sizeof(flat));
    int row = 0;
    char* p = flat;
    while (*p && row < 5) {
      while (*p == ' ') ++p;
      if (!*p) break;
      char lineBuf[30] = {};
      int len = 0;
      while (*p && len < 25) {
        char* word = p;
        while (*p && *p != ' ') ++p;
        int wordLen = p - word;
        if (len && len + 1 + wordLen > 25) { p = word; break; }
        if (len) lineBuf[len++] = ' ';
        if (wordLen > 25 - len) wordLen = 25 - len;
        memcpy(lineBuf + len, word, wordLen);
        len += wordLen;
        while (*p == ' ') ++p;
      }
      lineBuf[len] = 0;
      text(lineBuf, 120, 43 + row * 13, TFT_WHITE);
      ++row;
    }
  }

  static uint32_t utf8Next(const char*& p) {
    uint8_t a = static_cast<uint8_t>(*p++);
    if ((a & 0xE0) == 0xC0 && *p)
      return ((a & 0x1F) << 6) | (static_cast<uint8_t>(*p++) & 0x3F);
    if ((a & 0xF0) == 0xE0 && p[0] && p[1]) {
      uint32_t cp = (a & 0x0F) << 12;
      cp |= (static_cast<uint8_t>(*p++) & 0x3F) << 6;
      cp |= static_cast<uint8_t>(*p++) & 0x3F;
      return cp;
    }
    return a;
  }
  static const TftGlyph* glyph(uint32_t cp) {
    int lo = 0, hi = TFT_FONT_GLYPH_COUNT - 1;
    while (lo <= hi) {
      int mid = (lo + hi) / 2;
      uint16_t found = pgm_read_word(&TFT_FONT_GLYPHS[mid].codepoint);
      if (found == cp) return &TFT_FONT_GLYPHS[mid];
      if (found < cp) lo = mid + 1;
      else hi = mid - 1;
    }
    return glyph('?');
  }
  static int viWidth(const char* value) {
    int width = 0;
    const char* p = value;
    while (*p) {
      const TftGlyph* g = glyph(utf8Next(p));
      width += pgm_read_byte(&g->advance);
    }
    return width;
  }
  void viText(const char* value, int x, int topY, uint16_t color) {
    const char* p = value;
    while (*p) {
      const TftGlyph* g = glyph(utf8Next(p));
      for (int row = 0; row < 16; ++row) {
        int screenY = topY + row;
        if (screenY < y0_ || screenY >= y0_ + STRIP_H) continue;
        uint16_t bits = pgm_read_word(&g->rows[row]);
        for (int col = 0; col < 14; ++col)
          if (bits & (1u << (13 - col)))
            strip_.drawPixel(x + shakeX_ + col, screenY - y0_, color);
      }
      x += pgm_read_byte(&g->advance);
    }
  }
  void viCentered(const char* value, int centerX, int topY, uint16_t color) {
    viText(value, centerX - viWidth(value) / 2, topY, color);
  }
  void viShortName(const char* value, int centerX, int topY, uint16_t color) {
    char buffer[48];
    char* end = buffer;
    const char* p = value;
    int width = 0;
    while (*p && static_cast<size_t>(end - buffer) < sizeof(buffer) - 4) {
      const char* start = p;
      const TftGlyph* g = glyph(utf8Next(p));
      int advance = pgm_read_byte(&g->advance);
      if (width + advance > 76) break;
      while (start < p) *end++ = *start++;
      width += advance;
    }
    *end = 0;
    viCentered(buffer, centerX, topY, color);
  }
  void viWrappedQuestion(const char* value) {
    const char* p = value;
    for (int row = 0; row < 5 && *p; ++row) {
      while (*p == ' ') ++p;
      const char* start = p;
      const char* cursor = p;
      const char* lastSpace = nullptr;
      int width = 0;
      while (*cursor) {
        const char* before = cursor;
        uint32_t cp = utf8Next(cursor);
        if (cp == ' ') lastSpace = before;
        width += pgm_read_byte(&glyph(cp)->advance);
        if (width > 184) {
          const char* after = cursor;
          cursor = lastSpace && lastSpace > start ? lastSpace : before;
          if (cursor == start) cursor = after;
          break;
        }
      }
      char lineBuf[260];
      size_t length = std::min<size_t>(cursor - start, sizeof(lineBuf) - 1);
      memcpy(lineBuf, start, length);
      lineBuf[length] = 0;
      viCentered(lineBuf, 120, 35 + row * 16, TFT_WHITE);
      p = cursor;
    }
  }

  void eiffelPart(int cx, int baseY, int part, uint16_t color, int dy = 0) {
    int y = baseY - part * 17 + dy;
    int half = 33 - part * 6;
    rect(cx - half, y - 3, half * 2, 4, color);
    rect(cx - half + 4, y - 16, 4, 14, color);
    rect(cx + half - 8, y - 16, 4, 14, color);
    line(cx - half + 6, y - 15, cx, y - 2, color);
    line(cx + half - 6, y - 15, cx, y - 2, color);
  }
  void eiffel(int cx, int baseY, int parts, uint16_t color) {
    for (int i = 0; i < parts && i < 5; ++i) eiffelPart(cx, baseY, i, color);
    if (parts >= 5) rect(cx - 2, baseY - 91, 4, 12, color);
  }
  void dragonPart(int left, int baseY, int part, uint16_t color, int dy = 0) {
    int x = left + part * 31;
    rect(x, baseY - 5 + dy, 28, 5, color);
    line(x + 2, baseY - 7 + dy, x + 15, baseY - 22 - (part & 1) * 6 + dy, color);
    line(x + 15, baseY - 22 - (part & 1) * 6 + dy, x + 28, baseY - 8 + dy, color);
    rect(x + 12, baseY - 23 - (part & 1) * 6 + dy, 6, 5, color);
    if (part == 4) { rect(x + 27, baseY - 24 + dy, 12, 7, color); rect(x + 35, baseY - 27 + dy, 4, 3, TFT_YELLOW); }
  }
  void dragon(int left, int baseY, int parts, uint16_t color) {
    for (int i = 0; i < parts && i < 5; ++i) dragonPart(left, baseY, i, color);
  }

  void drawLobbySplit() {
    rect(0, 0, 120, 240, 0x09AA);
    rect(120, 0, 120, 240, 0x6804);
    line(120, 13, 120, 226, TFT_GOLD);
    eiffel(60, 132, 5, 0x5B3B);
    dragon(129, 133, 5, 0xB9E0);
    viCentered("PHÁP", 60, 24, TFT_WHITE);
    viCentered("VIỆT NAM", 180, 24, TFT_WHITE);
    star(60, 56, 7, TFT_YELLOW);
    star(180, 56, 7, TFT_YELLOW);
    if (!frame_->roster) return;
    for (int team = 0; team < 2; ++team) {
      int cx = team ? 180 : 60;
      uint8_t shown = 0;
      int skip = (frame_->nowMs / 2200) % 4;
      const Player* main = nullptr;
      const Player* spectators[MAX_CLIENTS]{};
      uint8_t spectatorCount = 0;
      for (int i = 0; i < MAX_CLIENTS; ++i) {
        const Player& p = frame_->roster[i];
        if (!p.used || p.team != team) continue;
        if (p.role == ROLE_MAIN_PLAYER) main = &p;
        else spectators[spectatorCount++] = &p;
      }
      if (main) { crown(cx, 140, TFT_YELLOW); viShortName(main->name, cx, 153, TFT_WHITE); }
      for (uint8_t row = 0; row < 2 && row < spectatorCount; ++row) {
        uint8_t index = (skip + row) % spectatorCount;
        viShortName(spectators[index]->name, cx, 174 + row * 16, 0xD6BA);
        ++shown;
      }
      char count[24];
      snprintf(count, sizeof(count), "%u người", team ? frame_->membersVN : frame_->membersFR);
      viCentered(count, cx, 202, TFT_WHITE);
      (void)shown;
    }
  }

  void drawCoinTossAnim() {
    rect(0, 0, 240, 240, 0x0862);
    uint32_t ms = frame_->elapsedMs;
    bool settled = ms >= COIN_TOSS_MS;
    int width = settled ? 62 : 5 + int(fabsf(cosf(ms * 0.015f)) * 57);
    int height = 61;
    strip_.fillEllipse(120 + shakeX_, 115 - y0_, width + 5, height + 5, 0x7A40);
    strip_.fillEllipse(120 + shakeX_, 115 - y0_, width, height, TFT_GOLD);
    strip_.drawEllipse(120 + shakeX_, 115 - y0_, width - 3, height - 3, TFT_WHITE);
    Team face = settled ? frame_->shootingTeam : static_cast<Team>((ms / 120) & 1);
    if (width > 26) {
      if (face == TEAM_FR) {
        rect(120 - width / 2 + 13, 91, (width - 26) / 3, 48, TFT_BLUE);
        rect(120 - width / 2 + 13 + (width - 26) / 3, 91, (width - 26) / 3, 48, TFT_WHITE);
        rect(120 - width / 2 + 13 + 2 * (width - 26) / 3, 91, (width - 26) / 3, 48, TFT_RED);
      } else {
        rect(120 - width / 2 + 13, 91, width - 26, 48, TFT_RED);
        star(120, 115, 11, TFT_YELLOW);
      }
    }
    viCentered(settled ? (face == TEAM_FR ? "PHÁP ĐÁ TRƯỚC!" : "VIỆT NAM ĐÁ TRƯỚC!") : "TUNG ĐỒNG XU", 120, 184, TFT_WHITE);
  }

  void drawCountdown() {
    rect(0, 0, 240, 240, 0x0843);
    uint32_t remain = frame_->elapsedMs >= COUNTDOWN_MS ? 0 : COUNTDOWN_MS - frame_->elapsedMs;
    int shown = (remain + 999) / 1000;
    char number[2] = {static_cast<char>('0' + shown), 0};
    if (shown) text(number, 120, 119, TFT_WHITE, 7);
    else viCentered("BẮT ĐẦU!", 120, 108, TFT_WHITE);
    int segments = 36 - (36 * remain / COUNTDOWN_MS);
    for (int i = 0; i < segments; ++i) {
      float a = -1.5707963f + i * 0.1745329f;
      line(120 + int(cosf(a) * 105), 120 + int(sinf(a) * 105),
           120 + int(cosf(a) * 113), 120 + int(sinf(a) * 113), TFT_YELLOW);
    }
  }

  void drawTriviaSplit() {
    rect(0, 0, 240, 116, 0x0822);
    rect(0, 116, 120, 124, 0x09AA);
    rect(120, 116, 120, 124, 0x6804);
    line(120, 118, 120, 218, TFT_GOLD);
    char title[28];
    snprintf(title, sizeof(title), "CÂU HỎI #%u", frame_->questionId);
    viCentered(title, 120, 8, TFT_YELLOW);
    if (frame_->state == STATE_TRIVIA_5S) {
      int width = frame_->elapsedMs >= TRIVIA_MS ? 0 : 174 * (TRIVIA_MS - frame_->elapsedMs) / TRIVIA_MS;
      rect(33, 27, 174, 5, TFT_DARKGREY);
      rect(33, 27, width, 5, TFT_YELLOW);
    }
    if (frame_->question) viWrappedQuestion(frame_->question->q_vi);
    viCentered("PHÁP", 60, 128, TFT_WHITE);
    viCentered("VIỆT NAM", 180, 128, TFT_WHITE);
    char left[28], right[28];
    if (frame_->state == STATE_CHAR_SELECT_3S) {
      snprintf(left, sizeof(left), "%.0f%% ĐÚNG", frame_->pctFR);
      snprintf(right, sizeof(right), "%.0f%% ĐÚNG", frame_->pctVN);
    } else {
      snprintf(left, sizeof(left), "%u/%u ĐÃ CHỌN", frame_->answeredFR, frame_->membersFR);
      snprintf(right, sizeof(right), "%u/%u ĐÃ CHỌN", frame_->answeredVN, frame_->membersVN);
    }
    viCentered(left, 60, 156, TFT_WHITE);
    viCentered(right, 180, 156, TFT_WHITE);
    if (frame_->state == STATE_CHAR_SELECT_3S)
      viCentered(frame_->blockedLane >= 0 ? "KHÓA 1 GÓC - BẮT 50%" : "GIỮ 3 HƯỚNG SÚT", 120, 196, TFT_YELLOW);
    else viCentered("8 GIÂY TRẢ LỜI", 120, 196, TFT_WHITE);
  }

  static uint8_t frameAt(uint32_t elapsedMs, const uint16_t* durations) {
    uint32_t remain = elapsedMs;
    for (uint8_t i = 0; i < PLAYER_FRAME_COUNT - 1; ++i) {
      uint16_t duration = pgm_read_word(&durations[i]);
      if (remain < duration) return i;
      remain -= duration;
    }
    return PLAYER_FRAME_COUNT - 1;
  }
  static uint8_t packedPixel(const uint8_t* frame, int sx, int sy) {
    uint32_t bit = (sy * PLAYER_WIDTH + sx) * 5u;
    uint32_t byte = bit >> 3;
    uint16_t word = uint16_t(pgm_read_byte(frame + byte)) << 8;
    if (byte + 1 < PLAYER_FRAME_BYTES) word |= pgm_read_byte(frame + byte + 1);
    return (word >> (11 - (bit & 7))) & 31;
  }
  void sprite(uint8_t model, uint8_t animationFrame, int x, int y,
              int w, int h, bool flip = false) {
    if (model >= PLAYER_MODEL_COUNT || animationFrame >= PLAYER_FRAME_COUNT) return;
    auto* pixels = static_cast<const uint8_t*>(pgm_read_ptr(&PLAYER_FRAMES[model][animationFrame]));
    auto* palette = static_cast<const uint16_t*>(pgm_read_ptr(&PLAYER_PALETTES[model]));
    for (int py = 0; py < h; ++py) {
      int screenY = y + py;
      if (screenY < y0_ || screenY >= y0_ + STRIP_H) continue;
      int sy = py * PLAYER_HEIGHT / h;
      int runStart = -1;
      uint8_t runColor = 0;
      for (int px = 0; px <= w; ++px) {
        int sx = px < w ? px * PLAYER_WIDTH / w : 0;
        if (flip) sx = PLAYER_WIDTH - 1 - sx;
        uint8_t color = px < w ? packedPixel(pixels, sx, sy) : 0;
        if (color != runColor) {
          if (runColor && runStart >= 0)
            strip_.drawFastHLine(x + shakeX_ + runStart, screenY - y0_, px - runStart,
                                 pgm_read_word(&palette[runColor]));
          runColor = color;
          runStart = color ? px : -1;
        }
      }
    }
  }
  void ball(int x, int y, int radius) {
    strip_.fillCircle(x + shakeX_, y - y0_, radius + 1, 0x2104);
    strip_.fillCircle(x + shakeX_, y - y0_, radius, TFT_WHITE);
    rect(x - 1, y - 1, 3, 3, TFT_DARKGREY);
  }
  static int bezier(int a, int b, int c, int q) {
    int r = 1024 - q;
    return (a * r * r + 2 * b * r * q + c * q * q + 524288) / 1048576;
  }

  void drawPenaltyShootout() {
    const bool france = frame_->shootingTeam == TEAM_FR;
    rect(0, 0, 240, 240, france ? 0x2B3E : 0x345A);
    if (france) {
      rect(0, 119, 240, 121, 0x42A8);
      eiffel(120, 110, 5, 0xB576);
    } else {
      rect(0, 117, 240, 20, 0x3331);
      rect(0, 137, 240, 103, 0x0A8D);
      dragon(42, 128, 5, TFT_ORANGE);
    }
    // Goal frame and net.
    rect(39, 75, 4, 85, TFT_WHITE);
    rect(197, 75, 4, 85, TFT_WHITE);
    rect(39, 75, 162, 4, TFT_WHITE);
    for (int x = 55; x < 198; x += 18) line(x, 79, x, 158, 0xBDF7);
    for (int y = 94; y < 158; y += 16) line(43, y, 197, y, 0xBDF7);
    if (frame_->blockedLane >= 0 && frame_->blockedLane <= 2) {
      int left = 43 + frame_->blockedLane * 52;
      rect(left, 80, 51, 77, 0x3000);
      for (int d = -70; d < 75; d += 13)
        line(left + max(0, d), 80 + max(0, -d), left + min(50, d + 75), 80 + min(76, 75 - d), TFT_RED);
      text("X", left + 25, 117, TFT_RED, 3);
    }
    uint32_t ms = frame_->state == STATE_PENALTY_ANIM ? frame_->elapsedMs : 0;
    Team keeperTeam = static_cast<Team>(1 - frame_->shootingTeam);
    uint8_t keeperModel = keeperTeam == TEAM_FR ? 7 : 4;
    uint8_t shooterModel = france ? (frame_->selectedFR == 1 ? 5 : 1) : (frame_->selectedVN == 1 ? 2 : 0);
    uint8_t shooterFrame = frameAt(ms, SHOT_FRAME_MS);
    uint8_t keeperFrame = frameAt(ms, SAVE_FRAME_MS);
    int keeperShift = 0, keeperLift = 0;
    if (ms >= 390 && frame_->state == STATE_PENALTY_ANIM) {
      int q = std::min<uint32_t>(1024, (ms - 390) * 1024u / 410u);
      keeperShift = (frame_->saveDir == 0 ? -37 : frame_->saveDir == 2 ? 37 : 0) * q / 1024;
      keeperLift = 19 * q * (1024 - q) / 262144;
    }
    sprite(keeperModel, keeperFrame, 104 + keeperShift, 105 - keeperLift, 32, 48,
           (keeperTeam == TEAM_FR ? frame_->selectedFR : frame_->selectedVN) == 1);
    int shooterX = 85 + std::min<uint32_t>(11, ms / 60u);
    sprite(shooterModel, shooterFrame, shooterX, 131, 64, 96);
    if (frame_->state == STATE_PENALTY_ANIM && ms >= SHOT_CONTACT_MS) {
      int q = std::min<uint32_t>(1024, (ms - SHOT_CONTACT_MS) * 1024u / 420u);
      int target = frame_->shootDir == 0 ? 67 : frame_->shootDir == 2 ? 173 : 120;
      int bx = bezier(138, 130, target, q);
      int by = bezier(213, 157, 122, q);
      if (ms < SHOT_CONTACT_MS + 60)
        strip_.fillEllipse(138 + shakeX_, 213 - y0_, 6, 3, TFT_WHITE);
      else if (ms < SHOT_CONTACT_MS + 700) {
        ball(bx, by, 4 - q / 512);
        line(bx - 8, by + 5, bx - 3, by + 2, TFT_WHITE);
      }
    } else ball(138, 213, 4);
    if (ms > 1100 && frame_->state == STATE_PENALTY_ANIM) {
      rect(35, 40, 170, 30, 0x0822);
      viCentered(frame_->goal ? "VÀOOO! / GOAL!" : "CẢN PHÁ! / SAVE!", 120, 45,
                 frame_->goal ? TFT_YELLOW : TFT_WHITE);
    }
    viCentered(france ? "PARIS - THÁP EIFFEL" : "ĐÀ NẴNG - CẦU RỒNG", 120, 18, TFT_WHITE);
  }

  void drawMonumentAssembly() {
    Team team = frame_->shootingTeam;
    uint8_t score = team == TEAM_FR ? frame_->scoreFR : frame_->scoreVN;
    rect(0, 0, 240, 240, team == TEAM_FR ? 0x092C : 0x6005);
    viCentered(team == TEAM_FR ? "THÁP EIFFEL" : "CẦU RỒNG", 120, 22, TFT_WHITE);
    int drop = frame_->goal && frame_->elapsedMs < 950 ? -100 * (950 - frame_->elapsedMs) / 950 : 0;
    int completed = frame_->goal && score ? score - 1 : score;
    if (team == TEAM_FR) {
      eiffel(120, 195, completed, TFT_GOLD);
      if (frame_->goal && score) eiffelPart(120, 195, score - 1, TFT_YELLOW, drop);
    } else {
      dragon(42, 185, completed, TFT_ORANGE);
      if (frame_->goal && score) dragonPart(42, 185, score - 1, TFT_YELLOW, drop);
    }
    if (frame_->goal) {
      for (int i = 0; i < 8; ++i) {
        int x = 42 + (i * 31 + frame_->nowMs / 17) % 155;
        int y = 77 + (i * 19 + frame_->nowMs / 25) % 95;
        star(x, y, 2, TFT_WHITE);
      }
    }
    char progress[18];
    snprintf(progress, sizeof(progress), "%u / 5", score);
    viCentered(frame_->goal ? progress : "CẢN PHÁ - KHÔNG BÀN", 120, 203, TFT_WHITE);
  }

  void drawVictoryScreen() {
    Team winner = frame_->scoreFR >= GOALS_TO_WIN ? TEAM_FR : TEAM_VN;
    rect(0, 0, 240, 240, winner == TEAM_FR ? 0x092C : 0x6005);
    for (int i = 0; i < 6; ++i) {
      int x = 35 + (i * 37) % 175;
      int y = 48 + (i * 29) % 110;
      star(x, y, 4 + ((frame_->nowMs / 170 + i) % 5), i & 1 ? TFT_YELLOW : TFT_WHITE);
    }
    if (winner == TEAM_FR) eiffel(120, 182, 5, TFT_GOLD);
    else dragon(42, 177, 5, TFT_ORANGE);
    rect(23, 17, 194, 31, 0x0842);
    viCentered("ĐỘI CHIẾN THẮNG", 120, 22, TFT_WHITE);
    viCentered(winner == TEAM_FR ? "PHÁP 5/5" : "VIỆT NAM 5/5", 120, 189, TFT_YELLOW);
    if (!frame_->roster) return;
    const Player* winners[MAX_CLIENTS]{};
    uint8_t count = 0;
    for (uint8_t i = 0; i < MAX_CLIENTS; ++i)
      if (frame_->roster[i].used && frame_->roster[i].team == winner) winners[count++] = &frame_->roster[i];
    if (count) viShortName(winners[(frame_->elapsedMs / 1600) % count]->name, 120, 206, TFT_WHITE);
  }
};
