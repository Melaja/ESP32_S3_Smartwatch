/*
 * ============================================================================
 * WiFi Configuratie
 * ============================================================================
 *
 * Dit bestand bevat de WiFi en NTP instellingen.
 * BELANGRIJK: Voeg dit bestand toe aan .gitignore om te voorkomen dat
 * gevoelige gegevens per ongeluk gedeeld worden!
 *
 * ============================================================================
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// WiFi netwerk instellingen
const char *WIFI_SSID     = "ReLo";
const char *WIFI_PASSWORD = "wnjpre6m8ecZ";

// NTP server voor tijdsynchronisatie
const char *NTP_SERVER    = "be.pool.ntp.org";

// Tijdzone instelling voor Brussel/België
// Format: standaard offset, DST naam, DST start, DST end
const char *TZ_INFO       = "CET-1CEST,M3.5.0,M10.5.0/3";

#endif // WIFI_CONFIG_H
