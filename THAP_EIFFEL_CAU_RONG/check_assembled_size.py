from test_size_optimization import ar
from test_tail_optimization import tail_block
from compress_to_target import css_min, js_min, eiffel, head

g = '<linearGradient id="dg" x1="0" y1="0" x2="0" y2="1"><stop offset="0%" stop-color="#fef08a"/><stop offset="45%" stop-color="#fbcc02"/><stop offset="100%" stop-color="#b45309"/></linearGradient><linearGradient id="do" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#ea580c"/><stop offset="100%" stop-color="#9a3412"/></linearGradient><linearGradient id="dp" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#334155"/><stop offset="50%" stop-color="#64748b"/><stop offset="100%" stop-color="#1e293b"/></linearGradient>'
pr = '<g id="r-pr"><path d="M0 0l6 35h36l6-35z" fill="url(#dp)"/><ellipse cx="24" cy="0" rx="24" ry="4" fill="#94a3b8"/><path d="M0 0l6 35h8l-4-35z" fill="#0f172a" opacity=".4"/><path d="M16 2v33m16-33v33" stroke="#334155" stroke-width="1.2" opacity=".6"/></g>'
dk = '<g id="r-dk"><path d="M10 15l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9z" fill="#0b1329"/><rect y="12" width="180" height="4" fill="#1e293b"/><rect width="180" height="13" rx="2" fill="url(#do)"/><rect width="180" height="2.5" fill="#fed7aa"/><rect y="-8" width="180" height="8" fill="none" stroke="#475569" stroke-width="1.2"/><path d="M0-8h180M0-4h180" stroke="#94a3b8" stroke-width=".8"/><path d="M12-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8" stroke="#64748b" stroke-width=".8" opacity=".7"/></g>'
lp = '<g id="r-lp"><path d="M0 0v24M-5-2h5l2 4M5-2h-5" stroke="#cbd5e1" stroke-width="1.2" fill="none"/><circle cx="-5" cy="-2" r="1.5" fill="#fde047"/><circle cx="5" cy="-2" r="1.5" fill="#fde047"/></g>'

defs = f'<defs>{g}{pr}{dk}{lp}{ar}</defs>'

s2 = '<g id="dragon-2" class="slice dragon-slice"><use href="#r-pr" x="200" y="215"/><use href="#r-dk" x="150" y="192"/><use href="#r-ar" x="150"/></g>'
s3 = '<g id="dragon-3" class="slice dragon-slice"><use href="#r-pr" x="370" y="215"/><use href="#r-dk" x="320" y="192"/><use href="#r-ar" x="320"/></g>'
s4 = '<g id="dragon-4" class="slice dragon-slice"><use href="#r-pr" x="540" y="215"/><use href="#r-dk" x="490" y="192"/><use href="#r-ar" x="490"/></g>'
br = '<g class="dragon-breath"><path d="M790 105q18-6 34-14-8 16-26 22z" fill="#f97316" opacity=".85"/><circle cx="814" cy="100" r="4" fill="#ef4444" opacity=".8"/><circle cx="826" cy="94" r="2.5" fill="#fbbf24" opacity=".9"/></g>'
s5 = f'<g id="dragon-5" class="slice dragon-slice"><use href="#r-pr" x="705" y="215"/><use href="#r-dk" x="650" y="192"/>{head}{br}</g>'
water = '<path d="M0 240h800v60H0z" fill="#0b172a"/><path d="M0 240h800" stroke="#0284c7" stroke-width="2" stroke-dasharray="10 6" opacity=".5"/>'
dragon_svg = f'<svg id="svg-dragon" class="svg-art" viewBox="0 0 800 300">{defs}{water}{tail_block}{s2}{s3}{s4}{s5}</svg>'

jmps = '<div class="jmps" onclick="const b=event.target.closest(\'.jmp\');if(b)setGameStage(cur,+b.textContent)"><button class="jmp active">0</button><button class="jmp">1</button><button class="jmp">2</button><button class="jmp">3</button><button class="jmp">4</button><button class="jmp">5</button></div>'

html = f'<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0"><title>ESP32 Landmark Builder</title><style>{css_min}</style></head><body><div class="app"><header><div><strong>ESP32 LANDMARK BUILDER</strong><div style="font-size:10px;color:#94a3b8">PROGMEM Inline SVG Engine</div></div><span class="chip">&lt;15KB OK • API READY</span></header><div class="tabs"><button id="te" class="tab active" onclick="switchLandmark(\'eiffel\')">🗼 Eiffel Tower</button><button id="td" class="tab" onclick="switchLandmark(\'dragon\')">🐉 Dragon Bridge</button></div><div class="stage">{eiffel}{dragon_svg}</div><div class="hud"><div class="row"><span id="lmk">EIFFEL TOWER</span><span id="stg" style="color:#38bdf8">STAGE 0/5 (0%)</span></div><div class="trk"><div id="bar" class="fill"></div></div>{jmps}</div><div class="ctrls"><button class="btn btn-p" onclick="nextBlock()">➕ Next Block (+1)</button><button id="ba" class="btn" onclick="toggleAuto()"><span id="ai">▶</span> Auto Build</button><button class="btn btn-d" onclick="setGameStage(cur,0)">↺ Reset</button></div><div class="term"><div>ESP32 HOOK: <span id="cmd" style="color:#facc15">setGameStage("eiffel", 0);</span></div><div style="color:#94a3b8">WS / SERIAL READY</div></div></div><script>{js_min}</script></body></html>'

size = len(html.encode('utf-8'))
print(f"Total size: {size} bytes ({size/1024:.2f} KB)")
if size <= 15360:
    print(f"SUCCESS! Within 15KB with {15360 - size} bytes to spare!")
    with open('index.html', 'w', encoding='utf-8') as f:
        f.write(html)
else:
    print(f"Still needs {size - 15360} bytes")
