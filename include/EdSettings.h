#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

// ESPressDuino: Laufzeit-Einstellungen im NVS (Namespace "espressduino").
// Solange nichts gespeichert ist, startet das Einrichtungs-Portal. Im Quellcode
// stehen keine Zugangsdaten mehr.

struct EdPool {
    String user;   // Duino-Coin-Benutzername (leer = Pool nicht genutzt)
    String key;    // Mining-Key (leer = keiner gesetzt)
};

struct EdSettings {
    String   rigName;    // Name des Miners (Wallet-Liste, Dashboard-Titel)
    String   hostname;   // Netzwerkname (mDNS, OTA)
    String   wifiSsid;
    String   wifiPass;
    EdPool   pools[2];   // [0] = Pool 1, [1] = Pool 2 (optional)
    uint16_t poolAltMin; // 0 = nur per Knopf wechseln, sonst alle N Minuten
};

extern EdSettings     g_ed;
extern volatile uint8_t g_edActivePool;   // 0 oder 1, wird zur Laufzeit umgeschaltet

bool        ed_settings_load();          // true = gespeicherte Konfiguration vorhanden
bool        ed_settings_configured();
bool        ed_settings_from_secrets();   // true = Werte kommen aus secrets.ini (use_secrets = 1)
bool        ed_settings_save(const EdSettings& s);
void        ed_settings_clear();
void        ed_settings_apply_json(JsonVariantConst j, EdSettings& s);
const char* ed_settings_validate(const EdSettings& s);
void        ed_settings_to_json(JsonObject o);  // ohne WLAN-Passwort und ohne Mining-Keys

bool          ed_has_pool2();
const String& ed_active_user();
const String& ed_active_key();
void          ed_switch_pool();             // zwischen Pool 1 und 2 wechseln
