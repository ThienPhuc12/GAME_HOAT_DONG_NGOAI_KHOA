import re
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class PixelAssetsTest(unittest.TestCase):
    def test_generated_frames_match_source(self):
        subprocess.run(["node", str(ROOT / "gen_pixel_assets.js")], cwd=ROOT, check=True)
        header = (ROOT / "pixel_assets.h").read_text(encoding="utf-8")
        self.assertEqual(len(re.findall(r"static const uint8_t frame_\d+_\d+\[3840\] PROGMEM", header)), 96)
        self.assertIn("PLAYER_MODEL_COUNT = 8", header)
        self.assertIn("PLAYER_FRAME_COUNT = 12", header)
        self.assertIn("SHOT_CONTACT_MS = 645", header)
        self.assertIn("KEEPER_CONTACT_MS = 610", header)


if __name__ == "__main__":
    unittest.main()
