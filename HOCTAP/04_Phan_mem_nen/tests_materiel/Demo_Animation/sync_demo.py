"""Copie display.h, config.h (écran rond forcé) et assets.h du jeu dans la démo. À relancer après toute modification."""
import pathlib, re
H = pathlib.Path(__file__).resolve().parent; S = H.parent.parent / "LePont"
(H / "display.h").write_text((S / "display.h").read_text())
cfg = re.sub(r"#define DISPLAY_TYPE \d", "#define DISPLAY_TYPE 2", (S / "config.h").read_text())
(H / "config.h").write_text(cfg)
(H / "assets.h").write_text((S / "assets.h").read_text())
print("démo synchronisée")
