# ESP32-S3 Smartwatch

A complete smartwatch application for the Waveshare ESP32-S3 Touch AMOLED 2.06 inch display module.

**Version:** 2.3.1
**Author:** JWP van Renen
**Date:** January 31, 2026

---

## Table of Contents

1. [Hardware](#hardware)
2. [Features](#features)
3. [Navigation](#navigation)
4. [SD Card Structure](#sd-card-structure)
5. [Power Saving](#power-saving)
6. [Installation](#installation)
7. [Configuration](#configuration)
8. [Project Structure](#project-structure)
9. [Troubleshooting](#troubleshooting)

---

## Hardware

| Component | Type | Specifications |
|-----------|------|----------------|
| Microcontroller | ESP32-S3 | Dual-core Xtensa LX7 @ 240MHz, 8MB PSRAM, 16MB Flash |
| Display | CO5300 AMOLED | 410 x 502 pixels, 16-bit RGB565, QSPI interface |
| Touch | FT3168 | Capacitive multi-touch, I2C @ 0x38, interrupt-driven |
| Power | AXP2101 | Li-Ion/Li-Po charging, battery monitoring, voltage regulation |
| Audio | ES8311 | I2S codec with speaker output and microphone input |
| RTC | PCF85063 | Real-time clock with battery backup |
| Storage | MicroSD | 1-bit SDMMC interface, supports SDSC/SDHC |

### Pin Configuration

| Function | Pin |
|----------|-----|
| Display QSPI | GPIO 4, 5, 6, 7 (SDIO0-3), 11 (SCLK), 12 (CS), 8 (RESET) |
| I2C Bus | GPIO 15 (SDA), 14 (SCL) |
| Touch INT | GPIO 38 |
| SD Card | GPIO 2 (CLK), 1 (CMD), 3 (DATA), 17 (CS) |
| Audio I2S | GPIO 16 (MCLK), 41 (BCLK), 45 (LRCK), 40 (DOUT), 42 (DIN) |
| Speaker | GPIO 46 (PA Control) |

---

## Features

### Screen 1 - Clock (ui_time)
- Large digital time display (hours:minutes)
- Separate seconds display
- Full date with Dutch day names (e.g., "Vrijdag 17 Januari 2026")
- Automatic NTP synchronization via WiFi
- Timezone: Europe/Brussels (CET/CEST with automatic DST)

### Screen 2 - Step Counter (ui_steps)
- Step counter with visual display
- **Tap**: Pause/resume counting
- **Long press (>1 sec)**: Reset to 0
- Steps are automatically saved to SD card

### Screen 3 - Settings (ui_settings)
- Brightness slider (0-255)
- Battery percentage with color coding:
  - Green (>50%): Well charged
  - Yellow (20-50%): Moderate
  - Red (<20%): Low battery
- WiFi connection status

### Screen 4 - Files (ui_files)
- SD card file browser with folder navigation
- File type icons with color coding:
  - Blue: Folders
  - Green: Images (JPG, PNG, GIF, BMP)
  - Orange: Audio (MP3, WAV, OGG, FLAC)
  - Cyan: Text (TXT, LOG, CSV)
- Tap to open files or navigate folders
- ".." to go back to parent folder

### Audio Player (ui_player)
- Playback controls: Previous, Play/Pause, Next, Repeat
- Volume control via swipe gestures
- Background artwork display
- Track information

---

## Navigation

### Screen Navigation
| Gesture | Action |
|---------|--------|
| Swipe left | Go to next screen |
| Swipe right | Go to previous screen |
| Tap | Select / Activate |
| Long press | Context action |

Screen order: Clock → Steps → Settings → Files → (wraps to Clock)

### File Viewer Navigation
| Gesture | Action |
|---------|--------|
| Swipe up | Previous file of same type |
| Swipe down | Next file of same type |
| Swipe left/right | Volume down/up (audio only) |
| Long press (3 sec) | Return to file list |

---

## SD Card Structure

The smartwatch requires a specific folder structure on the MicroSD card. Create the following folders and add your files:

```
SD Card Root/
│
├── audio/                    # Audio files for playback
│   ├── music1.mp3
│   ├── music2.wav
│   ├── podcast.ogg
│   └── ...
│
├── afbeeldingen/             # Images for viewing
│   ├── photo1.jpg
│   ├── picture2.png
│   ├── animation.gif
│   └── ...
│
├── tekst/                    # Text files for reading
│   ├── notes.txt
│   ├── data.csv
│   ├── log.log
│   └── ...
│
├── images/                   # System UI images (required!)
│   ├── BG_Watch_Audio.png    # Audio player background (410x502)
│   ├── button_play_75.png    # Play button icon (75x75)
│   ├── button_pause_75.png   # Pause button icon (75x75)
│   ├── button_prev_75.png    # Previous button icon (75x75)
│   ├── button_next_75.png    # Next button icon (75x75)
│   └── button_repeat_75.png  # Repeat button icon (75x75)
│
└── Steps/                    # Auto-created by system
    └── steps_2026-01-31.txt  # Daily step logs
```

### Folder Descriptions

| Folder | Purpose | Supported Formats |
|--------|---------|-------------------|
| `/audio/` | Music and audio files | MP3, WAV, OGG, FLAC |
| `/afbeeldingen/` | Photos and images | JPG, JPEG, PNG, GIF, BMP |
| `/tekst/` | Text documents | TXT, LOG, CSV |
| `/images/` | System UI assets | PNG (specific sizes) |
| `/Steps/` | Step counter logs | Auto-generated TXT |

### UI Asset Requirements

The audio player requires these PNG images in `/images/`:

| File | Size | Description |
|------|------|-------------|
| `BG_Watch_Audio.png` | 410 x 502 px | Audio player background |
| `button_play_75.png` | 75 x 75 px | Play button |
| `button_pause_75.png` | 75 x 75 px | Pause button |
| `button_prev_75.png` | 75 x 75 px | Previous track button |
| `button_next_75.png` | 75 x 75 px | Next track button |
| `button_repeat_75.png` | 75 x 75 px | Repeat button |

---

## Power Saving

The system implements a 3-stage power saving mechanism:

| State | Brightness | Trigger | Description |
|-------|------------|---------|-------------|
| Active | 200 | Any touch | Full brightness, all updates active |
| Dimmed | 50 | 15 sec inactivity | Reduced brightness, updates continue |
| Sleep | 0 | 30 sec inactivity | Display off, updates paused |

Any touch interaction immediately returns to Active state.

---

## Installation

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

Install these libraries via Arduino Library Manager or include from the `libraries/` folder:

| Library | Version | Purpose |
|---------|---------|---------|
| LVGL | 9.3.0 | Graphics user interface |
| Arduino_GFX_Library | Latest | Display driver (CO5300) |
| Arduino_DriveBus_Library | Latest | I2C driver (FT3168 touch) |
| XPowersLib | Latest | Power management (AXP2101) |
| ESP32-audioI2S | Latest | Audio playback |
| TJpg_Decoder | Latest | JPEG decoding |
| PNGdec | Latest | PNG decoding |
| AnimatedGIF | Latest | GIF decoding |

### Upload Steps

1. Connect the ESP32-S3 via USB
2. Select the correct COM port
3. Hold BOOT button, press RESET, release BOOT (if needed)
4. Click Upload in Arduino IDE
5. Wait for upload to complete
6. Press RESET to start the application

---

## Configuration

### WiFi Settings

Edit `wifi_config.h`:

```c
#define WIFI_SSID "your_network_name"
#define WIFI_PASSWORD "your_password"
#define NTP_SERVER "pool.ntp.org"
```

### LVGL Configuration

The file `lv_conf.h` contains LVGL settings. Key settings:

```c
#define LV_COLOR_DEPTH 16          // RGB565
#define LV_HOR_RES_MAX 410         // Display width
#define LV_VER_RES_MAX 502         // Display height
#define LV_USE_PSRAM 1             // Use PSRAM for buffers
```

---

## Project Structure

```
nieuw/
├── nieuw.ino               # Main application (v2.3.1)
├── pin_config.h            # Hardware pin definitions
├── wifi_config.h           # WiFi and NTP settings
├── lv_conf.h               # LVGL configuration
│
├── ui.h / ui.c             # UI initialization
├── ui_helpers.h / ui_helpers.c
├── ui_events.h / ui_events.cpp
│
├── ui_time.h / ui_time.c       # Clock screen
├── ui_steps.h / ui_steps.c     # Step counter screen
├── ui_settings.h / ui_settings.c # Settings screen
├── ui_files.h / ui_files.c     # File browser screen
├── ui_player.h / ui_player.c   # Audio player screen
├── ui_audio.h / ui_audio.c     # Audio list screen
├── ui_images.h / ui_images.c   # Image list screen
├── ui_keyboard.h / ui_keyboard.c
├── ui_viewerfiles.h / ui_viewerfiles.c
├── ui_viewerimages.h / ui_viewerimages.c
│
├── ui_img_*.c              # Embedded image assets
│
└── libraries/              # External libraries
    ├── AnimatedGIF/
    ├── ESP32-audioI2S-master/
    ├── GFX_Library_for_Arduino/
    ├── lvgl/
    ├── PNGdec/
    ├── TJpg_Decoder/
    └── XPowersLib/
```

---

## Troubleshooting

### SD Card Not Detected
- Ensure card is formatted as FAT32
- Check card is properly inserted
- Try a different SD card (max 32GB recommended)
- Verify SD card pins in `pin_config.h`

### Display Not Working
- Check QSPI connections
- Verify `lv_conf.h` settings match hardware
- Ensure PSRAM is enabled in board settings

### WiFi Not Connecting
- Verify SSID and password in `wifi_config.h`
- Check WiFi signal strength
- Ensure 2.4GHz network (5GHz not supported)

### Audio Not Playing
- Check files are in `/audio/` folder
- Verify supported format (MP3, WAV, OGG, FLAC)
- Ensure ES8311 codec is properly connected
- Check speaker connection to PA_CTRL pin

### Touch Not Responding
- Verify I2C connections (SDA, SCL)
- Check touch interrupt pin (TP_INT)
- Ensure FT3168 address is 0x38

---

## License

Copyright (c) 2026 JWP van Renen

---

## Links

- [Waveshare ESP32-S3 Touch AMOLED 2.06](https://www.waveshare.com/esp32-s3-touch-amoled-2.06.htm)
- [LVGL Documentation](https://docs.lvgl.io/)
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/)
