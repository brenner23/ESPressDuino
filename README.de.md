# ☕ ESPressDuino

🇬🇧 [English](README.md) · 🇩🇪 **Deutsch**

Ein **Duino-Coin-Miner für ESP32-Displays**, mit dem Web-Dashboard und dem Einrichtungs-Portal von
[ESPressMiner32](https://github.com/brenner23/ESPressMiner32): WLAN-Einrichtung vom Handy, zwei Pools
(Duino-Coin-Konten), Hashrate-Verlauf und die Wallet auf einen Blick. Keine Zugangsdaten im Quellcode.

🔗 **Projektseite:** https://github.com/brenner23/ESPressDuino

> ⚠️ **Hobbyprojekt.** Duino-Coin ist eine Spaß-Währung für kleine Geräte. Es geht ums Lernen und Basteln.

<img src="docs/espressduino_cyd.webp" height="300" alt="ESPressDuino auf einem CYD"> <img src="docs/espressduino_nmtv154.png" height="300" alt="ESPressDuino auf einem NM-TV-154"> <img src="docs/espressduino_esp32_devkit.webp" height="300" alt="ESPressDuino auf einem einfachen ESP32-DevKit">

**Web-Dashboard** (links) und **Einstellungen** (rechts):

<img src="docs/web_dashboard.png" width="560" alt="Dashboard"> <img src="docs/web_settings.png" width="250" alt="Settings">

## Unterstützte Geräte

| Gerät | Display | Stand |
|---|---|---|
| **CYD** ESP32-2432S028 (2 USB-Buchsen) | ST7789 320×240, Touch, Lichtsensor | ✅ Version 5.0, getestet (~190 kH/s, 2 Kerne) |
| **NM-TV-154** (NerdMiner TV) | ST7789 240×240, Touchfläche | ✅ Version 5.0, getestet (~190 kH/s, 2 Kerne) |
| **ESP32 ohne Display** (DevKit, USB-C / Micro-USB) | – (blaue LED: an = mint, blinkt = Einrichtungs-Portal) | ✅ Version 5.0, getestet (~190 kH/s, 2 Kerne), auch auf Chip-**Revision 1.0** |
| MiniTV ESP32-C2 | ST7789 240×240, ein Kern | 🔧 wird eingebaut (bisher `duinocoin-WeatheTV-ESP32-C2`) |

## Funktionen

- Mining auf **beiden ESP32-Kernen** (~90–100 kH/s pro Kern auf dem CYD)
  Eigene Messung auf demselben CYD: Der unveränderte offizielle Miner kam nur auf etwa **160 kH/s** – ESPressDuino auf etwa **190 kH/s** (+~19 %).
  > **Hinweis zur Hashrate:** Die ~190 kH/s auf dem CYD sind **ohne offenes Dashboard** gemessen. Eine offene Seite
  > (egal welcher Browser-Tab) fragt den Miner alle 5 s ab; das läuft auf Kern 0, der auch mint, und kostet etwa 1–2 %.
  > Die Seite ist sparsam gebaut (die Wallet-Tabelle wird nur bei Änderung geschickt), aber nicht kostenlos.
- **Einrichtungs-Portal:** WLAN `ESPressDuino-XXXX`, WLAN-Scan, alles im Gerät gespeichert (NVS)
- **Pool 1 / Pool 2:** jeweils Duino-Coin-**Benutzername + Mining-Key**; Wechsel per Knopf oder automatisch alle N Minuten
- **Web-Dashboard** unter `http://espressduino.local/`: Hashrate-Verlauf (10 min), Shares, Guthaben, Laufzeit
- **Wallet-Ansicht** wie die offizielle Wallet-Seite: Guthaben, USD-Wert, Geräte/Threads/Gesamt-Hashrate und eine Zeile
  pro Gerät (alle Kerne eines Geräts zusammengefasst, z. B. `CYD47-Test • 2 threads`), Link zur öffentlichen Wallet-Seite
- **Display-Einstellungen** im Browser: Helligkeit (Live-Vorschau), Auto-Licht (Lichtsensor), Drehung 0/90/180/270°
- **BOOT-Knopf 5 s halten** (NM-TV: Touchfläche 10 s) → Einrichtungs-Portal öffnet sich (Einstellungen bleiben, Miner läuft weiter)
- **WLAN-Wächter:** verbindet sich selbst neu, Neustart nach 5 min ohne WLAN
- Wahlweise: alles aus `secrets.ini` statt Portal (siehe unten)
- Nur **freie Schriften** (Archivo Narrow Bold, SIL OFL 1.1), Lizenztexte in `include/Fonts/`

## 🐞 Der Weißer-Bildschirm-Fehler – das Display war unschuldig

Die Basis dieses Projekts (der offizielle Duino-Coin-ESP32-Miner, für den CYD angepasst) hat den Bildschirm nach
einiger Zeit immer wieder **weiß** gemacht oder Datenmüll gezeigt. Verdächtigt wurden Display, SPI-Bus und
TFT-Bibliothek. **Es war nichts davon** – und das herauszufinden war eine lange Fummelarbeit.

Der offizielle Miner lässt bei jedem Share die „eingebaute LED“ blinken und legt sie für den ESP32 auf
**`LED_BUILTIN 2` (GPIO 2)**. Beim CYD (und beim NM-TV-154) ist **GPIO 2 aber `TFT_DC`** – die
Daten/Befehl-Leitung des Displays. (Beim CYD würde man als Status-LED die RGB-LED auf der Rückseite erwarten – der Code hat aber GPIO 2 geschaltet.)
Jeder Share hat diese Leitung umgeschaltet, oft mitten in einer
Display-Übertragung. Das Display bekam Pixeldaten als Befehle und umgekehrt → weißer Bildschirm.

**Lösung (FIX03):** Alle LED-Funktionen des Originals sind abgeschaltet (`FIX03_001 LED DISABLED` im Code).
GPIO 2 gehört jetzt nur noch dem Display. Seitdem: kein weißer Bildschirm mehr.

Zwei weitere Fehler aus dem Original wurden dabei behoben:

- **FIX14 – SHA-1-Warmup las über das Ende hinaus:** `DSHA1::warmup()` hat 20 Bytes aus einem Text gehasht, der nur
  15 Bytes lang ist (inkl. NUL) – also 5 Bytes hinter dem Objektende gelesen. Jetzt fester 20-Byte-Puffer.
- **V11 – Watchdog-Timer von beiden Kernen geteilt:** `max_micros_elapsed()` hatte einen `static` Zeitstempel, damit
  teilten sich Kern 0 und Kern 1 einen Timer und nahmen sich gegenseitig die Watchdog-Pause weg. Jetzt hat jeder
  Mining-Job seinen eigenen Timer.

Wer selbst eine Duino-Coin-Firmware für ein Display-Board baut: **prüfen, auf welchem Pin `LED_BUILTIN` liegt.**

## Ersteinrichtung

Solange nichts gespeichert ist, mint der Miner **nicht**, sondern öffnet das Einrichtungs-Portal:

1. Mit dem WLAN **`ESPressDuino-XXXX`** verbinden (Passwort `espressduino`). Die Seite öffnet sich von selbst,
   sonst `http://192.168.4.1/` aufrufen.
2. Miner-Namen, WLAN (🔍 Scan) sowie **Duino-Coin-Benutzername** und **Mining-Key** (Pool 1) eintragen.
   Pool 2 ist optional.
3. **Save & restart.** Der Miner verbindet sich und legt los. Dashboard: `http://<hostname>.local/`.

Das Portal öffnet sich außerdem, wenn das gespeicherte WLAN 30 s nicht erreichbar ist (der Miner versucht es im
Hintergrund weiter), und wenn man den **BOOT-Knopf 5 s hält** (das Display zählt herunter).

### Alternative: `secrets.ini` statt Portal

`secrets.example.ini` nach `secrets.ini` kopieren, ausfüllen und **`use_secrets = 1`** setzen. Die Firmware nimmt
WLAN, Konten, Miner-Namen und Hostnamen dann von dort und startet ohne Portal. Im Browser sind die Einstellungen in
diesem Modus nur lesbar (Display und Wallet gehen weiter). `secrets.ini` steht in `.gitignore` und kommt nie ins
Repository. Ohne `secrets.ini` (oder mit `use_secrets = 0`) wird das Portal verwendet.

In `secrets.ini` stehen außerdem das **OTA-Passwort** und die IP-Adresse für die `*_ota`-Umgebungen (optional).

## Bauen & flashen (VS Code + PlatformIO)

1. Ordner in VS Code mit PlatformIO öffnen.
2. Umgebung wählen (`cyd_usb`) und **→ Upload** klicken.
3. Serieller Monitor: 500000 Baud.

Es gibt bewusst keine fertigen Firmware-Dateien: Geflasht wird, was man im Quellcode lesen kann.

## Danksagung & Lizenz

Basiert auf dem offiziellen ESP32-Miner von [Duino-Coin](https://github.com/revoxhere/duino-coin) (MIT).
Web-Dashboard und Einrichtungs-Portal übernommen von [ESPressMiner32](https://github.com/brenner23/ESPressMiner32).
Schriften: Archivo Narrow Bold (SIL OFL 1.1). Entwickelt mit Unterstützung von Claude.

MIT – siehe [LICENSE](LICENSE).
