# ESP32-S3 Smartwatch

Een complete smartwatch applicatie voor de Waveshare ESP32-S3 Touch AMOLED 2.06 inch display module.

**Versie:** 2.3.1
**Auteur:** JWP van Renen
**Datum:** 31 januari 2026

## Hardware

| Component | Type | Specificaties |
|-----------|------|---------------|
| Microcontroller | ESP32-S3 | Dual-core 240MHz, 8MB PSRAM, 16MB Flash |
| Display | CO5300 AMOLED | 410 x 502 pixels, 16-bit RGB565, QSPI |
| Touch | FT3168 | Capacitief multi-touch, I2C @ 0x38 |
| Power | AXP2101 | Li-Ion/Li-Po laden, batterij monitoring |
| Audio | ES8311 | I2S codec met speaker en microfoon |
| RTC | PCF85063 | Real-time clock met backup |
| Opslag | SD kaart | 1-bit SDMMC interface |

## Functionaliteiten

### Scherm 1 - Klok
- Digitale tijdweergave (uren:minuten:seconden)
- Datum met dagnaam in het Nederlands
- Automatische NTP synchronisatie
- Tijdzone: Europa/Brussel (CET/CEST)

### Scherm 2 - Stappenteller
- Stappen tellen met pauze/hervat (tik)
- Reset naar 0 (lang drukken >1 sec)

### Scherm 3 - Instellingen
- Helderheidsregeling (0-255)
- Batterijpercentage met kleurcodering
- WiFi verbindingsstatus

### Scherm 4 - Bestanden
- SD kaart browser
- Ondersteunde formaten:
  - Afbeeldingen: JPG, PNG, GIF, BMP
  - Audio: MP3, WAV, OGG, FLAC
  - Tekst: TXT, LOG, CSV

## Navigatie

| Actie | Functie |
|-------|---------|
| Swipe links | Volgend scherm |
| Swipe rechts | Vorig scherm |
| Tik | Selecteren / Openen |
| Lang drukken | Context actie |

### In bestandsviewer
| Actie | Functie |
|-------|---------|
| Swipe omhoog/omlaag | Vorig/volgend bestand |
| Swipe links/rechts | Volume (alleen audio) |
| Lang drukken (3 sec) | Terug naar lijst |

## Energiebesparing

| Status | Helderheid | Activatie |
|--------|------------|-----------|
| Actief | 200 | Na touch |
| Gedimd | 50 | Na 15 sec inactiviteit |
| Slaap | 0 | Na 30 sec inactiviteit |

## Compilatie

### Arduino IDE Instellingen

| Instelling | Waarde |
|------------|--------|
| Board | ESP32S3 Dev Module |
| CPU Frequency | 240MHz (WiFi) |
| Flash Mode | QIO 80MHz |
| Flash Size | 16MB (128Mb) |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| Upload Speed | 921600 |
| USB Mode | Hardware CDC and JTAG |

### Vereiste Libraries

- LVGL (v9.3.0)
- Arduino_GFX_Library
- Arduino_DriveBus_Library
- XPowersLib
- ESP32-audioI2S

## Configuratie

### WiFi
Pas `wifi_config.h` aan met je netwerkinstellingen:
```c
#define WIFI_SSID "jouw_ssid"
#define WIFI_PASSWORD "jouw_wachtwoord"
```

### Pin Configuratie
Zie `pin_config.h` voor alle hardware pin definities.

## Projectstructuur

```
nieuw/
├── nieuw.ino           # Hoofdprogramma
├── pin_config.h        # Hardware pin definities
├── wifi_config.h       # WiFi instellingen
├── lv_conf.h           # LVGL configuratie
├── ui.h/c              # UI initialisatie
├── ui_*.h/c            # Scherm definities (SquareLine Studio)
└── libraries/          # Externe libraries
    ├── AnimatedGIF/
    ├── ESP32-audioI2S-master/
    ├── GFX_Library_for_Arduino/
    ├── lvgl/
    ├── PNGdec/
    ├── TJpg_Decoder/
    └── XPowersLib/
```

## Licentie

Copyright (c) 2026 JWP van Renen
