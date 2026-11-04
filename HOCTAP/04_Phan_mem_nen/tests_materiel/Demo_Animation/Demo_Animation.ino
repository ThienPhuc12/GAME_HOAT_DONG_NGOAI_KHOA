/*
  DÉMO ANIMATION — Le Pont · Cầu
  Fait tourner les écrans du jeu (display.h, le même fichier que le jeu) sans Wi-Fi ni
  téléphones, en boucle : accueil → questions → révélations (travée posée, grue) → rencontre → fin.
  Sert à tester l'écran rond, à mesurer la fluidité, et à voir vos images en situation.

  Avant de compiler : python3 sync_demo.py  (copie display.h, config.h et assets.h du jeu ici)
  Broches : celles de config.h. Moniteur série 115200 : affiche les images/seconde de l'animation.
*/
#include "config.h"
#include "display.h"

BridgeView v = {{0, 0}, {false, false}, false, 0, false, false};
enum Step { S_LOBBY, S_Q, S_REVEAL, S_CHANTIER, S_END };
Step step = S_LOBBY; uint32_t at = 0; int left = 20; uint32_t lastSec = 0; int n = 0;
bool okL, okR;

void setup() {
  Serial.begin(115200); delay(300);
  displayBegin(); delay(1500);
  displayLobby("LePont-3", 4, 3); at = millis();
}

void loop() {
  displayTick();
  uint32_t t = millis() - at;
  switch (step) {
    case S_LOBBY: if (t > 4000) { v.q = 1; v.hint = false; left = 20; lastSec = millis(); displayQuestion(v, left, 20); step = S_Q; at = millis(); } break;
    case S_Q:
      if (millis() - lastSec >= 1000) { lastSec = millis(); left--; displayQuestion(v, left, 20); }
      if (left <= 12) {                       // on révèle au bout de 8 s pour la démo
        n++; v.dbl = (v.side[0] >= 4 && v.side[1] >= 4);
        okL = (n % 3) != 2 || v.side[0] >= 4; okR = (n % 4) != 1 || v.side[1] >= 4;   // quelques échecs pour montrer la grue
        if (v.dbl) { okL = okR = (n % 2 == 0); if (okL) v.center = true; }
        else { if (okL && v.side[0] < 4) v.side[0]++; if (okR && v.side[1] < 4) v.side[1]++; }
        v.pending[0] = !okL && v.side[0] < 4; v.pending[1] = !okR && v.side[1] < 4;
        displayReveal(v, okL, okR); step = S_REVEAL; at = millis();
      }
      break;
    case S_REVEAL: if (t > 3000) { if (v.center) { displayEnd(v, n, n * 25); step = S_END; } else { displayChantier(v); step = S_CHANTIER; } at = millis(); } break;
    case S_CHANTIER: if (t > 2000) { v.q++; v.hint = v.pending[0] || v.pending[1] || v.dbl; left = 20; lastSec = millis(); displayQuestion(v, left, 20); step = S_Q; at = millis(); } break;
    case S_END: if (t > 12000) { v = {{0, 0}, {false, false}, false, 0, false, false}; n = 0; displayLobby("LePont-3", 4, 3); step = S_LOBBY; at = millis(); } break;
  }
  static uint32_t fpsAt = 0; static int frames = 0; frames++;
  if (millis() - fpsAt >= 2000) { Serial.printf("[demo] etape %d  boucle %d tours/s\n", (int)step, frames / 2); frames = 0; fpsAt = millis(); }
}
