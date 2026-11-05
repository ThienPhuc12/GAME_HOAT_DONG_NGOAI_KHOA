# ESP32 Landmark Builder Web App

Dự án giao diện mô phỏng lắp ghép công trình (Tháp Eiffel & Cầu Rồng Đà Nẵng) với hiệu ứng đồ họa SVG, được tối ưu cho vi điều khiển ESP32 và các ứng dụng web/desktop.

## Cấu trúc thư mục

- `index.html`: Giao diện chính đã được bóc tách và định dạng chuẩn (HTML + SVG).
- `style.css`: Toàn bộ định dạng giao diện, hiệu ứng chuyển động (animations) và responsive.
- `script.js`: Toàn bộ logic tương tác, chuyển đổi công trình, nút Auto Build, phím tắt và hook API điều khiển.
- `server.py`: Backend Python mẫu tích hợp HTTP server và REST API `/api/status`, `/api/set-stage`.
- `index.esp32.html`: Bản gốc All-in-one (HTML + CSS + JS + SVG nén chung 1 file) chuyên dùng để nhúng trực tiếp vào bộ nhớ flash `PROGMEM` của ESP32.
- `Caurong.png` & `eiffel-tower-paris-in-national-colors-symbol-2M6W4YH.jpg`: Hình ảnh tư liệu gốc dùng để thiết kế đồ họa vector SVG.

## Cách chạy thử với Python
```bash
python server.py
```
Truy cập: `http://localhost:8000`

