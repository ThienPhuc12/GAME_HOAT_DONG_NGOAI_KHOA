import re
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class WebSpritesTest(unittest.TestCase):
    def test_new_svg_models_and_animation_are_embedded_reproducibly(self):
        source = ROOT / "web" / "player.html"
        before = source.read_bytes()
        subprocess.run(["python", str(ROOT / "gen_actor_assets.py")], cwd=ROOT, check=True)
        self.assertEqual(source.read_bytes(), before)
        html = before.decode("utf-8")
        models = re.search(r"<!-- ACTOR_MODELS_START -->(.*?)<!-- ACTOR_MODELS_END -->", html, re.S)
        self.assertIsNotNone(models)
        for actor in ("quang-hai", "cong-phuong", "van-lam", "mbappe", "zidane", "lloris"):
            for view in ("front", "back"):
                self.assertIn(f'id="{actor}-{view}"', models[1])
        self.assertIn("function renderKeeperHug", html)
        self.assertIn("function renderRestoredShot", html)
        self.assertNotIn("const SPRITE_DATA", html)
        self.assertNotIn("function drawActor", html)


if __name__ == "__main__":
    unittest.main()
