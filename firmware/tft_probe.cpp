#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GC9A01A.h>
#include "../config.h"

// Standalone hardware probe. It uses Adafruit's driver, not LovyanGFX,
// and deliberately leaves Wi-Fi and the OLED off while diagnosing the TFT.
Adafruit_GC9A01A probe(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
uint32_t lastFrameMs = 0;
uint8_t frame = 0;

void drawProbe(uint8_t index) {
  if (index == 0) {
    probe.fillRect(0, 0, 120, 120, GC9A01A_RED);
    probe.fillRect(120, 0, 120, 120, GC9A01A_GREEN);
    probe.fillRect(0, 120, 120, 120, GC9A01A_BLUE);
    probe.fillRect(120, 120, 120, 120, GC9A01A_YELLOW);
    probe.drawRect(12, 12, 216, 216, GC9A01A_WHITE);
    probe.setCursor(35, 109);
    probe.setTextColor(GC9A01A_WHITE, GC9A01A_BLACK);
    probe.setTextSize(2);
    probe.print("TFT TEST");
  } else {
    static const uint16_t colors[] = {
      GC9A01A_RED, GC9A01A_GREEN, GC9A01A_BLUE, GC9A01A_WHITE
    };
    probe.fillScreen(colors[index - 1]);
  }
  Serial.printf("TFT probe frame %u, heap %u\n", index, ESP.getFreeHeap());
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TFT_BL, OUTPUT);
  digitalWrite(PIN_TFT_BL, HIGH);
  SPI.begin(PIN_TFT_SCLK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
  probe.begin(4000000);
  probe.setRotation(0);
  drawProbe(frame);
  lastFrameMs = millis();
  Serial.println("Independent GC9A01A probe ready");
}

void loop() {
  const uint32_t now = millis();
  if (now - lastFrameMs >= 2500) {
    lastFrameMs = now;
    frame = (frame + 1) % 5;
    drawProbe(frame);
  }
}
