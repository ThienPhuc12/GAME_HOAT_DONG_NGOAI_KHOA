#pragma once
// =============================================================================
//  LE PONT · CẦU — images et polices du téléphone, servies sous /a/...
//  Les données sont dans web_assets.cpp (GÉNÉRÉ par gen_web_h.py depuis web/a/).
//  Elles sont à part pour que la compilation reste rapide : ce gros fichier n'est
//  compilé qu'une fois, puis réutilisé tant qu'il ne change pas.
//  VI: Hình ảnh và phông chữ của điện thoại, phục vụ tại /a/... Dữ liệu nằm trong
//      web_assets.cpp (do gen_web_h.py tạo). Tách riêng để biên dịch nhanh: tệp lớn
//      này chỉ biên dịch một lần rồi được dùng lại khi không thay đổi.
//  Empreinte · Mã băm : 9d6f412c
// =============================================================================
#include <Arduino.h>
struct WebAsset { const char* path; const char* type; const uint8_t* data; size_t len; };
extern const WebAsset WEB_ASSETS[];
extern const int WEB_ASSET_COUNT;

