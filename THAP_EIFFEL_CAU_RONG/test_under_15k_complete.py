import re

# 1. Optimize Eiffel SVG by merging paths of identical fills
with open('test_redesigned_bridge.html', 'r', encoding='utf-8') as f:
    orig = f.read()

e_s = orig.find('<svg id="svg-eiffel"')
e_e = orig.find('</svg>', e_s) + 6
eiffel = orig[e_s:e_e]
print("Original Eiffel length:", len(eiffel))

# Let's inspect Eiffel's 5 slices
# We preserve the exact 5 slices <g id="eiffel-1"> to <g id="eiffel-5">!
# Just minify whitespace and consolidate paths within each slice
eiffel_min = re.sub(r'>\s+<', '><', eiffel)
eiffel_min = re.sub(r'\s+', ' ', eiffel_min)

# Let's inspect each slice in eiffel
slices = []
for i in range(1, 6):
    s = eiffel.find(f'<g id="eiffel-{i}"')
    e = eiffel.find('</g>', s) + 4
    sl = eiffel[s:e]
    sl = re.sub(r'>\s+<', '><', sl)
    sl = re.sub(r'\s+', ' ', sl)
    slices.append(sl)

# Header of Eiffel
h_eiffel = '<svg id="svg-eiffel" class="svg-art" viewBox="0 0 300 500"><defs><linearGradient id="er" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#b91c1c"/><stop offset="100%" stop-color="#ef4444"/></linearGradient><linearGradient id="ew" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#cbd5e1"/><stop offset="100%" stop-color="#fff"/></linearGradient><linearGradient id="eb" x1="0" y1="0" x2="1" y2="0"><stop offset="0%" stop-color="#1e3a8a"/><stop offset="100%" stop-color="#091834"/></linearGradient></defs><path d="M25 460h250" stroke="#475569" stroke-width="2" stroke-dasharray="6 4" opacity=".6"/>'

eiffel_opt = f"{h_eiffel}{''.join(slices)}</svg>"
print("Optimized Eiffel length:", len(eiffel_opt))
