# optimize_full_bundle.py
import re

with open('test_redesigned_bridge.html', 'r', encoding='utf-8') as f:
    html = f.read()

print("Original size:", len(html.encode('utf-8')))

# Let's inspect size of each section
s_css = html.find('<style>')
e_css = html.find('</style>')
css = html[s_css+7:e_css]

s_eiffel = html.find('<svg id="svg-eiffel"')
e_eiffel = html.find('</svg>', s_eiffel) + 6
eiffel = html[s_eiffel:e_eiffel]

s_dragon = html.find('<svg id="svg-dragon"')
e_dragon = html.find('</svg>', s_dragon) + 6
dragon = html[s_dragon:e_dragon]

s_js = html.find('<script>')
e_js = html.find('</script>')
js = html[s_js+8:e_js]

print(f"CSS: {len(css)} bytes")
print(f"Eiffel: {len(eiffel)} bytes")
print(f"Dragon: {len(dragon)} bytes")
print(f"JS: {len(js)} bytes")
print(f"Scaffolding: {len(html) - len(css) - len(eiffel) - len(dragon) - len(js)} bytes")
