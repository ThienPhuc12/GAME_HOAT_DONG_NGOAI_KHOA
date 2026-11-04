import re
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class WebPackageTest(unittest.TestCase):
    def test_self_contained_html_round_trips_into_progmem(self):
        source = ROOT / "web" / "player.html"
        html = source.read_text(encoding="utf-8")
        self.assertEqual(html.count("<script>"), 1)
        self.assertEqual(html.count("<style>"), 1)
        self.assertNotRegex(html, r"<(?:script|link)[^>]+(?:src|href)=")
        self.assertIn("el('canvas')", html)
        with tempfile.TemporaryDirectory() as d:
            output = Path(d) / "web_player.h"
            subprocess.run([sys.executable, str(ROOT / "gen_web_h.py"),
                            "--source", str(source), "--output", str(output)], check=True)
            header = output.read_text(encoding="utf-8")
            self.assertIn("PROGMEM", header)
            self.assertIn(html, header)
            self.assertEqual(header, (ROOT / "web_player.h").read_text(encoding="utf-8"))


if __name__ == "__main__":
    unittest.main()
