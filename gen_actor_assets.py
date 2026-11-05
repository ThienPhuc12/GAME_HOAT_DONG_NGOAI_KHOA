"""Embed the self-contained SVG models and animation rigs from the new source.

The gallery links in cau-thu-pixel-esp32-moi.html point to absent files, but its
first SVG contains all eight model symbols. The pure animation functions live
at the start of its script. Both are embedded so the captive portal stays
fully offline and serves one HTML file from flash.
"""

from pathlib import Path
import re


ROOT = Path(__file__).resolve().parent
SOURCE = ROOT / "cau-thu-pixel-esp32-moi.html"
TARGET = ROOT / "web" / "player.html"


def main():
    source = SOURCE.read_text(encoding="utf-8")
    html = TARGET.read_text(encoding="utf-8")
    model_defs = [block for block in re.findall(r"<defs>.*?</defs>", source, re.S)
                  if "<symbol " in block]
    if len(model_defs) != 2:
        raise ValueError("Expected Vietnamese and French inline SVG model sets")
    models = "<defs>" + "".join(block[6:-7] for block in model_defs) + "</defs>"
    for actor in ("quang-hai", "cong-phuong", "van-lam", "mbappe", "zidane", "lloris"):
        for view in ("front", "back"):
            if f'id="{actor}-{view}"' not in models:
                raise ValueError(f"Missing SVG model: {actor}-{view}")

    script = source.split("<script>", 1)[1].split("</script>", 1)[0]
    pure_end = script.index("const viewNames")
    engine = script[:pure_end].rstrip()
    for name in ("getKeeperHugState", "renderKeeperHug", "renderRestoredShot"):
        if f"function {name}" not in engine:
            raise ValueError(f"Missing animation function: {name}")

    svg_block = (
        '<!-- ACTOR_MODELS_START -->\n'
        '<svg xmlns="http://www.w3.org/2000/svg" class="actor-models" '
        'aria-hidden="true" focusable="false">'
        + models + '</svg>\n<!-- ACTOR_MODELS_END -->'
    )
    if "<!-- ACTOR_MODELS_START -->" in html:
        html = re.sub(r"<!-- ACTOR_MODELS_START -->.*?<!-- ACTOR_MODELS_END -->",
                      lambda _: svg_block, html, count=1, flags=re.S)
    else:
        html = html.replace("<body>", "<body>\n  " + svg_block, 1)

    engine_block = "// ACTOR_ENGINE_START\n  " + engine + "\n  // ACTOR_ENGINE_END"
    if "// ACTOR_ENGINE_START" in html:
        html = re.sub(r"// ACTOR_ENGINE_START.*?// ACTOR_ENGINE_END",
                      lambda _: engine_block, html, count=1, flags=re.S)
    else:
        html, count = re.subn(r"// PIXEL_ASSETS_START.*?// PIXEL_ASSETS_END",
                              lambda _: engine_block, html, count=1, flags=re.S)
        if count != 1:
            raise ValueError("Cannot locate old sprite block")

    TARGET.write_bytes(html.replace("\r\n", "\n").encode("utf-8"))
    print(f"Embedded {len(models.encode()):,} SVG model bytes and {len(engine.encode()):,} animation bytes")


if __name__ == "__main__":
    main()
