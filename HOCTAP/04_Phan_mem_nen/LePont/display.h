#pragma once
#include "config.h"
// =============================================================================
//  LE PONT · CẦU — l'écran de l'objet
// VI: LE PONT · CẦU — màn hình của vật thể
//
//  Le jeu appelle ces fonctions avec une « vue du chantier » (BridgeView) :
//    side[2]   travées posées par rive (0..4)     pending[2]  travée suspendue à la grue
//    center    pont fermé (clé de voûte posée)    q           numéro de question
//    dbl       question « rencontre »             hint        le chef de chantier aide
//  displayTick() est appelée à chaque tour de loop() : elle anime (travée qui
//  descend, dragon qui traverse, feux d'artifice) sans bloquer le jeu.
// VI: Trò chơi gọi các hàm này với một « góc nhìn công trường » (BridgeView):
//       side[2]   số nhịp đã đặt mỗi bờ (0..4)     pending[2]  nhịp cầu treo trên cần cẩu
//       center    cầu đã nối liền (đá khóa vòm)     q           số thứ tự câu hỏi
//       dbl       câu hỏi « gặp gỡ »                hint        đốc công đang giúp
//     displayTick() được gọi ở mỗi vòng loop(): nó tạo hoạt hình (nhịp cầu hạ xuống,
//     rồng bay qua, pháo hoa) mà không chặn trò chơi.
//
//  Les images viennent de assets.h, généré par assets/png2assets.py :
//    IMG_FOND_RIVES   le décor (240x240)        IMG_FOND_VICTOIRE  la scène finale
//    IMG_PONT         l'arc doré (130x56), découpé en 10 tranches de 13 px :
//                     tranches 1-4 depuis la rive France, 7-10 depuis la rive
//                     Vietnam, 5-6 = la clé de voûte posée à la rencontre
//    IMG_GRUE         la grue (52x52, crochet en 26,47)   IMG_CHEF_1..4  Cầu, 4 poses
//    IMG_DRAGON_1..4  le dragon de la victoire             IMG_ECLAT      étincelle
// VI: Hình ảnh lấy từ assets.h, được tạo bởi assets/png2assets.py:
//       IMG_FOND_RIVES   cảnh nền (240x240)         IMG_FOND_VICTOIRE  cảnh chiến thắng
//       IMG_PONT         vòm cầu vàng (130x56), cắt thành 10 lát rộng 13 px:
//                        lát 1-4 từ bờ Pháp, 7-10 từ bờ Việt Nam,
//                        5-6 = đá khóa vòm đặt khi hai bờ gặp nhau
//       IMG_GRUE         cần cẩu (52x52, móc ở 26,47)   IMG_CHEF_1..4  Cầu, 4 tư thế
//       IMG_DRAGON_1..4  con rồng chiến thắng            IMG_ECLAT      tia lấp lánh
//
//  C'est ICI que ton groupe personnalise l'objet : remplace une image dans
//  assets/, relance png2assets.py, ou change les animations ci-dessous.
// VI: Chính Ở ĐÂY nhóm của bạn tùy biến vật thể: thay một hình trong assets/, chạy lại
//     png2assets.py, hoặc thay đổi các hoạt hình bên dưới.
// =============================================================================

struct BridgeView { int side[2]; bool pending[2]; bool center; int q; bool dbl; bool hint; };

void displayBegin();
void displayLobby(const char* ssid, int nLeft, int nRight);
void displayQuestion(const BridgeView& v, int secLeft, int secTotal);
void displayReveal(const BridgeView& v, bool leftOk, bool rightOk);
void displayChantier(const BridgeView& v);
void displayEnd(const BridgeView& v, int questions, int durationSec);
void displayTick();

#if DISPLAY_TYPE == 0
// ------------------------- Aucun écran : moniteur série · không có màn hình: Serial Monitor -----------------------
void displayBegin() { Serial.println("[ecran] aucun ecran (DISPLAY_TYPE 0) : etats affiches ici"); }
void displayLobby(const char* ssid, int nL, int nR) { Serial.printf("[ecran] SALON  %s  rive F %d  rive V %d\n", ssid, nL, nR); }
void displayQuestion(const BridgeView& v, int left, int tot) { Serial.printf("[ecran] Q%d %2d s  pont %d+%d%s%s\n", v.q, left, v.side[0], v.side[1], v.dbl ? " RENCONTRE" : "", v.hint ? " (indice)" : ""); }
void displayReveal(const BridgeView& v, bool l, bool r) { Serial.printf("[ecran] rive F %s  rive V %s%s\n", l ? "travee posee" : "travee a la grue", r ? "travee posee" : "travee a la grue", v.center ? "  PONT FERME" : ""); }
void displayChantier(const BridgeView& v) { Serial.printf("[ecran] chantier %d+%d  grue F:%d V:%d\n", v.side[0], v.side[1], v.pending[0], v.pending[1]); }
void displayEnd(const BridgeView& v, int q, int s) { Serial.printf("[ecran] FIN  pont ferme en %d questions, %d s\n", q, s); }
void displayTick() {}

#else
// ------------------- Écran SPI via « GFX Library for Arduino » · màn hình SPI qua « GFX Library for Arduino » ----------------
#include <Arduino_GFX_Library.h>
#include "assets.h"

static Arduino_DataBus* bus = new Arduino_ESP32SPI(PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI, GFX_NOT_DEFINED);
#if DISPLAY_TYPE == 1
static Arduino_GFX* panel = new Arduino_ST7789(bus, PIN_RST, 0 /*rotation*/, true /*IPS*/, 240, 240, 0, 0, 0, 80);
#else
static Arduino_GFX* panel = new Arduino_GC9A01(bus, PIN_RST, 0 /*rotation*/, true /*IPS*/);
#endif
static Arduino_Canvas* gfx = new Arduino_Canvas(240, 240, panel);   // tout se dessine en mémoire, puis flush()
                                                                    // VI: mọi thứ được vẽ trong bộ nhớ, rồi
                                                                    //     flush() để đưa lên màn hình

// Géométrie mesurée sur le décor : l'arc se pose en (55,123), ses extrémités sur les deux piles
// VI: Kích thước đo trên cảnh nền: vòm cầu đặt tại (55,123), hai đầu tựa lên hai trụ
static const int CX = 120, CY = 120;
static const int PONT_X = 55, PONT_Y = 123, SLICE_W = 13, SLICES = 10;
static const int GRUE_HOOK_X = 26, GRUE_HOOK_Y = 47;
static const uint16_t C_WHITE = 0xFFFF, C_GOLD = 0xFE60, C_LIGHT = 0xC618, C_BLUE = 0x2B57, C_RED = 0xC186, C_DARK = 0x2104;

// ---- primitives · hàm vẽ cơ bản ---------------------------------------------------------------
static void img(const uint16_t* data, int w, int h, int x, int y, bool flip = false) {
  for (int yy = 0; yy < h; yy++) for (int xx = 0; xx < w; xx++) {
    uint16_t c = pgm_read_word(&data[yy * w + xx]);
    if (c != IMG_KEY) gfx->drawPixel(x + (flip ? w - 1 - xx : xx), y + yy, c);
  }
}
// Une tranche verticale de l'arc (i = 0..9), posée à sa place exacte, avec un décalage vertical (animation)
// VI: Một lát dọc của vòm cầu (i = 0..9), đặt đúng vị trí, có độ lệch theo chiều dọc (để tạo hoạt
//     hình)
static void slice(int i, int dy = 0) {
  for (int yy = 0; yy < IMG_PONT_H; yy++) for (int xx = i * SLICE_W; xx < (i + 1) * SLICE_W; xx++) {
    uint16_t c = pgm_read_word(&IMG_PONT[yy * IMG_PONT_W + xx]);
    if (c != IMG_KEY) gfx->drawPixel(PONT_X + xx, PONT_Y + yy + dy, c);
  }
}
static void centerText(const char* s, int y, int size, uint16_t color) {
  int w = strlen(s) * 6 * size;
  gfx->setTextSize(size); gfx->setTextColor(color); gfx->setCursor(CX - w / 2, y); gfx->print(s);
}
static float easeOutBounce(float t) {
  if (t < 1 / 2.75f) return 7.5625f * t * t;
  if (t < 2 / 2.75f) { t -= 1.5f / 2.75f; return 7.5625f * t * t + 0.75f; }
  if (t < 2.5f / 2.75f) { t -= 2.25f / 2.75f; return 7.5625f * t * t + 0.9375f; }
  t -= 2.625f / 2.75f; return 7.5625f * t * t + 0.984375f;
}

// Quelles tranches sont posées : 0..3 depuis la gauche, 9..6 depuis la droite, 4-5 au centre
// VI: Những lát nào đã được đặt: 0..3 từ bên trái, 9..6 từ bên phải, 4-5 ở giữa
static bool placed(const BridgeView& v, int i) {
  if (i < 4) return i < v.side[0];
  if (i > 5) return (9 - i) < v.side[1];
  return v.center;
}
static int nextSlice(const BridgeView& v, int r) { return r == 0 ? v.side[0] : 9 - v.side[1]; }   // prochaine tranche de la rive r
                                                                                                  // VI: lát tiếp theo của bờ r

static void drawBridge(const BridgeView& v, int skip = -1) {
  for (int i = 0; i < SLICES; i++) if (placed(v, i) && i != skip) slice(i);
}
// Grue + tranche suspendue pour une rive : la tranche flotte 28 px au-dessus de sa place
// VI: Cần cẩu + lát cầu đang treo của một bờ: lát cầu lơ lửng cao hơn vị trí của nó 28 px
static void drawCrane(const BridgeView& v, int r, int lift = 28) {
  int i = nextSlice(v, r); if (i < 0 || i > 9) return;
  slice(i, -lift);
  int hookX = PONT_X + i * SLICE_W + SLICE_W / 2, hookY = PONT_Y - lift + 8;
  bool flip = (r == 1);
  int gx = flip ? hookX - (IMG_GRUE_W - 1 - GRUE_HOOK_X) : hookX - GRUE_HOOK_X;
  img(IMG_GRUE, IMG_GRUE_W, IMG_GRUE_H, gx, hookY - GRUE_HOOK_Y, flip);
}

// ---- animation (non bloquante) · hoạt hình (không chặn) -------------------------------------------------
enum Anim { A_NONE, A_DROP, A_END };
static Anim anim = A_NONE; static uint32_t animAt = 0; static BridgeView animV; static bool animOk[2]; static int animQ, animDur;

static void renderReveal(const BridgeView& v, bool l, bool r, float t);
static void renderEnd(const BridgeView& v, int q, int s, float t);

void displayTick() {
  if (anim == A_NONE) return;
  static uint32_t last = 0; if (millis() - last < 40) return; last = millis();   // 25 images/s
                                                                                 // VI: 25 khung hình/giây
  float t = (millis() - animAt) / 1000.0f;
  if (anim == A_DROP) { renderReveal(animV, animOk[0], animOk[1], t); if (t > 1.6f) anim = A_NONE; }
  else if (anim == A_END) { renderEnd(animV, animQ, animDur, t); }
}

// ---- écrans · các màn hình -------------------------------------------------------------------
void displayBegin() {
  if (PIN_BL >= 0) { pinMode(PIN_BL, OUTPUT); digitalWrite(PIN_BL, HIGH); }
  if (!gfx->begin(40000000)) Serial.println("[ecran] gfx->begin a echoue (memoire du canvas ?)");
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t*)IMG_FOND_RIVES, 240, 240);
  centerText("LE PONT", 70, 3, C_WHITE);
  centerText("CAU", 104, 3, C_GOLD);
  gfx->flush();
}

void displayLobby(const char* ssid, int nL, int nR) {
  anim = A_NONE;
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t*)IMG_FOND_RIVES, 240, 240);
  centerText("WI-FI", 40, 2, C_LIGHT);
  centerText(ssid, 64, 2, C_WHITE);
  centerText("192.168.4.1", 88, 1, C_LIGHT);
  for (int i = 0; i < nL && i < 8; i++) { float a = 3.1416f + (i + 1) * 0.33f; gfx->fillCircle(CX + 108 * cosf(a), CY + 108 * sinf(a), 5, C_BLUE); }
  for (int i = 0; i < nR && i < 8; i++) { float a = -(i + 1) * 0.33f; gfx->fillCircle(CX + 108 * cosf(a), CY + 108 * sinf(a), 5, C_RED); }
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", nL); gfx->setTextSize(4); gfx->setTextColor(C_BLUE); gfx->setCursor(66, 104); gfx->print(buf);
  snprintf(buf, sizeof(buf), "%d", nR); gfx->setTextColor(C_RED); gfx->setCursor(154, 104); gfx->print(buf);
  img(IMG_CHEF_1, IMG_CHEF_1_W, IMG_CHEF_1_H, 100, 116);   // Cầu attend entre les deux piles
                                                           // VI: Cầu đứng chờ giữa hai trụ
  gfx->flush();
}

void displayQuestion(const BridgeView& v, int left, int tot) {
  anim = A_NONE;
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t*)IMG_FOND_RIVES, 240, 240);
  drawBridge(v);
  for (int r = 0; r < 2; r++) if (v.pending[r] && !v.center) drawCrane(v, r);
  float frac = tot > 0 ? (float)left / tot : 0;
  uint16_t col = frac > 0.5f ? 0x07E0 : (frac > 0.25f ? 0xFD20 : 0xF800);
  gfx->drawArc(CX, CY, 119, 111, 0, 360, C_DARK);
  if (frac > 0) gfx->fillArc(CX, CY, 119, 111, -90, -90 + 360.0f * frac, col);
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", left); centerText(buf, 54, 6, C_WHITE);
  if (v.dbl) centerText("RENCONTRE", 108, 2, C_GOLD);
  else { snprintf(buf, sizeof(buf), "Q %d", v.q); centerText(buf, 108, 2, C_LIGHT); }
  if (v.hint) img(IMG_CHEF_2, IMG_CHEF_2_W, IMG_CHEF_2_H, 24, 60);   // le chef de chantier souffle
                                                                     // VI: đốc công thì thầm gợi ý
  gfx->flush();
}

// Révélation : la nouvelle tranche de chaque rive qui a réussi descend et rebondit (1,2 s) ;
// la rive qui a raté voit sa tranche rester accrochée à la grue, avec Cầu assis sur le crochet.
// VI: Công bố đáp án: lát cầu mới của mỗi bờ trả lời đúng hạ xuống và nảy lên (1,2 giây); bờ trả
//     lời sai thấy lát cầu của mình vẫn treo trên cần cẩu, với Cầu ngồi trên móc.
static void renderReveal(const BridgeView& v, bool l, bool r, float t) {
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t*)IMG_FOND_RIVES, 240, 240);
  bool ok[2] = {l, r};
  int fresh[2] = {-1, -1};
  BridgeView before = v;                                   // état avant la pose, pour animer la nouvelle tranche
                                                           // VI: trạng thái trước khi đặt, để tạo hoạt
                                                           //     hình cho lát mới
  for (int rr = 0; rr < 2; rr++) if (ok[rr] && !v.center && v.side[rr] > 0 && v.side[rr] <= 4) { before.side[rr] = v.side[rr] - 1; fresh[rr] = nextSlice(before, rr); }
  if (v.center) { before.center = false; }
  float u = t / 1.2f; if (u > 1) u = 1;
  int dy = (int)(-40 * (1.0f - easeOutBounce(u)));
  drawBridge(before);
  for (int rr = 0; rr < 2; rr++) if (fresh[rr] >= 0) slice(fresh[rr], dy);
  if (v.center) { slice(4, dy); slice(5, dy); }
  for (int rr = 0; rr < 2; rr++) if (!ok[rr] && !v.center && v.side[rr] < 4) { drawCrane(v, rr); int i = nextSlice(v, rr); img(IMG_CHEF_3, IMG_CHEF_3_W, IMG_CHEF_3_H, PONT_X + i * SLICE_W + SLICE_W / 2 - 20, PONT_Y - 28 - 38); }
  if (v.center) { centerText("PONT", 44, 3, C_GOLD); centerText("FERME !", 76, 3, C_GOLD); }
  else {
    gfx->setTextSize(2);
    gfx->setTextColor(l ? 0x07E0 : 0xFD20); gfx->setCursor(50, 24); gfx->print(l ? "POSEE" : "GRUE");
    gfx->setTextColor(r ? 0x07E0 : 0xFD20); gfx->setCursor(142, 24); gfx->print(r ? "POSEE" : "GRUE");
    if (v.dbl) centerText("ON RETENTE", 92, 2, C_LIGHT);
  }
  gfx->flush();
}
void displayReveal(const BridgeView& v, bool l, bool r) {
  animV = v; animOk[0] = l; animOk[1] = r; animAt = millis(); anim = A_DROP;
  renderReveal(v, l, r, 0);
}

void displayChantier(const BridgeView& v) {
  anim = A_NONE;
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t*)IMG_FOND_RIVES, 240, 240);
  drawBridge(v);
  for (int r = 0; r < 2; r++) if (v.pending[r] && !v.center) drawCrane(v, r);
  char buf[16];
  snprintf(buf, sizeof(buf), "%d + %d", v.side[0], v.side[1]); centerText(buf, 58, 3, C_GOLD);
  centerText("travees / 4 + 4", 90, 1, C_LIGHT);
  gfx->flush();
}

// Fin : la scène de victoire, le dragon qui traverse, des étincelles, Cầu qui fête, la fiche
// VI: Kết thúc: cảnh chiến thắng, rồng bay qua, tia lấp lánh, Cầu ăn mừng, phiếu công trường
static void renderEnd(const BridgeView& v, int q, int s, float t) {
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t*)IMG_FOND_VICTOIRE, 240, 240);
  const uint16_t* poses[4] = {IMG_DRAGON_1, IMG_DRAGON_2, IMG_DRAGON_3, IMG_DRAGON_2};
  int frame = ((int)(t * 6)) % 4;
  int dx = (int)(-100 + fmodf(t * 60, 360));           // traverse de gauche à droite toutes les 6 s
                                                       // VI: bay từ trái sang phải mỗi 6 giây
  int dyn = 96 + (int)(5 * sinf(t * 4));
  if (fmodf(t, 6) > 4.5f) img(IMG_DRAGON_4, IMG_DRAGON_4_W, IMG_DRAGON_4_H, dx, dyn);   // crache le feu en fin de traversée
                                                                                        // VI: phun lửa ở cuối đường bay
  else img(poses[frame], IMG_DRAGON_1_W, IMG_DRAGON_1_H, dx, dyn);
  for (int i = 0; i < 6; i++) {                        // étincelles qui clignotent dans le ciel
                                                       // VI: tia lấp lánh nhấp nháy trên bầu trời
    float a = i * 1.05f + t * 0.7f; int ex = CX + (int)(80 * cosf(a)), ey = 40 + (int)(22 * sinf(a * 1.7f));
    if (((int)(t * 5) + i) % 3) img(IMG_ECLAT, IMG_ECLAT_W, IMG_ECLAT_H, ex, ey);
  }
  img(IMG_CHEF_4, IMG_CHEF_4_W, IMG_CHEF_4_H, 4, 118 + (int)(3 * sinf(t * 8)));   // Cầu saute de joie sur le quai
                                                                                  // VI: Cầu nhảy lên vui mừng trên bến
  centerText("PONT FERME", 26, 2, C_GOLD);
  char buf[24];
  snprintf(buf, sizeof(buf), "%d questions", q); centerText(buf, 48, 2, C_WHITE);
  snprintf(buf, sizeof(buf), "%d min %02d", s / 60, s % 60); centerText(buf, 70, 2, C_WHITE);
  gfx->flush();
}
void displayEnd(const BridgeView& v, int q, int s) {
  animV = v; animQ = q; animDur = s; animAt = millis(); anim = A_END;
  renderEnd(v, q, s, 0);
}
#endif
