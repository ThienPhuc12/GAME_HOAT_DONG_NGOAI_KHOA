#pragma once
// =============================================================================
//  LE PONT · CẦU — page des joueurs (téléphones) — http://192.168.4.1/
//  Généré depuis web/player.html : modifie le .html puis relance gen_web_h.py,
//  ou édite directement ci-dessous (c'est du HTML/JS ordinaire).
//  VI: Tạo từ web/player.html: sửa tệp .html rồi chạy lại gen_web_h.py.
// =============================================================================
const char PLAYER_HTML[] PROGMEM = R"html(
<!doctype html>
<html lang="fr"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#04254e">
<title>Le Pont · Cầu</title>
<!-- Polices (Be Vietnam Pro, Playfair Display) et images servies par l'objet : /a/...
     VI: Phông chữ và hình ảnh do thiết bị phục vụ: /a/... -->
<link rel="stylesheet" href="/a/f.css?v=9d6f412c">
<style>
:root{--ink:#071530;--navy:#0b1d42;--card:#13295a;--line:#24407a;--cream:#f4efe3;--mut:#a9b8d6;--gold:#f2b544;--gold2:#ffd27a;--fr:#3b74d6;--vn:#d8463c;--ok:#5ad08a;--warn:#f6a623;
 --ui:BVP,system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;--title:PF,"Iowan Old Style","Palatino Linotype","Noto Serif",Georgia,serif}
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent}
html,body{height:100%;margin:0}
body{background:var(--ink);color:var(--cream);font-family:var(--ui);font-weight:500;-webkit-font-smoothing:antialiased;overscroll-behavior:none}
#app{position:relative;max-width:480px;margin:0 auto;height:100vh;height:100dvh;display:flex;flex-direction:column;overflow:hidden;background:var(--navy)}
.hide{display:none!important}

/* ---------- la scène (le monde = l'image 1536 x 1024, déplacé comme une caméra) ---------- */
#stage{position:relative;flex:none;height:42vh;overflow:hidden;background:#04254e;transition:height .9s cubic-bezier(.4,0,.2,1)}
#world{position:absolute;left:0;top:0;width:1536px;height:1024px;transform-origin:0 0;transition:transform .9s cubic-bezier(.4,0,.2,1)}
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
.pc{transition:opacity .5s}
.pc.ghost{opacity:.14}
.pc.new{animation:rise 1.2s cubic-bezier(.2,.9,.3,1.15) both}
@keyframes rise{0%{opacity:0;transform:translateY(130px) scale(.92)}60%{opacity:1}100%{transform:none}}
.ch{left:0;top:0;transition:filter .4s}
.ch.me .sp{filter:drop-shadow(0 0 3px rgba(255,246,214,.9)) drop-shadow(0 0 16px rgba(255,200,90,.85))}
.ch .tagme{position:absolute;left:-110px;top:-352px;width:220px;text-align:center;opacity:0;transition:opacity .4s;animation:bob 1.6s ease-in-out infinite}
.ch .tagme span{display:inline-block;padding:6px 18px 8px;border-radius:40px;background:linear-gradient(180deg,#ffe08f,#f2b544);color:#3a2606;font:700 38px/1 var(--ui);box-shadow:0 0 26px rgba(255,200,90,.85)}
.ch .tagme span::after{content:"";position:absolute;left:50%;bottom:-14px;margin-left:-14px;border:14px solid transparent;border-bottom:0;border-top-color:#f2b544}
.ch.me .tagme{opacity:1}
.ch.hop .sp{animation:hop .5s ease}
.pc.want{opacity:.55;animation:want 1.5s ease-in-out infinite}
@keyframes want{50%{opacity:.9;filter:drop-shadow(0 0 22px rgba(255,214,110,1))}}
.boat{left:0;top:0;opacity:0;transition:opacity .8s}
.boat.on{opacity:1}
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
.lotus{opacity:0;transition:opacity 2s}
.lotus.on{opacity:1}
.lotus .sp{animation:bob 4s ease-in-out infinite}
.lotus::before{content:"";position:absolute;left:-90px;top:-150px;width:180px;height:180px;border-radius:50%;background:radial-gradient(circle,rgba(255,200,120,.55),transparent 65%)}

/* ---------- bandeau du haut ---------- */
#hud{position:absolute;left:0;right:0;top:0;z-index:3;display:flex;align-items:flex-start;justify-content:space-between;gap:8px;padding:calc(10px + env(safe-area-inset-top)) 12px 0;pointer-events:none}
.chip{display:flex;align-items:center;gap:7px;max-width:62%;background:rgba(5,18,48,.74);-webkit-backdrop-filter:blur(6px);backdrop-filter:blur(6px);border:1px solid rgba(255,255,255,.12);border-radius:999px;padding:5px 12px 5px 7px;font-size:13px;font-weight:700;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.chip .dot{flex:none;width:12px;height:12px;border-radius:50%;background:var(--mut)}
.hr{display:flex;gap:8px;align-items:center;margin-left:auto}
#snd{pointer-events:auto;width:36px;height:36px;border-radius:50%;border:1px solid rgba(255,255,255,.14);background:rgba(5,18,48,.74);color:var(--cream);font-size:16px;line-height:1;padding:0}
#snd.off{opacity:.5;text-decoration:line-through}
#timer{position:relative;width:52px;height:52px;display:flex;align-items:center;justify-content:center;border-radius:50%;background:rgba(5,18,48,.8);font-weight:700;font-size:18px}
#timer svg{position:absolute;inset:0}
#timer circle{fill:none;stroke-width:4}
#tring{stroke:var(--gold);stroke-linecap:round;transition:stroke-dashoffset 1s linear,stroke .3s}
#timer.low{animation:pulse .5s ease-in-out infinite alternate}#timer.low #tring{stroke:var(--warn)}
@keyframes pulse{to{transform:scale(1.1)}}

/* ---------- le panneau ---------- */
#sheet{position:relative;z-index:2;flex:1;margin-top:-20px;overflow-y:auto;-webkit-overflow-scrolling:touch;border-radius:22px 22px 0 0;background:linear-gradient(180deg,#10265a,#0b1d42 140px,#081633);box-shadow:0 -10px 26px rgba(0,0,0,.35),inset 0 1px 0 rgba(255,210,122,.28);padding:16px 16px calc(22px + env(safe-area-inset-bottom))}
section{animation:in .45s ease both}
@keyframes in{from{opacity:0;transform:translateY(10px)}}
h1,h2{font-family:var(--title);font-weight:700;margin:0;letter-spacing:.005em}
h1{font-size:34px;color:var(--gold2);text-align:center;line-height:1.05;text-shadow:0 2px 18px rgba(242,181,68,.35)}
h1 span{color:var(--cream);opacity:.6;margin:0 .15em}
h2{font-size:26px;line-height:1.15}
.tag{text-align:center;color:var(--mut);font-size:14px;line-height:1.4;margin:6px 0 14px}
.vi{color:var(--mut);font-weight:500}
i.vi{font-style:normal}
.lbl{font-size:13px;font-weight:700;color:var(--mut);margin:14px 2px 8px;letter-spacing:.02em}
.stonebox{position:relative;height:100px;display:flex;align-items:center;justify-content:center}
.stonebox .sp{position:absolute;left:50%;top:50%;transform:translate(-50%,-50%);filter:drop-shadow(0 6px 10px rgba(0,0,0,.35))}
.stonebox input{position:relative;width:230px;max-width:70%;font:700 24px/1.2 var(--title);text-align:center;color:#6b4515;background:transparent;border:0;border-bottom:2px dashed rgba(107,69,21,.35);padding:6px 4px;text-shadow:0 1px 0 rgba(255,240,200,.55);outline:none}
.stonebox input::placeholder{color:rgba(107,69,21,.5);font-size:18px}
.stonelbl{text-align:center;font-size:12px;color:var(--mut);margin:2px 0 0}
.rives{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.rive{position:relative;display:flex;flex-direction:column;align-items:center;gap:2px;padding:10px 8px 12px;border-radius:18px;border:2px solid transparent;background:var(--card);text-align:center;line-height:1.2;cursor:pointer;transition:transform .15s,border-color .2s,box-shadow .2s}
.rive:active{transform:scale(.97)}
.rive b{font-size:15px}.rive small{font-size:11.5px;color:var(--mut)}
.rive .por{height:80px;display:flex;align-items:flex-end;justify-content:center}
.rive.r0{background:linear-gradient(170deg,#1d3d7e,#142a5c)}
.rive.r1{background:linear-gradient(170deg,#5a1f2c,#3a1530)}
.rive.sel{border-color:var(--gold2);box-shadow:0 0 0 3px rgba(255,210,122,.18),0 8px 22px rgba(0,0,0,.3)}
.rive.rnd{grid-column:1/3;flex-direction:row;justify-content:center;gap:8px;padding:10px;background:#162d60}
button{font-family:var(--ui)}
.primary{display:block;width:100%;margin:16px 0 4px;padding:15px 14px;border:0;border-radius:16px;font-size:17px;font-weight:700;color:#2a1a05;background:linear-gradient(180deg,#ffd77e,#f2b544);box-shadow:inset 0 -3px 0 rgba(0,0,0,.15),0 8px 20px rgba(242,181,68,.3);cursor:pointer}
.primary:active{transform:translateY(1px) scale(.99)}
.primary small{display:block;font-size:12.5px;font-weight:600;opacity:.75}
.stick{position:sticky;bottom:calc(-22px - env(safe-area-inset-bottom));margin:0 -16px calc(-22px - env(safe-area-inset-bottom));padding:10px 16px calc(14px + env(safe-area-inset-bottom));background:linear-gradient(180deg,rgba(8,22,51,0),#081633 30%)}
.stick .primary{margin:6px 0 0}
.note{font-size:12.5px;color:var(--mut);text-align:center;line-height:1.45;margin:10px 0 0}

.hello{font-size:16px;line-height:1.45;text-align:center;margin:2px 0 12px}
.hello b{color:var(--gold2)}
.lt2{display:grid;grid-template-columns:1fr 1fr;gap:10px}
.lcol{background:rgba(19,41,90,.6);border:1px solid var(--line);border-radius:16px;padding:10px 8px;min-height:120px}
.lcol h4{margin:0 0 6px;font-size:12px;letter-spacing:.04em;text-transform:uppercase;color:var(--mut);display:flex;align-items:center;gap:6px;justify-content:center}
.lcol h4 i{width:9px;height:9px;border-radius:50%}
.lts{display:flex;flex-wrap:wrap;justify-content:center;gap:4px 6px}
.lt{width:64px;display:flex;flex-direction:column;align-items:center;font-size:12px;line-height:1.15;text-align:center;word-break:break-word}
.lt .sp{filter:drop-shadow(0 0 10px rgba(255,190,80,.75))}
.lt.new .sp{animation:light 1.4s ease-out}
@keyframes light{0%{filter:brightness(.3) drop-shadow(0 0 0 transparent);transform:scale(.7)}60%{filter:brightness(1.5) drop-shadow(0 0 18px rgba(255,200,90,1))}}
.lt.mine span{color:var(--gold2);font-weight:700}
.how{margin:14px 0 0;display:flex;flex-direction:column;gap:8px}
.how h3{margin:4px 0 2px;font-size:13px;color:var(--mut);letter-spacing:.04em;text-transform:uppercase;text-align:center}
.step{display:flex;gap:10px;align-items:center;background:rgba(19,41,90,.55);border-radius:14px;padding:8px}
.step .tile{flex:none;width:84px;height:82px;border-radius:10px;background:url(/a/howto.webp?v=9d6f412c) no-repeat;background-size:252px 82px}
.step p{margin:0;font-size:13.5px;line-height:1.35}.step p i{display:block;color:var(--mut);font-style:normal;font-size:12.5px;margin-top:2px}

.prog{display:flex;align-items:center;justify-content:center;gap:5px;margin:0 0 10px}
.prog i{width:11px;height:16px;border-radius:6px 6px 5px 5px;border:1.5px solid #4a6296}
.prog i.on{border-color:transparent;background:radial-gradient(circle at 50% 40%,#fff1b8,#f2b544 60%,#c9801c);box-shadow:0 0 9px rgba(242,181,68,.8)}
.prog i.wait{border-style:dashed;border-color:var(--gold)}
.prog em{width:13px;height:13px;margin:0 6px;transform:rotate(45deg);border:1.5px solid #4a6296}
.prog em.on{background:var(--gold2);border-color:var(--gold2);box-shadow:0 0 12px var(--gold2)}
.prog b{font-size:11px;font-weight:700;color:var(--mut);margin:0 4px}
.pills{display:flex;flex-wrap:wrap;gap:6px;margin-bottom:8px}
.pill{font-size:11px;font-weight:700;letter-spacing:.04em;text-transform:uppercase;border-radius:999px;padding:4px 10px;background:#1d3770;color:#cfdaf0}
.pill.n{background:var(--gold);color:#1a1205}
.pill.dbl{background:linear-gradient(90deg,#ffd77e,#f791a6);color:#3a0f1c}
.q{font-size:18.5px;line-height:1.35;font-weight:700;margin:2px 0 4px}
.qvi{font-size:15px;line-height:1.4;color:var(--mut);margin:0 0 4px}
.dblnote{font-size:13px;color:#ffc7d2;margin:4px 0}
.say{display:flex;gap:8px;align-items:flex-end;margin:10px 0 4px}
#dsay .bub{font-size:13.5px;padding:8px 11px}
.say .sp{flex:none}
.bub{position:relative;flex:1;background:#fff6dc;color:#3a2606;border-radius:16px 16px 16px 4px;padding:9px 12px;font-size:14px;line-height:1.38;box-shadow:0 6px 16px rgba(0,0,0,.25)}
.bub b{display:block;font-size:11.5px;letter-spacing:.03em;text-transform:uppercase;color:#a0611c;margin-bottom:2px}
.bub i{display:block;font-style:normal;color:#6e5328;margin-top:3px;font-size:13px}
.say.pop .sp{animation:hop .7s ease}
@keyframes hop{30%{transform:translateY(-10px)}}
.opts{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin:12px 0 4px}
.opt{position:relative;min-height:82px;border:0;border-radius:16px;color:#fff;font-size:15.5px;font-weight:700;line-height:1.25;text-align:left;padding:36px 12px 12px;box-shadow:inset 0 -4px 0 rgba(0,0,0,.18),0 6px 14px rgba(0,0,0,.25);transition:transform .12s,opacity .35s,filter .35s,box-shadow .2s;cursor:pointer;overflow-wrap:anywhere}
.opt:active{transform:scale(.96)}
.opt .sh{position:absolute;left:12px;top:12px;width:20px;height:20px;background:#fff}
.opt.k0{background:linear-gradient(160deg,#4f8aea,#2d5fbf)}.opt.k0 .sh{border-radius:50%}
.opt.k1{background:linear-gradient(160deg,#e85d52,#b5352d)}.opt.k1 .sh{border-radius:3px}
.opt.k2{background:linear-gradient(160deg,#f7c45a,#dc9b2c);color:#2a1a05}.opt.k2 .sh{background:#2a1a05;clip-path:polygon(50% 0,100% 100%,0 100%)}
.opt.k3{background:linear-gradient(160deg,#1c9a8a,#0f6a60)}.opt.k3 .sh{transform:rotate(45deg) scale(.82);border-radius:2px}
.opt.sel{box-shadow:0 0 0 3px #fff,0 0 24px rgba(255,214,110,.7);transform:scale(1.02)}
.opts.done .opt:not(.sel){opacity:.32;filter:saturate(.5)}
.opt.off{opacity:.13;filter:grayscale(1);pointer-events:none}
.opt.ok{box-shadow:0 0 0 3px var(--ok),0 0 20px rgba(90,208,138,.6)}
.opt.ko{opacity:.4}
.opt.ok::after{content:"✓";position:absolute;right:12px;top:8px;font-size:20px}
.ropts .opt{min-height:56px;padding:30px 10px 9px;font-size:13.5px}.ropts .opt .sh{width:15px;height:15px;top:10px;left:10px}
.numrow{display:flex;gap:8px;margin:12px 0 4px}
.numrow input{flex:1;min-width:0;font:700 22px var(--ui);padding:12px 14px;border-radius:14px;border:2px solid var(--line);background:#0a1838;color:var(--cream);outline:none}
.numrow input:focus{border-color:var(--gold)}
.numrow .primary{width:auto;margin:0;padding:12px 18px}
.team{display:flex;align-items:center;justify-content:center;gap:8px;font-size:12.5px;color:var(--mut);margin:10px 0 0;text-align:center;flex-wrap:wrap}
.team .dots{display:flex;gap:4px}
.team .dots i{width:9px;height:9px;border-radius:50%;background:#2a4580}
.team .dots i.on{background:var(--gold);box-shadow:0 0 6px var(--gold)}
#qmsg{color:var(--gold2);font-weight:700}

.verdict{font-family:var(--title);font-size:25px;line-height:1.12;text-align:center;margin:2px 0 2px}
.verdict small{display:block;font-family:var(--ui);font-size:14px;font-weight:600;opacity:.8;margin-top:4px}
.verdict.good{color:var(--gold2);text-shadow:0 0 22px rgba(255,210,122,.45)}
.verdict.wait{color:#ffc58a}
.pts{text-align:center;margin:6px 0 0}
.pts span{display:inline-block;padding:4px 12px;border-radius:999px;background:rgba(242,181,68,.16);color:var(--gold2);font-weight:700;font-size:14px;animation:pop .6s cubic-bezier(.2,1.4,.4,1) both}
@keyframes pop{from{transform:scale(.4);opacity:0}}
.res{display:flex;justify-content:center;flex-wrap:wrap;gap:6px;margin:12px 0 2px}
.res span{border-radius:999px;padding:5px 11px;font-size:12.5px;line-height:1.2;font-weight:700}
.res .r0{background:rgba(59,116,214,.28);border:1px solid rgba(59,116,214,.6)}
.res .r1{background:rgba(216,70,60,.25);border:1px solid rgba(216,70,60,.55)}
.ans{display:flex;align-items:center;gap:10px;margin:10px 0 0;padding:10px 12px;border-radius:14px;background:rgba(19,41,90,.7);border:1px solid var(--line)}
.ans small{display:block;font-size:11.5px;color:var(--mut);font-weight:700;letter-spacing:.02em}
.ans b{font-size:15.5px;line-height:1.3}
.ans.ok{border-color:rgba(90,208,138,.7);box-shadow:0 0 16px rgba(90,208,138,.25)}
.ans.ko{opacity:.75;padding:7px 12px}.ans.ko b{font-size:14px}
.shp{flex:none;width:30px;height:30px;border-radius:9px;display:flex;align-items:center;justify-content:center}
.shp i{width:13px;height:13px;background:#fff}
.shp.k0{background:#3b74d6}.shp.k0 i{border-radius:50%}.shp.k1{background:#d8463c}.shp.k1 i{border-radius:2px}.shp.k2{background:#e9ad3e}.shp.k2 i{background:#2a1a05;clip-path:polygon(50% 0,100% 100%,0 100%)}.shp.k3{background:#14806f}.shp.k3 i{transform:rotate(45deg) scale(.85)}
.target{background:rgba(19,41,90,.7);border:1px solid var(--line);border-radius:14px;padding:10px 12px;font-size:14px;line-height:1.5;margin:8px 0}
.target b{color:var(--gold2);font-size:20px}
.savoir{margin:12px 0 4px;padding:12px 14px;border-radius:16px;background:linear-gradient(170deg,#fff3d1,#f6dfa6);color:#3a2606;font-size:14.5px;line-height:1.45;box-shadow:0 8px 20px rgba(0,0,0,.25)}
.savoir h4{margin:0 0 4px;font:700 15px var(--title);color:#8a4d12}
.savoir .vi{color:#6e5328;margin-top:6px;font-size:13.5px}

.endt{text-align:center}
.endt h2{color:var(--gold2);font-size:32px;text-shadow:0 0 24px rgba(255,210,122,.4)}
.endt h2 span{display:block;color:var(--cream);font-size:22px;opacity:.85;margin-top:2px}
.endt p{color:var(--mut);font-size:14px;line-height:1.45;margin:8px 0 0}
.souv{margin:14px 0 0;padding:14px;border-radius:18px;background:rgba(19,41,90,.7);border:1px solid var(--line)}
.souv .row{display:flex;justify-content:space-between;gap:10px;font-size:14px;padding:6px 0;border-bottom:1px solid rgba(255,255,255,.06)}
.souv .row:last-child{border:0}
.souv .row span:first-child{color:var(--mut)}
.souv .row b{color:var(--gold2)}
.names{display:flex;flex-wrap:wrap;gap:6px;justify-content:center;margin:12px 0 0}
.names span{padding:6px 12px;border-radius:8px;background:linear-gradient(180deg,#f1cf8d,#d9a75b);color:#5a3810;font:700 14px var(--title);box-shadow:inset 0 -2px 0 rgba(0,0,0,.15),0 3px 8px rgba(0,0,0,.25);border-bottom:3px solid var(--fr)}
.names span.t1{border-bottom-color:var(--vn)}
.shot{text-align:center;color:var(--mut);font-size:12.5px;margin:14px 0 0}
#st{position:fixed;top:0;left:0;right:0;z-index:9;padding:6px 10px;font-size:12.5px;text-align:center;background:#7f1d1d;color:#fff}
</style></head><body>
<div id="st" class="hide">Connexion à l'objet… · Đang kết nối…</div>
<div id="app">
 <div id="stage">
  <div id="world">
   <div id="bg"></div>
   <div id="moon"></div>
   <div id="stars"></div>
   <div id="chars"></div>
   <div id="bridge"></div>
   <div id="boats"></div>
   <div id="fx"></div>
   <div id="endimg"></div>
   <div id="lotus"></div>
   <div id="fly"></div>
  </div>
  <div id="hud">
   <div class="chip hide" id="who"><i class="dot" id="wdot"></i><span id="sub">Le Pont · Cầu</span></div>
   <div class="hr"><div id="timer" class="hide"><svg viewBox="0 0 52 52"><circle cx="26" cy="26" r="22" stroke="#24407a"/><circle id="tring" cx="26" cy="26" r="22" stroke-dasharray="138.2" transform="rotate(-90 26 26)"/></svg><span id="tsec"></span></div>
   <button id="snd" onclick="toggleSnd()" aria-label="Son · Âm thanh">♪</button></div>
  </div>
 </div>

 <div id="sheet">
  <section id="s-join">
   <h1>Le Pont<span>·</span>Cầu</h1>
   <p class="tag">Deux rives, un pont à construire ensemble<br><i class="vi">Hai bờ sông, một cây cầu cùng nhau xây</i></p>
   <div class="stonebox" id="stonebox"><input id="name" placeholder="Ton prénom · Tên bạn" maxlength="16" autocomplete="off" autocapitalize="words" enterkeyhint="done"></div>
   <p class="stonelbl">Grave ton prénom sur la pierre · Khắc tên bạn lên phiến đá</p>
   <p class="lbl">Choisis ta rive · Chọn bờ của bạn</p>
   <div class="rives" id="rives"></div>
   <p class="note hide" id="full">L'objet est complet · Đã đủ người chơi</p>
   <div class="stick"><button class="primary" onclick="join()">Rejoindre le pont<small>Tham gia xây cầu</small></button></div>
  </section>

  <section id="s-lobby" class="hide">
   <p class="hello" id="hello"></p>
   <div class="lt2"><div class="lcol"><h4><i style="background:var(--fr)"></i><span id="ln0">France</span></h4><div class="lts" id="lf"></div></div>
                    <div class="lcol"><h4><i style="background:var(--vn)"></i><span id="ln1">Vietnam</span></h4><div class="lts" id="lv"></div></div></div>
   <p class="note">Chaque joueur allume une lanterne. Le pont commence quand l'animateur lance la partie. <span id="np"></span><br><i class="vi">Mỗi người chơi thắp một chiếc đèn lồng. Trò chơi bắt đầu khi người điều phối bấm bắt đầu.</i></p>
   <div class="how">
    <h3>Comment jouer · Cách chơi</h3>
    <div class="step"><span class="tile" style="background-position:0 0"></span><p>Deux enfants se font signe d'une rive à l'autre.<i>Hai bạn nhỏ vẫy chào nhau từ hai bờ sông.</i></p></div>
    <div class="step"><span class="tile" style="background-position:-84px 0"></span><p>Chaque bonne réponse de ta rive pose une travée et allume une lanterne.<i>Mỗi câu trả lời đúng của bờ bạn đặt thêm một nhịp cầu và thắp sáng một chiếc đèn.</i></p></div>
    <div class="step"><span class="tile" style="background-position:-168px 0"></span><p>Quand les deux moitiés se touchent, réussissez ensemble la Rencontre.<i>Khi hai nửa cầu chạm nhau, hãy cùng trả lời đúng câu Gặp gỡ.</i></p></div>
   </div>
  </section>

  <section id="s-q" class="hide">
   <div class="prog" id="prog"></div>
   <div class="pills"><span class="pill n" id="qn"></span><span class="pill" id="qcat"></span><span class="pill dbl hide" id="qdbl">★ Rencontre · Gặp gỡ</span></div>
   <p class="q" id="qfr"></p>
   <p class="qvi" id="qvi"></p>
   <p class="dblnote hide" id="dblnote">Les deux rives doivent réussir pour poser la clé du pont · Cả hai bờ phải trả lời đúng để đặt viên đá khóa</p>
   <div class="say hide" id="hint"><span id="hdr"></span><div class="bub"><b>Le petit dragon · Rồng nhỏ mách</b><span id="hinttxt"></span></div></div>
   <div class="opts" id="opts"></div>
   <div id="num" class="hide"><div class="numrow"><input id="numin" type="number" inputmode="numeric" placeholder="Ton estimation · Ước lượng"><button class="primary" onclick="answerNum()">Envoyer<small>Gửi</small></button></div></div>
   <div class="team"><span id="qmsg"></span><span id="tmate"></span><span class="dots" id="tdots"></span></div>
  </section>

  <section id="s-rev" class="hide">
   <div class="verdict" id="verdict" aria-live="polite"></div>
   <div class="pts" id="pts"></div>
   <div class="say" id="dsay"><span id="ddr"></span><div class="bub" id="dtxt"></div></div>
   <div id="ropts"></div>
   <div class="savoir"><h4>Le saviez-vous ? · Bạn có biết?</h4><div id="rfr"></div><div class="vi" id="rvi"></div></div>
   <div class="res" id="res"></div>
  </section>

  <section id="s-end" class="hide">
   <div class="endt"><h2>Le pont est fermé<span>Cầu đã nối</span></h2>
   <p>Paris et Hội An se sont rejoints au milieu du fleuve.<br><i class="vi">Paris và Hội An đã gặp nhau giữa dòng sông.</i></p></div>
   <div class="souv" id="fiche"></div>
   <p class="lbl" style="text-align:center">Les bâtisseurs · Những người xây cầu</p>
   <div class="names" id="names"></div>
   <p class="shot">📸 Fais une capture d'écran pour garder ce souvenir<br>Chụp màn hình để giữ kỷ niệm này</p>
  </section>
 </div>
</div>
<script>
// =====================================================================================
//  Le Pont · Cầu — page du téléphone. Protocole WebSocket inchangé (voir LePont.ino).
//  VI: Trang của điện thoại. Giao thức WebSocket không đổi (xem LePont.ino).
// =====================================================================================
var SP=/*SP*/{"W":1536,"H":909,"k":0.833333,"s":{"bL1":[1041,462,112,139,0,0,0.8333],"bL2":[221,668,92,122,0,0,0.8333],"bL3":[614,668,101,120,0,0,0.8333],"bL4":[814,668,92,113,0,0,0.8333],"bR4":[719,668,91,115,0,0,0.8333],"bR3":[317,668,100,122,0,0,0.8333],"bR2":[121,668,96,127,0,0,0.8333],"bR1":[0,668,117,128,0,0,0.8333],"bC":[1157,462,60,136,0,0,0.8333],"boyP1":[0,0,133,242,57.1,241.9,0.42],"boyP2":[137,0,127,237,63.3,237.3,0.42],"boyP3":[409,0,162,234,78.4,233.9,0.42],"boyP4":[872,0,234,222,126.5,222.2,0.42],"boyW1":[114,246,143,211,68.6,211.3,0.37],"boyW2":[0,246,110,212,53.2,212.4,0.37],"boyW3":[799,246,147,204,75.1,203.9,0.37],"boyW4":[487,246,120,207,52.0,206.8,0.37],"girlP1":[268,0,137,236,64.6,235.6,0.42],"girlP2":[575,0,132,231,51.6,230.6,0.42],"girlP3":[711,0,157,226,76.0,225.5,0.42],"girlP4":[1110,0,218,218,102.7,217.6,0.42],"girlW1":[950,246,143,204,54.5,204.2,0.37],"girlW2":[1097,246,134,204,42.8,203.5,0.37],"girlW3":[193,462,147,200,56.0,199.8,0.37],"girlW4":[1235,246,137,203,46.7,202.8,0.37],"dragon1":[611,246,184,206,91.8,206.3,0.38],"dragon2":[261,246,222,209,111.0,208.6,0.38],"dragon3":[1332,0,192,216,95.8,216.2,0.38],"dragon4":[0,462,189,202,94.6,201.8,0.38],"dragonFly":[1221,462,253,131,126.5,65.5,0.5],"lan0":[344,462,89,158,44.6,78.9,0.3],"lan1":[437,462,89,158,44.4,78.9,0.3],"lotus":[421,668,189,121,94.4,121.0,0.34],"boatF":[910,668,319,109,159.4,109.1,0.32],"boatV":[0,800,324,109,161.9,109.4,0.32],"stone":[530,462,507,152,253.5,76.0,0.5]},"bridge":{"bL1":[330,494],"bL2":[446,479],"bL3":[538,468],"bL4":[640,468],"bR4":[784,468],"bR3":[874,468],"bR2":[975,476],"bR1":[1071,498],"bC":[732,393]},"deck":[0.00034,-0.52381,741.666667],"lant":[[406,620],[503,585],[598,571],[701,563],[830,565],[934,573],[1025,587],[1123,612]]}/*SP*/;                       // atlas des images (rempli par build_assets.py)
var VI_BANK=['Bờ Pháp','Bờ Việt Nam'];
var TEAMS=[],me={name:'',team:-1,id:0},ws,cur=null,myAns=-1,B={side:[0,0],pending:[false,false],center:false,per:4};
var VIEW='join',players=[],teamN=[0,0],lastPts=0,lastTick=-1,seen={};
function $(i){return document.getElementById(i)}
function esc(s){return String(s==null?'':s).replace(/[&<>"]/g,function(c){return{'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]})}
function tname(i){return TEAMS[i]?TEAMS[i].n:(i?'Rive Vietnam':'Rive France')}
function tshort(i){return tname(i).replace(/^Rive\s+/i,'')}
function tcol(i){return i===1?'#d8463c':i===0?'#3b74d6':'#a9b8d6'}

// ---------------------------------------------------------------- images ---------
// frame(el, nom, pps) : affiche l'image « nom » de l'atlas ; pps = px de l'écran par px de l'image d'origine.
// L'élément est placé par son ancre (les pieds pour un personnage).
// VI: hiển thị hình « nom » trong atlas; pps = số px màn hình cho mỗi px ảnh gốc; neo ở chân nhân vật.
function frame(el,n,pps){var r=SP.s[n],k=pps/r[6],s=el.style;s.width=r[2]*k+'px';s.height=r[3]*k+'px';s.backgroundSize=SP.W*k+'px '+SP.H*k+'px';s.backgroundPosition=(-r[0]*k)+'px '+(-r[1]*k)+'px';s.marginLeft=(-r[4]*k)+'px';s.marginTop=(-r[5]*k)+'px'}
function ui(n,h,extra){if(!SP)return'';var r=SP.s[n],k=h/r[3];return '<i class="sp ui'+(extra?' '+extra:'')+'" style="width:'+(r[2]*k).toFixed(1)+'px;height:'+h+'px;background-size:'+(SP.W*k).toFixed(1)+'px '+(SP.H*k).toFixed(1)+'px;background-position:'+(-r[0]*k).toFixed(1)+'px '+(-r[1]*k).toFixed(1)+'px"></i>'}
function mk(cls,parent){var e=document.createElement('div');e.className=cls;(parent||$('fx')).appendChild(e);return e}
function at(e,x,y){e.style.transform='translate('+x+'px,'+y+'px)'}

// ---------------------------------------------------------------- caméra ---------
var FOC={join:[120,170,1296,700],lobby:[160,170,1216,620],q:[250,270,1036,480],rev:[220,230,1096,540],end:[150,80,1236,820]};
var HF={join:.36,lobby:.34,q:.31,rev:.4,end:.5};
// En jeu, la caméra suit les deux enfants : plus le pont avance, plus elle se rapproche.
// VI: Trong lúc chơi, camera theo hai bạn nhỏ: cầu càng dài, camera càng lại gần.
function focus(){
 if(VIEW!=='q'&&VIEW!=='rev')return FOC[VIEW];
 var a=B.center?712:tipX(0,B.side[0]),b=B.center?828:tipX(1,B.side[1]),x0=Math.min(a,b)-175,x1=Math.max(a,b)+175,w=Math.max(x1-x0,760),cx=(x0+x1)/2;
 return [cx-w/2,250,w,VIEW==='rev'?540:490];
}
function camera(){
 if(!SP)return;
 var W=$('app').clientWidth,Hs=Math.round(Math.max(175,Math.min(innerHeight*HF[VIEW],400))),H=Hs-20,f=focus();
 $('stage').style.height=Hs+'px';
 var s=Math.max(Math.min(W/f[2],H/f[3]),W/1536,Hs/1024),tx=W/2-s*(f[0]+f[2]/2),ty=H/2-s*(f[1]+f[3]/2);
 tx=Math.min(0,Math.max(W-1536*s,tx));ty=Math.min(0,Math.max(Hs-1024*s,ty));
 $('world').style.transform='translate('+tx.toFixed(1)+'px,'+ty.toFixed(1)+'px) scale('+s.toFixed(4)+')';
}
function show(id){
 ['s-join','s-lobby','s-q','s-rev','s-end'].forEach(function(s){$(s).classList.toggle('hide',s!==id)});
 VIEW={'s-join':'join','s-lobby':'lobby','s-q':'q','s-rev':'rev','s-end':'end'}[id];
 $('timer').classList.toggle('hide',VIEW!=='q');$('sheet').scrollTop=0;camera();
}
addEventListener('resize',camera);

// ---------------------------------------------------------------- le pont --------
// Postes (x) : rive gauche du bord vers le centre, rive droite du bord vers le centre ; LANT = lanternes sous chaque travée.
// VI: Vị trí các cột: bờ trái từ mép vào giữa, bờ phải từ mép vào giữa; LANT = đèn lồng dưới mỗi nhịp.
var POST=[[353,455,547,649,747],[1180,1080,984,883,787]];
function deck(x){var d=SP.deck;return d[0]*x*x+d[1]*x+d[2]}
function ground(x){if(x<=330||x>=1206)return 630;var d=deck(x);if(x<372)return 630+(d-630)*(x-330)/42;if(x>1164)return 630+(d-630)*(1206-x)/42;return d}
function tipX(r,n){if(B.center||n>B.per)return r?828:712;if(n===0)return r?1256:280;if(n>=B.per)return r?858:676;return r?POST[1][n]+36:POST[0][n]-36}
var PC={};
function buildBridge(){
 var h=$('bridge');h.innerHTML='';
 ['L1','L2','L3','L4','C','R4','R3','R2','R1'].forEach(function(k){var e=document.createElement('i');e.className='sp pc ghost';frame(e,'b'+k,1);e.style.margin='0';var p=SP.bridge['b'+k];e.style.left=p[0]+'px';e.style.top=p[1]+'px';h.appendChild(e);PC[k]=e});
}
function pieceKey(r,i){return (r?'R':'L')+i}
function lantXY(r,i){return r?SP.lant[8-i]:SP.lant[i-1]}
function setPiece(k,on,anim){var e=PC[k];if(!e)return;e.classList.toggle('ghost',!on);if(anim&&on){e.classList.remove('new');void e.offsetWidth;e.classList.add('new')}}
function burst(x,y,n){var h=mk('halo');h.style.left=x+'px';h.style.top=y+'px';setTimeout(function(){h.remove()},1900);
 for(var i=0;i<(n||10);i++){var s=mk('spark'),a=Math.random()*Math.PI*2,d=60+Math.random()*120;s.style.setProperty('--dx',Math.cos(a)*d+'px');s.style.setProperty('--dy',(Math.sin(a)*d-90)+'px');s.style.left=x+'px';s.style.top=y+'px';s.style.animationDelay=(Math.random()*.3)+'s';(function(s){setTimeout(function(){s.remove()},2100)})(s)}}

// ---------------------------------------------------------------- personnages ----
var WHO=['boy','girl'],REFH=[570,555],CHH=250,CH=[];
function pps(r){return CHH/REFH[r]}
function initChars(){
 for(var r=0;r<2;r++){var w=mk('ch',$('chars')),s=document.createElement('i');s.className='sp';w.appendChild(s);w.insertAdjacentHTML('beforeend','<b class="tagme"><span>toi · bạn</span></b>');CH[r]={el:w,sp:s,x:0,y:0,pose:'',raf:0,tmo:0};setPose(r,'P1');place(r,tipX(r,0),ground(tipX(r,0)))}
}
function setPose(r,p){var c=CH[r];if(c.pose===p)return;c.pose=p;frame(c.sp,WHO[r]+p,pps(r))}
function place(r,x,y){var c=CH[r];c.x=x;c.y=y;at(c.el,x,y)}
function idle(r){var n=B.side[r];return VIEW==='join'||VIEW==='lobby'?'P1':(n>=B.per&&B.side[1-r]>=B.per&&!B.center?'P4':'P2')}
function walkTo(r,x1,then){
 var c=CH[r];cancelAnimationFrame(c.raf);clearTimeout(c.tmo);var x0=c.x,t0=performance.now(),dur=Math.max(350,Math.abs(x1-x0)/115*1000),fi=0,lf=0;
 if(Math.abs(x1-x0)<2){setPose(r,idle(r));then&&then();return}
 (function step(t){var k=Math.min(1,(t-t0)/dur),x=x0+(x1-x0)*k;place(r,x,ground(x));
  if(t-lf>135){lf=t;fi=(fi+1)%4;setPose(r,'W'+(fi+1))}
  if(k<1)c.raf=requestAnimationFrame(step);else{setPose(r,idle(r));then&&then()}})(t0);
}
function cheer(r){var c=CH[r];setPose(r,'P3');clearTimeout(c.tmo);c.tmo=setTimeout(function(){setPose(r,idle(r))},900)}
function snapChars(){for(var r=0;r<2;r++){var c=CH[r];cancelAnimationFrame(c.raf);var x=tipX(r,B.side[r]);place(r,x,ground(x));c.pose='';setPose(r,idle(r))}}
function hopMe(){var c=CH[me.team];if(!c)return;c.el.classList.remove('hop');void c.el.offsetWidth;c.el.classList.add('hop')}
function markMe(){for(var r=0;r<2;r++)CH[r].el.classList.toggle('me',r===me.team&&!!me.id)}

// ---------------------------------------------------------------- barques --------
// Une travée ratée attend sur la barque de sa rive, sous le trou du pont.
// VI: Nhịp cầu chưa đặt được chờ trên thuyền của bờ đó, dưới chỗ trống của cầu.
var BOAT=[];
function initBoats(){for(var r=0;r<2;r++){var w=mk('boat',$('boats')),s=document.createElement('i');s.className='sp';w.appendChild(s);frame(s,r?'boatV':'boatF',.27);if(r)s.style.transform='scaleX(-1)';BOAT[r]=w}}
function boatTo(r,on){var w=BOAT[r],i=Math.min(B.per,B.side[r]+1),l=lantXY(r,i),x=on?l[0]:(r?1700:-180);
 w.style.transition='opacity .8s, transform '+(on?'1.4s':'2.2s')+' ease-in-out';at(w,x,748);w.classList.toggle('on',on)}

// ---------------------------------------------------------------- état du pont ---
function syncBridge(nb,anim){
 var old=B;B=nb;if(!B.per)B.per=4;
 for(var r=0;r<2;r++){
  var from=old.side?old.side[r]:0,to=B.side[r];
  for(var i=1;i<=4;i++)setPiece(pieceKey(r,i),i<=to,anim&&i>from&&i<=to);
  if(anim&&to>from){(function(r,to){var l=lantXY(r,to);setTimeout(function(){burst(l[0],l[1]-30,12);snd('place')},450);setTimeout(function(){walkTo(r,tipX(r,to),function(){if(VIEW==='rev')cheer(r)})},900)})(r,to)}
  else if(!anim||to<from){var x=tipX(r,to);cancelAnimationFrame(CH[r].raf);place(r,x,ground(x));CH[r].pose='';setPose(r,idle(r))}
  else setPose(r,idle(r));
  boatTo(r,!!(B.pending&&B.pending[r])&&!B.center&&to<B.per);
 }
 setPiece('C',B.center,anim&&B.center&&!old.center);
 PC.C&&PC.C.classList.toggle('want',!B.center&&B.side[0]>=B.per&&B.side[1]>=B.per);
 if(anim&&B.center&&!old.center)meet();
 if(!B.center)$('endimg').classList.remove('on');
 progress();camera();
}
function meet(){
 setTimeout(function(){burst(770,420,22);snd('place')},400);
 for(var r=0;r<2;r++)(function(r){setTimeout(function(){walkTo(r,tipX(r,9),function(){setPose(r,'P4')})},800)})(r);
 setTimeout(function(){$('endimg').classList.add('on');snd('end')},3600);
}
function progress(){
 var h='<b>'+esc(tshort(0))+'</b>';
 for(var i=1;i<=4;i++)h+='<i class="'+(i<=B.side[0]?'on':(i===B.side[0]+1&&B.pending[0]?'wait':''))+'"></i>';
 h+='<em class="'+(B.center?'on':'')+'"></em>';
 for(var i=4;i>=1;i--)h+='<i class="'+(i<=B.side[1]?'on':(i===B.side[1]+1&&B.pending[1]?'wait':''))+'"></i>';
 $('prog').innerHTML=h+'<b>'+esc(tshort(1))+'</b>';
}

// ---------------------------------------------------------------- décor vivant ---
function decor(){
 var st=[[120,60],[300,150],[470,40],[560,250],[1010,60],[1120,170],[1290,90],[1430,40],[930,300],[650,120],[220,300],[1360,260]];
 st.forEach(function(p,i){var e=mk('star',$('stars'));e.style.left=p[0]+'px';e.style.top=p[1]+'px';e.style.animationDelay=(i*.53%3.2)+'s'});
 [[330,930],[560,880],[800,965],[1010,895],[1230,940]].forEach(function(p,i){var w=mk('lotus',$('lotus')),s=document.createElement('i');s.className='sp';w.appendChild(s);frame(s,'lotus',.24);at(w,p[0],p[1]);s.style.animationDelay=(i*.7)+'s'});
 var f=$('fly'),s=document.createElement('i');s.className='sp';f.appendChild(s);frame(s,'dragonFly',.5);
}
function endDecor(on){$('fly').classList.toggle('on',on);[].forEach.call(document.querySelectorAll('.lotus'),function(e,i){setTimeout(function(){e.classList.toggle('on',on)},on?600+i*500:0)})}

// ---------------------------------------------------------------- sons -----------
// Petits sons synthétisés (aucun fichier). Gamme pentatonique, timbre de cloche.
// VI: Âm thanh tổng hợp (không cần tệp). Thang âm ngũ cung, âm sắc chuông.
var AC=null,muted=false;
try{muted=localStorage.getItem('lp_mute')==='1'}catch(x){}
function actx(){if(!AC){try{AC=new(window.AudioContext||window.webkitAudioContext)()}catch(x){}}if(AC&&AC.state==='suspended')AC.resume();return AC}
function tone(f,t,d,v,ty){var o=AC.createOscillator(),g=AC.createGain();o.type=ty||'sine';o.frequency.value=f;g.gain.setValueAtTime(.0001,t);g.gain.exponentialRampToValueAtTime(v,t+.012);g.gain.exponentialRampToValueAtTime(.0001,t+d);o.connect(g);g.connect(AC.destination);o.start(t);o.stop(t+d+.05)}
function bell(f,t,d,v){tone(f,t,d,v);tone(f*2.01,t,d*.55,v*.32);tone(f*3.03,t,d*.3,v*.12)}
var NOTE={G4:392,A4:440,C5:523.3,D5:587.3,E5:659.3,G5:784,A5:880,C6:1046.5};
var SND={tap:function(t){tone(540,t,.09,.07,'triangle')},tick:function(t){tone(1300,t,.05,.045)},
 ok:function(t){['C5','D5','E5','G5','A5'].forEach(function(n,i){bell(NOTE[n],t+i*.085,.9,.11)})},
 ko:function(t){bell(NOTE.E5,t,.7,.09);bell(NOTE.C5,t+.2,1,.09)},
 place:function(t){bell(NOTE.C6,t,1.3,.08);bell(NOTE.G5,t+.1,1.3,.07)},
 join:function(t){bell(NOTE.G5,t,.8,.08);bell(NOTE.C6,t+.12,1,.08)},
 end:function(t){'G4 A4 C5 D5 E5 D5 C5 A4 C5 D5 E5 G5 A5 G5 E5 D5 C5'.split(' ').forEach(function(n,i){bell(NOTE[n],t+i*.26,i===16?2.4:1.1,.1)})}};
function snd(n){if(muted)return;var c=actx();if(!c)return;try{SND[n](c.currentTime+.03)}catch(x){}}
function buzz(p){try{navigator.vibrate&&navigator.vibrate(p)}catch(x){}}
function toggleSnd(){muted=!muted;$('snd').classList.toggle('off',muted);try{localStorage.setItem('lp_mute',muted?'1':'0')}catch(x){}if(!muted){actx();snd('tap')}}

// ---------------------------------------------------------------- réseau ---------
function connect(){
 ws=new WebSocket('ws://'+location.host+'/ws');
 ws.onopen=function(){$('st').classList.add('hide');if(me.name&&me.id)send({t:'join',name:me.name,team:me.team})};
 ws.onclose=function(){$('st').classList.remove('hide');setTimeout(connect,2000)};
 ws.onerror=function(){};
 ws.onmessage=function(e){var m;try{m=JSON.parse(e.data)}catch(x){return}handle(m)};
}
function send(o){if(ws&&ws.readyState===1)ws.send(JSON.stringify(o))}

// ---------------------------------------------------------------- écran 1 : rejoindre
function renderRives(){
 var h='';for(var i=0;i<2;i++)h+='<div class="rive r'+i+(i===me.team?' sel':'')+'" onclick="pickTeam('+i+')"><span class="por">'+ui(WHO[i]+'P1',92)+'</span><b>'+esc(tname(i))+'</b><small>'+VI_BANK[i]+' · '+(i?'construit depuis la droite':'construit depuis la gauche')+'</small></div>';
 h+='<div class="rive rnd'+(me.team<0?' sel':'')+'" onclick="pickTeam(-1)"><b>Au hasard · Ngẫu nhiên</b><small>l\'objet équilibre les rives</small></div>';
 $('rives').innerHTML=h;
}
function pickTeam(i){me.team=i;renderRives();actx();snd('tap');if(i>=0){cheer(i);var c=CH[i];burst(c.x,c.y-120,8)}}
function join(){
 var n=$('name').value.trim();if(!n){$('name').focus();$('stonebox').animate&&$('stonebox').animate([{transform:'translateX(-6px)'},{transform:'translateX(6px)'},{transform:'none'}],{duration:260});return}
 me.name=n;try{localStorage.setItem('lp_name',n);localStorage.setItem('lp_team',me.team)}catch(x){}
 actx();snd('join');send({t:'join',name:n,team:me.team});
}
function setWho(extra){$('who').classList.remove('hide');$('wdot').style.background=tcol(me.team);$('sub').textContent=me.name+' · '+tname(me.team)+(extra||'')}

// ---------------------------------------------------------------- écran 2 : salon
function renderLobby(){
 var cols=[[],[]];players.forEach(function(p){if(p.team===0||p.team===1)cols[p.team].push(p)});
 ['lf','lv'].forEach(function(id,r){$(id).innerHTML=cols[r].map(function(p){var k=r+'|'+p.n,nw=!seen[k];seen[k]=1;return '<div class="lt'+(nw?' new':'')+(p.n===me.name&&r===me.team?' mine':'')+'">'+ui('lan1',46)+'<span>'+esc(p.n)+'</span></div>'}).join('')||'<span class="note" style="margin:8px 0 0">…</span>'});
 $('ln0').textContent=tshort(0);$('ln1').textContent=tshort(1);
 $('np').textContent='('+players.length+(players.length>1?' joueurs · người chơi)':' joueur · người chơi)');
 $('hello').innerHTML='Bienvenue <b>'+esc(me.name)+'</b> sur la <b>'+esc(tname(me.team))+'</b> !<br><i class="vi">Chào mừng '+esc(me.name)+' đến '+VI_BANK[me.team]+'!</i>';
}

// ---------------------------------------------------------------- écran 3 : question
function teamCount(){teamN=[0,0];players.forEach(function(p){if(p.team===0||p.team===1)teamN[p.team]++})}
function showTeam(n){var t=Math.max(teamN[me.team]||1,n||0),h='';for(var i=0;i<t;i++)h+='<i class="'+(i<n?'on':'')+'"></i>';$('tdots').innerHTML=h;$('tmate').textContent='Ta rive · Bờ của bạn : '+n+'/'+t}
function setTimer(left,total){var k=Math.max(0,Math.min(1,left/(total||20)));$('tring').style.strokeDashoffset=(138.2*(1-k)).toFixed(1);$('tsec').textContent=left;$('timer').classList.toggle('low',left<=5&&left>0&&myAns<0)}

// ---------------------------------------------------------------- messages -------
function handle(m){
 if(m.t==='config'){TEAMS=m.teams||[];if(m.per)B.per=m.per;renderRives()}
 else if(m.t==='joined'){me.id=m.id;if(m.team!==undefined)me.team=m.team;
  setWho(m.moved?' — rive vide, tu la tiens · bạn giữ bờ này':'');markMe();
  if(m.moved){try{localStorage.setItem('lp_team',me.team)}catch(x){}}
  if(VIEW==='join')show('s-lobby');renderLobby();}
 else if(m.t==='full'){$('full').classList.remove('hide')}
 else if(m.t==='lobby'){
  players=m.players||[];teamCount();if(m.bridge)syncBridge(m.bridge,false);
  if(me.id){if(VIEW!=='lobby')show('s-lobby');renderLobby();snapChars()}
 }
 else if(m.t==='question'){
  if(!me.id)return;
  var again=cur&&cur.i===m.i;cur=m;if(!again)myAns=-1;
  endDecor(false);show('s-q');if(m.bridge)syncBridge(m.bridge,false);snapChars();
  $('qn').textContent='Question '+m.i;$('qcat').textContent=m.cat||'';$('qdbl').classList.toggle('hide',!m.dbl);$('dblnote').classList.toggle('hide',!m.dbl);
  $('qfr').textContent=m.fr;$('qvi').textContent=m.vi;$('qmsg').textContent='';
  var hide=(m.hide&&m.hide[me.team])||[];
  if(m.hint&&(m.dbl||(m.assist&&m.assist[me.team]))){$('hint').classList.remove('hide');$('hdr').innerHTML=ui('dragon2',74);$('hinttxt').textContent=m.hint;$('hint').classList.add('pop')}else $('hint').classList.add('hide');
  if(m.num){$('opts').innerHTML='';$('num').classList.remove('hide');if(!again){$('numin').value=''}}
  else{$('num').classList.add('hide');$('opts').className='opts';$('opts').innerHTML=m.o.map(function(o,k){return '<button class="opt k'+k+(hide.indexOf(k)>=0?' off':'')+'" id="o'+k+'" onclick="answer('+k+')"><span class="sh"></span><span>'+esc(o)+'</span></button>'}).join('')}
  showTeam(0);setTimer(m.sec,m.sec);lastPts=0;lastTick=-1;
  if(!again){snd('tap');buzz(20)}
 }
 else if(m.t==='tick'){if(!cur)return;setTimer(m.left,cur.sec||20);if(m.answers)showTeam(m.answers[me.team]||0);
  if(m.left<=5&&m.left>0&&m.left!==lastTick&&myAns<0)snd('tick');lastTick=m.left}
 else if(m.t==='you'){lastPts=m.pts||0}
 else if(m.t==='reveal'){
  if(!me.id||!cur)return;
  show('s-rev');var nb=m.bridge||B,mine=m.res?m.res[me.team]:false,already=B.side[me.team]>=B.per&&!m.dbl;
  var v=$('verdict'),d='';
  if(m.bridge&&nb.center&&!B.center){v.className='verdict good';v.innerHTML='Le pont est fermé !<small>Cầu đã nối!</small>';d='<b>Rồng nhỏ</b>Les deux rives se sont trouvées. Regardez le fleuve !<i>Hai bờ đã gặp nhau. Hãy nhìn dòng sông!</i>'}
  else if(m.dbl){v.className='verdict '+(mine?'good':'wait');v.innerHTML=mine?'Ta rive a réussi la Rencontre<small>Bờ của bạn đã trả lời đúng</small>':'Rencontre manquée, on réessaie<small>Chưa gặp được nhau — thử lại nhé</small>';d=mine?'<b>Rồng nhỏ</b>Il faut aussi l\'autre rive. Encore un effort !<i>Cần cả bờ bên kia nữa. Cố lên!</i>':'<b>Rồng nhỏ</b>Pas grave, je vous souffle un indice à la prochaine.<i>Không sao, lần sau mình sẽ mách bạn.</i>'}
  else if(already){v.className='verdict good';v.innerHTML='Ta moitié est prête<small>Nửa cầu của bạn đã xong</small>';d='<b>Rồng nhỏ</b>On attend l\'autre rive pour la Rencontre.<i>Chờ bờ bên kia để cùng Gặp gỡ.</i>'}
  else if(mine){v.className='verdict good';v.innerHTML='Travée posée !<small>Đã đặt nhịp cầu!</small>';d='<b>Rồng nhỏ</b>Bravo ! Une lanterne de plus brille sur le fleuve.<i>Giỏi lắm! Thêm một chiếc đèn sáng trên sông.</i>'}
  else{v.className='verdict wait';v.innerHTML='La travée attend sur la barque<small>Nhịp cầu đang chờ trên thuyền</small>';d='<b>Rồng nhỏ</b>Pas grave ! Cette question reviendra, et je t\'aiderai.<i>Không sao! Câu hỏi này sẽ quay lại, và mình sẽ giúp bạn.</i>'}
  $('ddr').innerHTML=ui(mine||nb.center?'dragon3':'dragon1',70);$('dtxt').innerHTML=d;$('dsay').classList.remove('pop');void $('dsay').offsetWidth;$('dsay').classList.add('pop');
  $('pts').innerHTML=lastPts>0?'<span>+'+lastPts+' points de vitesse · điểm tốc độ</span>':'';
  var rs=m.res||[false,false];
  $('res').innerHTML=[0,1].map(function(r){var ok=rs[r],t=m.target!==undefined?('médiane · trung vị '+(m.med?Number(m.med[r]).toLocaleString('fr-FR'):'?')):((m.nc?m.nc[r]:0)+'/'+(m.na?m.na[r]:0)+' · đúng');return '<span class="r'+r+'">'+(ok?'✓ ':'⏳ ')+esc(tshort(r))+' · '+t+'</span>'}).join('');
  if(cur.num){$('ropts').innerHTML='<div class="target">Réponse · Đáp án : <b>'+Number(m.target).toLocaleString('fr-FR')+'</b>'+(myAns!==-1&&myAns!==null?'<br>Toi · Bạn : '+Number(myAns).toLocaleString('fr-FR'):'')+'</div>'}
  else{var A=function(k,cls,lab){return '<div class="ans '+cls+'"><span class="shp k'+k+'"><i></i></span><div><small>'+lab+'</small><b>'+esc(cur.o[k])+'</b></div></div>'};
   $('ropts').innerHTML=(m.r>=0&&cur.o[m.r]!==undefined?A(m.r,'ok','Bonne réponse · Đáp án đúng'):'')+(myAns>=0&&myAns!==m.r&&cur.o[myAns]!==undefined?A(myAns,'ko','Ta réponse · Bạn đã chọn'):'')}
  $('rfr').textContent=m.pfr||'';$('rvi').textContent=m.pvi||'';
  if(m.bridge)syncBridge(m.bridge,true);
  if(mine||nb.center){snd('ok');buzz([30,40,70])}else{snd('ko');buzz(60)}
 }
 else if(m.t==='end'){
  if(m.bridge)syncBridge(m.bridge,false);show('s-end');$('endimg').classList.add('on');endDecor(true);
  var dd=m.dur||0,mn=Math.floor(dd/60),sc=('0'+(dd%60)).slice(-2);
  $('fiche').innerHTML='<div class="row"><span>Pont fermé en · Cầu nối sau</span><b>'+m.qs+' questions</b></div>'+
   '<div class="row"><span>Durée · Thời gian</span><b>'+mn+' min '+sc+'</b></div>'+
   '<div class="row"><span>Indices du dragon · Gợi ý của rồng</span><b>'+(m.hints||0)+'</b></div>'+
   '<div class="row"><span>'+esc(tname(0))+' : 4 travées · 4 nhịp</span><b>question '+(m.doneAt?m.doneAt[0]:'?')+'</b></div>'+
   '<div class="row"><span>'+esc(tname(1))+' : 4 travées · 4 nhịp</span><b>question '+(m.doneAt?m.doneAt[1]:'?')+'</b></div>'+
   '<div class="row"><span>Vitesse · Tốc độ</span><b>'+(m.scores?m.scores[0]:0)+' · '+(m.scores?m.scores[1]:0)+'</b></div>';
  $('names').innerHTML=players.map(function(p){return '<span class="t'+p.team+'">'+esc(p.n)+'</span>'}).join('');
 }
 else if(m.t==='reset'){me.id=0;cur=null;seen={};endDecor(false);$('who').classList.add('hide');syncBridge({side:[0,0],pending:[false,false],center:false,per:B.per},false);show('s-join');renderRives();snapChars();markMe()}
}

function answer(k){
 if(myAns>=0||!cur)return;myAns=k;actx();snd('tap');buzz(25);
 $('opts').classList.add('done');for(var i=0;i<4;i++){var b=$('o'+i);if(b){b.classList.toggle('sel',i===k);b.disabled=true}}
 send({t:'answer',a:k});$('qmsg').textContent='Réponse posée · Đã chọn';hopMe();$('timer').classList.remove('low');
}
function answerNum(){
 if(myAns!==-1||!cur)return;var v=parseInt($('numin').value,10);if(isNaN(v)){$('numin').focus();return}myAns=v;actx();snd('tap');
 send({t:'answer',v:v});hopMe();$('qmsg').textContent='Estimation envoyée · Đã gửi : '+v.toLocaleString('fr-FR');$('num').classList.add('hide');
}

// ---------------------------------------------------------------- démarrage ------
try{var n=localStorage.getItem('lp_name');if(n)$('name').value=n;var t=localStorage.getItem('lp_team');if(t!==null)me.team=parseInt(t)}catch(x){}
$('snd').classList.toggle('off',muted);
if(SP){$('stonebox').insertAdjacentHTML('afterbegin',ui('stone',96));buildBridge();initChars();initBoats();decor();progress()}
renderRives();show('s-join');connect();
</script></body></html>

)html";
