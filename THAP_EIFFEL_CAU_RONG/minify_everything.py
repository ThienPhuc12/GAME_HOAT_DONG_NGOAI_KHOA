import re

# 1. Minify CSS
raw_css = """
*{box-sizing:border-box;margin:0;padding:0;user-select:none;-webkit-tap-highlight-color:transparent}
body{background:radial-gradient(circle at 50% 10%,#1e293b,#090d16 85%);color:#f1f5f9;font:12px system-ui,sans-serif;min-height:100vh;display:flex;flex-direction:column;align-items:center;padding:8px}
.app{width:100%;max-width:820px;display:flex;flex-direction:column;gap:8px}
header,.hud,.term{background:rgba(19,28,46,.9);border:1px solid #1e293b;border-radius:10px;padding:8px 12px;display:flex;justify-content:space-between;align-items:center}
.chip{background:#0284c722;color:#38bdf8;border:1px solid #38bdf844;font:700 11px system-ui;padding:2px 8px;border-radius:10px}
.tabs,.ctrls{display:grid;gap:6px}
.tabs{grid-template-columns:1fr 1fr}
.ctrls{grid-template-columns:1.4fr 1fr 1fr}
button{border:1px solid #1e293b;border-radius:8px;font:700 12px system-ui;cursor:pointer;display:flex;align-items:center;justify-content:center;background:#131c2e;color:#f1f5f9}
button:active{transform:scale(.97)}
.tab{padding:9px;font-size:13px;color:#94a3b8;gap:6px}
.tab.active,.btn-p{background:#0284c7;color:#fff;border-color:#38bdf8}
.stage{background:radial-gradient(circle at 50% 45%,#1a263d,#060911);border:1px solid #1e293b;border-radius:12px;padding:8px;min-height:320px;display:flex;align-items:center;justify-content:center}
.svg-art{width:100%;max-height:310px;display:none;filter:drop-shadow(0 6px 12px rgba(0,0,0,.6))}
.svg-art.active{display:block}
.slice{opacity:0;pointer-events:none}
.slice.built{opacity:1;transform:none!important;pointer-events:auto;animation:fl .55s cubic-bezier(.34,1.56,.64,1) forwards}
@keyframes fl{0%{filter:brightness(1.8) drop-shadow(0 0 14px #38bdf8)}60%{filter:brightness(1.2) drop-shadow(0 0 6px #f59e0b)}100%{filter:brightness(1) drop-shadow(0 6px 12px rgba(0,0,0,.6))}}
.eiffel-slice{transform:translateY(-85px);transition:.65s cubic-bezier(.34,1.56,.64,1),opacity .4s}
.dragon-slice{transform:translateX(-90px) scale(.96);transition:.6s cubic-bezier(.34,1.56,.64,1),opacity .35s}
.dragon-breath{animation:br 1.2s infinite alternate}
@keyframes br{to{opacity:.3;transform:scale(.85)}}
.beacon{animation:pl 1s infinite alternate}
@keyframes pl{to{opacity:.2;r:2.5}}
.hud{display:flex;flex-direction:column;gap:6px}
.row{display:flex;justify-content:space-between;width:100%;font-weight:700}
.trk{height:8px;width:100%;background:#060911;border-radius:6px;overflow:hidden;border:1px solid #1e293b}
.fill{height:100%;width:0%;background:linear-gradient(90deg,#0284c7,#38bdf8,#f59e0b);border-radius:6px;transition:width .4s}
.jmps{display:flex;gap:5px;width:100%;justify-content:center}
.jmp{flex:1;max-width:44px;background:#060911;color:#94a3b8;padding:4px 0;font-size:11px}
.jmp.active,.jmp:hover{color:#fff;background:#0284c7;border-color:#38bdf8}
.btn{padding:10px 6px;gap:5px}
.btn:hover{background:#1e293b;border-color:#38bdf8}
.btn-d{color:#fca5a5}
.btn-d:hover{background:#450a0a;border-color:#b91c1c}
.term{font-family:monospace;font-size:11px;color:#38bdf8}
@media(max-width:600px){.ctrls{grid-template-columns:1fr}.stage{min-height:250px}}
"""
css_clean = re.sub(r'\s+', '', raw_css)
css_clean = css_clean.replace(';}', '}')
print("Clean CSS length:", len(css_clean))

# 2. Minify JS
js_clean = "let cur='eiffel',stgs={eiffel:0,dragon:0},tmr=null;const $=i=>document.getElementById(i);window.setGameStage=(s,g)=>{s=(s||'eiffel').toLowerCase();g=Math.max(0,Math.min(5,+g||0));if(s!==cur)switchLandmark(s);stgs[s]=g;const p=s==='eiffel'?'eiffel':'dragon';for(let i=1;i<=5;i++){const el=$(p+'-'+i);if(el)el.classList.toggle('built',i<=g)}const pct=Math.round((g/5)*100);$('stg').textContent=`STAGE ${g}/5 (${pct}%)`;$('bar').style.width=pct+'%';$('cmd').textContent=`setGameStage(\"${s}\", ${g});`;document.querySelectorAll('.jmp').forEach((b,i)=>b.classList.toggle('active',i===g));if(g>=5&&tmr)stopAuto();};function switchLandmark(n){cur=n;$('te').classList.toggle('active',n==='eiffel');$('td').classList.toggle('active',n==='dragon');$('svg-eiffel').classList.toggle('active',n==='eiffel');$('svg-dragon').classList.toggle('active',n==='dragon');$('lmk').textContent=n==='eiffel'?'EIFFEL TOWER':'DRAGON BRIDGE';stopAuto();setGameStage(n,stgs[n]||0);}function nextBlock(){setGameStage(cur,(stgs[cur]+1)%6);}function toggleAuto(){if(tmr)return stopAuto();if(stgs[cur]>=5)setGameStage(cur,0);$('ba').classList.add('btn-p');$('ai').textContent='⏸';tmr=setInterval(()=>{stgs[cur]<5?setGameStage(cur,stgs[cur]+1):stopAuto()},650);}function stopAuto(){if(tmr){clearInterval(tmr);tmr=null;$('ba').classList.remove('btn-p');$('ai').textContent='▶';}}document.addEventListener('keydown',e=>{if(e.code==='Space'||e.code==='ArrowRight'){e.preventDefault();nextBlock()}if(e.code==='KeyR')setGameStage(cur,0);if(e.code==='KeyA')toggleAuto()});switchLandmark('eiffel');"
print("Clean JS length:", len(js_clean))
