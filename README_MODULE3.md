# Module 3 · Two physical displays

`display_oled.h` uses the 128×64 U8g2 full monochrome buffer (1024 bytes).
The central score uses the 38-pixel number font. The lower line shows the
current shooting team, construction progress, or the winner.

`display_tft.h` renders seven scenes to one 240×40 RGB565 LovyanGFX sprite
(19,200 pixel bytes). It pushes six strips per frame and never creates a
240×240 framebuffer. The backend calls it at a 40 ms frame interval.

`pixel_assets.h` contains 96 original action frames (8 models × 12 frames),
packed at 5 bits/pixel in flash. Each frame is 3,840 bytes. To regenerate
after editing the HTML model, run:

```text
node gen_pixel_assets.js
```

`tft_font_vi.h` contains a small bitmap font in flash for Vietnamese
questions and member names. To regenerate it after editing the Markdown
questions, install Pillow and run:

```text
python gen_tft_font.py
```

The source HTML supplies one goalkeeper model per country. The `charId` 1
goalkeeper choice uses a mirrored pose of that country's model; the
shooter choices select distinct source models.

The TFT uses direct pixel art for the Eiffel Tower, Dragon Bridge, goal,
coin, shielded lane, construction pieces, and fireworks. Physical colors,
brightness, and achieved frame rate still require a check on the actual
GC9A01 and OLED modules.
