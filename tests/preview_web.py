"""Generate temporary standalone UI states for manual browser screenshots."""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
base = {
    "t": "state", "scoreFR": 3, "scoreVN": 4, "shootingTeam": 1,
    "remainingMs": 3000, "membersFR": 3, "membersVN": 4,
    "blockedLane": 1, "activeLanes": 2, "charFR": 0, "charVN": 1,
    "pctFR": 33.3, "pctVN": 75.0,
}
states = {
    "host": {"state": "LOBBY", "scoreFR": 0, "scoreVN": 0, "soloMode": True},
    "coin": {"state": "COIN_TOSS", "winnerTeam": 1, "remainingMs": 2200},
    "trivia": {"state": "TRIVIA_5S", "remainingMs": 7100, "questionId": 4,
               "qVi": "Món ăn nổi tiếng của Việt Nam gồm bánh phở, nước dùng và thịt bò hoặc gà là món gì?",
               "qFr": "Quel plat vietnamien se compose de nouilles de riz, de bouillon et de bœuf ou de poulet ?",
               "options": ["Bún chả", "Bánh xèo", "Cơm tấm", "Phở"]},
    "characters": {"state": "CHAR_SELECT_3S", "remainingMs": 14600},
    "aim": {"state": "AIM_AND_SHOOT", "remainingMs": 7000},
    "shot": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootDir": 2, "saveDir": 0, "goal": True},
    "build": {"state": "MONUMENT_BUILD", "remainingMs": 2500, "goal": True},
    "victory": {"state": "VICTORY_CEREMONY", "scoreFR": 4, "scoreVN": 5,
                "winnerTeam": 1, "winners": ["Quang Hải", "Lan", "Minh"],
                "runnersUp": ["Mbappé", "Claire", "Louis"], "correctPredictors": ["Lan", "Louis"]},
}
mode = sys.argv[1]
if mode not in states:
    raise SystemExit(f"Choose one of: {', '.join(states)}")
payload = {**base, **states[mode]}
joined = {"t": "joined", "name": "Minh", "team": 1, "role": 0, "prediction": -1,
          "playerId": "aabbccdd", "host": mode == "host"}
script = "<script>onPacket(%s);onPacket(%s);%s</script>" % (
    json.dumps(joined, ensure_ascii=False), json.dumps(payload, ensure_ascii=False),
    "openHost();" if mode == "host" else "")
source = (ROOT / "web" / "player.html").read_text(encoding="utf-8")
output = ROOT / "web" / f".preview_{mode}.html"
output.write_text(source.replace("</body>", script + "</body>"), encoding="utf-8")
print(output)
