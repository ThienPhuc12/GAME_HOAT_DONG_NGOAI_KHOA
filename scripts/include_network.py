from pathlib import Path

Import("env")

# Arduino-ESP32 3.x framework libraries are detected by LDF, but their include
# directories are not passed to sibling libraries such as LovyanGFX.  Without
# Wire.h in LovyanGFX's include path, it selects ESP-IDF's new I2C driver while
# U8g2/Arduino Wire selects the legacy driver. Both together abort at startup.
framework = Path(env.PioPlatform().get_package_dir("framework-arduinoespressif32"))
env.Append(CPPPATH=[
    str(framework / "libraries" / "Network" / "src"),
    str(framework / "libraries" / "Wire" / "src"),
])
