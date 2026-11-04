#!/usr/bin/env python3
"""Convertit les images du dossier assets/ en tableaux RGB565 pour l'ESP32 (assets.h).

Conventions de nom de fichier (ce que les générateurs d'images ne savent pas faire, le script le fait) :
  fond_rives.png                 image telle quelle, redimensionnée à 240x240 si plus grande
  travee@120x22.png              recadrage automatique sur le contenu, puis redimension à 120x22
  dragon@96x56@grid2x2.png       grille de 2x2 poses -> dragon_1 … dragon_4, chacune 96x56
Fond transparent OU fond magenta pur (#FF00FF) : les deux deviennent transparents (couleur clé 0xF81F).
Sortie : LePont/assets.h et tests_materiel/Demo_Animation/assets.h (ou --out fichier.h).
"""
import argparse, pathlib, re, sys
from PIL import Image

HERE = pathlib.Path(__file__).resolve().parent
KEY = 0xF81F
BUDGET = 600 * 1024

def rgb565(r, g, b): return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
def is_magenta(r, g, b): return r > 200 and b > 200 and g < 90

def magenta_to_alpha(im):
    """Fond magenta -> transparent ; les pixels de bord mêlés de magenta sont rabattus sur la couleur voisine."""
    im = im.convert("RGBA"); px = im.load()
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = px[x, y]
            if is_magenta(r, g, b): px[x, y] = (0, 0, 0, 0)
            elif r > 150 and b > 150 and g < 140 and a > 0:      # frange rosâtre d'anticrénelage
                px[x, y] = (min(255, r - 60), g, min(255, b - 60), 255)
    return im

def autocrop(im):
    bbox = im.getchannel("A").getbbox()
    return im.crop(bbox) if bbox else im

def fit(im, w, h):
    """Redimensionne dans w x h en gardant les proportions, centré sur fond transparent."""
    im = im.copy(); im.thumbnail((w, h), Image.LANCZOS)
    out = Image.new("RGBA", (w, h), (0, 0, 0, 0)); out.paste(im, ((w - im.width) // 2, (h - im.height) // 2)); return out

def to_words(im):
    px = im.load(); words = []
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = px[x, y]
            v = KEY if a < 128 else rgb565(r, g, b)
            if a >= 128 and v == KEY: v = 0xF81E
            words.append(v)
    return words

def load(path):
    """Renvoie une liste de (nom, image RGBA finale) selon les conventions du nom de fichier."""
    stem = path.stem; parts = stem.split("@"); name = parts[0]
    size = grid = None
    for p in parts[1:]:
        if m := re.fullmatch(r"(\d+)x(\d+)", p): size = (int(m[1]), int(m[2]))
        elif m := re.fullmatch(r"grid(\d+)x(\d+)", p): grid = (int(m[1]), int(m[2]))
    im = magenta_to_alpha(Image.open(path))
    if grid:
        cols, rows = grid; cw, ch = im.width // cols, im.height // rows; out = []
        for r in range(rows):
            for c in range(cols):
                cell = autocrop(im.crop((c * cw, r * ch, (c + 1) * cw, (r + 1) * ch)))
                out.append((f"{name}_{r * cols + c + 1}", fit(cell, *size) if size else cell))
        return out
    if size: return [(name, fit(autocrop(im), *size))]
    if im.width > 240 or im.height > 240: im = im.resize((240, 240), Image.LANCZOS)
    return [(name, im)]

def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", nargs="*"); ap.add_argument("--preview", action="store_true", help="écrit aussi assets/_preview/*.png pour contrôle"); a = ap.parse_args()
    outs = [pathlib.Path(o) for o in a.out] if a.out else [HERE.parent / "LePont" / "assets.h", HERE.parent / "tests_materiel" / "Demo_Animation" / "assets.h"]
    files = sorted(p for p in HERE.iterdir() if p.suffix.lower() in (".png", ".jpg", ".jpeg", ".webp") and not p.name.startswith("_"))
    if not files: sys.exit("aucune image dans assets/")
    lines = ["#pragma once", "// Généré par assets/png2assets.py — ne pas éditer à la main. Couleur clé transparente : 0xF81F.", "#include <stdint.h>", "#include <pgmspace.h>", "#define IMG_KEY 0xF81F", ""]
    total = 0
    for f in files:
        for name, im in load(f):
            if im.width > 240 or im.height > 240: sys.exit(f"{f.name}: {im.width}x{im.height} dépasse 240x240 ; ajoutez @LxH au nom du fichier.")
            words = to_words(im); total += len(words) * 2
            cname = "IMG_" + name.upper().replace("-", "_")
            lines += [f"// {f.name} -> {name} : {im.width}x{im.height}, {len(words)*2} octets", f"#define {cname}_W {im.width}\n#define {cname}_H {im.height}",
                      f"const uint16_t {cname}[{len(words)}] PROGMEM = {{{','.join(f'0x{v:04X}' for v in words)}}};\n"]
            print(f"  {f.name:34s} -> {name:14s} {im.width:3d}x{im.height:<3d} {len(words)*2:7d} octets")
            if a.preview:
                (HERE / "_preview").mkdir(exist_ok=True); im.save(HERE / "_preview" / f"{name}.png")
    for o in outs: o.parent.mkdir(parents=True, exist_ok=True); o.write_text("\n".join(lines)); print("écrit", o)
    print(f"total en flash : {total/1024:.0f} Ko (budget {BUDGET//1024} Ko)" + ("  ← TROP LOURD" if total > BUDGET else ""))

if __name__ == "__main__":
    main()
