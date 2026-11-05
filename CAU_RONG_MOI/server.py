"""
ESP32 Landmark Builder - Python Server & API Backend
---------------------------------------------------
File này giúp chạy ứng dụng web cục bộ và cung cấp API điều khiển
trạng thái (stage) cho các công trình (Eiffel Tower & Dragon Bridge).

Cách chạy:
    python server.py

Sau đó mở trình duyệt tại: http://localhost:8000
"""

import http.server
import socketserver
import json
from urllib.parse import urlparse, parse_qs

PORT = 8000
DIRECTORY = "."

# Lưu trữ trạng thái hiện tại của mô hình
STATE = {
    "current_landmark": "eiffel",
    "eiffel_stage": 0,
    "dragon_stage": 0
}

class LandmarkRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        parsed = urlparse(self.path)
        
        # API lấy trạng thái hiện tại: GET /api/status
        if parsed.path == "/api/status":
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()
            self.wfile.write(json.dumps(STATE).encode("utf-8"))
            return
            
        return super().do_GET()

    def do_POST(self):
        parsed = urlparse(self.path)

        # API cập nhật trạng thái: POST /api/set-stage
        # Body JSON: {"landmark": "eiffel" | "dragon", "stage": 0..5}
        if parsed.path == "/api/set-stage":
            content_length = int(self.headers.get("Content-Length", 0))
            body = self.rfile.read(content_length).decode("utf-8")
            try:
                data = json.loads(body)
                lmk = data.get("landmark", "eiffel").lower()
                stg = max(0, min(5, int(data.get("stage", 0))))

                STATE["current_landmark"] = lmk
                if lmk == "eiffel":
                    STATE["eiffel_stage"] = stg
                elif lmk == "dragon":
                    STATE["dragon_stage"] = stg

                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.end_headers()
                self.wfile.write(json.dumps({"status": "success", "state": STATE}).encode("utf-8"))
            except Exception as e:
                self.send_response(400)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                self.wfile.write(json.dumps({"error": str(e)}).encode("utf-8"))
            return

        self.send_response(404)
        self.end_headers()


if __name__ == "__main__":
    Handler = LandmarkRequestHandler
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        print(f"==================================================")
        print(f"  Landmark Builder Server đang chạy tại:")
        print(f"  👉 http://localhost:{PORT}")
        print(f"  Ấn Ctrl+C để dừng server.")
        print(f"==================================================")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nĐã tắt server.")

