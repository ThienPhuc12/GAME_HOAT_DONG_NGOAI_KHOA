# Read generate_all.py
import generate_all

# Get html from generate_all
html = generate_all.html

with open('index.html', 'w', encoding='utf-8') as f:
    f.write(html)

# Test dragon stage 5
d5 = html.replace("switchLandmark('eiffel');", "switchLandmark('dragon');setGameStage('dragon',5);")
with open('v_gen_dragon5.html', 'w', encoding='utf-8') as f:
    f.write(d5)

# Test dragon stage 2
d2 = html.replace("switchLandmark('eiffel');", "switchLandmark('dragon');setGameStage('dragon',2);")
with open('v_gen_dragon2.html', 'w', encoding='utf-8') as f:
    f.write(d2)

# Test eiffel stage 5
e5 = html.replace("switchLandmark('eiffel');", "switchLandmark('eiffel');setGameStage('eiffel',5);")
with open('v_gen_eiffel5.html', 'w', encoding='utf-8') as f:
    f.write(e5)

print("Generated and written to index.html and test HTMLs.")
