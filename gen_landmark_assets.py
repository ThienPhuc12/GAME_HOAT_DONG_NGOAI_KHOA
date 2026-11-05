"""Replace only the Dragon Bridge SVG sprites from CAU_RONG_MOI/index.html."""

from __future__ import annotations

import argparse
import base64
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent
DEFAULT_SOURCE = ROOT / "CAU_RONG_MOI" / "index.html"
DEFAULT_HTML = ROOT / "web" / "player.html"
BEGIN = "  // LANDMARK_SPRITES_BEGIN"
END = "  // LANDMARK_SPRITES_END"
TAG_RE = re.compile(r"<(/?)([a-zA-Z][\w:-]*)\b[^>]*?>", re.DOTALL)
TABLE_RE = re.compile(r"const LANDMARK_SPRITE_DATA=(\{.*?\});", re.DOTALL)


def _attr(text: str, name: str) -> str | None:
    match = re.search(rf"\b{re.escape(name)}\s*=\s*([\"'])(.*?)\1", text, re.IGNORECASE | re.DOTALL)
    return match.group(2) if match else None


def _element(markup: str, tag: str, identifier: str | None = None) -> str:
    wanted = tag.lower()
    start_match = None
    for match in TAG_RE.finditer(markup):
        if match.group(1) or match.group(2).lower() != wanted:
            continue
        if identifier is not None and _attr(match.group(0), "id") != identifier:
            continue
        start_match = match
        break
    if start_match is None:
        raise ValueError(f"Could not find <{tag}> with id={identifier!r}")

    depth = 0
    for match in TAG_RE.finditer(markup, start_match.start()):
        if match.group(2).lower() != wanted:
            continue
        if match.group(1):
            depth -= 1
            if depth == 0:
                return markup[start_match.start():match.end()]
        elif not match.group(0).rstrip().endswith("/>"):
            depth += 1
    raise ValueError(f"Unclosed <{tag}> with id={identifier!r}")


def render_dragon_data(source_html: str) -> list[str]:
    """Extract the five Dragon groups only; never read or generate Eiffel artwork."""
    svg = _element(source_html, "svg", "svg-dragon")
    view_box = _attr(svg.split(">", 1)[0], "viewBox")
    if not view_box:
        raise ValueError("The Dragon SVG is missing its viewBox")
    defs = _element(svg, "defs")
    sprites: list[str] = []
    for part in range(1, 6):
        group = _element(svg, "g", f"dragon-{part}")
        # The source page hides/build-animates slices with these classes. The game
        # positions the isolated groups itself, so remove only each root class.
        group = re.sub(r"\s+class=(['\"]).*?\1", "", group, count=1, flags=re.IGNORECASE | re.DOTALL)
        wrapper = (
            f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="{view_box}">'
            f"{defs}{group}</svg>"
        )
        encoded = base64.b64encode(wrapper.encode("utf-8")).decode("ascii")
        sprites.append(f"data:image/svg+xml;base64,{encoded}")
    return sprites


def _property_text(table: str, name: str) -> str:
    match = re.search(rf'("{re.escape(name)}"\s*:\s*)\[[^\]]*\]', table, re.DOTALL)
    if not match:
        raise ValueError(f"Sprite table is missing the {name!r} array")
    return match.group(0)


def replace_dragon_array(html: str, sprites: list[str]) -> str:
    start = html.find(BEGIN)
    end = html.find(END, start + len(BEGIN))
    if start < 0 or end < 0:
        raise ValueError("web/player.html is missing the landmark asset markers")
    section = html[start + len(BEGIN):end]
    table_match = TABLE_RE.search(section)
    if not table_match:
        raise ValueError("The current landmark sprite table is missing")

    old_table = table_match.group(1)
    current = json.loads(old_table)
    if len(current.get("eiffel", [])) != 5:
        raise ValueError("Expected the existing five Eiffel sprites to remain in place")
    if len(sprites) != 5:
        raise ValueError("Expected exactly five new Dragon sprites")

    old_eiffel_property = _property_text(old_table, "eiffel")
    replacement = json.dumps(sprites, separators=(",", ":"), ensure_ascii=True)
    new_table, count = re.subn(
        r'("dragon"\s*:\s*)\[[^\]]*\]',
        lambda match: match.group(1) + replacement,
        old_table,
        count=1,
        flags=re.DOTALL,
    )
    if count != 1 or _property_text(new_table, "eiffel") != old_eiffel_property:
        raise ValueError("Dragon update would alter the existing Eiffel sprite data")

    updated = section[:table_match.start(1)] + new_table + section[table_match.end(1):]
    return html[:start + len(BEGIN)] + updated + html[end:]


def inject_landmark_assets(html: str, source_path: Path = DEFAULT_SOURCE) -> str:
    source = source_path.read_text(encoding="utf-8-sig")
    return replace_dragon_array(html, render_dragon_data(source))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--html", type=Path, default=DEFAULT_HTML)
    args = parser.parse_args()
    html = args.html.read_text(encoding="utf-8-sig")
    updated = inject_landmark_assets(html, args.source)
    args.html.write_text(updated, encoding="utf-8", newline="\n")
    print(f"Updated only five Dragon SVG sprites from {args.source} in {args.html}")


if __name__ == "__main__":
    main()
