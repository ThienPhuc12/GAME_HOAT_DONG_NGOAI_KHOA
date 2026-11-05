def get_dragon_defs():
    return '''<defs>
<linearGradient id="dg" x1="0" y1="0" x2="0" y2="1"><stop offset="0%" stop-color="#fef08a"/><stop offset="45%" stop-color="#fbcc02"/><stop offset="100%" stop-color="#b45309"/></linearGradient>
<linearGradient id="do" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#ea580c"/><stop offset="100%" stop-color="#9a3412"/></linearGradient>
<linearGradient id="dp" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#334155"/><stop offset="50%" stop-color="#64748b"/><stop offset="100%" stop-color="#1e293b"/></linearGradient>

<!-- Pier Pillar Module -->
<g id="r-pr">
  <path d="M0 0l6 35h36l6-35z" fill="url(#dp)"/>
  <ellipse cx="24" cy="0" rx="24" ry="4" fill="#94a3b8"/>
  <path d="M0 0l6 35h8l-4-35z" fill="#0f172a" opacity=".4"/>
  <path d="M16 2v33m16-33v33" stroke="#334155" stroke-width="1.2" opacity=".6"/>
</g>

<!-- Bridge Deck Module with Railing and Cantilever Trusses -->
<g id="r-dk">
  <path d="M10 15l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9zm34 0l-4 9h12l-2-9z" fill="#0b1329"/>
  <rect y="12" width="180" height="4" fill="#1e293b"/>
  <rect width="180" height="13" rx="2" fill="url(#do)"/>
  <rect width="180" height="2.5" fill="#fed7aa"/>
  <rect y="-8" width="180" height="8" fill="none" stroke="#475569" stroke-width="1.2"/>
  <path d="M0-8h180M0-4h180" stroke="#94a3b8" stroke-width=".8"/>
  <path d="M12-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8m24-8v8" stroke="#64748b" stroke-width=".8" opacity=".7"/>
</g>

<!-- Streetlamp Post -->
<g id="r-lp">
  <path d="M0 0v24M-5-2h5l2 4M5-2h-5" stroke="#cbd5e1" stroke-width="1.2" fill="none"/>
  <circle cx="-5" cy="-2" r="1.5" fill="#fde047"/>
  <circle cx="5" cy="-2" r="1.5" fill="#fde047"/>
</g>

<!-- 3D Dragon Body Arch Module -->
<g id="r-ar">
  <!-- Diagonal V-shaped suspension cables -->
  <path d="M16 190L38 146M38 146L60 190M60 190L85 132M85 132L110 190M110 190L132 146M132 146L154 190" stroke="#94a3b8" stroke-width="1.2" opacity=".85"/>
  <path d="M14 190h4m40 0h4m46 0h4m42 0h4" stroke="#334155" stroke-width="2.5" stroke-linecap="round"/>

  <!-- Underbelly shadow arch -->
  <path d="M-2 205q87-68 174 0v9q-87-68-174 0z" fill="#7a3e00"/>

  <!-- Swept-back 3D Faceted Dorsal Fins -->
  <path d="M16 172l-12-20 16 8 4 12zM42 153l-14-22 18 8 4 14zM72 138l-15-24 20 8 4 16zM102 138l-14-24 19 8 4 16zM132 153l-14-22 18 8 4 14zM156 172l-12-20 16 8 4 12z" fill="#ea580c"/>
  <path d="M18 172l-10-20 10 7 4 13zM44 153l-12-22 12 7 4 15zM74 138l-13-24 13 7 4 17zM104 138l-12-24 13 7 4 17zM134 153l-12-22 12 7 4 15zM158 172l-10-20 10 7 4 13z" fill="#fbcc02"/>
  <path d="M12 160l6-7 2 8zM38 139l7-7 2 8zM68 123l8-8 2 9zM98 123l8-8 2 9zM128 139l7-7 2 8zM152 160l6-7 2 8z" fill="#fef08a"/>

  <!-- Multi-tubular Golden Pipes (3 Parallel Steel Tubes) -->
  <path d="M-3 194q88-75 176 0v8q-88-75-176 0z" fill="#b45309"/>
  <path d="M-3 186q88-77 176 0v8q-88-77-176 0z" fill="url(#dg)"/>
  <path d="M-2 178q87-79 174 0v8q-87-79-174 0z" fill="#fde047"/>
  <path d="M-2 186q87-77 174 0m-174 8q87-75 174 0" stroke="#7a3e00" stroke-width="1.2" fill="none"/>

  <!-- Ribbed Collar Rings -->
  <path d="M26 172l-5 28 6 1 5-28zm30-18l-3 29 6 1 3-29zm30-8l0 29 6 0 0-29zm30 8l3 29 6-1-3-29zm30 18l5 28 6-1-5-28z" fill="#7a3e00"/>
  <path d="M27 172l-5 26 2 1 5-26zm30-18l-3 27 2 1 3-27zm30-8l0 27 2 0 0-27zm30 8l3 27 2-1-3-27zm30 18l5 26 2-1-5-26z" fill="#fbcc02"/>

  <!-- Continuous Row of Glowing Circular Nodes Along Spine -->
  <path d="M10 174v.1m18-14v.1m20-13v.1m20-9v.1m18-3v.1m18 3v.1m20 9v.1m20 13v.1m18 14v.1" stroke="#fef08a" stroke-width="4.5" stroke-linecap="round"/>
  <path d="M10 174v.1m18-14v.1m20-13v.1m20-9v.1m18-3v.1m18 3v.1m20 9v.1m20 13v.1m18 14v.1" stroke="#ea580c" stroke-width="2" stroke-linecap="round"/>

  <!-- Lamppost on Deck -->
  <use href="#r-lp" x="85" y="166"/>
</g>
</defs>'''

def get_tail_svg():
    return '''<g id="dragon-1" class="slice dragon-slice">
  <use href="#r-pr" x="40" y="215"/>
  <use href="#r-dk" x="15" y="192"/>
  
  <!-- 3D Flame/Lotus Tail Fin -->
  <path d="M42 206l-16-8-14-32 8 8 10-24 5 14 16-26 3 16 15-20-3 20 12-14-4 22 14-12-6 22 12-8-6 20z" fill="#3a1504"/>
  <path d="M44 204l-14-7-12-28 7 7 8-20 5 12 14-22 3 14 13-17-3 17 11-12-4 19 12-10-6 19 10-6-5 17z" fill="#ea580c"/>
  <path d="M46 202l-12-6-10-24 6 6 7-16 4 10 12-18 3 12 11-14-2 14 10-10-3 16 10-8-5 16 8-5-4 14z" fill="#f59e0b"/>
  <path d="M48 200l-10-5-8-20 5 5 6-13 4 8 10-14 3 10 9-11-2 11 8-8-3 13 8-6-4 13 6-4-3 11z" fill="#fbcc02"/>
  <path d="M38 174l-6-16m12 12l-4-12m12 10l-2-10m11 9l-1-8" stroke="#fef08a" stroke-width="1.2" stroke-linecap="round"/>

  <!-- Tail Collar Clamp Band -->
  <path d="M52 195l10-18 6 3-10 18z" fill="#7a3e00"/>
  <path d="M53 194l9-16 3 1-9 16z" fill="#fbcc02"/>
  <path d="M56 192v.1m3-6v.1m3-5v.1" stroke="#3a1504" stroke-width="2.5" stroke-linecap="round"/>

  <!-- Body Section 1 -->
  <path d="M60 196q30 20 55 12 25-10 38-26v8q-15 18-40 28-26 8-53-14z" fill="#7a3e00"/>
  <path d="M60 190q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="#b45309"/>
  <path d="M60 183q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="url(#dg)"/>
  <path d="M60 176q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="#fde047"/>
  <path d="M60 183q30 20 55 12 25-10 38-26m-93 7q30 20 55 12 25-10 38-26" stroke="#7a3e00" stroke-width="1.2" fill="none"/>

  <!-- Dorsal Fins on Section 1 -->
  <path d="M96 190l-8-14 12 5 2 9zM124 176l-10-16 14 6 2 10zM144 162l-10-18 14 6 2 12z" fill="#ea580c"/>
  <path d="M97 190l-7-14 8 4 2 10zM125 176l-8-16 9 5 2 11zM145 162l-8-18 9 5 2 13z" fill="#fbcc02"/>

  <!-- Spine Nodes on Section 1 -->
  <path d="M75 188v.1m20 4v.1m20-4v.1m18-10v.1m16-14v.1" stroke="#fef08a" stroke-width="4" stroke-linecap="round"/>
  <path d="M75 188v.1m20 4v.1m20-4v.1m18-10v.1m16-14v.1" stroke="#ea580c" stroke-width="1.8" stroke-linecap="round"/>
</g>'''

def get_bridge_slices():
    defs = get_dragon_defs()
    s1 = get_tail_svg()
    s2 = '<g id="dragon-2" class="slice dragon-slice"><use href="#r-pr" x="200" y="215"/><use href="#r-dk" x="150" y="192"/><use href="#r-ar" x="150"/></g>'
    s3 = '<g id="dragon-3" class="slice dragon-slice"><use href="#r-pr" x="370" y="215"/><use href="#r-dk" x="320" y="192"/><use href="#r-ar" x="320"/></g>'
    s4 = '<g id="dragon-4" class="slice dragon-slice"><use href="#r-pr" x="540" y="215"/><use href="#r-dk" x="490" y="192"/><use href="#r-ar" x="490"/></g>'
    
    with open('index.html', 'r', encoding='utf-8') as f:
        html = f.read()
    h_s = html.find('<g id="dragon-head-exact">')
    h_e = html.find('</g><g class="dragon-breath">') + 4
    head = html[h_s:h_e]
    
    br = '<g class="dragon-breath"><path d="M790 105q18-6 34-14-8 16-26 22z" fill="#f97316" opacity=".85"/><circle cx="814" cy="100" r="4" fill="#ef4444" opacity=".8"/><circle cx="826" cy="94" r="2.5" fill="#fbbf24" opacity=".9"/></g>'
    s5 = f'<g id="dragon-5" class="slice dragon-slice"><use href="#r-pr" x="705" y="215"/><use href="#r-dk" x="650" y="192"/>{head}{br}</g>'
    
    water = '<path d="M0 240h800v60H0z" fill="#0b172a"/><path d="M0 240h800" stroke="#0284c7" stroke-width="2" stroke-dasharray="10 6" opacity=".5"/>'
    return f'<svg id="svg-dragon" class="svg-art" viewBox="0 0 800 300">{defs}{water}{s1}{s2}{s3}{s4}{s5}</svg>'

svg_dragon = get_bridge_slices()

with open('index.html', 'r', encoding='utf-8') as f:
    text = f.read()

d_s = text.find('<svg id="svg-dragon"')
d_e = text.find('</svg>', d_s) + 6
test_html = text[:d_s] + svg_dragon + text[d_e:]
test_html = test_html.replace("switchLandmark('eiffel');", "switchLandmark('dragon');setGameStage('dragon',5);")

with open('test_redesigned_bridge.html', 'w', encoding='utf-8') as f:
    f.write(test_html)

print("Saved test_redesigned_bridge.html. Size:", len(test_html.encode('utf-8')), "bytes")
