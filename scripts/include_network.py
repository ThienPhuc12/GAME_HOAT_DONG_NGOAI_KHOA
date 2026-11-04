from pathlib import Path

Import("env")

# Keep framework Network and Wire headers visible to dependent libraries.
framework = Path(env.PioPlatform().get_package_dir("framework-arduinoespressif32"))
env.Append(CPPPATH=[
    str(framework / "libraries" / "Network" / "src"),
    str(framework / "libraries" / "Wire" / "src"),
])
