#include "EdSettings.h"
#include <Preferences.h>

// Zugangsdaten wahlweise aus secrets.ini (use_secrets = 1, siehe platformio.ini)
#ifndef ED_USE_SECRETS
  #define ED_USE_SECRETS 0
#endif
#ifndef ED_S_RIG
  #define ED_S_RIG   "ESPressDuino"
  #define ED_S_HOST  "espressduino"
  #define ED_S_SSID  ""
  #define ED_S_WPASS ""
  #define ED_S_USER1 ""
  #define ED_S_KEY1  ""
  #define ED_S_USER2 ""
  #define ED_S_KEY2  ""
#endif

EdSettings       g_ed;
volatile uint8_t g_edActivePool = 0;

static const char* NVS_NS        = "espressduino";
static bool        s_configured  = false;

bool ed_settings_load()
{
    Preferences p;
    p.begin(NVS_NS, true);
    s_configured        = p.getBool("cfg", false);
    g_ed.rigName        = p.getString("rig", "ESPressDuino");
    g_ed.hostname       = p.getString("host", "espressduino");
    g_ed.wifiSsid       = p.getString("ssid", "");
    g_ed.wifiPass       = p.getString("wpass", "");
    g_ed.pools[0].user  = p.getString("p1user", "");
    g_ed.pools[0].key   = p.getString("p1key", "");
    g_ed.pools[1].user  = p.getString("p2user", "");
    g_ed.pools[1].key   = p.getString("p2key", "");
    g_ed.poolAltMin     = p.getUShort("paltm", 0);
    p.end();
    g_edActivePool = 0;
#if ED_USE_SECRETS
    // secrets.ini hat Vorrang: kein Portal, Werte bei jedem Start aus der Firmware
    if (strlen(ED_S_SSID) && strlen(ED_S_USER1)) {
        g_ed.rigName       = strlen(ED_S_RIG)  ? ED_S_RIG  : "ESPressDuino";
        g_ed.hostname      = strlen(ED_S_HOST) ? ED_S_HOST : "espressduino";
        g_ed.wifiSsid      = ED_S_SSID;
        g_ed.wifiPass      = ED_S_WPASS;
        g_ed.pools[0].user = ED_S_USER1;
        g_ed.pools[0].key  = ED_S_KEY1;
        g_ed.pools[1].user = ED_S_USER2;
        g_ed.pools[1].key  = strlen(ED_S_USER2) ? ED_S_KEY2 : "";
        s_configured = true;
    }
#endif
    return s_configured;
}

bool ed_settings_from_secrets()
{
#if ED_USE_SECRETS
    return strlen(ED_S_SSID) && strlen(ED_S_USER1);
#else
    return false;
#endif
}

bool ed_settings_configured() { return s_configured; }

bool ed_settings_save(const EdSettings& s)
{
    Preferences p;
    if (!p.begin(NVS_NS, false)) return false;
    p.putString("rig", s.rigName);
    p.putString("host", s.hostname);
    p.putString("ssid", s.wifiSsid);
    p.putString("wpass", s.wifiPass);
    p.putString("p1user", s.pools[0].user);
    p.putString("p1key", s.pools[0].key);
    p.putString("p2user", s.pools[1].user);
    p.putString("p2key", s.pools[1].key);
    p.putUShort("paltm", s.poolAltMin);
    p.putBool("cfg", true);
    p.end();
    g_ed = s;
    s_configured = true;
    return true;
}

void ed_settings_clear()
{
    Preferences p;
    if (p.begin(NVS_NS, false)) { p.clear(); p.end(); }
    s_configured = false;
}

static void assign_trimmed(String& dst, JsonVariantConst v)
{
    if (v.isNull()) return;
    dst = v.as<const char*>() ? v.as<const char*>() : "";
    dst.trim();
}

// Leeres WLAN-Passwort bzw. "keyKeep" = bisherigen Wert behalten
void ed_settings_apply_json(JsonVariantConst j, EdSettings& s)
{
    assign_trimmed(s.rigName, j["rigName"]);
    assign_trimmed(s.hostname, j["hostname"]);
    assign_trimmed(s.wifiSsid, j["wifiSsid"]);
    if (!j["poolAltMin"].isNull()) s.poolAltMin = j["poolAltMin"].as<uint16_t>();
    const char* wpass = j["wifiPass"] | "";
    if (wpass[0]) s.wifiPass = wpass;
    for (int i = 0; i < 2; i++) {
        JsonVariantConst o = j["pools"][i];
        if (o.isNull()) continue;
        assign_trimmed(s.pools[i].user, o["user"]);
        if (!(o["keyKeep"] | false)) assign_trimmed(s.pools[i].key, o["key"]);
        if (s.pools[i].user.isEmpty()) s.pools[i].key = "";
    }
}

static bool valid_host(const String& h)
{
    if (h.isEmpty() || h.length() > 31) return false;
    for (char c : h) if (!isalnum((unsigned char)c) && c != '-') return false;
    return true;
}

const char* ed_settings_validate(const EdSettings& s)
{
    if (s.rigName.isEmpty())           return "Miner name missing";
    if (!valid_host(s.hostname))       return "Hostname: only letters, digits and '-'";
    if (s.wifiSsid.isEmpty())          return "Wi-Fi name (SSID) missing";
    if (s.pools[0].user.isEmpty())     return "Duino-Coin username (Pool 1) missing";
    if (s.poolAltMin > 1440)           return "Switching interval: 0 to 1440 minutes";
    return nullptr;
}

void ed_settings_to_json(JsonObject o)
{
    o["rigName"]     = g_ed.rigName;
    o["hostname"]    = g_ed.hostname;
    o["wifiSsid"]    = g_ed.wifiSsid;
    o["wifiPassSet"] = g_ed.wifiPass.length() > 0;
    o["configured"]  = s_configured;
    o["fromSecrets"] = ed_settings_from_secrets();
    o["poolAltMin"]  = g_ed.poolAltMin;
    JsonArray pools = o["pools"].to<JsonArray>();
    for (const EdPool& pc : g_ed.pools) {
        JsonObject po = pools.add<JsonObject>();
        po["user"]   = pc.user;
        po["keySet"] = pc.key.length() > 0;
    }
}

bool ed_has_pool2() { return g_ed.pools[1].user.length() > 0; }

const String& ed_active_user() { return g_ed.pools[(g_edActivePool && ed_has_pool2()) ? 1 : 0].user; }
const String& ed_active_key()  { return g_ed.pools[(g_edActivePool && ed_has_pool2()) ? 1 : 0].key; }

void ed_switch_pool()
{
    if (!ed_has_pool2()) { g_edActivePool = 0; return; }
    g_edActivePool = g_edActivePool ? 0 : 1;
}
