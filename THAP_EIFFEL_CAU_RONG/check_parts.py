from build_optimized_bundle import get_minified_defs, get_minified_tail, css, js, eiffel, head

print("CSS:", len(css))
print("JS:", len(js))
print("Eiffel:", len(eiffel))
print("Defs:", len(get_minified_defs()))
print("Tail:", len(get_minified_tail()))
print("Head:", len(head))
