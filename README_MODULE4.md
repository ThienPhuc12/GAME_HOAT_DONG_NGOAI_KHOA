# Module 4 — WebApp điện thoại

`web/player.html` chứa toàn bộ HTML, CSS, JavaScript và Canvas Pixel Art. Trang chạy offline từ ESP32 qua Wi-Fi AP và WebSocket `/ws`; không cần CDN, font hay ảnh từ mạng ngoài.

## Tạo lại tài nguyên Flash

```powershell
node gen_web_sprites.js
python gen_web_h.py
```

Lệnh đầu trích 72 khung hình 24×36 từ `cau-thu-pixel-esp32.html`, đóng gói 5 bit màu mỗi pixel vào `web/player.html`. Lệnh sau tạo `web_player.h` dưới dạng `WEB_PLAYER_HTML[] PROGMEM`. Nếu chỉ sửa giao diện, chỉ cần chạy lệnh Python. Sau đó build bằng `platformio run -e esp32dev`.

## Giao thức dùng trên điện thoại

- Điện thoại gửi `join`, `predict`, `answer`, `select_char`, `shoot` hoặc `save` qua `/ws`.
- Chủ phòng mở biểu tượng góc trên hoặc `/host`, nhập `HOST_PIN` trong `config.h`, rồi gửi `host_start` hoặc `host_replay`.
- ESP32 phát `state` cho mỗi chuyển cảnh; WebApp dùng `remainingMs` để đếm ngược. Gói `coin_result` chốt lá cờ, `trivia_result` cho biết tỷ lệ đúng và hướng bị khóa, `characters` cùng `shot` báo kết quả nhân vật và cú sút.
- Danh tính `{name, team}` được lưu trên điện thoại để tự tham gia lại sau khi WebSocket rớt; ESP32 giữ vai trò cũ theo tên và đội.

Hai thẻ thủ môn thứ hai dùng biến thể lật và đổi màu của khung hình thủ môn gốc vì file Pixel Art nguồn chỉ có một mẫu thủ môn cho mỗi đội.
