#pragma once
// =============================================================================
//  LE PONT · CẦU — page grand écran + télécommande — http://192.168.4.1/host
//  Généré depuis web/host.html : modifie le .html puis relance gen_web_h.py,
//  ou édite directement ci-dessous (c'est du HTML/JS ordinaire).
//  VI: Tạo từ web/host.html: sửa tệp .html rồi chạy lại gen_web_h.py.
// =============================================================================
const char HOST_HTML[] PROGMEM = R"html(
<!doctype html>
<html lang="fr"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Le Pont · Cầu — grand écran</title>
<!-- Grand écran (vidéoprojecteur) + télécommande de l'animateur. Mêmes images et polices que les téléphones (/a/...).
     Clavier : → ou Espace ou Page suivante = suite · R = révéler · F = plein écran
     VI: Màn hình lớn (máy chiếu) + điều khiển của người điều phối. Bàn phím: → hoặc Space = tiếp · R = công bố · F = toàn màn hình -->
<link rel="stylesheet" href="/a/f.css?v=9d6f412c">
<style>
:root{--ink:#071530;--navy:#0b1d42;--glass:rgba(6,20,52,.78);--line:rgba(255,255,255,.14);--cream:#f4efe3;--mut:#a9b8d6;--gold:#f2b544;--gold2:#ffd27a;--fr:#3b74d6;--vn:#d8463c;--ok:#5ad08a;--warn:#f6a623;
 --ui:BVP,system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;--title:PF,"Iowan Old Style","Palatino Linotype","Noto Serif",Georgia,serif}
*{box-sizing:border-box}
html{font-size:calc(8px + .95vh)}
html,body{height:100%;margin:0;overflow:hidden}
body{background:var(--ink);color:var(--cream);font-family:var(--ui);font-weight:500;-webkit-font-smoothing:antialiased}
.hide{display:none!important}

/* ---------- la scène (même moteur que le téléphone) ---------- */
#stage{position:fixed;left:0;top:0;right:0;height:100vh;overflow:hidden;background:#04254e;transition:height 1s cubic-bezier(.4,0,.2,1)}
#world{position:absolute;left:0;top:0;width:1536px;height:1024px;transform-origin:0 0;transition:transform 1s cubic-bezier(.4,0,.2,1)}
#world>*{position:absolute}
#bg,#endimg{left:0;top:0;width:1536px;height:1024px;background:#04254e url(/a/scene.webp?v=9d6f412c) 0 0/1536px 1024px no-repeat}
#endimg{background-image:url(/a/end.webp?v=9d6f412c);opacity:0;transition:opacity 1.8s ease;pointer-events:none}
#endimg.on{opacity:1}
.sp{position:absolute;display:block;background:url(/a/sprites.webp?v=9d6f412c) no-repeat}
.sp.ui{position:relative;display:inline-block;flex:none}
#moon{left:581px;top:42px;width:360px;height:360px;border-radius:50%;background:radial-gradient(circle,rgba(255,236,170,.55),rgba(255,226,150,.18) 45%,transparent 70%);animation:breathe 6s ease-in-out infinite}
@keyframes breathe{50%{opacity:.45;transform:scale(1.06)}}
.star{width:7px;height:7px;border-radius:50%;background:#fff6d6;box-shadow:0 0 10px 3px rgba(255,240,190,.7);opacity:.2;animation:tw 3.2s ease-in-out infinite}
@keyframes tw{50%{opacity:1}}
.pc{transition:opacity .5s}.pc.ghost{opacity:.14}
.pc.new{animation:rise 1.2s cubic-bezier(.2,.9,.3,1.15) both}
@keyframes rise{0%{opacity:0;transform:translateY(130px) scale(.92)}60%{opacity:1}100%{transform:none}}
.pc.want{opacity:.55;animation:want 1.5s ease-in-out infinite}
@keyframes want{50%{opacity:.9;filter:drop-shadow(0 0 22px rgba(255,214,110,1))}}
.ch{left:0;top:0}
.boat{left:0;top:0;opacity:0;transition:opacity .8s}.boat.on{opacity:1}
.boat .sp{animation:bob 3s ease-in-out infinite}
@keyframes bob{50%{transform:translateY(-7px) rotate(-1.2deg)}}
.halo,.spark{position:absolute}
.halo{width:240px;height:240px;margin:-120px 0 0 -120px;border-radius:50%;pointer-events:none;background:radial-gradient(circle,rgba(255,226,140,.95),rgba(255,180,60,.35) 35%,transparent 68%);animation:halo 1.8s ease-out both}
@keyframes halo{0%{opacity:0;transform:scale(.3)}25%{opacity:1}100%{opacity:0;transform:scale(1.6)}}
.spark{width:12px;height:12px;margin:-6px;border-radius:50%;background:#fff3c4;box-shadow:0 0 12px 4px rgba(255,205,90,.9);animation:spark 1.6s ease-out both}
@keyframes spark{0%{opacity:0;transform:translate(0,0) scale(.4)}20%{opacity:1}100%{opacity:0;transform:translate(var(--dx),var(--dy)) scale(1)}}
#fly{left:0;top:0;opacity:0}
#fly.on{opacity:1;animation:fly 11s linear infinite}
#fly .sp{animation:wave 1.6s ease-in-out infinite}
@keyframes fly{0%{transform:translate(1760px,300px)}100%{transform:translate(-420px,190px)}}
@keyframes wave{50%{transform:translateY(-26px) rotate(3deg)}}
.lotus{opacity:0;transition:opacity 2s}.lotus.on{opacity:1}
.lotus .sp{animation:bob 4s ease-in-out infinite}
.lotus::before{content:"";position:absolute;left:-90px;top:-150px;width:180px;height:180px;border-radius:50%;background:radial-gradient(circle,rgba(255,200,120,.55),transparent 65%)}

/* ---------- bandeau du haut ---------- */
.top{position:fixed;left:0;right:0;top:0;z-index:3;display:flex;align-items:center;gap:1rem;padding:1.1rem 1.6rem;pointer-events:none}
.chip{display:flex;align-items:center;gap:.6rem;background:var(--glass);-webkit-backdrop-filter:blur(8px);backdrop-filter:blur(8px);border:1px solid var(--line);border-radius:999px;padding:.45rem 1.1rem;font-size:1.05rem;font-weight:700;white-space:nowrap}
.chip .dot{width:.75rem;height:.75rem;border-radius:50%;background:var(--gold)}
#qn{display:flex;gap:.5rem;align-items:center}
#qn:empty{display:none}
.pill{white-space:nowrap;font-size:.95rem;font-weight:700;letter-spacing:.05em;text-transform:uppercase;border-radius:999px;padding:.4rem 1rem;background:var(--glass);border:1px solid var(--line)}
.pill.n{background:var(--gold);color:#1a1205;border-color:transparent}
.pill.dbl{background:linear-gradient(90deg,#ffd77e,#f791a6);color:#3a0f1c;border-color:transparent}
.sp1{flex:1}
#timer{position:relative;width:5.6rem;height:5.6rem;display:flex;align-items:center;justify-content:center;border-radius:50%;background:var(--glass);font-weight:700;font-size:2.2rem}
#timer svg{position:absolute;inset:0;width:100%;height:100%}
#timer circle{fill:none;stroke-width:4}
#tring{stroke:var(--gold);stroke-linecap:round;transition:stroke-dashoffset 1s linear,stroke .3s}
#timer.low{animation:pulse .5s ease-in-out infinite alternate}#timer.low #tring{stroke:var(--warn)}
@keyframes pulse{to{transform:scale(1.08)}}

/* ---------- salon ---------- */
h1,h2{font-family:var(--title);font-weight:700;margin:0}
#v-lobby{position:fixed;inset:0;z-index:2;pointer-events:none}
.title{position:absolute;left:0;right:0;top:9vh;text-align:center}
.title h1{font-size:5.4rem;line-height:1;color:var(--gold2);text-shadow:0 0 1.2rem rgba(4,20,52,.95),0 0 3rem rgba(4,20,52,.7),0 4px 40px rgba(242,181,68,.45)}
.title h1 span{color:var(--cream);opacity:.6;margin:0 .15em}
.title p{margin:.7rem 0 0;font-size:1.45rem;color:#e6ecf8;text-shadow:0 2px 10px rgba(0,0,0,.8)}
.join{position:absolute;left:50%;bottom:5vh;transform:translateX(-50%);width:min(48rem,52vw);text-align:center;background:var(--glass);-webkit-backdrop-filter:blur(10px);backdrop-filter:blur(10px);border:1px solid rgba(255,210,122,.4);border-radius:1.6rem;padding:1.2rem 1.6rem;box-shadow:0 1rem 3rem rgba(0,0,0,.4)}
.join .st{display:flex;justify-content:center;gap:2.4rem;margin:.2rem 0 .4rem}
.join .st div{display:flex;flex-direction:column;align-items:center;gap:.2rem}
.join small{font-size:.95rem;color:var(--mut)}
.join b{font-size:2rem;color:var(--gold2);letter-spacing:.01em;white-space:nowrap}
.note{font-size:1.1rem;color:var(--mut);min-height:1.4rem;margin:.4rem 0 0}
.bank{position:absolute;bottom:5vh;width:min(24rem,21vw);background:var(--glass);-webkit-backdrop-filter:blur(8px);backdrop-filter:blur(8px);border:1px solid var(--line);border-radius:1.4rem;padding:.9rem 1rem;min-height:9rem}
.bank.b0{left:2vw;border-top:3px solid var(--fr)}.bank.b1{right:2vw;border-top:3px solid var(--vn)}
.bank h4{margin:0 0 .5rem;font-size:1rem;letter-spacing:.06em;text-transform:uppercase;color:var(--mut);text-align:center}
.lts{display:flex;flex-wrap:wrap;justify-content:center;gap:.4rem .8rem}
.lt{width:5.2rem;display:flex;flex-direction:column;align-items:center;font-size:1.05rem;line-height:1.15;text-align:center;word-break:break-word}
.lt .sp{filter:drop-shadow(0 0 12px rgba(255,190,80,.8))}
.lt.new .sp{animation:light 1.4s ease-out}
@keyframes light{0%{filter:brightness(.3);transform:scale(.7)}60%{filter:brightness(1.5) drop-shadow(0 0 18px rgba(255,200,90,1))}}

/* ---------- question ---------- */
#v-q{position:fixed;left:0;right:0;bottom:0;height:56vh;overflow:hidden;z-index:2;background:linear-gradient(180deg,rgba(11,29,66,.0),#0b1d42 3.2rem,#081633);padding:2.6rem 3vw 1.4rem;display:flex;flex-direction:column}
.qwrap{display:grid;grid-template-columns:minmax(0,1fr);align-content:start;gap:.6rem;max-width:1500px;width:100%;margin:0 auto;flex:1;min-height:0}
.q{font-size:2.15rem;line-height:1.22;font-weight:700;margin:0}
.qvi{font-size:1.5rem;line-height:1.3;color:var(--mut);margin:0}
.opts{display:grid;grid-template-columns:1fr 1fr;gap:.8rem;margin:.4rem 0 0}
.opt{position:relative;display:flex;align-items:center;min-height:4.2rem;padding:.7rem 1.2rem .7rem 4rem;border-radius:1.1rem;font-size:1.55rem;font-weight:700;line-height:1.2;color:#fff;box-shadow:inset 0 -4px 0 rgba(0,0,0,.18),0 .4rem 1rem rgba(0,0,0,.3);transition:opacity .4s,filter .4s,box-shadow .3s,transform .3s}
.opt .sh{position:absolute;left:1.2rem;top:50%;margin-top:-.85rem;width:1.7rem;height:1.7rem;background:#fff}
.opt.k0{background:linear-gradient(160deg,#4f8aea,#2d5fbf)}.opt.k0 .sh{border-radius:50%}
.opt.k1{background:linear-gradient(160deg,#e85d52,#b5352d)}.opt.k1 .sh{border-radius:3px}
.opt.k2{background:linear-gradient(160deg,#f7c45a,#dc9b2c);color:#2a1a05}.opt.k2 .sh{background:#2a1a05;clip-path:polygon(50% 0,100% 100%,0 100%)}
.opt.k3{background:linear-gradient(160deg,#1c9a8a,#0f6a60)}.opt.k3 .sh{transform:rotate(45deg) scale(.82);border-radius:2px}
.opt.num{grid-column:1/3;background:rgba(19,41,90,.85);border:1px solid var(--line);padding-left:1.4rem}
.opt.ok{box-shadow:0 0 0 .3rem var(--ok),0 0 2rem rgba(90,208,138,.6);transform:scale(1.02)}
.opt.ok::after{content:"✓";position:absolute;right:1.2rem;font-size:2rem}
.opt.dim{opacity:.28;filter:saturate(.4)}
.say{display:flex;gap:.9rem;align-items:flex-end}
.bub{flex:1;background:#fff6dc;color:#3a2606;border-radius:1.2rem 1.2rem 1.2rem .3rem;padding:.7rem 1.1rem;font-size:1.35rem;line-height:1.35;box-shadow:0 .4rem 1rem rgba(0,0,0,.3)}
.bub b{display:block;font-size:.95rem;letter-spacing:.04em;text-transform:uppercase;color:#a0611c}
.say.pop .sp{animation:hop .7s ease}
@keyframes hop{30%{transform:translateY(-10px)}}
#v-q.rev .qvi,#v-q.rev #hint{display:none!important}
#v-q.rev .q{font-size:1.6rem}
#v-q.rev .opts{grid-template-columns:repeat(4,minmax(0,1fr));gap:.6rem}
#v-q.rev .opt{min-height:3rem;font-size:1.15rem;padding:.5rem .8rem .5rem 2.8rem}
#v-q.rev .opt .sh{left:.8rem;width:1.3rem;height:1.3rem;margin-top:-.65rem}
#v-q.rev .opt.ok{padding-right:2.2rem}
#v-q.rev .opt.ok::after{right:.7rem;font-size:1.5rem}
#v-q.rev .opt.num{grid-column:1/5}
.revrow{display:grid;grid-template-columns:minmax(0,1.5fr) minmax(0,1fr);gap:1rem;align-items:start}
.savoir{padding:.8rem 1.2rem;border-radius:1.2rem;background:linear-gradient(170deg,#fff3d1,#f6dfa6);color:#3a2606;font-size:1.2rem;line-height:1.4;box-shadow:0 .5rem 1.4rem rgba(0,0,0,.3)}
.savoir h4{margin:0 0 .2rem;font:700 1.25rem var(--title);color:#8a4d12}
.savoir .vi{color:#6e5328;margin-top:.35rem;font-size:1.02rem}
.res{display:flex;flex-direction:column;gap:.6rem}
.res div{display:flex;align-items:center;gap:.8rem;border-radius:1rem;padding:.7rem 1rem;font-size:1.3rem;font-weight:700;line-height:1.2}
.res div small{display:block;font-size:1rem;font-weight:600;opacity:.85}
.res .r0{background:rgba(59,116,214,.3);border:1px solid rgba(59,116,214,.7)}
.res .r1{background:rgba(216,70,60,.28);border:1px solid rgba(216,70,60,.6)}
.res .big{font-size:2rem}

/* ---------- fin ---------- */
#v-end{position:fixed;inset:0;z-index:2;pointer-events:none}
#v-end .title{top:3.2vh}
#v-end .title h1{font-size:4rem}
#v-end .title h1 span{color:var(--cream);opacity:.85;margin:0 0 0 .3em}
#v-end .title p{font-size:1.25rem}
.endband{position:absolute;left:50%;bottom:3vh;transform:translateX(-50%);animation:bandin 1.2s 6s cubic-bezier(.2,.8,.3,1) both;width:min(70rem,90vw);display:grid;grid-template-columns:minmax(0,1.1fr) minmax(0,1fr);gap:1.2rem;background:var(--glass);-webkit-backdrop-filter:blur(10px);backdrop-filter:blur(10px);border:1px solid rgba(255,210,122,.35);border-radius:1.6rem;padding:1.1rem 1.4rem}
@keyframes bandin{from{opacity:0;transform:translate(-50%,40px)}to{opacity:1;transform:translate(-50%,0)}}
.fiche .row{display:flex;justify-content:space-between;gap:1rem;font-size:1.08rem;padding:.28rem 0;border-bottom:1px solid rgba(255,255,255,.07)}
.fiche .row:last-child{border:0}.fiche .row span{color:var(--mut)}.fiche .row b{color:var(--gold2)}
.names{display:flex;flex-wrap:wrap;gap:.5rem;align-content:flex-start;justify-content:center}
.names h4{width:100%;margin:0 0 .2rem;text-align:center;font-size:1rem;letter-spacing:.06em;text-transform:uppercase;color:var(--mut)}
.names span{padding:.4rem .9rem;border-radius:.5rem;background:linear-gradient(180deg,#f1cf8d,#d9a75b);color:#5a3810;font:700 1.2rem var(--title);box-shadow:inset 0 -2px 0 rgba(0,0,0,.15),0 3px 8px rgba(0,0,0,.25);border-bottom:3px solid var(--fr)}
.names span.t1{border-bottom-color:var(--vn)}

/* ---------- télécommande ---------- */
.ctl{position:fixed;top:7.6rem;right:1.6rem;z-index:5;display:flex;gap:.5rem;opacity:.35;transition:opacity .2s}
.ctl:hover,.ctl.wait{opacity:1}
.ctl button{font:600 1rem var(--ui);padding:.65rem 1rem;border:1px solid var(--line);border-radius:.8rem;background:rgba(19,41,90,.92);color:#fff;cursor:pointer}
.ctl button.p{background:linear-gradient(180deg,#ffd77e,#f2b544);color:#2a1a05;font-weight:700;border-color:transparent}
.ctl button:disabled{opacity:.45;cursor:not-allowed}
.ctl button:focus-visible{outline:3px solid #fff;outline-offset:2px}
.ctl #snd.off{opacity:.5;text-decoration:line-through}
#st{position:fixed;top:0;left:0;right:0;z-index:9;padding:6px 10px;font-size:14px;text-align:center;background:#7f1d1d;color:#fff}
</style></head><body>
<div id="st" class="hide">Connexion à l'objet… · Đang kết nối…</div>
<div id="stage">
 <div id="world">
  <div id="bg"></div><div id="moon"></div><div id="stars"></div><div id="chars"></div><div id="bridge"></div><div id="boats"></div><div id="fx"></div><div id="endimg"></div><div id="lotus"></div><div id="fly"></div>
 </div>
</div>

<div class="top">
 <div class="chip"><i class="dot"></i><span id="ssid">Le Pont · Cầu</span></div>
 <div id="qn"></div>
 <div class="sp1"></div>
 <div class="chip" id="cnt"></div>
 <div id="timer" class="hide"><svg viewBox="0 0 52 52"><circle cx="26" cy="26" r="22" stroke="#24407a"/><circle id="tring" cx="26" cy="26" r="22" stroke-dasharray="138.2" transform="rotate(-90 26 26)"/></svg><span id="tsec"></span></div>
</div>

<div id="v-lobby">
 <div class="title"><h1>Le Pont<span>·</span>Cầu</h1><p>Deux rives, un pont à construire ensemble · Hai bờ sông, một cây cầu cùng nhau xây</p></div>
 <div class="bank b0"><h4 id="ln0">Rive France</h4><div class="lts" id="lf"></div></div>
 <div class="bank b1"><h4 id="ln1">Rive Vietnam</h4><div class="lts" id="lv"></div></div>
 <div class="join">
  <div class="st"><div><small>1 · Wi-Fi</small><b id="ssid2">LePont-1</b></div><div><small>2 · Page du jeu · Trang trò chơi</small><b>http://192.168.4.1</b></div></div>
  <small>Rejoins le Wi-Fi puis ouvre la page · Kết nối Wi-Fi rồi mở trang</small>
  <div class="note" id="note"></div>
 </div>
</div>

<div id="v-q" class="hide">
 <div class="qwrap">
  <p class="q" id="fr"></p>
  <p class="qvi" id="vi"></p>
  <div class="say hide" id="hint"><span id="hdr"></span><div class="bub"><b>Le petit dragon souffle · Rồng nhỏ mách</b><span id="hinttxt"></span></div></div>
  <div class="opts" id="opts"></div>
  <div class="revrow hide" id="pont"><div class="savoir"><h4>Le saviez-vous ? · Bạn có biết?</h4><div id="pfr"></div><div class="vi" id="pvi"></div></div><div class="res" id="res"></div></div>
 </div>
</div>

<div id="v-end" class="hide">
 <div class="title"><h1>Le pont est fermé<span>· Cầu đã nối</span></h1><p>Paris et Hội An se sont rejoints au milieu du fleuve · Paris và Hội An đã gặp nhau giữa dòng sông</p></div>
 <div class="endband"><div class="fiche" id="fiche"></div><div class="names" id="names"></div></div>
</div>

<div class="ctl">
 <button id="snd" onclick="toggleSnd()" title="Son · Âm thanh">♪</button>
 <button onclick="fullscreen()" title="Plein écran (F) · Toàn màn hình">⛶</button>
 <button onclick="cmd('reset')">Reset</button>
 <button onclick="cmd('reveal')">Révéler</button>
 <button class="p" id="btn" onclick="next()">Lancer</button>
</div>
<script>
// =====================================================================================
//  Le Pont · Cầu — grand écran. Même moteur de scène que web/player.html (images, caméra, pont, personnages).
//  VI: Màn hình lớn. Cùng bộ máy cảnh với web/player.html (hình ảnh, camera, cầu, nhân vật).
// =====================================================================================
var SP=/*SP*/{"W":1536,"H":909,"k":0.833333,"s":{"bL1":[1041,462,112,139,0,0,0.8333],"bL2":[221,668,92,122,0,0,0.8333],"bL3":[614,668,101,120,0,0,0.8333],"bL4":[814,668,92,113,0,0,0.8333],"bR4":[719,668,91,115,0,0,0.8333],"bR3":[317,668,100,122,0,0,0.8333],"bR2":[121,668,96,127,0,0,0.8333],"bR1":[0,668,117,128,0,0,0.8333],"bC":[1157,462,60,136,0,0,0.8333],"boyP1":[0,0,133,242,57.1,241.9,0.42],"boyP2":[137,0,127,237,63.3,237.3,0.42],"boyP3":[409,0,162,234,78.4,233.9,0.42],"boyP4":[872,0,234,222,126.5,222.2,0.42],"boyW1":[114,246,143,211,68.6,211.3,0.37],"boyW2":[0,246,110,212,53.2,212.4,0.37],"boyW3":[799,246,147,204,75.1,203.9,0.37],"boyW4":[487,246,120,207,52.0,206.8,0.37],"girlP1":[268,0,137,236,64.6,235.6,0.42],"girlP2":[575,0,132,231,51.6,230.6,0.42],"girlP3":[711,0,157,226,76.0,225.5,0.42],"girlP4":[1110,0,218,218,102.7,217.6,0.42],"girlW1":[950,246,143,204,54.5,204.2,0.37],"girlW2":[1097,246,134,204,42.8,203.5,0.37],"girlW3":[193,462,147,200,56.0,199.8,0.37],"girlW4":[1235,246,137,203,46.7,202.8,0.37],"dragon1":[611,246,184,206,91.8,206.3,0.38],"dragon2":[261,246,222,209,111.0,208.6,0.38],"dragon3":[1332,0,192,216,95.8,216.2,0.38],"dragon4":[0,462,189,202,94.6,201.8,0.38],"dragonFly":[1221,462,253,131,126.5,65.5,0.5],"lan0":[344,462,89,158,44.6,78.9,0.3],"lan1":[437,462,89,158,44.4,78.9,0.3],"lotus":[421,668,189,121,94.4,121.0,0.34],"boatF":[910,668,319,109,159.4,109.1,0.32],"boatV":[0,800,324,109,161.9,109.4,0.32],"stone":[530,462,507,152,253.5,76.0,0.5]},"bridge":{"bL1":[330,494],"bL2":[446,479],"bL3":[538,468],"bL4":[640,468],"bR4":[784,468],"bR3":[874,468],"bR2":[975,476],"bR1":[1071,498],"bC":[732,393]},"deck":[0.00034,-0.52381,741.666667],"lant":[[406,620],[503,585],[598,571],[701,563],[830,565],[934,573],[1025,587],[1123,612]]}/*SP*/;
var VI_BANK=['Bờ Pháp','Bờ Việt Nam'];
var ws,TEAMS=[],state='lobby',cur=null,B={side:[0,0],pending:[false,false],center:false,per:4},ans=[0,0],players=[],teamN=[0,0],VIEW='lobby',seen={},lastTick=-1;
function $(i){return document.getElementById(i)}
function esc(s){return String(s==null?'':s).replace(/[&<>"]/g,function(c){return{'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]})}
function tname(i){return TEAMS[i]?TEAMS[i].n:(i?'Rive Vietnam':'Rive France')}
function tshort(i){return tname(i).replace(/^Rive\s+/i,'')}

// ---------------------------------------------------------------- images ---------
function frame(el,n,pps){var r=SP.s[n],k=pps/r[6],s=el.style;s.width=r[2]*k+'px';s.height=r[3]*k+'px';s.backgroundSize=SP.W*k+'px '+SP.H*k+'px';s.backgroundPosition=(-r[0]*k)+'px '+(-r[1]*k)+'px';s.marginLeft=(-r[4]*k)+'px';s.marginTop=(-r[5]*k)+'px'}
function ui(n,h){if(!SP)return'';var r=SP.s[n],k=h/r[3];return '<i class="sp ui" style="width:'+(r[2]*k).toFixed(1)+'px;height:'+h+'px;background-size:'+(SP.W*k).toFixed(1)+'px '+(SP.H*k).toFixed(1)+'px;background-position:'+(-r[0]*k).toFixed(1)+'px '+(-r[1]*k).toFixed(1)+'px"></i>'}
function mk(cls,parent){var e=document.createElement('div');e.className=cls;(parent||$('fx')).appendChild(e);return e}
function at(e,x,y){e.style.transform='translate('+x+'px,'+y+'px)'}
function rem(){return parseFloat(getComputedStyle(document.documentElement).fontSize)}

// ---------------------------------------------------------------- caméra ---------
// Salon et fin : toute la scène. Question : la scène occupe le haut de l'écran et suit les deux enfants.
// VI: Phòng chờ và kết thúc: toàn cảnh. Câu hỏi: cảnh ở nửa trên màn hình và theo hai bạn nhỏ.
function focus(){
 if(VIEW==='lobby')return [60,60,1416,900];
 if(VIEW==='end')return [100,60,1336,900];
 var a=B.center?712:tipX(0,B.side[0]),b=B.center?828:tipX(1,B.side[1]),x0=Math.min(a,b)-190,x1=Math.max(a,b)+190,w=Math.max(x1-x0,820),cx=(x0+x1)/2;
 return [cx-w/2,285,w,400];
}
function camera(){
 if(!SP)return;
 var W=innerWidth,Hs=VIEW==='q'?Math.round(innerHeight*.47):innerHeight,f=focus(),top=VIEW==='q'?4.4*rem():0,H=Hs-top;
 $('stage').style.height=Hs+'px';
 var s=Math.max(Math.min(W/f[2],H/f[3]),W/1536,Hs/1024),tx=W/2-s*(f[0]+f[2]/2),ty=top+H/2-s*(f[1]+f[3]/2);
 tx=Math.min(0,Math.max(W-1536*s,tx));ty=Math.min(0,Math.max(Hs-1024*s,ty));
 $('world').style.transform='translate('+tx.toFixed(1)+'px,'+ty.toFixed(1)+'px) scale('+s.toFixed(4)+')';
}
addEventListener('resize',camera);

// ---------------------------------------------------------------- le pont --------
var POST=[[353,455,547,649,747],[1180,1080,984,883,787]];
function deck(x){var d=SP.deck;return d[0]*x*x+d[1]*x+d[2]}
function ground(x){if(x<=330||x>=1206)return 630;var d=deck(x);if(x<372)return 630+(d-630)*(x-330)/42;if(x>1164)return 630+(d-630)*(1206-x)/42;return d}
function tipX(r,n){if(B.center||n>B.per)return r?828:712;if(n===0)return r?1256:280;if(n>=B.per)return r?858:676;return r?POST[1][n]+36:POST[0][n]-36}
var PC={};
function buildBridge(){var h=$('bridge');h.innerHTML='';['L1','L2','L3','L4','C','R4','R3','R2','R1'].forEach(function(k){var e=document.createElement('i');e.className='sp pc ghost';frame(e,'b'+k,1);e.style.margin='0';var p=SP.bridge['b'+k];e.style.left=p[0]+'px';e.style.top=p[1]+'px';h.appendChild(e);PC[k]=e})}
function pieceKey(r,i){return (r?'R':'L')+i}
function lantXY(r,i){return r?SP.lant[8-i]:SP.lant[i-1]}
function setPiece(k,on,anim){var e=PC[k];if(!e)return;e.classList.toggle('ghost',!on);if(anim&&on){e.classList.remove('new');void e.offsetWidth;e.classList.add('new')}}
function burst(x,y,n){var h=mk('halo');h.style.left=x+'px';h.style.top=y+'px';setTimeout(function(){h.remove()},1900);
 for(var i=0;i<(n||10);i++){var s=mk('spark'),a=Math.random()*Math.PI*2,d=60+Math.random()*120;s.style.setProperty('--dx',Math.cos(a)*d+'px');s.style.setProperty('--dy',(Math.sin(a)*d-90)+'px');s.style.left=x+'px';s.style.top=y+'px';s.style.animationDelay=(Math.random()*.3)+'s';(function(s){setTimeout(function(){s.remove()},2100)})(s)}}

// ---------------------------------------------------------------- personnages ----
var WHO=['boy','girl'],REFH=[570,555],CHH=250,CH=[];
function pps(r){return CHH/REFH[r]}
function initChars(){for(var r=0;r<2;r++){var w=mk('ch',$('chars')),s=document.createElement('i');s.className='sp';w.appendChild(s);CH[r]={el:w,sp:s,x:0,y:0,pose:'',raf:0,tmo:0};setPose(r,'P1');place(r,tipX(r,0),ground(tipX(r,0)))}}
function setPose(r,p){var c=CH[r];if(!c||c.pose===p)return;c.pose=p;frame(c.sp,WHO[r]+p,pps(r))}
function place(r,x,y){var c=CH[r];c.x=x;c.y=y;at(c.el,x,y)}
function idle(r){var n=B.side[r];return VIEW==='lobby'?'P1':(n>=B.per&&B.side[1-r]>=B.per&&!B.center?'P4':'P2')}
function walkTo(r,x1,then){var c=CH[r];cancelAnimationFrame(c.raf);clearTimeout(c.tmo);var x0=c.x,t0=performance.now(),dur=Math.max(350,Math.abs(x1-x0)/115*1000),fi=0,lf=0;
 if(Math.abs(x1-x0)<2){setPose(r,idle(r));then&&then();return}
 (function step(t){var k=Math.min(1,(t-t0)/dur),x=x0+(x1-x0)*k;place(r,x,ground(x));if(t-lf>135){lf=t;fi=(fi+1)%4;setPose(r,'W'+(fi+1))}
  if(k<1)c.raf=requestAnimationFrame(step);else{setPose(r,idle(r));then&&then()}})(t0)}
function cheer(r){var c=CH[r];setPose(r,'P3');clearTimeout(c.tmo);c.tmo=setTimeout(function(){setPose(r,idle(r))},900)}
function snapChars(){if(!SP)return;for(var r=0;r<2;r++){var c=CH[r];cancelAnimationFrame(c.raf);var x=tipX(r,B.side[r]);place(r,x,ground(x));c.pose='';setPose(r,idle(r))}}

// ---------------------------------------------------------------- barques --------
var BOAT=[];
function initBoats(){for(var r=0;r<2;r++){var w=mk('boat',$('boats')),s=document.createElement('i');s.className='sp';w.appendChild(s);frame(s,r?'boatV':'boatF',.22);if(r)s.style.transform='scaleX(-1)';BOAT[r]=w}}
function boatTo(r,on){var w=BOAT[r],i=Math.min(B.per,B.side[r]+1),l=lantXY(r,i),x=on?l[0]:(r?1700:-180);w.style.transition='opacity .8s, transform '+(on?'1.4s':'2.2s')+' ease-in-out';at(w,x,652);w.classList.toggle('on',on)}  // plus haut que sur le téléphone : visible dans la scène du grand écran

// ---------------------------------------------------------------- état du pont ---
function syncBridge(nb,anim){
 if(!nb)return;var old=B;B=nb;if(!B.per)B.per=4;if(!SP)return;
 for(var r=0;r<2;r++){
  var from=old.side?old.side[r]:0,to=B.side[r];
  for(var i=1;i<=4;i++)setPiece(pieceKey(r,i),i<=to,anim&&i>from&&i<=to);
  if(anim&&to>from){(function(r,to){var l=lantXY(r,to);setTimeout(function(){burst(l[0],l[1]-30,12);snd('place')},450);setTimeout(function(){walkTo(r,tipX(r,to),function(){if(state==='rev')cheer(r)})},900)})(r,to)}
  else if(!anim||to<from){var x=tipX(r,to);cancelAnimationFrame(CH[r].raf);place(r,x,ground(x));CH[r].pose='';setPose(r,idle(r))}
  else setPose(r,idle(r));
  boatTo(r,!!(B.pending&&B.pending[r])&&!B.center&&to<B.per);
 }
 setPiece('C',B.center,anim&&B.center&&!old.center);
 PC.C&&PC.C.classList.toggle('want',!B.center&&B.side[0]>=B.per&&B.side[1]>=B.per);
 if(anim&&B.center&&!old.center)meet();
 if(!B.center)$('endimg').classList.remove('on');
 camera();
}
function meet(){
 setTimeout(function(){burst(770,420,22);snd('place')},400);
 for(var r=0;r<2;r++)(function(r){setTimeout(function(){walkTo(r,tipX(r,9),function(){setPose(r,'P4')})},800)})(r);
 setTimeout(function(){$('endimg').classList.add('on');snd('end')},3600);
}

// ---------------------------------------------------------------- décor vivant ---
function decor(){
 [[120,60],[300,150],[470,40],[560,250],[1010,60],[1120,170],[1290,90],[1430,40],[930,300],[650,120],[220,300],[1360,260]].forEach(function(p,i){var e=mk('star',$('stars'));e.style.left=p[0]+'px';e.style.top=p[1]+'px';e.style.animationDelay=(i*.53%3.2)+'s'});
 [[330,930],[560,880],[800,965],[1010,895],[1230,940]].forEach(function(p,i){var w=mk('lotus',$('lotus')),s=document.createElement('i');s.className='sp';w.appendChild(s);frame(s,'lotus',.24);at(w,p[0],p[1]);s.style.animationDelay=(i*.7)+'s'});
 var f=$('fly'),s=document.createElement('i');s.className='sp';f.appendChild(s);frame(s,'dragonFly',.5);
}
function endDecor(on){$('fly').classList.toggle('on',on);[].forEach.call(document.querySelectorAll('.lotus'),function(e,i){setTimeout(function(){e.classList.toggle('on',on)},on?600+i*500:0)})}

// ---------------------------------------------------------------- sons -----------
// Le son part du premier clic de l'animateur (règle des navigateurs). VI: Âm thanh bật sau lần bấm đầu tiên.
var AC=null,muted=false;
try{muted=localStorage.getItem('lp_host_mute')==='1'}catch(x){}
function actx(){if(!AC){try{AC=new(window.AudioContext||window.webkitAudioContext)()}catch(x){}}if(AC&&AC.state==='suspended')AC.resume();return AC}
function tone(f,t,d,v,ty){var o=AC.createOscillator(),g=AC.createGain();o.type=ty||'sine';o.frequency.value=f;g.gain.setValueAtTime(.0001,t);g.gain.exponentialRampToValueAtTime(v,t+.012);g.gain.exponentialRampToValueAtTime(.0001,t+d);o.connect(g);g.connect(AC.destination);o.start(t);o.stop(t+d+.05)}
function bell(f,t,d,v){tone(f,t,d,v);tone(f*2.01,t,d*.55,v*.32);tone(f*3.03,t,d*.3,v*.12)}
var NOTE={G4:392,A4:440,C5:523.3,D5:587.3,E5:659.3,G5:784,A5:880,C6:1046.5};
var SND={tap:function(t){bell(NOTE.G5,t,.6,.07)},tick:function(t){tone(1300,t,.05,.05)},
 ok:function(t){['C5','D5','E5','G5','A5'].forEach(function(n,i){bell(NOTE[n],t+i*.085,.9,.12)})},
 ko:function(t){bell(NOTE.E5,t,.7,.09);bell(NOTE.C5,t+.2,1,.09)},
 place:function(t){bell(NOTE.C6,t,1.3,.09);bell(NOTE.G5,t+.1,1.3,.08)},
 join:function(t){bell(NOTE.G5,t,.8,.06);bell(NOTE.C6,t+.12,1,.06)},
 end:function(t){'G4 A4 C5 D5 E5 D5 C5 A4 C5 D5 E5 G5 A5 G5 E5 D5 C5'.split(' ').forEach(function(n,i){bell(NOTE[n],t+i*.26,i===16?2.4:1.1,.11)})}};
function snd(n){if(muted)return;var c=AC;if(!c||c.state!=='running')return;try{SND[n](c.currentTime+.03)}catch(x){}}
function toggleSnd(){muted=!muted;$('snd').classList.toggle('off',muted);try{localStorage.setItem('lp_host_mute',muted?'1':'0')}catch(x){}actx();if(!muted)setTimeout(function(){snd('tap')},50)}
function fullscreen(){try{if(document.fullscreenElement)document.exitFullscreen();else document.documentElement.requestFullscreen()}catch(x){}}

// ---------------------------------------------------------------- réseau ---------
function connect(){
 ws=new WebSocket('ws://'+location.host+'/ws');
 ws.onopen=function(){$('st').classList.add('hide');ws.send(JSON.stringify({t:'hello',role:'host'}))};
 ws.onclose=function(){$('st').classList.remove('hide');setTimeout(connect,2000)};
 ws.onerror=function(){};
 ws.onmessage=function(e){var m;try{m=JSON.parse(e.data)}catch(x){return}handle(m)};
}
function cmd(c){actx();if(ws&&ws.readyState===1)ws.send(JSON.stringify({t:'host',cmd:c}))}
function next(){cmd(state==='lobby'?'start':'next')}
addEventListener('keydown',function(e){if(e.target&&e.target.tagName==='INPUT')return;
 if(e.key==='ArrowRight'||e.key==='PageDown'||e.key===' '||e.key==='Enter'){e.preventDefault();if(!$('btn').disabled)next()}
 else if(e.key==='r'||e.key==='R')cmd('reveal');else if(e.key==='f'||e.key==='F')fullscreen()});

function view(v){
 ['v-lobby','v-q','v-end'].forEach(function(x){$(x).classList.toggle('hide',x!==v)});
 VIEW={'v-lobby':'lobby','v-q':'q','v-end':'end'}[v];$('timer').classList.toggle('hide',v!=='v-q'||state!=='q');
 $('btn').disabled=false;document.querySelector('.ctl').classList.remove('wait');camera();
}
function teamCount(){teamN=[0,0];players.forEach(function(p){if(p.team===0||p.team===1)teamN[p.team]++})}
function counts(){$('cnt').textContent=tshort(0)+' '+ans[0]+(teamN[0]?'/'+teamN[0]:'')+' · '+tshort(1)+' '+ans[1]+(teamN[1]?'/'+teamN[1]:'')+' — réponses · trả lời'}
function setTimer(left,total){var k=Math.max(0,Math.min(1,left/(total||20)));$('tring').style.strokeDashoffset=(138.2*(1-k)).toFixed(1);$('tsec').textContent=left;$('timer').classList.toggle('low',left<=5&&left>0)}
function renderLobby(){
 var cols=[[],[]];players.forEach(function(p){if(p.team===0||p.team===1)cols[p.team].push(p)});
 ['lf','lv'].forEach(function(id,r){$(id).innerHTML=cols[r].map(function(p){var k=r+'|'+p.n,nw=!seen[k];if(nw)setTimeout(function(){snd('join')},80);seen[k]=1;return '<div class="lt'+(nw?' new':'')+'">'+ui('lan1',Math.round(4.4*rem()))+'<span>'+esc(p.n)+'</span></div>'}).join('')||'<span class="note">…</span>'});
 $('ln0').textContent=tname(0)+' · '+VI_BANK[0];$('ln1').textContent=tname(1)+' · '+VI_BANK[1];
}

// ---------------------------------------------------------------- messages -------
function handle(m){
 if(m.t==='config'){TEAMS=m.teams||[];if(m.per)B.per=m.per;$('ssid').textContent=m.ssid+' · 192.168.4.1';$('ssid2').textContent=m.ssid;renderLobby()}
 else if(m.t==='lobby'){
  state='lobby';view('v-lobby');endDecor(false);$('btn').textContent='Lancer';$('qn').innerHTML='';players=m.players||[];teamCount();syncBridge(m.bridge,false);snapChars();
  var n=players.length,n0=teamN[0],n1=teamN[1];
  $('cnt').textContent=n+' joueur'+(n>1?'s':'')+' · '+n+' người chơi';renderLobby();
  if(!n){$('btn').disabled=true;$('btn').textContent='En attente d\'un joueur';document.querySelector('.ctl').classList.add('wait')}
  $('note').textContent=!n?'Aucun joueur pour l\'instant · Chưa có người chơi':(n===1?'1 joueur : la rive vide suit la sienne, il construit tout le pont':((n0===0||n1===0)?'Au lancement, un joueur passera sur la rive vide':''));
 }
 else if(m.t==='question'){
  state='q';cur=m;endDecor(false);view('v-q');$('btn').textContent='Révéler';syncBridge(m.bridge,false);snapChars();
  $('qn').innerHTML='<span class="pill n">Question '+m.i+'</span><span class="pill">'+esc(m.cat||'')+'</span>'+(m.dbl?'<span class="pill dbl">★ La rencontre · Gặp gỡ</span>':'');
  $('fr').textContent=m.fr;$('vi').textContent=m.vi;
  if(m.hint){$('hint').classList.remove('hide');$('hdr').innerHTML=ui('dragon2',Math.round(5.6*rem()));$('hinttxt').textContent=m.hint;$('hint').classList.remove('pop');void $('hint').offsetWidth;$('hint').classList.add('pop')}else $('hint').classList.add('hide');
  $('opts').innerHTML=m.num?'<div class="opt num">Chaque joueur tape une estimation · Mỗi người nhập một con số</div>':m.o.map(function(o,k){return '<div class="opt k'+k+'" id="o'+k+'"><span class="sh"></span>'+esc(o)+'</div>'}).join('');
  $('pont').classList.add('hide');ans=[0,0];lastTick=-1;setTimer(m.sec,m.sec);$('v-q').classList.remove('rev');counts();snd('tap');
 }
 else if(m.t==='tick'){
  setTimer(m.left,cur?cur.sec:20);if(m.left<=5&&m.left>0&&m.left!==lastTick)snd('tick');lastTick=m.left;
  if(m.answers){ans=m.answers;counts()}
 }
 else if(m.t==='reveal'){
  state='rev';$('btn').textContent=m.center?'Fiche du pont':'Suivante';$('timer').classList.add('hide');
  for(var k=0;k<4;k++){var o=$('o'+k);if(o)o.className='opt k'+k+' '+(k===m.r?'ok':'dim')}
  if(m.target!==undefined)$('opts').innerHTML='<div class="opt num ok">Réponse · Đáp án : '+Number(m.target).toLocaleString('fr-FR')+' — médianes · trung vị : '+esc(tshort(0))+' '+(m.med?Number(m.med[0]).toLocaleString('fr-FR'):'?')+' · '+esc(tshort(1))+' '+(m.med?Number(m.med[1]).toLocaleString('fr-FR'):'?')+'</div>';
  $('pfr').textContent=m.pfr||'';$('pvi').textContent=m.pvi||'';$('hint').classList.add('hide');
  var rs=m.res||[false,false];
  $('res').innerHTML=[0,1].map(function(r){var ok=rs[r],c=m.target!==undefined?'':((m.nc?m.nc[r]:0)+'/'+(m.na?m.na[r]:0)+' justes · đúng');
   return '<div class="r'+r+'"><span class="big">'+(ok?'✓':'⏳')+'</span><span>'+esc(tname(r))+'<small>'+(m.center?'clé du pont posée · đã đặt viên đá khóa':(ok?'travée posée · đã đặt nhịp':'travée sur la barque · nhịp chờ trên thuyền'))+(c?' · '+c:'')+'</small></span></div>'}).join('');
  $('pont').classList.remove('hide');$('v-q').classList.add('rev');
  syncBridge(m.bridge,true);
  if(rs[0]&&rs[1])snd('ok');else if(!rs[0]&&!rs[1])snd('ko');
 }
 else if(m.t==='end'){
  state='end';syncBridge(m.bridge,false);view('v-end');$('endimg').classList.add('on');endDecor(true);$('qn').innerHTML='';$('cnt').textContent='';var d=m.dur||0;
  $('fiche').innerHTML='<div class="row"><span>Pont fermé en · Cầu nối sau</span><b>'+m.qs+' questions</b></div>'+
   '<div class="row"><span>Durée · Thời gian</span><b>'+Math.floor(d/60)+' min '+('0'+(d%60)).slice(-2)+'</b></div>'+
   '<div class="row"><span>Indices du petit dragon · Gợi ý của rồng</span><b>'+(m.hints||0)+'</b></div>'+
   '<div class="row"><span>'+esc(tname(0))+' : 4 travées · 4 nhịp</span><b>question '+(m.doneAt?m.doneAt[0]:'?')+'</b></div>'+
   '<div class="row"><span>'+esc(tname(1))+' : 4 travées · 4 nhịp</span><b>question '+(m.doneAt?m.doneAt[1]:'?')+'</b></div>'+
   '<div class="row"><span>Vitesse · Tốc độ</span><b>'+esc(tshort(0))+' '+(m.scores?m.scores[0]:0)+' · '+esc(tshort(1))+' '+(m.scores?m.scores[1]:0)+'</b></div>';
  $('names').innerHTML='<h4>Les bâtisseurs · Những người xây cầu</h4>'+players.map(function(p){return '<span class="t'+p.team+'">'+esc(p.n)+'</span>'}).join('');
  $('btn').textContent='Rejouer';
 }
 else if(m.t==='reset'){state='lobby';seen={};endDecor(false);syncBridge({side:[0,0],pending:[false,false],center:false,per:B.per},false);view('v-lobby');snapChars()}
}

// ---------------------------------------------------------------- démarrage ------
$('snd').classList.toggle('off',muted);
if(SP){buildBridge();initChars();initBoats();decor()}
view('v-lobby');connect();
</script></body></html>

)html";
