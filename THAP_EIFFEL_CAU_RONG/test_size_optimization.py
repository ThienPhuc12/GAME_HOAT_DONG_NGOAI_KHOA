# test_size_optimization.py

# Optimized V-cables
v_cables = '<path d="M16 190l22-44 22 44 25-58 25 58 22-44 22 44" stroke="#94a3b8" stroke-width="1.2" opacity=".85" fill="none"/><path d="M14 190h4m40 0h4m46 0h4m42 0h4" stroke="#334155" stroke-width="2.5" stroke-linecap="round"/>'

# Optimized Fins (5 sharp swept-back 3D fins along the arch)
fins_shadow = 'M20 172l-12-18 15 7 3 11zM50 152l-14-20 17 8 3 12zM85 138l-15-22 18 8 3 14zM120 152l-14-20 17 8 3 12zM150 172l-12-18 15 7 3 11z'
fins_light = 'M22 172l-10-18 10 6 3 12zM52 152l-12-20 11 7 3 13zM87 138l-13-22 12 7 3 15zM122 152l-12-20 11 7 3 13zM152 172l-10-18 10 6 3 12z'
fins_accent = 'M16 162l5-6 2 7zM45 141l6-6 2 8zM79 125l7-7 2 9zM115 141l6-6 2 8zM146 162l5-6 2 7z'

fins = f'<path d="{fins_shadow}" fill="#ea580c"/><path d="{fins_light}" fill="#fbcc02"/><path d="{fins_accent}" fill="#fef08a"/>'

# Multi-tubular pipes
tubes = '<path d="M-2 205q87-68 174 0v9q-87-68-174 0z" fill="#7a3e00"/><path d="M-3 194q88-75 176 0v8q-88-75-176 0z" fill="#b45309"/><path d="M-3 186q88-77 176 0v8q-88-77-176 0z" fill="url(#dg)"/><path d="M-2 178q87-79 174 0v8q-87-79-174 0z" fill="#fde047"/><path d="M-2 186q87-77 174 0m-174 8q87-75 174 0" stroke="#7a3e00" stroke-width="1.2" fill="none"/>'

# Ribbed rings
rings = '<path d="M30 170l-4 27 5 1 4-27zm38-18l-2 28 5 1 2-28zm38 0l2 28 5-1-2-28zm38 18l4 27 5-1-4-27z" fill="#7a3e00"/><path d="M31 170l-4 25 2 1 4-25zm38-18l-2 26 2 1 2-26zm38 0l2 26 2-1-2-26zm38 18l4 25 2-1-4-25z" fill="#fbcc02"/>'

# Spine nodes
nodes = '<path d="M15 170v.1m20-14v.1m25-13v.1m25-6v.1m25 6v.1m25 13v.1m20 14v.1" stroke="#fef08a" stroke-width="4.5" stroke-linecap="round"/><path d="M15 170v.1m20-14v.1m25-13v.1m25-6v.1m25 6v.1m25 13v.1m20 14v.1" stroke="#ea580c" stroke-width="2" stroke-linecap="round"/>'

ar = f'<g id="r-ar">{v_cables}{tubes}{fins}{rings}{nodes}<use href="#r-lp" x="85" y="166"/></g>'
print("Optimized Arch (r-ar) length:", len(ar))
