import json, sys

src = sys.argv[1]
dst = sys.argv[2]
rows = json.load(open(src, encoding="utf-8"))

out = []
for r in rows:
    opts = [r["option_A"], r["option_B"], r["option_C"], r["option_D"]]
    opts = [o for o in opts if o]
    q = {
        "cat": r["categorie"],
        "fr": r["question_fr"],
        "vi": r["question_vi"],
        "o": opts,
        "r": "ABCD".index(r["reponse"]) if r["reponse"] in "ABCD" and r["reponse"] else -1,
        "pfr": r["pont_fr"],
        "pvi": r["pont_vi"],
    }
    if r.get("cible_estimation"):
        q["num"] = int(r["cible_estimation"])
    q["lv"] = int(r.get("niveau") or 2)   # 1 facile, 2 moyen, 3 difficile (chantier adaptatif)
    if r.get("indice_fr"):                # facultatif : indice du chef de chantier (sinon 1re phrase de pont_fr)
        q["hfr"] = r["indice_fr"]
    out.append(q)

body = json.dumps(out, ensure_ascii=False, indent=1)
assert ')json"' not in body

header = '''#pragma once
// =============================================================================
//  LE PONT · CẦU — base de questions (JSON embarqué dans le programme)
//  VI: bộ câu hỏi (JSON nhúng trong chương trình)
//  Fichier GÉNÉRÉ par gen_questions_h.py depuis 05_Questions/PONT_questions_FR_VI.json
//  VI: Tệp được TẠO TỰ ĐỘNG bởi gen_questions_h.py từ 05_Questions/PONT_questions_FR_VI.json
//
//  Format d'une question · Định dạng một câu hỏi :
//   {"cat":"Catégorie","fr":"Question en français","vi":"Câu hỏi tiếng Việt",
//    "o":["A","B","C","D"],"r":0,"lv":2,"pfr":"Révélation FR","pvi":"Tiết lộ VI"}
//   r = index de la bonne réponse (0 à 3) ; lv = niveau 1 facile, 2 moyen, 3 difficile
//   VI: r = chỉ số của đáp án đúng (0 đến 3); lv = cấp độ 1 dễ, 2 trung bình, 3 khó
//   Estimation : pas de "o", mais "num" = valeur cible (les deux rives tapent un nombre).
//   VI: Câu ước lượng: không có "o", mà có "num" = giá trị đích (hai bờ nhập một con số).
//   "hfr" (facultatif) = indice du chef de chantier ; sinon la 1re phrase de "pfr" sert d'indice.
//   VI: "hfr" (không bắt buộc) = gợi ý của đốc công; nếu không có, câu đầu tiên của "pfr" được dùng.
//  Tu peux aussi charger des questions sans reflasher : http://192.168.4.1/admin
//  VI: Có thể tải câu hỏi mà không cần nạp lại chương trình: http://192.168.4.1/admin
// =============================================================================
const char QUESTIONS_JSON[] PROGMEM = R"json(
'''
footer = '''
)json";
'''
open(dst, "w", encoding="utf-8").write(header + body + footer)
print(len(out), "questions ->", dst, len(body), "bytes")
