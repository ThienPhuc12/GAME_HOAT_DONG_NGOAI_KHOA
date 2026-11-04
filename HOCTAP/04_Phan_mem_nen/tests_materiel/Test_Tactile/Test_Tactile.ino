/*
  TEST TACTILE — Le Pont · Cầu (Tutoriel 2, étape 9, écran CST816S)
  Vérifie le câblage I2C du tactile. Chaque contact du doigt dessine un point
  sur l'écran et écrit ses coordonnées sur le moniteur série (115200 baud).
  Un tap écrit « TAP », un appui long de 1,2 s écrit « LONG » et efface l'écran.

  Broches (mêmes que config.h du jeu) :
    écran  : SCL 12  SDA 11  RES 8  DC 9  CS 10  BLK 7
    tactile: TP_SDA 5  TP_SCL 6  TP_INT 4  TP_RST 15
  Aucune bibliothèque tactile : le CST816S se lit directement en I2C (adresse 0x15).
*/
#include <Wire.h>
#include <Arduino_GFX_Library.h>

#define PIN_SCK  12
#define PIN_MOSI 11
#define PIN_CS   10
#define PIN_DC    9
#define PIN_RST   8
#define PIN_BL    7
#define PIN_TP_SDA  5
#define PIN_TP_SCL  6
#define PIN_TP_INT  4
#define PIN_TP_RST 15

Arduino_DataBus* bus = new Arduino_ESP32SPI(PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI, GFX_NOT_DEFINED);
Arduino_GFX* gfx = new Arduino_GC9A01(bus, PIN_RST, 0, true);

const uint8_t CST816S_ADDR = 0x15;

bool readTouch(uint8_t& fingers, int& x, int& y) {
  Wire.beginTransmission(CST816S_ADDR);
  Wire.write(0x01);
  if (Wire.endTransmission(false) != 0) return false;   // le controleur dort : normal sans contact
  if (Wire.requestFrom((int)CST816S_ADDR, 6) != 6) return false;
  uint8_t b[6];
  for (int i = 0; i < 6; i++) b[i] = Wire.read();
  fingers = b[1] & 0x0F;
  x = ((b[2] & 0x0F) << 8) | b[3];
  y = ((b[4] & 0x0F) << 8) | b[5];
  return true;
}

void drawHome() {
  gfx->fillScreen(RGB565_BLACK);
  gfx->drawCircle(120, 120, 116, RGB565_DARKGREY);
  gfx->setTextColor(RGB565_WHITE); gfx->setTextSize(2);
  gfx->setCursor(120 - 13 * 6, 100); gfx->print("touchez-moi !");
  gfx->setTextSize(1); gfx->setTextColor(RGB565_LIGHTGREY);
  gfx->setCursor(120 - 15 * 3, 130); gfx->print("appui long = effacer");
}

void setup() {
  Serial.begin(115200);
  delay(300);
  if (PIN_BL >= 0) { pinMode(PIN_BL, OUTPUT); digitalWrite(PIN_BL, HIGH); }
  gfx->begin();
  drawHome();

  pinMode(PIN_TP_INT, INPUT_PULLUP);
  pinMode(PIN_TP_RST, OUTPUT);
  digitalWrite(PIN_TP_RST, LOW);  delay(10);
  digitalWrite(PIN_TP_RST, HIGH); delay(60);
  Wire.begin(PIN_TP_SDA, PIN_TP_SCL, 400000);

  // Scan I2C : le tactile doit apparaitre en 0x15 (il peut ne pas repondre tant qu'on ne l'a pas touche)
  Serial.println("[test tactile] scan I2C...");
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) Serial.printf("[test tactile]   peripherique en 0x%02X %s\n", a, a == 0x15 ? "<- CST816S" : "");
  }
  Serial.println("[test tactile] touchez l'ecran : les coordonnees s'affichent ici");
}

void loop() {
  static bool down = false; static uint32_t downAt = 0; static bool longSent = false;
  uint8_t fingers = 0; int x = 0, y = 0;
  bool ok = readTouch(fingers, x, y);
  bool now = ok && fingers > 0;

  if (now) {
    gfx->fillCircle(x, y, 4, RGB565_ORANGE);
    Serial.printf("[test tactile] x=%3d  y=%3d\n", x, y);
  }
  if (now && !down) { down = true; downAt = millis(); longSent = false; }
  else if (now && down && !longSent && millis() - downAt > 1200) {
    longSent = true; Serial.println("[test tactile] LONG -> efface"); drawHome();
  }
  else if (!now && down) {
    down = false;
    if (!longSent && millis() - downAt < 400) Serial.println("[test tactile] TAP");
  }
  delay(20);
}
