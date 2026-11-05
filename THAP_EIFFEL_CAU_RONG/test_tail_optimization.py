# test_tail_optimization.py

tail_shadow = 'M44 204l-14-6-12-28 6 6 8-18 4 10 12-20 2 12 11-15-2 15 9-11-3 16 10-8-5 16 8-5-4 13z'
tail_orange = 'M45 203l-12-5-10-25 5 5 7-16 4 9 10-18 2 11 9-13-2 13 8-10-3 14 9-7-4 14 7-4-3 11z'
tail_gold = 'M46 201l-10-4-8-22 4 4 6-14 3 8 9-16 2 10 8-11-2 11 7-9-2 12 8-6-4 12 6-3-3 10z'
tail_spines = 'M38 174l-6-16m11 12l-4-12m11 10l-2-10m10 9l-1-8'

tail_fin = (
    f'<path d="{tail_shadow}" fill="#3a1504"/>'
    f'<path d="{tail_orange}" fill="#ea580c"/>'
    f'<path d="{tail_gold}" fill="#fbcc02"/>'
    f'<path d="{tail_spines}" stroke="#fef08a" stroke-width="1.2" stroke-linecap="round"/>'
    '<path d="M52 195l10-18 6 3-10 18z" fill="#7a3e00"/>'
    '<path d="M53 194l9-16 3 1-9 16z" fill="#fbcc02"/>'
    '<path d="M56 192v.1m3-6v.1m3-5v.1" stroke="#3a1504" stroke-width="2.5" stroke-linecap="round"/>'
)

body1 = (
    '<path d="M60 196q30 20 55 12 25-10 38-26v8q-15 18-40 28-26 8-53-14z" fill="#7a3e00"/>'
    '<path d="M60 190q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="#b45309"/>'
    '<path d="M60 183q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="url(#dg)"/>'
    '<path d="M60 176q30 20 55 12 25-10 38-26v7q-15 18-40 28-26 8-53-14z" fill="#fde047"/>'
    '<path d="M60 183q30 20 55 12 25-10 38-26m-93 7q30 20 55 12 25-10 38-26" stroke="#7a3e00" stroke-width="1.2" fill="none"/>'
    '<path d="M96 190l-8-14 12 5 2 9zM124 176l-10-16 14 6 2 10zM144 162l-10-18 14 6 2 12z" fill="#ea580c"/>'
    '<path d="M97 190l-7-14 8 4 2 10zM125 176l-8-16 9 5 2 11zM145 162l-8-18 9 5 2 13z" fill="#fbcc02"/>'
    '<path d="M75 188v.1m20 4v.1m20-4v.1m18-10v.1m16-14v.1" stroke="#fef08a" stroke-width="4" stroke-linecap="round"/>'
    '<path d="M75 188v.1m20 4v.1m20-4v.1m18-10v.1m16-14v.1" stroke="#ea580c" stroke-width="1.8" stroke-linecap="round"/>'
)

tail_block = f'<g id="dragon-1" class="slice dragon-slice"><use href="#r-pr" x="40" y="215"/><use href="#r-dk" x="15" y="192"/>{tail_fin}{body1}</g>'
print("Optimized Tail (dragon-1) length:", len(tail_block))
