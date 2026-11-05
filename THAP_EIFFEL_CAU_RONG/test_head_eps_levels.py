import cv2
import numpy as np

img = cv2.imread(r'C:/Users/truon/.gemini/antigravity/brain/18c792ce-7915-4ee7-9d19-e864608b78e8/.user_uploaded/media_1793868317142_a907891c.jpg')
h, w = img.shape[:2]
filtered = cv2.bilateralFilter(img, d=9, sigmaColor=75, sigmaSpace=75)
rgb = cv2.cvtColor(filtered, cv2.COLOR_BGR2RGB)

white_bg = (rgb[:,:,0] > 235) & (rgb[:,:,1] > 235) & (rgb[:,:,2] > 235)
bg_flood = np.zeros((h+2, w+2), np.uint8)
teeth_u8 = white_bg.astype(np.uint8) * 255
cv2.floodFill(teeth_u8, bg_flood, (0, 0), 0)
cv2.floodFill(teeth_u8, bg_flood, (w-1, 0), 0)
cv2.floodFill(teeth_u8, bg_flood, (0, h-1), 0)
cv2.floodFill(teeth_u8, bg_flood, (w-1, h-1), 0)
teeth_mask = teeth_u8 > 0
pure_bg = white_bg & (~teeth_mask)

dark_mask = (rgb[:,:,0] < 115) & (rgb[:,:,1] < 65) & (rgb[:,:,2] < 45) & (~pure_bg)
med_brown_mask = (rgb[:,:,0] >= 115) & (rgb[:,:,0] < 200) & (rgb[:,:,1] < 125) & (rgb[:,:,2] < 50) & (~pure_bg)
orange_mask = (rgb[:,:,0] >= 200) & (rgb[:,:,1] >= 90) & (rgb[:,:,1] < 165) & (rgb[:,:,2] < 50) & (~pure_bg)
amber_mask = (rgb[:,:,0] >= 200) & (rgb[:,:,1] >= 165) & (rgb[:,:,1] < 205) & (rgb[:,:,2] < 50) & (~pure_bg)
yellow_mask = (rgb[:,:,0] >= 200) & (rgb[:,:,1] >= 205) & (rgb[:,:,2] < 70) & (~pure_bg)
pale_mask = (rgb[:,:,0] >= 200) & (rgb[:,:,1] >= 200) & (rgb[:,:,2] >= 70) & (~pure_bg) & (~teeth_mask)

kernel_sm = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))

layers = [
    ("dark_brown", dark_mask, "#3a1504"),
    ("med_brown", med_brown_mask, "#92400e"),
    ("orange", orange_mask, "#f97316"),
    ("amber", amber_mask, "#f59e0b"),
    ("yellow", yellow_mask, "#fbcc02"),
    ("pale", pale_mask, "#fef08a"),
    ("teeth", teeth_mask, "#ffffff"),
]

ys, xs = np.where(~pure_bg)
min_x, max_x = xs.min(), xs.max()
min_y, max_y = ys.min(), ys.max()
orig_w = max_x - min_x
orig_h = max_y - min_y

target_w = 208.0
scale = target_w / orig_w
target_h = orig_h * scale
offset_x = 590.0
offset_y = 42.0

def to_subpath(pts):
    if not pts:
        return ""
    d = [f"M{pts[0][0]} {pts[0][1]}l"]
    prev_x, prev_y = pts[0]
    for x, y in pts[1:]:
        dx = x - prev_x
        dy = y - prev_y
        s_dx = f"{dx}"
        s_dy = f" {dy}" if dy >= 0 else f"{dy}"
        d.append(f" {s_dx}" if dx >= 0 else f"{s_dx}")
        d.append(s_dy)
        prev_x, prev_y = x, y
    d.append("z")
    return "".join(d)

for eps, min_area in [(6.8, 300), (7.2, 340), (7.6, 380)]:
    head_paths = []
    total_pts = 0
    for name, mask, color in layers:
        u8 = cv2.morphologyEx(mask.astype(np.uint8)*255, cv2.MORPH_OPEN, kernel_sm)
        u8 = cv2.morphologyEx(u8, cv2.MORPH_CLOSE, kernel_sm)
        contours, _ = cv2.findContours(u8, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)
        
        path_d = []
        for cnt in contours:
            if cv2.contourArea(cnt) < min_area:
                continue
            approx = cv2.approxPolyDP(cnt, eps, True)
            if len(approx) < 3:
                continue
            pts = approx.reshape(-1, 2)
            total_pts += len(pts)
            
            t_pts = []
            for p in pts:
                tx = int(round(offset_x + (p[0] - min_x) * scale))
                ty = int(round(offset_y + (p[1] - min_y) * scale))
                t_pts.append((tx, ty))
            
            path_d.append(to_subpath(t_pts))
            
        if path_d:
            head_paths.append(f'<path fill="{color}" fill-rule="evenodd" d="{"".join(path_d)}"/>')
            
    out_head = f'<g id="dragon-head-exact">{"".join(head_paths)}</g>'
    print(f"eps={eps}, min_area={min_area}: {total_pts} pts, {len(out_head)} bytes")
