#pragma once
// =============================================================================
//  LE PONT · CẦU — configuration
//  Change uniquement ce qui concerne ton groupe. Tout le reste du jeu s'adapte.
// VI: LE PONT · CẦU — cấu hình
//     Chỉ thay đổi những gì liên quan đến nhóm của bạn. Phần còn lại của trò chơi tự thích nghi.
// =============================================================================

#define GROUP_ID 1              // Numéro de ton groupe (1 à 7). Donne le nom du Wi-Fi : LePont-1, LePont-2…
                                // VI: Số nhóm của bạn (1 đến 7). Tạo ra tên Wi-Fi: LePont-1,
                                //     LePont-2…
#define QUESTION_SECONDS 20     // Temps pour répondre à chaque question
                                // VI: Thời gian trả lời mỗi câu hỏi (giây)
#define QUESTIONS_PER_GAME 10   // Hérité de la V1, NON utilisé en V2 : la partie dure jusqu'au pont fermé
                                // VI: Có từ bản V1, KHÔNG dùng trong V2: ván chơi kéo dài đến khi cầu
                                //     được nối liền
#define MAX_PLAYERS 10          // Joueurs maximum connectés à l'objet
                                // VI: Số người chơi tối đa kết nối với vật thể
#define TEAM_COUNT 2            // Hérité de la V1, NON utilisé en V2 : le jeu a toujours deux rives
                                // VI: Có từ bản V1, KHÔNG dùng trong V2: trò chơi luôn có hai bờ

// Noms et couleurs des équipes (téléphones et grand écran)
// VI: Tên và màu của các đội (điện thoại và màn hình lớn)
static const char* TEAM_NAMES[4]  = {"Rive France", "Rive Vietnam", "Equipe 3", "Equipe 4"};
static const char* TEAM_COLORS[4] = {"#2b6cb0", "#c53030", "#2f855a", "#b7791f"};

// ------ Écran de l'objet · màn hình của vật thể ------------------------------
// 0 = aucun écran (Tutoriel 1 : tout se lit sur le moniteur série)
// 2 = GC9A01 1.28" 240x240 rond — l'écran de l'atelier (Tutoriel 2)
// 1 = ST7789 1.3" 240x240 carré (si un groupe a cet écran-là)
// VI: 0 = không có màn hình (Hướng dẫn 1: mọi thứ hiện trên Serial Monitor)
//     2 = GC9A01 1.28" 240x240 tròn — màn hình của xưởng (Hướng dẫn 2)
//     1 = ST7789 1.3" 240x240 vuông (nếu nhóm có loại màn hình này)
#define DISPLAY_TYPE 0

// Câblage SPI de l'écran sur l'ESP32-S3 DevKitC (broches libres, hors PSRAM et USB)
//   écran   -> ESP32-S3
//   VCC     -> 3V3
//   GND     -> GND
// VI: Đấu nối SPI của màn hình với ESP32-S3 DevKitC (các chân còn trống, không dùng chân PSRAM và USB)
//       màn hình -> ESP32-S3
#define PIN_SCK  12   // SCL / CLK
                      // VI: SCL / CLK (xung nhịp)
#define PIN_MOSI 11   // SDA / DIN / MOSI
                      // VI: SDA / DIN / MOSI (dữ liệu)
#define PIN_CS   10   // CS
                      // VI: CS (chọn chip)
#define PIN_DC    9   // DC / RS
                      // VI: DC / RS (lệnh hay dữ liệu)
#define PIN_RST   8   // RES / RST
                      // VI: RES / RST (khởi động lại)
#define PIN_BL    7   // BLK / LED (rétroéclairage) ; mettre -1 si câblé directement sur 3V3
                      // VI: BLK / LED (đèn nền); đặt -1 nếu nối thẳng vào 3V3

// ----------- Tactile (Niveau 2) · cảm ứng (mức 2) ----------------------------
// 0 = écran sans tactile (7 broches) — tout le jeu se pilote depuis la page /host
// 1 = écran tactile CST816S (12 broches) : un tap = question suivante, appui long = nouvelle partie
// VI: 0 = màn hình không cảm ứng (7 chân) — toàn bộ trò chơi điều khiển từ trang /host
//     1 = màn hình cảm ứng CST816S (12 chân): chạm = câu hỏi tiếp theo, nhấn giữ = ván mới
#define TOUCH_ENABLE 0

// Câblage I2C du tactile (broches libres de l'ESP32-S3 DevKitC)
//   écran    -> ESP32-S3
//   TP_SDA   -> 5
//   TP_SCL   -> 6
//   TP_INT   -> 4     (facultatif : mettre -1 si non câblé)
//   TP_RST   -> 15    (facultatif : mettre -1 si non câblé)
// VI: Đấu nối I2C của cảm ứng (các chân còn trống của ESP32-S3 DevKitC)
//     TP_INT và TP_RST không bắt buộc: đặt -1 nếu không nối.
#define PIN_TP_SDA  5
#define PIN_TP_SCL  6
#define PIN_TP_INT  4
#define PIN_TP_RST 15

// ----------------------------- Wi-Fi -----------------------------------------
// Sept objets dans la même salle : chaque groupe a son canal pour ne pas gêner les autres.
// VI: Bảy vật thể trong cùng một phòng: mỗi nhóm có kênh Wi-Fi riêng để không làm nhiễu nhau.
static const uint8_t WIFI_CHANNELS[7] = {1, 6, 11, 1, 6, 11, 1};
#define WIFI_PASSWORD ""        // Réseau ouvert par défaut. 8 caractères minimum pour le protéger.
                                // VI: Mặc định là mạng mở. Cần tối thiểu 8 ký tự để đặt mật khẩu.
