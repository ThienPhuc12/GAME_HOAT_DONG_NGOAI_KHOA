import json
import re
import shutil
import subprocess
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class FlagRenderTest(unittest.TestCase):
    def test_vietnam_star_is_centered_and_france_has_equal_stripes(self):
        node = shutil.which("node")
        if not node:
            self.skipTest("Node.js is not installed")
        source = (ROOT / "web" / "player.html").read_text(encoding="utf-8")
        pixel = re.search(r"function pixelRect\([^\n]+", source)
        flag = re.search(r"function drawFlag\([\s\S]*?\n  }(?=\n  function drawEiffel)", source)
        self.assertIsNotNone(pixel)
        self.assertIsNotNone(flag)
        js = f"""
{pixel.group(0)}
{flag.group(0)}
const rects=[], points=[];
const ctx={{fillStyle:'',strokeStyle:'',beginPath(){{}},closePath(){{}},fill(){{}},stroke(){{}},
  fillRect(x,y,w,h){{rects.push([x,y,w,h,this.fillStyle])}},
  moveTo(x,y){{points.push([x,y])}},lineTo(x,y){{points.push([x,y])}},
  strokeRect(){{}},fillText(){{}}}};
drawFlag(ctx,1,10,20,90,60);
const vietnam={{rects:rects.slice(),points:points.slice(),starColor:ctx.fillStyle}};
rects.length=0;points.length=0;
drawFlag(ctx,0,10,20,90,60);
process.stdout.write(JSON.stringify({{vietnam,france:rects}}));
"""
        result = subprocess.run([node, "-e", js], text=True, encoding="utf-8",
                                capture_output=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)
        flags = json.loads(result.stdout)
        vn = flags["vietnam"]
        self.assertIn([10, 20, 90, 60, "#DA251D"], vn["rects"])
        self.assertEqual(vn["starColor"], "#FFFF00")
        self.assertEqual(len(vn["points"]), 10)
        self.assertAlmostEqual(vn["points"][0][0], 55)
        self.assertAlmostEqual(vn["points"][0][1], 32)
        fr = flags["france"]
        self.assertIn([10, 20, 30, 60, "#002395"], fr)
        self.assertIn([40, 20, 30, 60, "#FFFFFF"], fr)
        self.assertIn([70, 20, 30, 60, "#ED2939"], fr)


if __name__ == "__main__":
    unittest.main()
