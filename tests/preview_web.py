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
    "arena": {"state": "LOBBY", "scoreFR": 0, "scoreVN": 0, "soloMode": False,
              "roster": [{"team": 0, "role": 0, "name": "Camille"},
                         {"team": 0, "role": 1, "name": "Louis"},
                         {"team": 1, "role": 0, "name": "Minh"},
                         {"team": 1, "role": 1, "name": "Lan"}]},
    "tutorial1": {"state": "TUTORIAL_SLIDES", "tutorialSlide": 0, "remainingMs": 6000},
    "tutorial2": {"state": "TUTORIAL_SLIDES", "tutorialSlide": 1, "remainingMs": 6000},
    "tutorial3": {"state": "TUTORIAL_SLIDES", "tutorialSlide": 2, "remainingMs": 6000},
    "coin": {"state": "COIN_TOSS", "winnerTeam": 1, "remainingMs": 2200},
    "trivia": {"state": "TRIVIA_8S", "remainingMs": 8000, "triviaMs": 8000, "questionId": 4,
               "qVi": "Món ăn nổi tiếng của Việt Nam gồm bánh phở, nước dùng và thịt bò hoặc gà là món gì?",
               "qFr": "Quel plat vietnamien se compose de nouilles de riz, de bouillon et de bœuf ou de poulet ?",
               "options": ["Bún chả", "Bánh xèo", "Cơm tấm", "Phở"]},
    "characters": {"state": "CHAR_SELECT_7S", "remainingMs": 7000},
    "aim": {"state": "AIM_STANDOFF", "remainingMs": 7000},
    "host_aim": {"state": "AIM_STANDOFF", "remainingMs": 7000},
    "shot": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootDir": 2, "saveDir": 0, "goal": True},
    "shot_save": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootDir": 0, "saveDir": 0, "goal": False},
    "host_shot_save": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootDir": 0, "saveDir": 0, "goal": False},
    "build": {"state": "MONUMENT_BUILD", "remainingMs": 2500, "goal": True},
    "victory": {"state": "VICTORY_CEREMONY", "scoreFR": 4, "scoreVN": 5,
                "winnerTeam": 1, "winners": ["Quang Hải", "Lan", "Minh"],
                "runnersUp": ["Mbappé", "Claire", "Louis"], "correctPredictors": ["Lan", "Louis"]},
    "host_characters": {"state": "CHAR_SELECT_7S", "remainingMs": 7000},
    "host_victory": {"state": "VICTORY_CEREMONY", "scoreFR": 4, "scoreVN": 5,
                     "winnerTeam": 1, "winners": ["Minh", "Lan"],
                     "runnersUp": ["Camille", "Louis"], "correctPredictors": ["Lan"]},
}
mode = sys.argv[1]
if mode not in states:
    raise SystemExit(f"Choose one of: {', '.join(states)}")
payload = {**base, **states[mode]}
is_host = mode in ("host", "arena") or mode.startswith("tutorial") or mode.startswith("host_")
identity = {"t": "identity", "role": "HOST" if is_host else "GUEST",
            "host": is_host, "playerId": "aabbccdd"}
joined = {"t": "joined", "name": "Minh", "team": 1, "role": 0, "prediction": -1,
          "playerId": "aabbccdd", "host": False}
script = "<script>onPacket(%s);%sonPacket(%s);%s</script>" % (
    json.dumps(identity, ensure_ascii=False),
    "" if is_host else f"onPacket({json.dumps(joined, ensure_ascii=False)});",
    json.dumps(payload, ensure_ascii=False), "openHost();" if mode == "host" else "")
if mode in ("shot_save", "host_shot_save"):
    script += "<script>app.phaseStart=performance.now()-1400;tick(performance.now());</script>"
source = (ROOT / "web" / "player.html").read_text(encoding="utf-8")
output = ROOT / "web" / f".preview_{mode}.html"
output.write_text(source.replace("</body>", script + "</body>"), encoding="utf-8")
print(output)
