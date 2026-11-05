"""Generate temporary standalone UI states for manual browser screenshots."""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
base = {
    "t": "state", "scoreFR": 3, "scoreVN": 4, "shootingTeam": 1,
    "remainingMs": 3000, "membersFR": 3, "membersVN": 4,
    "blockedZones": [1, 4], "activeZones": 4, "charFR": 0, "charVN": 1,
    "pctFR": 33.3, "pctVN": 75.0,
}
states = {
    "host": {"state": "LOBBY", "scoreFR": 0, "scoreVN": 0, "soloMode": True},
    "arena": {"state": "LOBBY", "scoreFR": 0, "scoreVN": 0, "soloMode": False,
              "roster": [{"team": 0, "role": 0, "name": "Camille"},
                         {"team": 0, "role": 1, "name": "Louis"},
                         {"team": 1, "role": 0, "name": "Minh"},
                         {"team": 1, "role": 1, "name": "Lan"}]},
    "tutorial1": {"state": "TUTORIAL_SLIDES", "tutorialSlide": 0, "remainingMs": 10000},
    "tutorial2": {"state": "TUTORIAL_SLIDES", "tutorialSlide": 1, "remainingMs": 10000},
    "tutorial3": {"state": "TUTORIAL_SLIDES", "tutorialSlide": 2, "remainingMs": 10000},
    "coin": {"state": "COIN_TOSS", "winnerTeam": 1, "remainingMs": 2200},
    "trivia": {"state": "TRIVIA_10S", "remainingMs": 10000, "triviaMs": 10000, "questionId": 1,
               "qVi": "Đội tuyển bóng đá quốc gia Việt Nam có biệt danh phổ biến là gì?",
               "qFr": "Quel est le surnom populaire de l'équipe nationale de football du Vietnam ?",
               "options": ["Les Éléphants de guerre | Những chú voi chiến", "Les Guerriers de l'Étoile d'Or | Những chiến binh Sao Vàng", "Les Tigres de Malaisie | Những chú hổ Mã Lai", "Les Aigles Garuda | Đại bàng Garuda"]},
    "characters": {"state": "CHAR_SELECT_8S", "remainingMs": 8000},
    "aim": {"state": "AIM_STANDOFF", "remainingMs": 7000},
    "host_aim": {"state": "AIM_STANDOFF", "remainingMs": 7000},
    "shot": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootZone": 2, "saveZone1": 0, "saveZone2": 3, "goal": True},
    "shot_save": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootZone": 5, "saveZone1": 0, "saveZone2": 5, "goal": False},
    "host_shot_save": {"state": "PENALTY_ANIM", "remainingMs": 2800, "shootZone": 5, "saveZone1": 0, "saveZone2": 5, "goal": False},
    "build": {"state": "MONUMENT_BUILD", "remainingMs": 2500, "goal": True},
    "victory": {"state": "VICTORY_CEREMONY", "scoreFR": 4, "scoreVN": 5,
                "winnerTeam": 1, "winners": ["Quang Hải", "Lan", "Minh"],
                "runnersUp": ["Mbappé", "Claire", "Louis"], "correctPredictors": ["Lan", "Louis"],
                "spectatorRanking": [{"rank": 1, "name": "Lan", "team": 1, "correct": 7, "total": 8, "timeMs": 19000},
                                     {"rank": 2, "name": "Louis", "team": 0, "correct": 5, "total": 8, "timeMs": 27000}]},
    "host_characters": {"state": "CHAR_SELECT_8S", "remainingMs": 8000},
    "host_victory": {"state": "VICTORY_CEREMONY", "scoreFR": 4, "scoreVN": 5,
                     "winnerTeam": 1, "winners": ["Minh", "Lan"],
                     "runnersUp": ["Camille", "Louis"], "correctPredictors": ["Lan"],
                     "spectatorRanking": [{"rank": 1, "name": "Lan", "team": 1, "correct": 7, "total": 8, "timeMs": 19000}]},
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
