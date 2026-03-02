# ESP32-S3 Smartwatch

Een complete smartwatch applicatie voor de Waveshare ESP32-S3 Touch AMOLED 2.06 inch display module.

**Versie:** 2.3.1
**Auteur:** JWP van Renen
**Datum:** 31 januari 2026

---

## Inhoudsopgave

1. [Hardware](#hardware)
2. [Functionaliteiten](#functionaliteiten)
3. [Navigatie](#navigatie)
4. [SD Kaart Structuur](#sd-kaart-structuur)
5. [Energiebesparing](#energiebesparing)
6. [Installatie](#installatie)
7. [Configuratie](#configuratie)
8. [Projectstructuur](#projectstructuur)
9. [Probleemoplossing](#probleemoplossing)

---

## Hardware

| Component | Type | Specificaties |
|-----------|------|---------------|
| Microcontroller | ESP32-S3 | Dual-core Xtensa LX7 @ 240MHz, 8MB PSRAM, 16MB Flash |
| Display | CO5300 AMOLED | 410 x 502 pixels, 16-bit RGB565, QSPI interface |
| Touch | FT3168 | Capacitief multi-touch, I2C @ 0x38, interrupt-driven |
| Voeding | AXP2101 | Li-Ion/Li-Po laden, batterij monitoring, spanningsregeling |
| Audio | ES8311 | I2S codec met speaker output en microfoon input |
| RTC | PCF85063 | Real-time klok met batterij backup |
| Opslag | MicroSD | 1-bit SDMMC interface, ondersteunt SDSC/SDHC |

### Pin Configuratie

| Functie | Pin |
|---------|-----|
| Display QSPI | GPIO 4, 5, 6, 7 (SDIO0-3), 11 (SCLK), 12 (CS), 8 (RESET) |
| I2C Bus | GPIO 15 (SDA), 14 (SCL) |
| Touch INT | GPIO 38 |
| SD Kaart | GPIO 2 (CLK), 1 (CMD), 3 (DATA), 17 (CS) |
| Audio I2S | GPIO 16 (MCLK), 41 (BCLK), 45 (LRCK), 40 (DOUT), 42 (DIN) |
| Speaker | GPIO 46 (PA Control) |

---

## Functionaliteiten

### Scherm 1 - Klok (ui_time)
- Grote digitale tijdweergave (uren:minuten)
- Aparte secondenweergave
- Volledige datum met Nederlandse dagnamen (bijv. "Vrijdag 17 Januari 2026")
- Automatische NTP synchronisatie via WiFi
- Tijdzone: Europa/Brussel (CET/CEST met automatische zomer/wintertijd)

### Scherm 2 - Stappenteller (ui_steps)
- Stappenteller met visuele weergave
- **Tik**: Pauzeren/hervatten van tellen
- **Lang drukken (>1 sec)**: Reset naar 0
- Stappen worden automatisch opgeslagen op SD kaart

### Scherm 3 - Instellingen (ui_settings)
- Helderheidsschuifregelaar (0-255)
- Batterijpercentage met kleurcodering:
  - Groen (>50%): Goed geladen
  - Geel (20-50%): Matig
  - Rood (<20%): Bijna leeg
- WiFi verbindingsstatus

### Scherm 4 - Bestanden (ui_files)
- SD kaart bestandsbrowser met mapnavigatie
- Bestandstype iconen met kleurcodering:
  - Blauw: Mappen
  - Groen: Afbeeldingen (JPG, PNG, GIF, BMP)
  - Oranje: Audio (MP3, WAV, OGG, FLAC)
  - Cyaan: Tekst (TXT, LOG, CSV)
- Tik om bestanden te openen of door mappen te navigeren
- ".." om terug te gaan naar bovenliggende map

### Audio Speler (ui_player)
- Afspeelbediening: Vorige, Afspelen/Pauze, Volgende, Herhalen
- Volumeregeling via veeggebaren
- Achtergrondafbeelding weergave
- Track informatie

---

## Navigatie

### Scherm Navigatie
| Gebaar | Actie |
|--------|-------|
| Veeg naar links | Ga naar volgend scherm |
| Veeg naar rechts | Ga naar vorig scherm |
| Tik | Selecteren / Activeren |
| Lang drukken | Context actie |

Schermvolgorde: Klok → Stappen → Instellingen → Bestanden → (terug naar Klok)

### Bestandsviewer Navigatie
| Gebaar | Actie |
|--------|-------|
| Veeg omhoog | Vorig bestand van zelfde type |
| Veeg omlaag | Volgend bestand van zelfde type |
| Veeg links/rechts | Volume omlaag/omhoog (alleen audio) |
| Lang drukken (3 sec) | Terug naar bestandslijst |

---

## SD Kaart Structuur

De smartwatch vereist een specifieke mappenstructuur op de MicroSD kaart. Maak de volgende mappen aan en voeg je bestanden toe:

```
SD Kaart Root/
│
├── audio/                    # Audio bestanden voor afspelen
│   ├── muziek1.mp3
│   ├── muziek2.wav
│   ├── podcast.ogg
│   └── ...
│
├── afbeeldingen/             # Afbeeldingen voor weergave
│   ├── foto1.jpg
│   ├── plaatje2.png
│   ├── animatie.gif
│   └── ...
│
├── tekst/                    # Tekstbestanden voor lezen
│   ├── notities.txt
│   ├── data.csv
│   ├── logboek.log
│   └── ...
│
├── images/                   # Systeem UI afbeeldingen (verplicht!)
│   ├── BG_Watch_Audio.png    # Audio speler achtergrond (410x502)
│   ├── button_play_75.png    # Afspelen knop icoon (75x75)
│   ├── button_pause_75.png   # Pauze knop icoon (75x75)
│   ├── button_prev_75.png    # Vorige knop icoon (75x75)
│   ├── button_next_75.png    # Volgende knop icoon (75x75)
│   └── button_repeat_75.png  # Herhalen knop icoon (75x75)
│
└── Steps/                    # Automatisch aangemaakt door systeem
    └── steps_2026-01-31.txt  # Dagelijkse stappen logs
```

### Map Beschrijvingen

| Map | Doel | Ondersteunde Formaten |
|-----|------|----------------------|
| `/audio/` | Muziek en audio bestanden | MP3, WAV, OGG, FLAC |
| `/afbeeldingen/` | Foto's en afbeeldingen | JPG, JPEG, PNG, GIF, BMP |
| `/tekst/` | Tekstdocumenten | TXT, LOG, CSV |
| `/images/` | Systeem UI onderdelen | PNG (specifieke afmetingen) |
| `/Steps/` | Stappenteller logs | Automatisch gegenereerd TXT |

### UI Asset Vereisten

De audio speler vereist deze PNG afbeeldingen in `/images/`:

| Bestand | Afmeting | Beschrijving |
|---------|----------|--------------|
| `BG_Watch_Audio.png` | 410 x 502 px | Audio speler achtergrond |
| `button_play_75.png` | 75 x 75 px | Afspelen knop |
| `button_pause_75.png` | 75 x 75 px | Pauze knop |
| `button_prev_75.png` | 75 x 75 px | Vorige track knop |
| `button_next_75.png` | 75 x 75 px | Volgende track knop |
| `button_repeat_75.png` | 75 x 75 px | Herhalen knop |

---

## Energiebesparing

Het systeem implementeert een 3-staps energiebesparingsmechanisme:

| Status | Helderheid | Trigger | Beschrijving |
|--------|------------|---------|--------------|
| Actief | 200 | Elke aanraking | Volle helderheid, alle updates actief |
| Gedimd | 50 | 15 sec inactiviteit | Verminderde helderheid, updates doorgaan |
| Slaap | 0 | 30 sec inactiviteit | Display uit, updates gepauzeerd |

Elke aanraking keert onmiddellijk terug naar Actieve status.

---

## Installatie

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

Installeer deze libraries via Arduino Library Manager of gebruik de `libraries/` map:

| Library | Versie | Doel |
|---------|--------|------|
| LVGL | 9.3.0 | Grafische gebruikersinterface |
| Arduino_GFX_Library | Nieuwste | Display driver (CO5300) |
| Arduino_DriveBus_Library | Nieuwste | I2C driver (FT3168 touch) |
| XPowersLib | Nieuwste | Power management (AXP2101) |
| ESP32-audioI2S | Nieuwste | Audio afspelen |
| TJpg_Decoder | Nieuwste | JPEG decodering |
| PNGdec | Nieuwste | PNG decodering |
| AnimatedGIF | Nieuwste | GIF decodering |

### Upload Stappen

1. Verbind de ESP32-S3 via USB
2. Selecteer de juiste COM poort
3. Houd BOOT knop ingedrukt, druk RESET, laat BOOT los (indien nodig)
4. Klik op Upload in Arduino IDE
5. Wacht tot upload voltooid is
6. Druk op RESET om de applicatie te starten

---

## Configuratie

### WiFi Instellingen

Bewerk `wifi_config.h`:

```c
#define WIFI_SSID "jouw_netwerk_naam"
#define WIFI_PASSWORD "jouw_wachtwoord"
#define NTP_SERVER "pool.ntp.org"
```

### LVGL Configuratie

Het bestand `lv_conf.h` bevat LVGL instellingen. Belangrijke instellingen:

```c
#define LV_COLOR_DEPTH 16          // RGB565
#define LV_HOR_RES_MAX 410         // Display breedte
#define LV_VER_RES_MAX 502         // Display hoogte
#define LV_USE_PSRAM 1             // Gebruik PSRAM voor buffers
```

---

## Projectstructuur

```
nieuw/
├── nieuw.ino               # Hoofdapplicatie (v2.3.1)
├── pin_config.h            # Hardware pin definities
├── wifi_config.h           # WiFi en NTP instellingen
├── lv_conf.h               # LVGL configuratie
│
├── ui.h / ui.c             # UI initialisatie
├── ui_helpers.h / ui_helpers.c
├── ui_events.h / ui_events.cpp
│
├── ui_time.h / ui_time.c       # Klok scherm
├── ui_steps.h / ui_steps.c     # Stappenteller scherm
├── ui_settings.h / ui_settings.c # Instellingen scherm
├── ui_files.h / ui_files.c     # Bestandsbrowser scherm
├── ui_player.h / ui_player.c   # Audio speler scherm
├── ui_audio.h / ui_audio.c     # Audio lijst scherm
├── ui_images.h / ui_images.c   # Afbeeldingen lijst scherm
├── ui_keyboard.h / ui_keyboard.c
├── ui_viewerfiles.h / ui_viewerfiles.c
├── ui_viewerimages.h / ui_viewerimages.c
│
├── ui_img_*.c              # Ingebedde afbeelding assets
│
└── libraries/              # Externe libraries
    ├── AnimatedGIF/
    ├── ESP32-audioI2S-master/
    ├── GFX_Library_for_Arduino/
    ├── lvgl/
    ├── PNGdec/
    ├── TJpg_Decoder/
    └── XPowersLib/
```

---

## Probleemoplossing

### SD Kaart Niet Gedetecteerd
- Zorg dat de kaart geformatteerd is als FAT32
- Controleer of de kaart goed is geplaatst
- Probeer een andere SD kaart (max 32GB aanbevolen)
- Verifieer SD kaart pins in `pin_config.h`

### Display Werkt Niet
- Controleer QSPI verbindingen
- Verifieer dat `lv_conf.h` instellingen overeenkomen met hardware
- Zorg dat PSRAM is ingeschakeld in board instellingen

### WiFi Maakt Geen Verbinding
- Verifieer SSID en wachtwoord in `wifi_config.h`
- Controleer WiFi signaalsterkte
- Zorg voor 2.4GHz netwerk (5GHz wordt niet ondersteund)

### Audio Speelt Niet Af
- Controleer of bestanden in `/audio/` map staan
- Verifieer ondersteund formaat (MP3, WAV, OGG, FLAC)
- Zorg dat ES8311 codec correct is aangesloten
- Controleer speaker verbinding met PA_CTRL pin

### Touch Reageert Niet
- Verifieer I2C verbindingen (SDA, SCL)
- Controleer touch interrupt pin (TP_INT)
- Zorg dat FT3168 adres 0x38 is

---

## Licentie

Copyright (c) 2026 JWP van Renen

---

## Links

- [Waveshare ESP32-S3 Touch AMOLED 2.06](https://www.waveshare.com/esp32-s3-touch-amoled-2.06.htm)
- [LVGL Documentatie](https://docs.lvgl.io/)
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/)
