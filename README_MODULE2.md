# Module 2 · ESP32 backend

## Build and upload

Use PlatformIO from this directory:

```text
pio run -e esp32dev
pio run -e esp32dev -t upload
```

The build uses `esp32dev`, Arduino-ESP32 3.x, the Huge APP partition
(3 MB application / 1 MB filesystem), ESPAsyncWebServer, ArduinoJson 6,
LovyanGFX, and U8g2. `firmware/main.cpp` is the PlatformIO entry point for
the root `VIET_FRANCE_PENALTY.ino` source. No PSRAM is required.

Default OLED driver is SH1106. Add `-D USE_SSD1306` to `build_flags` in
`platformio.ini` for an SSD1306 display. Set the host PIN in `config.h` before
uploading.

Connect a phone to `VIET-FRANCE-PENALTY`, then open `http://192.168.4.1/`.
The host uses `http://192.168.4.1/host`. The AP is open; the host START
command requires the PIN.

## WebSocket packets

Client to ESP32: `join`, `predict`, `host_start`, `answer`, `select_char`,
`shoot`, and `save`. All use `/ws`. Team 0 is France; team 1 is Việt Nam.
Direction 0/1/2 means left/center/right. Character IDs are 0 or 1.

ESP32 to clients: `joined`, `state`, `predict_prompt`, `predict_ack`,
`answer_ack`, `coin_toss`, `coin_result`, `trivia_result`, `characters`,
`shot`, and `error`. A reconnect receives its restored role and current
state. The first valid answer for each member slot counts; registered
members who do not answer remain in the denominator. The game accepts
up to 12 registered players and requires one main player on each team
before START.

The two displays are implemented in `display_tft.h` and `display_oled.h`.
See `README_MODULE3.md` for artwork generation and display memory details.
