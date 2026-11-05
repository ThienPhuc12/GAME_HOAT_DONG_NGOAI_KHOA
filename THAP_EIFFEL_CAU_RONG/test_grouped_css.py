css = """
*{box-sizing:border-box;margin:0;padding:0;user-select:none}
body,header,.hud,.term,button,.stage{display:flex;align-items:center}
body{flex-direction:column;padding:8px;min-height:100vh;background:radial-gradient(circle at 50% 10%,#1e293b,#090d16 85%);color:#f1f5f9;font:12px system-ui}
.app{width:100%;max-width:820px;display:flex;flex-direction:column;gap:8px}
header,.hud,.term{justify-content:space-between;background:rgba(19,28,46,.9);border:1px solid #1e293b;border-radius:10px;padding:8px 12px}
.chip{background:#0284c722;color:#38bdf8;border:1px solid #38bdf844;font:700 11px system-ui;padding:2px 8px;border-radius:10px}
.tabs,.ctrls{display:grid;gap:6px}
.tabs{grid-template-columns:1fr 1fr}
.ctrls{grid-template-columns:1.4fr 1fr 1fr}
button,.stage{justify-content:center}
button{border:1px solid #1e293b;border-radius:8px;font:700 12px system-ui;cursor:pointer;background:#131c2e;color:#f1f5f9}
button:active{transform:scale(.97)}
.tab{padding:9px;font-size:13px;color:#94a3b8;gap:6px}
.tab.active,.btn-p{background:#0284c7;color:#fff;border-color:#38bdf8}
.stage{background:radial-gradient(circle at 50% 45%,#1a263d,#060911);border:1px solid #1e293b;border-radius:12px;padding:8px;min-height:320px}
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
.hud{flex-direction:column;gap:6px}
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
import re
css_c = re.sub(r'\s+', '', css)
print("Grouped CSS length:", len(css_c))
