"""Exercise the offline stadium synthesizer without a sound device."""

import shutil
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class WebAudioTest(unittest.TestCase):
    def test_four_effects_schedule_and_stop_audio_nodes(self):
        node = shutil.which("node")
        if not node:
            self.skipTest("Node.js is not installed")
        script = r"""
const fs=require('fs'),vm=require('vm'),assert=require('assert');
const html=fs.readFileSync('web/player.html','utf8');
const start=html.indexOf('class StadiumAudioEngine{');
const end=html.indexOf('const stadiumAudio=',start);
assert(start>=0&&end>start);
const events=[],sources=[];
class Param{
  constructor(){this.value=0}
  setValueAtTime(v,t){events.push(['set',v,t]);return this}
  linearRampToValueAtTime(v,t){events.push(['linear',v,t]);return this}
  exponentialRampToValueAtTime(v,t){events.push(['exponential',v,t]);return this}
}
class Node{
  constructor(){this.gain=new Param();this.frequency=new Param();this.Q=new Param();
    this.threshold=new Param();this.knee=new Param();this.ratio=new Param();sources.push(this)}
  connect(){return this}
  start(t){this.started=t}
  stop(t){this.stopped=true;this.stopAt=t}
}
class Context{
  constructor(){this.state='suspended';this.currentTime=1;this.sampleRate=8000;this.destination=new Node()}
  resume(){this.state='running';return Promise.resolve()}
  createGain(){return new Node()}
  createDynamicsCompressor(){return new Node()}
  createBufferSource(){return new Node()}
  createBiquadFilter(){return new Node()}
  createOscillator(){return new Node()}
  createBuffer(channels,length){return {getChannelData(){return new Float32Array(length)}}}
}
let timer=0;
const scope={window:{AudioContext:Context},setInterval:()=>++timer,clearInterval:()=>{timer=0}};
vm.runInNewContext(html.slice(start,end)+';this.Engine=StadiumAudioEngine',scope);
(async()=>{
  const audio=new scope.Engine();await audio.unlock();assert(audio.ready());
  audio.playMissOrSaveSound();audio.playGoalCelebrationSound();
  audio.playTrophyCeremonySound();audio.playSuspenseCountdownTick(3);
  audio.playSuspenseCountdownTick(0);audio.startCeremony();
  assert(timer>0);assert(audio.sources.size>30);
  assert(events.some(e=>e[1]===600));assert(events.some(e=>e[1]===180));
  assert(events.some(e=>e[1]===2800));assert(events.some(e=>e[1]===3400));
  const active=[...audio.sources];audio.stopAll();
  assert.equal(timer,0);assert.equal(audio.sources.size,0);
  assert(active.every(source=>source.stopped===true));
})().catch(error=>{console.error(error);process.exitCode=1});
"""
        result = subprocess.run([node, "-e", script], cwd=ROOT,
                                capture_output=True, text=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
