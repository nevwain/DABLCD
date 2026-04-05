# DABLCD - PlatformIO Project

This is the PlatformIO converted version of the DABShield Example Application - a DAB/FM radio receiver with LCD display control.

## Project Overview

**Version:** v2.0 (Updated for IDE 2.3.3 onwards)  
**Author:** AVIT Research Ltd  
**Hardware:** DAB Shield (supports both standard and Pro versions)

### Features
- DAB and FM radio reception
- 16x2 RGB LCD display with menu interface
- Non-volatile memory storage (EEPROM/Flash)
- Button controls for:
  - Volume up/down
  - Service/frequency navigation
  - FM seeking
  - Mode selection (DAB/FM)
  - Re-scanning
- Speaker output configuration (mono, differential stereo)

## Supported Boards

The project includes build configurations for multiple Arduino and compatible boards:

1. **Arduino UNO R4 Minima** (default) - Uses internal EEPROM (8KB)
2. **Arduino UNO R4 WiFi** - Uses internal EEPROM
3. **Arduino Zero** - Uses FlashStorage_SAMD for non-volatile memory
4. **Arduino M0 Pro** - Uses FlashStorage_SAMD for non-volatile memory
5. **Arduino Due** - Uses DueFlashStorage for non-volatile memory
6. **ESP32 D1 R32** - Custom build configuration

## Project Structure

```
DABLCD/
├── platformio.ini          # PlatformIO configuration with multiple environments
├── src/
│   └── DABLCD.ino         # Main application code
├── include/               # Header files (if needed)
├── lib/                   # Local libraries (DABShield should be here)
├── README.md              # This file
├── DABLCD.ino            # Original Arduino sketch (backup)
└── DABLCD.bak            # Backup file
```

## Library Dependencies

The project requires the following libraries:

- **DABShield** - DAB/FM receiver library (should be in `lib/` directory or installed via PlatformIO)
- **Adafruit RGB LCD Shield Library** - Controls the RGB LCD display
- **FlashStorage_SAMD** - Non-volatile storage for SAMD devices (M0/Zero)
- **DueFlashStorage** - Non-volatile storage for Arduino Due
- **EEPROM** - Built-in library for UNO R4 and other boards with native EEPROM support

## Building and Uploading

### Prerequisites

1. **PlatformIO** - Install [PlatformIO](https://platformio.org/) IDE or CLI
2. **DABShield Library** - Make sure the DABShield library is available:
   - Place it in the `lib/` folder, or
   - Install via PlatformIO registry if available

### Build for Default Board (UNO R4 Minima)

```bash
# Build the project
platformio run

# Build and upload to board
platformio run --target upload

# Monitor serial output
platformio run --target upload --target monitor
```

### Build for Specific Board

```bash
# Build for Arduino Zero
platformio run -e arduino_zero

# Build and upload to Arduino Zero
platformio run -e arduino_zero --target upload

# Build for Arduino Due
platformio run -e arduino_due --target upload

# Build for ESP32 D1 R32
platformio run -e esp32_d1_r32 --target upload
```

### Available Build Environments

| Environment | Board | Platform | Notes |
|---|---|---|---|
| `uno_r4_minima` | Arduino UNO R4 Minima | Arduino | Default, 8KB EEPROM |
| `uno_r4_wifi` | Arduino UNO R4 WiFi | Arduino | 8KB EEPROM |
| `arduino_zero` | Arduino Zero | SAMD21 | 12KB Flash storage |
| `arduino_m0` | Arduino M0 Pro | SAMD21 | 12KB Flash storage |
| `arduino_due` | Arduino Due | SAM3X | 4KB EEPROM + Flash |
| `esp32_d1_r32` | ESP32 D1 R32 | ESP32 | Custom GPIO mapping |

## Configuration

### Hardware Setup

The project can be configured via `#define` statements at the top of the code:

```cpp
#define SPEAKER_OUTPUT  SPEAKER_DIFF   // Options: SPEAKER_NONE, SPEAKER_DIFF, SPEAKER_STEREO
```

### SPI Configuration

For boards using bit-bang SPI (default for most boards):
```cpp
#define DAB_SPI_BITBANG
```

To use native SPI instead, comment out `DAB_SPI_BITBANG` in the source code.

### Pin Configuration

Default SPI pins depend on the board:

**Standard Arduino (UNO, Zero, M0):**
- SCK: Pin 13
- MOSI: Pin 11
- MISO: Pin 12
- SS: Pin 8

**ESP32:**
- SCK: Pin 18
- MOSI: Pin 23
- MISO: Pin 19
- SS: Pin 12

## Operation

### Button Controls

- **UP**: Increase volume
- **DOWN**: Decrease volume
- **LEFT**: Previous service / FM seek down
- **RIGHT**: Next service / FM seek up
- **SELECT**: Toggle text display modes / Access menu

### Text Display Modes

Cycle through display modes:
1. Service Data (Radio Text)
2. Date/Time
3. Ensemble Name (DAB mode only)

### Menu Options

**DAB Mode Menu:**
- Switch to FM mode
- Re-scan for DAB services

**FM Mode Menu:**
- Switch to DAB mode

Long press SELECT to open the menu, use LEFT/RIGHT to navigate.

## Memory Management

The application stores the following settings in non-volatile memory:
- Current ensemble selection
- Current service within ensemble
- Volume level
- Mode (DAB/FM)
- FM frequency
- Scanned ensemble list (varies by board capacity)

### Max Ensemble Storage

- UNO R4 boards: 8 ensembles
- SAMD boards (M0/Zero): 12 ensembles
- Arduino Due: 16 ensembles (default)

## Serial Monitor

Connect via Serial Monitor (115200 baud) for debugging output.

## Troubleshooting

### Build Fails for SAMD Boards
Ensure FlashStorage_SAMD library is installed:
```bash
platformio lib install khoih-prog/FlashStorage_SAMD@^1.3.2
```

### Build Fails for Arduino Due
Ensure DueFlashStorage library is installed:
```bash
platformio lib install khoih-prog/DueFlashStorage@^1.0.0
```

### DABShield Not Detected
- Verify DABShield library is in `lib/` directory or installed
- Check SPI connections (SCK, MOSI, MISO, SS pins)
- Verify `#define SPEAKER_OUTPUT` matches your hardware

### No DAB Stations Found
- Perform a re-scan using the menu (SELECT, RIGHT arrow)
- Verify antenna connection to DABShield
- Check for local DAB transmitters in your region

## Notes

- Maximum services per ensemble: 24
- Custom LCD character mapping for EBU Latin extended characters
- Non-volatile memory writes are rate-limited to prevent excessive flash wear
- The application waits 10 seconds after channel/volume changes before saving to prevent constant flash writes

## Version History

- **v2.0** (27/08/2025) - Updated for IDE 2.3.3 onwards, PlatformIO conversion
- **v1.1** (23/09/2020) - Added ESP32 D1 R32 Support
- **v1.0** (16/07/2020) - Display mapping for EU chars
- **v0.9** (05/06/2020) - Non-volatile memory storage for Due
- Earlier versions - See source code comments

## License and Attribution

Original work by AVIT Research Ltd. Converted to PlatformIO format for easier multi-platform development and distribution.

## Support

For issues related to:
- **PlatformIO setup**: Refer to [PlatformIO documentation](https://docs.platformio.org/)
- **DABShield library**: Check the DABShield library documentation
- **Adafruit LCD Shield**: See [Adafruit documentation](https://learn.adafruit.com/rgb-lcd-shield)
