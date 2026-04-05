# This directory should contain the DABShield library

## Setup Instructions

The DABShield library should be placed in this directory. To set up:

### Option 1: Copy from Parent Directory

If the DABShield library is located in the parent DABShield folder, copy it here:

```bash
cp -r ../DABShield ./
```

### Option 2: Create a Symlink

For development, you can create a symbolic link to avoid duplication:

```bash
ln -s ../.. DABShield
```

### Option 3: Install via PlatformIO Registry

If the DABShield library is published to the PlatformIO registry, add it to `platformio.ini`:

```ini
lib_deps =
    DABShield
```

## Library Structure

The DABShield library should have the following structure:

```
lib/
├── DABShield or similar/
│   ├── src/
│   │   ├── DABShield.h
│   │   ├── DABShield.cpp
│   │   └── ... other implementation files
│   ├── library.json       (PlatformIO manifest)
│   └── library.properties (Arduino IDE manifest)
```

## Notes

- The `lib/` directory is for local libraries
- PlatformIO automatically detects libraries here and compiles them as part of the build
- External dependencies (Adafruit libraries) are specified in `platformio.ini`
- Do NOT commit compiled objects or build artifacts to this directory
