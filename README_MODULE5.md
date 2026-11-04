# Module 5 — Điều khiển và đấu tập bằng điện thoại

Sau khi nạp firmware, kết nối điện thoại vào Wi-Fi `VIET-FRANCE-PENALTY` và mở `http://192.168.4.1`. Nhập tên, chọn đội, rồi bấm biểu tượng ⚙️ để mở thanh Chủ phòng. Điện thoại đầu tiên tham gia tự có quyền Host; điện thoại khác nhập mã `HOST_PIN` trong `config.h` để dùng các nút điều khiển.

## Chơi một mình

1. Vào một đội bằng điện thoại.
2. Trong menu ⚙️, bật **Bot Đấu Tập & Khán Giả Ảo**. ESP32 thêm một Bot Người chơi chính ở đội kia và bốn khán giả ảo, hai người mỗi đội.
3. Bấm **Bắt đầu trận đấu**. Bot và khán giả ảo trả lời câu hỏi; Bot chọn nhân vật và hướng sút/bắt. Điện thoại của bạn điều khiển đội đã chọn.

Có thể bật/tắt Bot tại sảnh chờ. Khi người thật vào đội của Bot, người thật nhận ngay vai trò Người chơi chính. Menu cũng có Reset bất cứ lúc nào, thử khóa một góc, cộng một bàn cho mỗi đội và nhảy thẳng tới lễ trao giải.

Trình duyệt lưu `playerId`, `playerName`, `team`, `role` trong `localStorage`. WebSocket gửi heartbeat hai giây một lần, tự kết nối lại khi Captive Portal bị tạm dừng và gửi lại `playerId` để ESP32 khôi phục vai trò. Nếu điện thoại xóa dữ liệu trang, ESP32 không thể nhận diện lại điện thoại cũ; trong trường hợp đó, dùng một tên mới hoặc Reset firmware để bắt đầu phòng mới.
