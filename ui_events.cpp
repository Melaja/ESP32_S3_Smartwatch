// ESP32-S3 Smartwatch - UI Events
// LVGL version: 9.3.0
//
// Event handlers voor alle schermen
// - Steps: Start/Stop/Wis/Save knoppen
// - Audio: lijst laden
// - Images: lijst laden
// - Files: lijst laden
// - Settings: helderheid, ssid, password, wifi, save
// - Player: play/pause/prev/next/repeat, volume
// - Keyboard: input afhandeling

#include "ui.h"
#include <Arduino.h>
#include <cstring>
#include <cstdio>

// ============================================================================
// EXTERNE VARIABELEN EN FUNCTIES (gedefinieerd in nieuw.ino)
// ============================================================================

// Stappen teller
extern bool stepCountingEnabled;
extern uint32_t stepCount;
extern void saveStepsToSD(void);

// Display
extern void setDisplayBrightness(uint8_t brightness);
extern void registerActivity(void);

// Lijsten vullen
extern void populateAudioList(void);
extern void populateImagesList(void);
extern void populateTextFilesList(void);

// Audio player
extern void startAudioPlayback(const char* filename);
extern void stopAudioPlayback(void);
extern void toggleAudioPlayback(void);
extern void playNextAudio(void);
extern void playPrevAudio(void);
extern void toggleRepeat(void);
extern void setAudioVolume(uint8_t volume);
extern bool audioPlaying;
extern bool audioRepeat;

// Settings
extern void saveWifiSettings(void);
extern void toggleWifi(void);
extern char pendingSSID[64];
extern char pendingPassword[64];
extern uint8_t keyboardTarget;  // 0=none, 1=ssid, 2=password
extern bool keyboardFirstKeystroke;  // true=wis veld bij eerste toetsaanslag

// ============================================================================
// STAPPEN SCHERM EVENT HANDLERS
// ============================================================================

// Start knop - begin met stappen tellen
void ui_event_start_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        stepCountingEnabled = true;
        // Groene kleur voor actief
        if(ui_stepscount) {
            lv_obj_set_style_text_color(ui_stepscount, lv_color_hex(0x00FF00), 0);
        }
        registerActivity();
    }
}

// Stop knop - stop met stappen tellen
void ui_event_stop_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        stepCountingEnabled = false;
        // Rode kleur voor gestopt
        if(ui_stepscount) {
            lv_obj_set_style_text_color(ui_stepscount, lv_color_hex(0xFF0000), 0);
        }
        registerActivity();
    }
}

// Wis knop - reset stappen naar 0
void ui_event_wis_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        stepCount = 0;
        if(ui_stepscount) {
            lv_label_set_text(ui_stepscount, "0");
        }
        registerActivity();
    }
}

// Save knop - sla stappen op naar SD kaart
void ui_event_save_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        saveStepsToSD();
        registerActivity();
    }
}

// ============================================================================
// AUDIO SCHERM EVENT HANDLER
// ============================================================================

void ui_event_audio_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        populateAudioList();
    }
}

// ============================================================================
// IMAGES SCHERM EVENT HANDLER
// ============================================================================

void ui_event_images_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        populateImagesList();
    }
}

// ============================================================================
// FILES SCHERM EVENT HANDLER
// ============================================================================

void ui_event_files_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        populateTextFilesList();
    }
}

// ============================================================================
// SETTINGS SCHERM EVENT HANDLERS
// ============================================================================

// Settings scherm geladen - toon opgeslagen WiFi credentials
void ui_event_settings_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        // Toon opgeslagen SSID (of placeholder)
        if(ui_ssid) {
            if(strlen(pendingSSID) > 0) {
                lv_label_set_text(ui_ssid, pendingSSID);
            } else {
                lv_label_set_text(ui_ssid, "Tik om SSID in te voeren");
            }
        }

        // Toon opgeslagen password (gemaskeerd of placeholder)
        if(ui_password) {
            if(strlen(pendingPassword) > 0) {
                // Toon sterretjes voor privacy
                char masked[64];
                int len = strlen(pendingPassword);
                if(len > 63) len = 63;
                for(int i = 0; i < len; i++) masked[i] = '*';
                masked[len] = '\0';
                lv_label_set_text(ui_password, masked);
            } else {
                lv_label_set_text(ui_password, "Tik om wachtwoord in te voeren");
            }
        }

        // Zet WiFi switch state op basis van huidige WiFi status
        extern lv_obj_t * ui_swwifi;
        extern bool wifiEnabled;
        if(ui_swwifi) {
            if(wifiEnabled) {
                lv_obj_add_state(ui_swwifi, LV_STATE_CHECKED);
            } else {
                lv_obj_remove_state(ui_swwifi, LV_STATE_CHECKED);
            }
        }

        // Initialiseer brightness slider met opgeslagen waarde
        extern lv_obj_t * ui_slider1;
        extern lv_obj_t * ui_percentage;
        extern uint8_t savedBrightness;
        if(ui_slider1) {
            lv_slider_set_value(ui_slider1, savedBrightness, LV_ANIM_OFF);
        }
        if(ui_percentage) {
            char buf[8];
            snprintf(buf, sizeof(buf), "%d%%", savedBrightness);
            lv_label_set_text(ui_percentage, buf);
        }

        registerActivity();
    }
}

// Helderheid slider gewijzigd
void ui_event_brightness_changed(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * slider = (lv_obj_t *)lv_event_get_target(e);
        int32_t value = lv_slider_get_value(slider);

        // Schaal van 0-100 naar 10-255
        uint8_t brightness = (uint8_t)(10 + (value * 245 / 100));
        setDisplayBrightness(brightness);

        // Sla brightness percentage op
        extern uint8_t savedBrightness;
        extern void saveBrightnessSettings();
        savedBrightness = (uint8_t)value;
        saveBrightnessSettings();

        // Update percentage label
        if(ui_percentage) {
            char buf[8];
            snprintf(buf, sizeof(buf), "%ld%%", value);
            lv_label_set_text(ui_percentage, buf);
        }

        registerActivity();
    }
}

// Extern keyboard textarea
extern lv_obj_t * ui_kbtextarea;

// Tijdelijke opslag voor keyboard tekst
static char keyboardInitText[64] = "";

// SSID label geklikt - open keyboard
void ui_event_ssid_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        keyboardTarget = 1;  // SSID
        keyboardFirstKeystroke = false;  // Tijdelijk uit tijdens init
        // Sla huidige SSID op voor later
        if(ui_ssid) {
            strncpy(keyboardInitText, lv_label_get_text(ui_ssid), sizeof(keyboardInitText) - 1);
        }
        _ui_screen_change(&ui_keyboard, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_keyboard_screen_init);
        // Na screen init, zet de tekst in textarea
        if(ui_kbtextarea) {
            lv_textarea_set_text(ui_kbtextarea, keyboardInitText);
            // NU activeren we de eerste toetsaanslag clearing
            keyboardFirstKeystroke = true;
        }
        registerActivity();
    }
}

// Password label geklikt - open keyboard
void ui_event_password_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        keyboardTarget = 2;  // Password
        keyboardFirstKeystroke = false;  // Tijdelijk uit tijdens init
        // Sla huidig password op voor later
        if(ui_password) {
            strncpy(keyboardInitText, lv_label_get_text(ui_password), sizeof(keyboardInitText) - 1);
        }
        _ui_screen_change(&ui_keyboard, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_keyboard_screen_init);
        // Na screen init, zet de tekst in textarea
        if(ui_kbtextarea) {
            lv_textarea_set_text(ui_kbtextarea, keyboardInitText);
            // NU activeren we de eerste toetsaanslag clearing
            keyboardFirstKeystroke = true;
        }
        registerActivity();
    }
}

// Wifi switch gewijzigd
void ui_event_wifi_toggled(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        toggleWifi();
        registerActivity();
    }
}

// Save knop settings - sla wifi instellingen op
void ui_event_settings_save_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        saveWifiSettings();
        registerActivity();
    }
}

// ============================================================================
// PLAYER SCHERM EVENT HANDLERS
// ============================================================================

// Player scherm geladen - initialiseer volume slider en play/pause buttons
void ui_event_player_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        extern lv_obj_t * ui_volume;
        extern lv_obj_t * ui_play;
        extern lv_obj_t * ui_pause;
        extern lv_obj_t * ui_playedsong;
        extern uint8_t audioVolume;
        extern bool audioPlaying;
        extern bool audioPaused;
        extern String currentAudioFile;

        // Initialiseer volume slider
        if(ui_volume) {
            // Lineaire mapping: van audioVolume (0-21) naar slider (0-100)
            int32_t sliderValue = (audioVolume * 100) / 21;
            if(sliderValue > 100) sliderValue = 100;
            lv_slider_set_value(ui_volume, sliderValue, LV_ANIM_OFF);
        }

        // Initialiseer play/pause buttons visibility
        if(audioPlaying && !audioPaused) {
            // Playing: verberg play, toon pause
            if(ui_play) lv_obj_add_flag(ui_play, LV_OBJ_FLAG_HIDDEN);
            if(ui_pause) lv_obj_remove_flag(ui_pause, LV_OBJ_FLAG_HIDDEN);
        } else {
            // Paused of gestopt: toon play, verberg pause
            if(ui_play) lv_obj_remove_flag(ui_play, LV_OBJ_FLAG_HIDDEN);
            if(ui_pause) lv_obj_add_flag(ui_pause, LV_OBJ_FLAG_HIDDEN);
        }

        // Update song label
        if(ui_playedsong && !currentAudioFile.isEmpty()) {
            String fn = currentAudioFile;
            int lastSlash = fn.lastIndexOf('/');
            if(lastSlash >= 0) fn = fn.substring(lastSlash + 1);
            lv_label_set_text(ui_playedsong, fn.c_str());
        }

        registerActivity();
    }
}

// Play knop
void ui_event_play_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        toggleAudioPlayback();
        registerActivity();
    }
}

// Pause knop
void ui_event_pause_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        toggleAudioPlayback();
        registerActivity();
    }
}

// Vorige knop
void ui_event_prev_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        playPrevAudio();
        registerActivity();
    }
}

// Volgende knop
void ui_event_next_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        playNextAudio();
        registerActivity();
    }
}

// Herhaal knop
void ui_event_repeat_clicked(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        toggleRepeat();
        registerActivity();
    }
}

// Volume slider
void ui_event_volume_changed(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * slider = (lv_obj_t *)lv_event_get_target(e);
        int32_t value = lv_slider_get_value(slider);

        // Map slider waarde (0-100) lineair naar audio volume (0-21)
        // 50% slider = 50% volume (ongeveer)
        uint8_t mappedVolume = (uint8_t)((value * 21) / 100);
        if (mappedVolume > 21) mappedVolume = 21;

        setAudioVolume(mappedVolume);
        registerActivity();
    }
}

// ============================================================================
// KEYBOARD SCHERM EVENT HANDLER
// ============================================================================

// Keyboard textarea value changed (toetsaanslag)
void ui_event_keyboard_value_changed(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        // Als dit de eerste toetsaanslag is, wis het veld eerst
        if(keyboardFirstKeystroke) {
            keyboardFirstKeystroke = false;  // Reset flag

            // Verwijder alle tekst behalve het laatst getypte karakter
            if(ui_kbtextarea) {
                const char * text = lv_textarea_get_text(ui_kbtextarea);
                if(text && strlen(text) > 0) {
                    // Haal laatste karakter op
                    char lastChar = text[strlen(text) - 1];
                    // Wis alles en zet alleen laatste karakter
                    lv_textarea_set_text(ui_kbtextarea, "");
                    lv_textarea_add_char(ui_kbtextarea, lastChar);
                }
            }
        }
        registerActivity();
    }
}

// Keyboard ready (vinkje geklikt)
void ui_event_keyboard_ready(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_READY) {
        // Haal getypte tekst op van textarea
        const char * text = "";
        if(ui_kbtextarea) {
            text = lv_textarea_get_text(ui_kbtextarea);
        }

        // Sla tekst op in tijdelijke buffer voordat screen wordt vernietigd
        static char savedText[64];
        strncpy(savedText, text, sizeof(savedText) - 1);
        savedText[sizeof(savedText) - 1] = '\0';

        uint8_t target = keyboardTarget;
        keyboardTarget = 0;

        // Ga terug naar settings (dit vernietigt keyboard screen)
        _ui_screen_change(&ui_settings, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_settings_screen_init);

        // Update het juiste veld na screen change
        if(target == 1) {
            if(ui_ssid) {
                lv_label_set_text(ui_ssid, savedText);
            }
            strncpy(pendingSSID, savedText, sizeof(pendingSSID) - 1);
        } else if(target == 2) {
            if(ui_password) {
                lv_label_set_text(ui_password, savedText);
            }
            strncpy(pendingPassword, savedText, sizeof(pendingPassword) - 1);
        }

        registerActivity();
    }
}

// Keyboard gesture (verticale swipe = terug zonder opslaan)
void ui_event_keyboard_gesture(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

        // Verticale swipe (omhoog of omlaag) = terug zonder opslaan
        if(dir == LV_DIR_TOP || dir == LV_DIR_BOTTOM) {
            Serial.println("[Keyboard] Verticale swipe gedetecteerd - terug zonder opslaan");

            // Reset keyboard state
            keyboardTarget = 0;
            keyboardFirstKeystroke = false;

            // Ga terug naar settings zonder wijzigingen op te slaan
            _ui_screen_change(&ui_settings, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_settings_screen_init);
            lv_indev_wait_release(lv_indev_active());
            registerActivity();
        }
    }
}

// ============================================================================
// VIEWER SCHERMEN EVENT HANDLERS
// ============================================================================

// ViewerImages scherm geladen
void ui_event_viewerimages_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        // Zet flag om GFX rendering uit te stellen tot NA lv_timer_handler()
        extern bool pendingImageRender;
        pendingImageRender = true;
        registerActivity();
    }
}

// ViewerFiles scherm geladen
void ui_event_viewerfiles_loaded(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        // Zet flag om GFX rendering uit te stellen tot NA lv_timer_handler()
        extern bool pendingTextRender;
        pendingTextRender = true;
        registerActivity();
    }
}
