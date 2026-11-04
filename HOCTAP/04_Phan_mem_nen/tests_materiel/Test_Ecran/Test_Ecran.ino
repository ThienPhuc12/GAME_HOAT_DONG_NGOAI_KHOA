/*
  TEST ÉCRAN — Le Pont · Cầu (Tutoriel 2, étape 3)
  Vérifie le câblage de l'écran rond GC9A01 avant de charger le jeu.
  Attendu : fond bleu, un anneau orange, le texte « LE PONT » et un point qui
  tourne. Si l'écran reste blanc ou noir, voir le dépannage du Tutoriel 2.

  Mêmes broches que config.h du jeu :
    VCC -> 3V3   GND -> GND   SCL -> 12   SDA -> 11   RES -> 8   DC -> 9   CS -> 10   BLK -> 7 (ou 3V3)
  Bibliothèque : GFX Library for Arduino (Moon On Our Nation)
*/
#include <Arduino_GFX_Library.h>

#define PIN_SCK  12
#define PIN_MOSI 11
#define PIN_CS   10
#define PIN_DC    9
#define PIN_RST   8
#define PIN_BL    7     // -1 si BLK est câblé sur 3V3

Arduino_DataBus* bus = new Arduino_ESP32SPI(PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI, GFX_NOT_DEFINED);
Arduino_GFX* gfx = new Arduino_GC9A01(bus, PIN_RST, 0 /*rotation*/, true /*IPS*/);

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("[test ecran] demarrage");
  if (PIN_BL >= 0) { pinMode(PIN_BL, OUTPUT); digitalWrite(PIN_BL, HIGH); }

  if (!gfx->begin()) { Serial.println("[test ecran] gfx->begin() a echoue : verifier SCL/SDA/CS/DC/RES"); }
  gfx->fillScreen(RGB565_BLUE);
  gfx->drawCircle(120, 120, 116, RGB565_ORANGE);
  gfx->drawCircle(120, 120, 115, RGB565_ORANGE);
  gfx->setTextColor(RGB565_WHITE);
  gfx->setTextSize(4);
  gfx->setCursor(120 - 7 * 6 * 4 / 2, 96);     // « LE PONT » = 7 caracteres de 6 px, taille 4
  gfx->print("LE PONT");
  gfx->setTextSize(2);
  gfx->setCursor(120 - 11 * 6 * 2 / 2, 140);   // « ecran OK ! » = 11 caracteres
  gfx->print("ecran OK !");
  Serial.println("[test ecran] si vous lisez LE PONT sur fond bleu, le cablage est bon");
}

void loop() {
  // Un point qui tourne sur l'anneau : prouve que l'ecran se rafraichit
  static float a = 0;
  static int px = -1, py = -1;
  if (px >= 0) gfx->fillCircle(px, py, 5, RGB565_BLUE);
  px = 120 + 100 * cosf(a); py = 120 + 100 * sinf(a);
  gfx->fillCircle(px, py, 5, RGB565_WHITE);
  a += 0.08f;
  delay(30);
}
