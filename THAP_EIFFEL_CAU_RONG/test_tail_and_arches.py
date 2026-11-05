# test_tail_and_arches.py
# Designing high-detail 3D puzzle tail fin and multi-tubular body arch

tail_svg = '''
<g id="dragon-tail-3d">
  <!-- Dark 3D cast shadows underneath -->
  <path d="M45 205l-18-12-14-35 8 10 12-25 4 14 18-28 2 16 16-22-4 22 14-16-6 24 16-12-8 22 12-8-6 20z" fill="#3a1504"/>
  <!-- Deep orange / amber flame core petals -->
  <path d="M48 202l-16-10-12-32 7 9 10-22 4 12 16-25 2 14 14-19-3 19 12-14-5 20 14-10-7 19 10-6-5 17z" fill="#ea580c"/>
  <!-- Warm golden mid-layer petals -->
  <path d="M52 198l-14-8-10-28 6 8 8-18 4 10 14-20 2 12 12-15-2 15 10-12-4 16 12-8-6 16 8-5-4 14z" fill="#f59e0b"/>
  <!-- Bright gold top faceted petals -->
  <path d="M56 195l-12-6-8-22 5 6 6-14 3 8 11-15 2 9 9-11-2 11 8-9-3 12 9-6-5 12 6-4-3 10z" fill="#fbcc02"/>
  <!-- Highlight facet spine on each flame tongue -->
  <path d="M46 172l-10-25m14 14l-6-15m14 11l-4-15m14 12l-2-12m12 10l-1-10" stroke="#fef08a" stroke-width="1.2" stroke-linecap="round"/>
  <!-- Tail root clamp band with rivets -->
  <path d="M55 192l12-18 8 5-12 18z" fill="#b45309" stroke="#3a1504" stroke-width="1"/>
  <path d="M57 190l10-15 4 2-10 15z" fill="#fbcc02"/>
  <path d="M59 188v.1m3-5v.1m3-4v.1" stroke="#3a1504" stroke-width="2.5" stroke-linecap="round"/>
</g>
'''

print("Tail template defined.")
