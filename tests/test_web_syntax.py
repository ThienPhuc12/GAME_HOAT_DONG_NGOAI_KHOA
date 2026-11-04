import re
import shutil
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class WebSyntaxTest(unittest.TestCase):
    def test_embedded_javascript_parses(self):
        node = shutil.which("node")
        if not node:
            self.skipTest("Node.js is not installed")
        source = (ROOT / "web_player.h").read_text(encoding="utf-8")
        script = re.search(r"<script>(.*?)</script>", source, re.S)
        self.assertIsNotNone(script)
        result = subprocess.run([node, "--check"], input=script[1], text=True,
                                encoding="utf-8", capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
