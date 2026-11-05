"""Check the goal and catch paths with the embedded SVG animation engine."""

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
const start=html.indexOf('function zoneCenter('),end=html.indexOf('function actorUse(',start);
assert(start>=0&&end>start);
const scope={};vm.runInNewContext(html.slice(start,end)+';this.geometry=shotGeometry;this.center=zoneCenter',scope);
for(let zone=0;zone<6;zone++){
  const other=(zone+1)%6,miss=(zone+2)%6;
  const before=scope.geometry(645+520*.75,zone,other,zone);
  assert.equal(before.caught,false);
  const caught=scope.geometry(645+520*.85,zone,other,zone);
  assert.equal(caught.caught,true);
  assert.equal(caught.ballX,caught.keeperCenterX);
  assert.equal(caught.ballY,caught.keeperChestY);
  const held=scope.geometry(3500,zone,other,zone);
  assert.equal(held.ballX,held.keeperCenterX);
  assert.equal(held.ballY,held.keeperChestY);
  assert.equal(held.ballX,caught.ballX);
  const goal=scope.geometry(3500,zone,other,miss);
  assert.equal(goal.caught,false);
  assert.equal(goal.ballX,scope.center(zone).x);
  assert.equal(goal.ballY,scope.center(zone).y);
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
const html=fs.readFileSync('cau-thu-pixel-esp32-moi.html','utf8');
const source=html.split('<script>')[1].split('const viewNames')[0];
const scope={};vm.runInNewContext(source+';this.getState=getKeeperHugState',scope);
for(const ms of [650,850,1200,1500]){
  const state=scope.getState(ms);
  assert.equal(state.caught,true);
  assert.equal(state.ball.x,120);
  assert(state.ball.x>state.leftHand[0]);
  assert(state.ball.x<state.rightHand[0]);
  assert(Math.abs(state.ball.y-state.leftHand[1])<=5);
}
"""
        result = subprocess.run([node, "-e", script], cwd=ROOT,
                                capture_output=True, text=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
