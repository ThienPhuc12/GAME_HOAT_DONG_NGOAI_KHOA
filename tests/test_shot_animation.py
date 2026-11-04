"""Check the shared Canvas flight geometry for all three save directions."""

import shutil
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class ShotAnimationTest(unittest.TestCase):
    def test_saves_finish_on_keeper_chest_and_goals_finish_in_net(self):
        node = shutil.which("node")
        if not node:
            self.skipTest("Node.js is not installed")
        script = r"""
const fs=require('fs'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync('web/player.html','utf8');
const start=html.indexOf('function shotGeometry('),end=html.indexOf('function drawPixelBall(',start);
assert(start>=0&&end>start);
const scope={};vm.runInNewContext(html.slice(start,end)+';this.geometry=shotGeometry',scope);
for(let lane=0;lane<3;lane++){
  const before=scope.geometry(645+520*.75,lane,lane);
  assert.equal(before.caught,false);
  const caught=scope.geometry(645+520*.85,lane,lane);
  assert.equal(caught.caught,true);
  assert.equal(caught.ballX,caught.keeperCenterX);
  assert.equal(caught.ballY,caught.keeperChestY);
  const held=scope.geometry(3500,lane,lane);
  assert.equal(held.ballX,held.keeperCenterX);
  assert.equal(held.ballY,held.keeperChestY);
  assert.equal(held.ballX,caught.ballX);
  const goal=scope.geometry(3500,lane,(lane+1)%3);
  assert.equal(goal.caught,false);
  assert.equal(goal.ballX,[83,180,277][lane]);
  assert.equal(goal.ballY,111);
}
"""
        result = subprocess.run([node, "-e", script], cwd=ROOT,
                                capture_output=True, text=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_source_keeper_holds_ball_after_contact(self):
        node = shutil.which("node")
        if not node:
            self.skipTest("Node.js is not installed")
        script = r"""
const fs=require('fs'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync('cau-thu-pixel-esp32.html','utf8');
const source=html.split('<script>')[1].split('// ---- UI / preview scene.')[0];
const scope={};scope.globalThis=scope;vm.runInNewContext(source,scope);
for(const ms of [610,850,1200,1500]){
  const state=scope.PIXEL_PLAYERS.getKeeperState(ms);
  assert.equal(state.ball.held,true);
  assert.equal(state.ball.visible,true);
  assert.equal(state.ball.x,state.originX+13);
  assert.equal(state.ball.y,state.originY+22);
}
"""
        result = subprocess.run([node, "-e", script], cwd=ROOT,
                                capture_output=True, text=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
