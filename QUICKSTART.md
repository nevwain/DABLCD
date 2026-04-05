# Quick Start Guide - DABLCD PlatformIO Project

## 1. Installation

### Install PlatformIO

**Option A: VSCode Extension**
- Open VSCode
- Go to Extensions (Ctrl+Shift+X / Cmd+Shift+X)
- Search for "PlatformIO IDE"
- Click Install

**Option B: Command Line**
```bash
pip install platformio
```

## 2. Project Setup

### Clone/Download the Project
```bash
# The project should be in:
cd /home/engineer/DABShield/examples/DABLCD
```

### Install Dependencies (if needed)
```bash
platformio pkg install
```

## 3. First Build

### For UNO R4 Minima (Default)
```bash
# Navigate to project directory
cd /home/engineer/DABShield/examples/DABLCD

# Build the project
platformio run

# Or build and upload directly
platformio run --target upload

# With serial monitor
platformio run --target upload --target monitor
```

### For Other Boards

**Arduino Zero:**
```bash
platformio run -e arduino_zero --target upload --target monitor
```

**Arduino Due:**
```bash
platformio run -e arduino_due --target upload --target monitor
```

**ESP32 D1 R32:**
```bash
platformio run -e esp32_d1_r32 --target upload --target monitor
```

## 4. Using with VSCode

### Open Project in VSCode
1. Open VSCode
2. File → Open Folder
3. Select `/home/engineer/DABShield/examples/DABLCD`
4. PlatformIO should auto-detect the project

### Build via VSCode
- Click **PlatformIO: Home** in the sidebar
- Or use keyboard shortcuts:
  - `Ctrl+Alt+B` - Build
  - `Ctrl+Alt+U` - Upload
  - `Ctrl+Alt+S` - Build + Monitor

### Select Build Environment
- Click the **Environment** selector in the status bar
- Choose from available boards
- The project will rebuild for that target

## 5. Configure for Your Hardware

### Connect Your Board
- Use USB cable to connect your Arduino/ESP32 board to your computer
- PlatformIO should auto-detect the serial port

### Verify Serial Port (if needed)
```bash
# List available serial ports
platformio device list
```

### Update platformio.ini if needed
For ESP32 or other boards, you may need to specify the serial port:
```ini
[env:esp32_d1_r32]
upload_port = /dev/ttyUSB0  # Linux
upload_port = COM3          # Windows
```

## 6. Typical Workflow

### Edit Code
1. Make changes to `src/DABLCD.ino`
2. Save the file

### Build
```bash
platformio run
```

### Upload and Test
```bash
platformio run --target upload --target monitor
```

### Debug
- View serial output in the terminal monitor
- Check compiler warnings/errors in the build output

## 7. Troubleshooting

### Build Fails - Library Not Found

**Error:** "Error: The dependency 'DABShield' is not installed"

**Solution:**
```bash
# Copy DABShield library to lib/ directory
cp -r ../../your-dabshield-location lib/DABShield
```

Or create a symbolic link:
```bash
ln -s ../../ lib/DABShield
```

### Upload Fails - Port Not Found

**Error:** "Upload port not found"

**Solution:**
1. Verify board is connected via USB
2. List ports: `platformio device list`
3. Update `upload_port` in `platformio.ini`
4. For Linux: May need to add user to `dialout` group:
   ```bash
   sudo usermod -a -G dialout $USER
   ```

### Serial Monitor Not Working

**Solution:**
1. Check baud rate (should be 115200)
2. Verify board is properly connected
3. Try manual monitor:
   ```bash
   platformio device monitor --port /dev/ttyUSB0 --baud 115200
   ```

## 8. Next Steps

- Read the full [README.md](README.md) for detailed documentation
- Explore the [platformio.ini](platformio.ini) configuration
- Modify `src/DABLCD.ino` to customize the application
- Check [DABShield documentation](../../../) for library details

## Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [PlatformIO CLI Manual](https://cli.platformio.org/)
- [Arduino Documentation](https://docs.arduino.cc/)
- [Adafruit RGB LCD Shield](https://learn.adafruit.com/rgb-lcd-shield)

## Support

For platform-specific issues:
- **PlatformIO**: Check [PlatformIO issues](https://github.com/platformio/platformio-core/issues)
- **Arduino**: Check [Arduino forums](https://forum.arduino.cc/)
- **SAMD boards**: Check [Arduino SAMD GitHub](https://github.com/arduino/ArduinoCore-samd)
