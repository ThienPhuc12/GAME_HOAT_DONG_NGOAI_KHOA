# generate_all.py
import re

# 1. Defs
defs = (
    '<defs>'
    '<linearGradient id="dg" x1="0" y1="0" x2="0" y2="1"><stop offset="0%" stop-color="#fef08a"/><stop offset="45%" stop-color="#fbcc02"/><stop offset="100%" stop-color="#b45309"/></linearGradient>'
    '<linearGradient id="do" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#ea580c"/><stop offset="100%" stop-color="#9a3412"/></linearGradient>'
    '<linearGradient id="dp" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#334155"/><stop offset="50%" stop-color="#64748b"/><stop offset="100%" stop-color="#1e293b"/></linearGradient>'
    '<g id="r-pr"><path d="M0 0l6 35h36l6-35z" fill="url(#dp)"/><ellipse cx="24" cy="0" rx="24" ry="4" fill="#94a3b8"/><path d="M0 0l6 35h8l-4-35z" fill="#0f172a" opacity=".4"/><path d="M16 2v33m16-33v33" stroke="#334155" stroke-width="1.2" opacity=".6"/></g>'
    '<g id="r-dk"><path d="M10 15l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9z" fill="#0b1329"/><rect y="12" width="180" height="4" fill="#1e293b"/><rect width="180" height="13" rx="2" fill="url(#do)"/><rect width="180" height="2.5" fill="#fed7aa"/><rect y="-8" width="180" height="8" fill="none" stroke="#475569" stroke-width="1.2"/><path d="M0-8h180M0-4h180" stroke="#94a3b8" stroke-width=".8"/><path d="M12-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8" stroke="#64748b" stroke-width=".8" opacity=".7"/></g>'
    '<g id="r-lp"><path d="M0 0v24M-5-2h5l2 4M5-2h-5" stroke="#cbd5e1" stroke-width="1.2" fill="none"/><circle cx="-5" cy="-2" r="1.5" fill="#fde047"/><circle cx="5" cy="-2" r="1.5" fill="#fde047"/></g>'
    '<g id="r-ar">'
    '<path d="M16 190l22-44 22 44 25-58 25 58 22-44 22 44" stroke="#94a3b8" stroke-width="1.2" opacity=".85" fill="none"/><path d="M14 190h4m40 0h4m46 0h4m42 0h4" stroke="#334155" stroke-width="2.5" stroke-linecap="round"/>'
    '<path d="M-2 205q87-68 174 0v9q-87-68-174 0z" fill="#7a3e00"/>'
    '<path d="M20 172l-12-18 15 7 3 11zM50 152l-14-20 17 8 3 12zM85 138l-15-22 18 8 3 14zM120 152l-14-20 17 8 3 12zM150 172l-12-18 15 7 3 11z" fill="#ea580c"/>'
    '<path d="M22 172l-10-18 10 6 3 12zM52 152l-12-20 11 7 3 13zM87 138l-13-22 12 7 3 15zM122 152l-12-20 11 7 3 13zM152 172l-10-18 10 6 3 12z" fill="#fbcc02"/>'
    '<path d="M16 162l5-6 2 7zM45 141l6-6 2 8zM79 125l7-7 2 9zM115 141l6-6 2 8zM146 162l5-6 2 7z" fill="#fef08a"/>'
    '<path d="M-3 194q88-75 176 0v8q-88-75-176 0z" fill="#b45309"/>'
    '<path d="M-3 186q88-77 176 0v8q-88-77-176 0z" fill="url(#dg)"/>'
    '<path d="M-2 178q87-79 174 0v8q-87-79-174 0z" fill="#fde047"/>'
    '<path d="M-2 186q87-77 174 0m-174 8q87-75 174 0" stroke="#7a3e00" stroke-width="1.2" fill="none"/>'
    '<path d="M30 170l-4 27 5 1 4-27zm38-18l-2 28 5 1 2-28zm38 0l2 28 5-1-2-28zm38 18l4 27 5-1-4-27z" fill="#7a3e00"/>'
    '<path d="M31 170l-4 25 2 1 4-25zm38-18l-2 26 2 1 2-26zm38 0l2 26 2-1-2-26zm38 18l4 25 2-1-4-25z" fill="#fbcc02"/>'
    '<path d="M15 170v.1m20-14v.1m25-13v.1m25-6v.1m25 6v.1m25 13v.1m20 14v.1" stroke="#fef08a" stroke-width="4.5" stroke-linecap="round"/>'
    '<path d="M15 170v.1m20-14v.1m25-13v.1m25-6v.1m25 6v.1m25 13v.1m20 14v.1" stroke="#ea580c" stroke-width="2" stroke-linecap="round"/>'
    '<use href="#r-lp" x="85" y="166"/>'
    '</g>'
    '</defs>'
)

# 2. Tail Block (dragon-1)
tail_block = (
    '<g id="dragon-1" class="slice dragon-slice"><use href="#r-pr" x="40" y="215"/><use href="#r-dk" x="15" y="192"/>'
    '<path d="M44 204l-14-6-12-28 6 6 8-18 4 10 12-20 2 12 11-15-2 15 9-11-3 16 10-8-5 16 8-5-4 13z" fill="#3a1504"/>'
    '<path d="M45 203l-12-5-10-25 5 5 7-16 4 9 10-18 2 11 9-13-2 13 8-10-3 14 9-7-4 14 7-4-3 11z" fill="#ea580c"/>'
    '<path d="M46 201l-10-4-8-22 4 4 6-14 3 8 9-16 2 10 8-11-2 11 7-9-2 12 8-6-4 12 6-3-3 10z" fill="#fbcc02"/>'
    '<path d="M38 174l-6-16m11 12l-4-12m11 10l-2-10m10 9l-1-8" stroke="#fef08a" stroke-width="1.2" stroke-linecap="round"/>'
    '<path d="M52 195l10-18 6 3-10 18z" fill="#7a3e00"/>'
    '<path d="M53 194l9-16 3 1-9 16z" fill="#fbcc02"/>'
    '<path d="M56 192v.1m3-6v.1m3-5v.1" stroke="#3a1504" stroke-width="2.5" stroke-linecap="round"/>'
    '<path d="M60 196q30 20 55 12 25-10 38-26v8q-15 18-40 28-26 8-53-14z" fill="#7a3e00"/>'
    '<path d="M60 190q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="#b45309"/>'
    '<path d="M60 183q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="url(#dg)"/>'
    '<path d="M60 176q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="#fde047"/>'
    '<path d="M60 183q30 20 55 12 25-10 38-26m-93 7q30 20 55 12 25-10 38-26" stroke="#7a3e00" stroke-width="1.2" fill="none"/>'
    '<path d="M96 190l-8-14 12 5 2 9zM124 176l-10-16 14 6 2 10zM144 162l-10-18 14 6 2 12z" fill="#ea580c"/>'
    '<path d="M97 190l-7-14 8 4 2 10zM125 176l-8-16 9 5 2 11zM145 162l-8-18 9 5 2 13z" fill="#fbcc02"/>'
    '<path d="M75 188v.1m20 4v.1m20-4v.1m18-10v.1m16-14v.1" stroke="#fef08a" stroke-width="4" stroke-linecap="round"/>'
    '<path d="M75 188v.1m20 4v.1m20-4v.1m18-10v.1m16-14v.1" stroke="#ea580c" stroke-width="1.8" stroke-linecap="round"/>'
    '</g>'
)

# 3. Slices 2, 3, 4, 5
s2 = '<g id="dragon-2" class="slice dragon-slice"><use href="#r-pr" x="200" y="215"/><use href="#r-dk" x="150" y="192"/><use href="#r-ar" x="150"/></g>'
s3 = '<g id="dragon-3" class="slice dragon-slice"><use href="#r-pr" x="370" y="215"/><use href="#r-dk" x="320" y="192"/><use href="#r-ar" x="320"/></g>'
s4 = '<g id="dragon-4" class="slice dragon-slice"><use href="#r-pr" x="540" y="215"/><use href="#r-dk" x="490" y="192"/><use href="#r-ar" x="490"/></g>'

with open('test_redesigned_bridge.html', 'r', encoding='utf-8') as f:
    orig = f.read()

h_s = orig.find('<g id="dragon-head-exact">')
h_e = orig.find('</g><g class="dragon-breath">') + 4
head = orig[h_s:h_e]

br = '<g class="dragon-breath"><path d="M790 105q18-6 34-14-8 16-26 22z" fill="#f97316" opacity=".85"/><circle cx="814" cy="100" r="4" fill="#ef4444" opacity=".8"/><circle cx="826" cy="94" r="2.5" fill="#fbbf24" opacity=".9"/></g>'
s5 = f'<g id="dragon-5" class="slice dragon-slice"><use href="#r-pr" x="705" y="215"/><use href="#r-dk" x="650" y="192"/>{head}{br}</g>'
water = '<path d="M0 240h800v60H0z" fill="#0b172a"/><path d="M0 240h800" stroke="#0284c7" stroke-width="2" stroke-dasharray="10 6" opacity=".5"/>'
dragon_svg = f'<svg id="svg-dragon" class="svg-art" viewBox="0 0 800 300">{defs}{water}{tail_block}{s2}{s3}{s4}{s5}</svg>'

# 4. Eiffel Tower
e_s = orig.find('<svg id="svg-eiffel"')
e_e = orig.find('</svg>', e_s) + 6
eiffel = orig[e_s:e_e]
eiffel = re.sub(r'>\s+<', '><', eiffel)

# 5. Clean CSS
css = (
    "*:focus{outline:none}*{box-sizing:border-box;margin:0;padding:0;user-select:none}"
    "body,header,.hud,.term,button,.stage{display:flex;align-items:center}"
    "body{flex-direction:column;padding:8px;min-height:100vh;background:radial-gradient(circle at 50% 10%,#1e293b,#090d16 85%);color:#f1f5f9;font:12px system-ui}"
    ".app{width:100%;max-width:820px;display:flex;flex-direction:column;gap:8px}"
    "header,.hud,.term{justify-content:space-between;background:rgba(19,28,46,.9);border:1px solid #1e293b;border-radius:10px;padding:8px 12px}"
    ".chip{background:#0284c722;color:#38bdf8;border:1px solid #38bdf844;font:700 11px system-ui;padding:2px 8px;border-radius:10px}"
    ".tabs,.ctrls{display:grid;gap:6px}"
    ".tabs{grid-template-columns:1fr 1fr}"
    ".ctrls{grid-template-columns:1.4fr 1fr 1fr}"
    "button,.stage{justify-content:center}"
    "button{border:1px solid #1e293b;border-radius:8px;font:700 12px system-ui;cursor:pointer;background:#131c2e;color:#f1f5f9}"
    "button:active{transform:scale(.97)}"
    ".tab{padding:9px;font-size:13px;color:#94a3b8;gap:6px}"
    ".tab.active,.btn-p,.jmp.active,.jmp:hover{background:#0284c7;color:#fff;border-color:#38bdf8}"
    ".stage{background:radial-gradient(circle at 50% 45%,#1a263d,#060911);border:1px solid #1e293b;border-radius:12px;padding:8px;min-height:320px}"
    ".svg-art{width:100%;max-height:310px;display:none;filter:drop-shadow(0 6px 12px rgba(0,0,0,.6))}"
    ".svg-art.active{display:block}"
    ".slice{opacity:0;pointer-events:none}"
    ".slice.built{opacity:1;transform:none!important;pointer-events:auto;animation:fl .55s cubic-bezier(.34,1.56,.64,1) forwards}"
    "@keyframes fl{0%{filter:brightness(1.8) drop-shadow(0 0 14px #38bdf8)}60%{filter:brightness(1.2) drop-shadow(0 0 6px #f59e0b)}100%{filter:brightness(1) drop-shadow(0 6px 12px rgba(0,0,0,.6))}}"
    ".eiffel-slice{transform:translateY(-85px);transition:.65s cubic-bezier(.34,1.56,.64,1),opacity .4s}"
    ".dragon-slice{transform:translateX(-90px) scale(.96);transition:.6s cubic-bezier(.34,1.56,.64,1),opacity .35s}"
    ".dragon-breath{animation:br 1.2s infinite alternate}"
    "@keyframes br{to{opacity:.3;transform:scale(.85)}}"
    ".beacon{animation:pl 1s infinite alternate}"
    "@keyframes pl{to{opacity:.2;r:2.5}}"
    ".hud{flex-direction:column;gap:6px}"
    ".row{display:flex;justify-content:space-between;width:100%;font-weight:700}"
    ".trk{height:8px;width:100%;background:#060911;border-radius:6px;overflow:hidden;border:1px solid #1e293b}"
    ".fill{height:100%;width:0%;background:linear-gradient(90deg,#0284c7,#38bdf8,#f59e0b);border-radius:6px;transition:width .4s}"
    ".jmps{display:flex;gap:5px;width:100%;justify-content:center}"
    ".jmp{flex:1;max-width:44px;background:#060911;color:#94a3b8;padding:4px 0;font-size:11px}"
    ".btn{padding:10px 6px;gap:5px}"
    ".btn:hover{background:#1e293b;border-color:#38bdf8}"
    ".btn-d{color:#fca5a5}"
    ".btn-d:hover{background:#450a0a;border-color:#b91c1c}"
    ".term{font-family:monospace;font-size:11px;color:#38bdf8}"
    "@media(max-width:600px){.ctrls{grid-template-columns:1fr}.stage{min-height:250px}}"
)

# 6. Clean JS
js = (
    "let cur='eiffel',stgs={eiffel:0,dragon:0},tmr=null;"
    "const $=i=>document.getElementById(i);"
    "window.setGameStage=(s,g)=>{"
    "s=(s||'eiffel').toLowerCase();g=Math.max(0,Math.min(5,+g||0));"
    "if(s!==cur)switchLandmark(s);"
    "stgs[s]=g;"
    "const p=s==='eiffel'?'eiffel':'dragon';"
    "for(let i=1;i<=5;i++){const el=$(p+'-'+i);if(el)el.classList.toggle('built',i<=g)}"
    "const pct=Math.round((g/5)*100);"
    "$('stg').textContent=`STAGE ${g}/5 (${pct}%)`;$('bar').style.width=pct+'%';"
    "$('cmd').textContent=`setGameStage(\"${s}\", ${g});`;"
    "document.querySelectorAll('.jmp').forEach((b,i)=>b.classList.toggle('active',i===g));"
    "if(g>=5&&tmr)stopAuto();"
    "};"
    "function switchLandmark(n){"
    "cur=n;$('te').classList.toggle('active',n==='eiffel');$('td').classList.toggle('active',n==='dragon');"
    "$('svg-eiffel').classList.toggle('active',n==='eiffel');$('svg-dragon').classList.toggle('active',n==='dragon');"
    "$('lmk').textContent=n==='eiffel'?'EIFFEL TOWER':'DRAGON BRIDGE';stopAuto();setGameStage(n,stgs[n]||0);"
    "}"
    "function nextBlock(){setGameStage(cur,(stgs[cur]+1)%6);}"
    "function toggleAuto(){"
    "if(tmr)return stopAuto();if(stgs[cur]>=5)setGameStage(cur,0);$('ba').classList.add('btn-p');$('ai').textContent='⏸';"
    "tmr=setInterval(()=>{stgs[cur]<5?setGameStage(cur,stgs[cur]+1):stopAuto()},650);"
    "}"
    "function stopAuto(){if(tmr){clearInterval(tmr);tmr=null;$('ba').classList.remove('btn-p');$('ai').textContent='▶';}}"
    "document.addEventListener('keydown',e=>{"
    "if(e.code==='Space'||e.code==='ArrowRight'){e.preventDefault();nextBlock()}"
    "if(e.code==='KeyR')setGameStage(cur,0);if(e.code==='KeyA')toggleAuto();"
    "});switchLandmark('eiffel');"
)

jmps = '<div class="jmps" onclick="const b=event.target.closest(\'.jmp\');if(b)setGameStage(cur,+b.textContent)"><button class="jmp active">0</button><button class="jmp">1</button><button class="jmp">2</button><button class="jmp">3</button><button class="jmp">4</button><button class="jmp">5</button></div>'

html = f'<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0"><title>ESP32 Landmark Builder</title><style>{css}</style></head><body><div class="app"><header><div><strong>ESP32 LANDMARK BUILDER</strong><div style="font-size:10px;color:#94a3b8">PROGMEM Inline SVG Engine</div></div><span class="chip">&lt;15KB OK • API READY</span></header><div class="tabs"><button id="te" class="tab active" onclick="switchLandmark(\'eiffel\')">🗼 Eiffel Tower</button><button id="td" class="tab" onclick="switchLandmark(\'dragon\')">🐉 Dragon Bridge</button></div><div class="stage">{eiffel}{dragon_svg}</div><div class="hud"><div class="row"><span id="lmk">EIFFEL TOWER</span><span id="stg" style="color:#38bdf8">STAGE 0/5 (0%)</span></div><div class="trk"><div id="bar" class="fill"></div></div>{jmps}</div><div class="ctrls"><button class="btn btn-p" onclick="nextBlock()">➕ Next Block (+1)</button><button id="ba" class="btn" onclick="toggleAuto()"><span id="ai">▶</span> Auto Build</button><button class="btn btn-d" onclick="setGameStage(cur,0)">↺ Reset</button></div><div class="term"><div>ESP32 HOOK: <span id="cmd" style="color:#facc15">setGameStage("eiffel", 0);</span></div><div style="color:#94a3b8">WS / SERIAL READY</div></div></div><script>{js}</script></body></html>'

size = len(html.encode('utf-8'))
print(f"Total HTML size: {size} bytes ({size/1024:.2f} KB)")
if size <= 15360:
    print(f"BINGO! Under 15KB with {15360 - size} bytes to spare!")
    with open('index.html', 'w', encoding='utf-8') as f:
        f.write(html)
else:
    print(f"Needs {size - 15360} bytes reduced")
