"""Transforme les pages web/*.html et les fichiers web/a/* (images, polices) en fichiers .h pour l'ESP32.
VI: Chuyển các trang web/*.html và tệp web/a/* (hình ảnh, phông chữ) thành tệp .h cho ESP32.

Usage : python3 gen_web_h.py web LePont
  - web_player.h, web_host.h, web_admin.h : les pages (texte)
  - web_assets.h + web_assets.cpp : les images et polices, servies sous /a/... avec un long cache
Le marqueur __V__ des pages est remplacé par une empreinte des fichiers de web/a/ :
quand une image change, les téléphones la rechargent au lieu de garder l'ancienne.
VI: Dấu __V__ trong các trang được thay bằng mã băm của các tệp trong web/a/:
khi một hình thay đổi, điện thoại sẽ tải lại thay vì giữ bản cũ.
"""
import hashlib, re, sys, pathlib
base = pathlib.Path(sys.argv[1])
out = pathlib.Path(sys.argv[2])
assets = sorted(p for p in (base / "a").glob("*") if p.is_file()) if (base / "a").is_dir() else []
ver = hashlib.sha1(b"".join(p.name.encode() + p.read_bytes() for p in assets)).hexdigest()[:8] if assets else "0"

pages = [("player.html", "web_player.h", "PLAYER_HTML", "page des joueurs (téléphones) — http://192.168.4.1/"),
         ("host.html", "web_host.h", "HOST_HTML", "page grand écran + télécommande — http://192.168.4.1/host"),
         ("admin.html", "web_admin.h", "ADMIN_HTML", "page de modification des questions — http://192.168.4.1/admin")]
for src, dst, name, desc in pages:
    html = (base / src).read_text(encoding="utf-8").replace("__V__", ver)
    assert ')html"' not in html, src
    txt = ("#pragma once\n"
           "// =============================================================================\n"
           f"//  LE PONT · CẦU — {desc}\n"
           f"//  Généré depuis web/{src} : modifie le .html puis relance gen_web_h.py,\n"
           "//  ou édite directement ci-dessous (c'est du HTML/JS ordinaire).\n"
           f"//  VI: Tạo từ web/{src}: sửa tệp .html rồi chạy lại gen_web_h.py.\n"
           "// =============================================================================\n"
           f"const char {name}[] PROGMEM = R\"html(\n{html}\n)html\";\n")
    (out / dst).write_text(txt, encoding="utf-8")
    print(dst, len(html), "octets")

TYPES = {".webp": "image/webp", ".png": "image/png", ".jpg": "image/jpeg", ".css": "text/css; charset=utf-8",
         ".js": "application/javascript", ".woff2": "font/woff2", ".svg": "image/svg+xml"}
hdr = ["#pragma once",
       "// =============================================================================",
       "//  LE PONT · CẦU — images et polices du téléphone, servies sous /a/...",
       "//  Les données sont dans web_assets.cpp (GÉNÉRÉ par gen_web_h.py depuis web/a/).",
       "//  Elles sont à part pour que la compilation reste rapide : ce gros fichier n'est",
       "//  compilé qu'une fois, puis réutilisé tant qu'il ne change pas.",
       "//  VI: Hình ảnh và phông chữ của điện thoại, phục vụ tại /a/... Dữ liệu nằm trong",
       "//      web_assets.cpp (do gen_web_h.py tạo). Tách riêng để biên dịch nhanh: tệp lớn",
       "//      này chỉ biên dịch một lần rồi được dùng lại khi không thay đổi.",
       f"//  Empreinte · Mã băm : {ver}",
       "// =============================================================================",
       "#include <Arduino.h>",
       "struct WebAsset { const char* path; const char* type; const uint8_t* data; size_t len; };",
       "extern const WebAsset WEB_ASSETS[];",
       "extern const int WEB_ASSET_COUNT;", ""]
(out / "web_assets.h").write_text("\n".join(hdr) + "\n", encoding="utf-8")
h = ["// =============================================================================",
     "//  LE PONT · CẦU — données des images et polices (/a/...)",
     "//  Fichier GÉNÉRÉ par gen_web_h.py depuis web/a/ : ne pas modifier à la main.",
     "//  VI: Dữ liệu hình ảnh và phông chữ. Tệp được TẠO bởi gen_web_h.py: không sửa bằng tay.",
     f"//  Empreinte · Mã băm : {ver}",
     "// =============================================================================",
     '#include "web_assets.h"', ""]
total = 0
for p in assets:
    data = p.read_bytes(); total += len(data)
    var = "A_" + re.sub(r"\W", "_", p.name).upper()
    h.append(f"// {p.name} — {len(data)} octets")
    h.append(f"static const uint8_t {var}[] PROGMEM = {{")
    for i in range(0, len(data), 32):
        h.append("".join(f"{b}," for b in data[i:i + 32]))
    h.append("};\n")
h.append("const WebAsset WEB_ASSETS[] = {")
for p in assets:
    var = "A_" + re.sub(r"\W", "_", p.name).upper()
    h.append(f'  {{"/a/{p.name}", "{TYPES.get(p.suffix, "application/octet-stream")}", {var}, sizeof({var})}},')
h.append("};")
h.append(f"extern const int WEB_ASSET_COUNT = {len(assets)};")
(out / "web_assets.cpp").write_text("\n".join(h) + "\n", encoding="utf-8")
print("web_assets.h + web_assets.cpp", len(assets), "fichiers,", total // 1024, "Ko — empreinte", ver)
