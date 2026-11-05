# test_compress_eiffel.py
import re

with open('test_redesigned_bridge.html', 'r', encoding='utf-8') as f:
    text = f.read()

e_s = text.find('<svg id="svg-eiffel"')
e_e = text.find('</svg>', e_s) + 6
eiffel = text[e_s:e_e]

print("Original Eiffel:", len(eiffel))

# Let's inspect eiffel content
print(eiffel[:500])
