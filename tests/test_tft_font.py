import re
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class TftFontTest(unittest.TestCase):
    def test_font_covers_question_and_scene_text(self):
        subprocess.run([sys.executable, str(ROOT / "gen_tft_font.py")], check=True)
        header = (ROOT / "tft_font_vi.h").read_text(encoding="utf-8")
        codes = {int(x, 16) for x in re.findall(r"\{0x([0-9A-F]{4}),", header)}
        text = (ROOT / "cau-hoi-van-hoa-viet-phap.md").read_text(encoding="utf-8")
        for char in text + "PHÁP VIỆT NAM CÂU HỎI ĐỘI CHIẾN THẮNG VÀOOO CẢN PHÁ":
            if char != "\n":
                self.assertIn(ord(char), codes, char)


if __name__ == "__main__":
    unittest.main()
