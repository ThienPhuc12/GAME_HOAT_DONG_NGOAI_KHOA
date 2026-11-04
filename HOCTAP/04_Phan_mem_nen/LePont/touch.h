#pragma once
#include "config.h"
// =============================================================================
//  LE PONT · CẦU — le tactile de l'objet (Niveau 2, écran CST816S)
// VI: LE PONT · CẦU — cảm ứng của vật thể (mức 2, màn hình CST816S)
//
//  Le contrôleur tactile CST816S parle en I2C à l'adresse 0x15. Il n'a besoin
//  d'aucune bibliothèque : on lit 6 octets à partir du registre 0x01 et on y
//  trouve le geste, le nombre de doigts et la position (x, y) du doigt.
// VI: Bộ điều khiển cảm ứng CST816S giao tiếp bằng I2C ở địa chỉ 0x15. Không cần thư viện nào: ta
//     đọc 6 byte bắt đầu từ thanh ghi 0x01 và tìm thấy cử chỉ, số ngón tay và vị trí (x, y) của
//     ngón tay.
//
//      registre  contenu
//      0x01      geste  (0x01 glisser haut, 0x02 bas, 0x03 gauche, 0x04 droite,
//                        0x05 tap simple, 0x0B double tap, 0x0C appui long)
//      0x02      nombre de doigts (0 ou 1)
//      0x03-0x04 X sur 12 bits      0x05-0x06 Y sur 12 bits
// VI: thanh ghi  nội dung
//     0x01       cử chỉ (0x01 vuốt lên, 0x02 xuống, 0x03 trái, 0x04 phải,
//                        0x05 chạm một lần, 0x0B chạm hai lần, 0x0C nhấn giữ)
//     0x02       số ngón tay (0 hoặc 1)
//     0x03-0x04  X trên 12 bit       0x05-0x06  Y trên 12 bit
//
//  Le jeu n'appelle que deux fonctions : touchBegin() au démarrage et
//  touchPoll() à chaque tour de loop(), qui renvoie l'événement détecté.
// VI: Trò chơi chỉ gọi hai hàm: touchBegin() khi khởi động và touchPoll() ở mỗi vòng loop(), hàm
//     này trả về sự kiện được phát hiện.
// =============================================================================

enum TouchEvent { TOUCH_NONE, TOUCH_TAP, TOUCH_LONG };

#if TOUCH_ENABLE == 0
void touchBegin() {}
TouchEvent touchPoll() { return TOUCH_NONE; }
int touchX() { return -1; }
int touchY() { return -1; }

#else
#include <Wire.h>

static const uint8_t CST816S_ADDR = 0x15;
static int  tpX = -1, tpY = -1;
static bool tpDown = false;
static uint32_t tpDownAt = 0;
static bool tpLongSent = false;

static const uint32_t TAP_MAX_MS  = 400;    // relâché avant 400 ms : tap
                                            // VI: thả ra trước 400 ms: chạm
static const uint32_t LONG_MIN_MS = 1200;   // maintenu 1,2 s : appui long
                                            // VI: giữ 1,2 giây: nhấn giữ

// Lit l'état du contrôleur. Renvoie false si le CST816S ne répond pas
// (il dort quand on ne le touche pas : c'est normal, pas une panne).
// VI: Đọc trạng thái của bộ điều khiển. Trả về false nếu CST816S không phản hồi (nó ngủ khi không
//     ai chạm vào: điều này bình thường, không phải lỗi).
static bool cst816sRead(uint8_t& fingers, uint8_t& gesture, int& x, int& y) {
  Wire.beginTransmission(CST816S_ADDR);
  Wire.write(0x01);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((int)CST816S_ADDR, 6) != 6) return false;
  uint8_t b[6];
  for (int i = 0; i < 6; i++) b[i] = Wire.read();
  gesture = b[0];
  fingers = b[1] & 0x0F;
  x = ((b[2] & 0x0F) << 8) | b[3];
  y = ((b[4] & 0x0F) << 8) | b[5];
  return true;
}

void touchBegin() {
  if (PIN_TP_INT >= 0) pinMode(PIN_TP_INT, INPUT_PULLUP);
  if (PIN_TP_RST >= 0) {               // impulsion de reset : le contrôleur démarre propre
                                       // VI: xung khởi động lại: bộ điều khiển khởi động sạch
    pinMode(PIN_TP_RST, OUTPUT);
    digitalWrite(PIN_TP_RST, LOW);  delay(10);
    digitalWrite(PIN_TP_RST, HIGH); delay(60);
  }
  Wire.begin(PIN_TP_SDA, PIN_TP_SCL, 400000);
  uint8_t f, g; int x, y;
  bool ok = cst816sRead(f, g, x, y);
  Serial.printf("[tactile] CST816S sur SDA %d / SCL %d : %s\n", PIN_TP_SDA, PIN_TP_SCL,
                ok ? "present" : "silencieux (il repond au premier contact)");
}

// À appeler souvent (chaque loop). Détecte tap et appui long à partir des
// transitions doigt posé / doigt levé : plus fiable que le registre « geste »,
// qui varie d'un lot de dalles à l'autre.
// VI: Cần gọi thường xuyên (mỗi vòng loop). Phát hiện chạm và nhấn giữ dựa trên chuyển đổi ngón tay
//     đặt xuống / nhấc lên: đáng tin cậy hơn thanh ghi « cử chỉ », vốn khác nhau giữa các lô màn
//     hình.
TouchEvent touchPoll() {
  static uint32_t lastRead = 0;
  if (millis() - lastRead < 20) return TOUCH_NONE;   // 50 lectures par seconde suffisent
                                                     // VI: 50 lần đọc mỗi giây là đủ
  lastRead = millis();

  // Si INT est câblé : haut = rien ne se passe, inutile d'interroger l'I2C
  // VI: Nếu có nối INT: mức cao = không có gì xảy ra, không cần hỏi qua I2C
  if (PIN_TP_INT >= 0 && !tpDown && digitalRead(PIN_TP_INT) == HIGH) return TOUCH_NONE;

  uint8_t fingers = 0, gesture = 0; int x = 0, y = 0;
  bool ok = cst816sRead(fingers, gesture, x, y);
  bool down = ok && fingers > 0;

  if (down) { tpX = x; tpY = y; }

  if (down && !tpDown) {                       // doigt posé
                                               // VI: ngón tay đặt xuống
    tpDown = true; tpDownAt = millis(); tpLongSent = false;
  }
  else if (down && tpDown && !tpLongSent && millis() - tpDownAt >= LONG_MIN_MS) {
    tpLongSent = true;                         // appui long : signalé une seule fois, sans attendre le relâché
                                               // VI: nhấn giữ: chỉ báo một lần, không chờ thả tay
    return TOUCH_LONG;
  }
  else if (!down && tpDown) {                  // doigt levé
                                               // VI: ngón tay nhấc lên
    tpDown = false;
    uint32_t held = millis() - tpDownAt;
    if (!tpLongSent && held < TAP_MAX_MS) return TOUCH_TAP;
  }
  return TOUCH_NONE;
}

int touchX() { return tpX; }
int touchY() { return tpY; }
#endif
