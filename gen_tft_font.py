"""Generate the tiny Vietnamese TFT font from Pillow's DejaVu Sans.

Usage: python gen_tft_font.py
The generated header is self-contained and needs no font files at firmware build.
"""
from pathlib import Path
import unicodedata
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parent
SOURCE = (ROOT / "cau-hoi-van-hoa-viet-phap.md").read_text(encoding="utf-8")
LABELS = "PHÁP VIỆT NAM CÂU HỎI ĐỘI CHIẾN THẮNG VÀOOO CẢN PHÁ THÁP EIFFEL CẦU RỒNG TRƯỚC"
chars = set(SOURCE + LABELS + ''.join(chr(i) for i in range(32, 127)))
for cp in range(0x00C0, 0x1F00):
    ch = chr(cp)
    base = unicodedata.normalize("NFD", ch)[0]
    if base in "AaEeIiOoUuYy" and unicodedata.category(ch).startswith("L"):
        chars.add(ch)
chars.update("Đđ")
chars.discard("\n")
chars.discard("\r")
chars.discard("\t")
codes = sorted(ord(ch) for ch in chars if ord(ch) <= 0xFFFF)
font = ImageFont.truetype("DejaVuSans.ttf", 13)
out = [
    "#pragma once", "#include <stdint.h>", "#include <pgmspace.h>",
    "// Generated from Markdown and DejaVu Sans by gen_tft_font.py.",
    "struct TftGlyph { uint16_t codepoint; uint8_t advance; uint16_t rows[16]; };",
    "static const TftGlyph TFT_FONT_GLYPHS[] PROGMEM = {",
]
for cp in codes:
    ch = chr(cp)
    image = Image.new("1", (14, 16), 0)
    ImageDraw.Draw(image).text((1, 12), ch, font=font, fill=1, anchor="ls")
    rows = []
    for y in range(16):
        bits = 0
        for x in range(14):
            if image.getpixel((x, y)):
                bits |= 1 << (13 - x)
        rows.append(bits)
    advance = max(3, min(14, round(font.getlength(ch)) + 1))
    out.append("  {0x%04X, %d, {%s}}," % (cp, advance, ", ".join("0x%04X" % v for v in rows)))
out += ["};", f"static constexpr uint16_t TFT_FONT_GLYPH_COUNT = {len(codes)};", ""]
(ROOT / "tft_font_vi.h").write_text("\n".join(out), encoding="utf-8", newline="\n")
print(f"Generated {len(codes)} glyphs")
