with open('test_redesigned_bridge.html', 'r', encoding='utf-8') as f:
    html = f.read()

s_d = html.find('<svg id="svg-dragon"')
e_d = html.find('</svg>', s_d) + 6
dragon = html[s_d:e_d]

print("Total Dragon SVG:", len(dragon))
defs_s = dragon.find('<defs>')
defs_e = dragon.find('</defs>') + 7
defs = dragon[defs_s:defs_e]
print("Defs length:", len(defs))

for i in range(1, 6):
    s = dragon.find(f'<g id="dragon-{i}"')
    e = dragon.find('</g>', s) + 4
    if i == 5:
        e = dragon.find('</svg>', s)
    part = dragon[s:e]
    print(f"dragon-{i}: {len(part)} bytes")
