import base64
import json
import re
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class WebSpritesTest(unittest.TestCase):
    def test_inline_frames_are_complete_and_reproducible(self):
        source = ROOT / "web" / "player.html"
        before = source.read_bytes()
        subprocess.run(["node", str(ROOT / "gen_web_sprites.js")], cwd=ROOT, check=True)
        self.assertEqual(source.read_bytes(), before)
        html = before.decode("utf-8")
        match = re.search(r"const SPRITE_DATA = (\{.*?\});\n  // PIXEL_ASSETS_END", html, re.S)
        self.assertIsNotNone(match)
        sprites = json.loads(match[1])
        self.assertEqual(set(sprites), {"vn", "cong_phuong", "fr", "zidane", "van_lam", "lloris"})
        for actor in sprites.values():
            self.assertEqual(len(actor["frames"]), 12)
            self.assertGreaterEqual(len(actor["pal"]), 30)
            self.assertTrue(all(len(base64.b64decode(frame)) == 540 for frame in actor["frames"]))


if __name__ == "__main__":
    unittest.main()
