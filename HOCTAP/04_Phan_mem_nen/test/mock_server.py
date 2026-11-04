"""Serveur de test : imite l'ESP32 (HTTP + WebSocket, même protocole v2 « chantier ») pour vérifier les pages
sans carte. C'est aussi une version lisible des règles du jeu, en Python.
Lance : python3 mock_server.py  -> http://localhost:8090/ et /host
"""
import asyncio, json, os, pathlib, random, threading, http.server, socketserver, time
import websockets

WEB = pathlib.Path(__file__).resolve().parent.parent / "web"
QUESTIONS = json.load(open(pathlib.Path(__file__).resolve().parent / "q.json", encoding="utf-8"))
TEAMS = [{"n": "Rive France", "c": "#2b6cb0"}, {"n": "Rive Vietnam", "c": "#c53030"}]
SEC = 5          # secondes par question (20 sur la carte ; court ici pour les tests)
PER = 4          # travées par rive avant la rencontre
HTTP_PORT = int(os.environ.get("LEPONT_HTTP", 8090)); WS_PORT = int(os.environ.get("LEPONT_WS", 8091))
HTTP_PORT = int(os.environ.get("LEPONT_HTTP", 8090)); WS_PORT = int(os.environ.get("LEPONT_WS", 8091))

class G:
    state = "lobby"; players = {}; clients = set(); tick_task = None
    played = set(); retry = []; cur = None; qplayed = 0; qsec = SEC; start = 0
    side = [0, 0]; pending = [False, False]; fails = [0, 0]; level = [2, 2]; streak = [0, 0]
    center = False; dbl = False; dbl_fails = 0; hints = 0; done_at = [0, 0]; scores = [0, 0]
    curO = []; curR = -1; curNum = False; target = 0; hide = [[], []]; assist = [False, False]; answers = {}

def q(): return QUESTIONS[G.cur]
def active_on(r): return sum(1 for p in G.players.values() if p["team"] == r)
def hint_of(x):
    if x.get("hfr"): return x["hfr"]
    h = x["pfr"]; cut = -1
    for sep in (". ", " : ", ", "):
        cut = h.find(sep)
        if cut > 20: h = h[:cut + 1]; break
    return h if len(h) <= 110 else h[:108] + "…"

async def send(ws, d):
    try: await ws.send(json.dumps(d, ensure_ascii=False))
    except Exception: pass
async def broadcast(d):
    for c in list(G.clients): await send(c, d)

def bridge(): return {"side": G.side, "pending": G.pending, "center": G.center, "per": PER, "level": G.level}
def lobby_doc(): return {"t": "lobby", "players": [{"n": p["n"], "team": p["team"], "s": p["s"]} for p in G.players.values()], "bridge": bridge(), "scores": G.scores}
def question_doc():
    x = q(); d = {"t": "question", "i": G.qplayed, "sec": G.qsec, "lv": x.get("lv", 2), "dbl": G.dbl, "cat": x["cat"], "fr": x["fr"], "vi": x["vi"], "hide": G.hide, "assist": G.assist, "bridge": bridge()}
    if G.curNum: d["num"] = True
    else: d["o"] = [x["o"][k] for k in G.curO]
    if G.assist[0] or G.assist[1] or G.dbl: d["hint"] = hint_of(x)
    return d

def pick_question():
    for k, (qi, at) in enumerate(G.retry):
        if G.qplayed >= at: del G.retry[k]; return qi
    want = min(G.level)
    for dist in range(3):
        c = [i for i, x in enumerate(QUESTIONS) if i not in G.played and abs(x.get("lv", 2) - want) == dist and not (G.dbl and "num" in x)]
        if c: return random.choice(c)
    return -1

def hide_wrong(r, n):
    G.hide[r] = []
    if G.curNum or n <= 0: return
    wrong = [k for k in range(len(G.curO)) if k != G.curR]; random.shuffle(wrong); G.hide[r] = wrong[:n]

async def start_question():
    if G.center: return await end_game()
    G.dbl = G.side[0] >= PER and G.side[1] >= PER
    qi = pick_question()
    if qi < 0:
        G.side = [PER, PER]; G.center = True; G.pending = [False, False]; return await end_game()
    G.cur = qi; G.played.add(qi); G.qplayed += 1; x = q()
    G.curNum = "num" in x; G.target = x.get("num", 0)
    if G.curNum: G.curO = []; G.curR = -1
    else: G.curO = [0, 1, 2, 3]; random.shuffle(G.curO); G.curR = G.curO.index(x["r"])
    for r in range(2):
        G.assist[r] = G.level[r] <= 1 or G.fails[r] >= 3; hide_wrong(r, 2 if G.assist[r] else 0)
    if G.dbl:
        n = min(G.dbl_fails, 2); hide_wrong(0, n); G.hide[1] = list(G.hide[0])
    if G.assist[0] or G.assist[1] or G.dbl: G.hints += 1
    G.qsec = SEC + 2 if (G.assist[0] or G.assist[1]) else SEC
    G.answers = {}; G.state = "q"; G.qstart = time.time()
    if G.qplayed == 1: G.start = time.time()
    await broadcast(question_doc())
    if G.tick_task: G.tick_task.cancel()
    G.tick_task = asyncio.create_task(ticker())

async def ticker():
    for left in range(G.qsec - 1, -1, -1):
        await asyncio.sleep(1)
        n = [0, 0]
        for ws in G.answers: n[G.players[ws]["team"]] += 1
        await broadcast({"t": "tick", "left": left, "answers": n})
    await reveal()

def side_succeeds(r):
    ans = [(ws, a) for ws, a in G.answers.items() if G.players[ws]["team"] == r]
    if G.curNum:
        vals = sorted(a for _, a in ans)
        if not vals: return False, 0, 0, 0
        med = vals[len(vals) // 2]; tol = 5 if 1800 <= G.target <= 2100 else max(1, G.target * 40 // 100); ok = abs(med - G.target) <= tol
        return ok, (len(vals) if ok else 0), len(vals), med
    nc = sum(1 for _, a in ans if a == G.curR); na = len(ans)
    return (na > 0 and nc * 2 >= na), nc, na, 0

async def reveal():
    if G.state != "q": return
    G.state = "rev"
    if G.tick_task: G.tick_task.cancel(); G.tick_task = None
    x = q()
    for ws, p in G.players.items():
        a = G.answers.get(ws); pts = 0; ok = (a is not None and not G.curNum and a == G.curR)
        if ok: pts = 100 + int(50 * max(0, 1 - (time.time() - G.qstart) / G.qsec)); p["s"] += pts; G.scores[p["team"]] += pts
        await send(ws, {"t": "you", "ok": ok, "pts": pts, "answered": a is not None})
    res = [False, False]; nc = [0, 0]; na = [0, 0]; med = [0, 0]; ok = [False, False]
    for r in range(2): ok[r], nc[r], na[r], med[r] = side_succeeds(r)
    for r in range(2):
        if active_on(r) == 0: ok[r] = ok[1 - r]
    if G.dbl:
        if ok[0] and ok[1]: G.center = True; res = [True, True]; G.pending = [False, False]
        else: G.dbl_fails += 1; G.pending = [not ok[0], not ok[1]]
    else:
        for r in range(2):
            if G.side[r] >= PER: res[r] = True; continue
            if ok[r]:
                G.side[r] += 1; G.pending[r] = False; G.fails[r] = 0; res[r] = True
                if G.side[r] == PER: G.done_at[r] = G.qplayed
                G.streak[r] = max(0, G.streak[r]) + 1
                if G.streak[r] >= 2 and G.level[r] < 3: G.level[r] += 1; G.streak[r] = 0
            else:
                G.pending[r] = True; G.fails[r] += 1
                G.streak[r] = min(0, G.streak[r]) - 1
                if G.streak[r] <= -2 and G.level[r] > 1: G.level[r] -= 1; G.streak[r] = 0
        if (not ok[0] or not ok[1]) and len(G.retry) < 16: G.retry.append((G.cur, G.qplayed + 3))
    d = {"t": "reveal", "r": G.curR, "pfr": x["pfr"], "pvi": x["pvi"], "dbl": G.dbl, "res": res, "nc": nc, "na": na, "center": G.center, "bridge": bridge(), "scores": G.scores}
    if G.curNum: d["target"] = G.target; d["med"] = med
    await broadcast(d)

async def end_game():
    G.state = "end"
    await broadcast({"t": "end", "qs": G.qplayed, "dur": int(time.time() - G.start) if G.start else 0, "hints": G.hints, "doneAt": G.done_at, "bridge": bridge(), "scores": G.scores})

async def lobby_start():
    # mêmes règles que le firmware : pas de partie sans joueur ; une rive vide reçoit un joueur si l'autre en a 2 ou plus
    if not G.players: return
    for r in (0, 1):
        if active_on(r) == 0 and active_on(1 - r) >= 2:
            ws, p = [(w, p) for w, p in G.players.items() if p["team"] == 1 - r][-1]
            p["team"] = r
            await send(ws, {"t": "joined", "id": id(ws), "team": r, "moved": True})
    await start_question()

async def reset(keep):
    G.state = "lobby"; G.played = set(); G.retry = []; G.cur = None; G.qplayed = 0; G.start = 0
    G.side = [0, 0]; G.pending = [False, False]; G.fails = [0, 0]; G.level = [2, 2]; G.streak = [0, 0]
    G.center = False; G.dbl = False; G.dbl_fails = 0; G.hints = 0; G.done_at = [0, 0]; G.scores = [0, 0]
    for p in G.players.values(): p["s"] = 0
    if not keep: G.players = {}
    await broadcast({"t": "reset"}); await broadcast(lobby_doc())

async def handler(ws):
    G.clients.add(ws)
    await send(ws, {"t": "config", "ssid": "LePont-1", "sec": SEC, "per": PER, "mode": "chantier", "teams": TEAMS})
    try:
        async for raw in ws:
            m = json.loads(raw); t = m.get("t")
            if t == "hello":
                if G.state == "lobby": await send(ws, lobby_doc())
                elif G.state in ("q", "rev"): await send(ws, question_doc())
            elif t == "join":
                team = int(m.get("team", -1))
                if team not in (0, 1): team = 0 if active_on(0) <= active_on(1) else 1
                G.players[ws] = {"n": m.get("name", "?"), "team": team, "s": 0}
                await send(ws, {"t": "joined", "id": id(ws), "team": team})
                if G.state == "lobby": await broadcast(lobby_doc())
                elif G.state == "q": await send(ws, question_doc())
            elif t == "answer":
                if G.state == "q" and ws in G.players and ws not in G.answers:
                    if G.curNum:
                        if "v" in m: G.answers[ws] = int(m["v"])
                    else:
                        a = int(m.get("a", -1))
                        if 0 <= a < len(G.curO): G.answers[ws] = a
                    if len(G.answers) >= len(G.players): await reveal()
            elif t == "host":
                c = m.get("cmd")
                if c == "start" and G.state == "lobby": await lobby_start()
                elif c == "next":
                    if G.state == "lobby": await lobby_start()
                    elif G.state == "q": await reveal()
                    elif G.state == "rev": await start_question()
                    elif G.state == "end": await reset(True)
                elif c == "reveal": await reveal()
                elif c == "reset": await reset(False)
    finally:
        G.clients.discard(ws)
        if ws in G.players:
            del G.players[ws]
            if G.state == "lobby": await broadcast(lobby_doc())

class H(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *a, **k): super().__init__(*a, directory=str(WEB), **k)
    def do_GET(self):
        if self.path == "/": self.path = "/player.html"
        elif self.path == "/host": self.path = "/host.html"
        elif self.path == "/admin": self.path = "/admin.html"
        elif self.path == "/api/questions":
            b = json.dumps(QUESTIONS, ensure_ascii=False).encode(); self.send_response(200)
            self.send_header("Content-Type", "application/json; charset=utf-8"); self.send_header("Content-Length", str(len(b))); self.end_headers(); self.wfile.write(b); return
        return super().do_GET()
    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0)); body = self.rfile.read(n)
        try: k = len(json.loads(body)); msg = f"{k} questions chargees dans l'objet"; code = 200
        except Exception as e: msg = f"JSON invalide: {e}"; code = 400
        b = msg.encode(); self.send_response(code); self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.send_header("Content-Length", str(len(b))); self.end_headers(); self.wfile.write(b)
    def log_message(self, *a): pass

async def main():
    httpd = socketserver.ThreadingTCPServer(("127.0.0.1", HTTP_PORT), H); httpd.daemon_threads = True
    threading.Thread(target=httpd.serve_forever, daemon=True).start()
    async with websockets.serve(handler, "127.0.0.1", WS_PORT):
        print(f"mock ready: http://127.0.0.1:{HTTP_PORT}/  ws://127.0.0.1:{WS_PORT}", flush=True)
        await asyncio.Future()

asyncio.run(main())
