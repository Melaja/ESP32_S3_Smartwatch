# ESP32-S3 Smartwatch

A complete smartwatch application for the Waveshare ESP32-S3 Touch AMOLED 2.06 inch display module.

**Version:** 2.3.1
**Author:** JWP van Renen
**Date:** January 31, 2026

## Hardware

| Component | Type | Specifications |
|-----------|------|----------------|
| Microcontroller | ESP32-S3 | Dual-core 240MHz, 8MB PSRAM, 16MB Flash |
| Display | CO5300 AMOLED | 410 x 502 pixels, 16-bit RGB565, QSPI |
| Touch | FT3168 | Capacitive multi-touch, I2C @ 0x38 |
| Power | AXP2101 | Li-Ion/Li-Po charging, battery monitoring |
| Audio | ES8311 | I2S codec with speaker and microphone |
| RTC | PCF85063 | Real-time clock with backup |
| Storage | SD card | 1-bit SDMMC interface |

## Features

### Screen 1 - Clock
- Digital time display (hours:minutes:seconds)
- Date with day name in Dutch
- Automatic NTP synchronization
- Timezone: Europe/Brussels (CET/CEST)

### Screen 2 - Step Counter
- Step counting with pause/resume (tap)
- Reset to 0 (long press >1 sec)

### Screen 3 - Settings
- Brightness control (0-255)
- Battery percentage with color coding
- WiFi connection status

### Screen 4 - Files
- SD card browser
- Supported formats:
  - Images: JPG, PNG, GIF, BMP
  - Audio: MP3, WAV, OGG, FLAC
  - Text: TXT, LOG, CSV

## Navigation

| Action | Function |
|--------|----------|
| Swipe left | Next screen |
| Swipe right | Previous screen |
| Tap | Select / Open |
| Long press | Context action |

### In File Viewer
| Action | Function |
|--------|----------|
| Swipe up/down | Previous/next file |
| Swipe left/right | Volume (audio only) |
| Long press (3 sec) | Back to list |

## Power Saving

| State | Brightness | Activation |
|-------|------------|------------|
| Active | 200 | After touch |
| Dimmed | 50 | After 15 sec inactivity |
| Sleep | 0 | After 30 sec inactivity |

## Compilation

### Arduino IDE Settings

| Setting | Value |
|---------|-------|
| Board | ESP32S3 Dev Module |
| CPU Frequency | 240MHz (WiFi) |
| Flash Mode | QIO 80MHz |
| Flash Size | 16MB (128Mb) |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Speed | 921600 |
| USB Mode | Hardware CDC and JTAG |

### Required Libraries

- LVGL (v9.3.0)
- Arduino_GFX_Library
- Arduino_DriveBus_Library
- XPowersLib
- ESP32-audioI2S

## Configuration

### WiFi
Edit `wifi_config.h` with your network settings:
```c
#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"
```

### Pin Configuration
See `pin_config.h` for all hardware pin definitions.

## Project Structure

```
nieuw/
├── nieuw.ino           # Main program
├── pin_config.h        # Hardware pin definitions
├── wifi_config.h       # WiFi settings
├── lv_conf.h           # LVGL configuration
├── ui.h/c              # UI initialization
├── ui_*.h/c            # Screen definitions (SquareLine Studio)
└── libraries/          # External libraries
    ├── AnimatedGIF/
    ├── ESP32-audioI2S-master/
    ├── GFX_Library_for_Arduino/
    ├── lvgl/
    ├── PNGdec/
    ├── TJpg_Decoder/
    └── XPowersLib/
```

## License

Copyright (c) 2026 JWP van Renen
