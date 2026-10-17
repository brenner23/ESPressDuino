#pragma once
// ESPressDuino: Geraeteprofile. Auswahl per Build-Flag in platformio.ini:
//   -DBOARD_CYD      ESP32-2432S028 "Cheap Yellow Display" (ST7789 240x320, XPT2046-Touch, LDR)
//   -DBOARD_NMTV154  NM-TV-154 / NerdMiner TV (ST7789 240x240, kapazitiver Touch)
//   -DBOARD_C2_DUCO  MiniTV ESP32-C2 (ST7789 240x240, ein Kern, kein Touch)

#if defined(BOARD_ESP32_HEADLESS)
  // Klassisches ESP32-Board ohne Display (z. B. DevKit mit USB-C). Mining, Portal, Dashboard, Wallet.
  #define ED_BOARD           "ESP32 (no display)"
  #define ED_LED_PIN         2     // blaue LED: an = mint, blinkt = Einrichtungs-Portal
  #define ED_HAS_TFT         0
  #define ED_TFT_W           240
  #define ED_TFT_H           240
  #define ED_BL_ACTIVE_LOW   0
  #define ED_HAS_XPT         0
  #define ED_HAS_CAP_TOUCH   0
  #define ED_HAS_LDR         0
  #define ED_HAS_ROTATION    0
  #define ED_FIXED_ROTATION  0
  #define ED_TFT_INVERT_OFF  0
  #define ED_TFT_SPI_MODE    SPI_MODE0
  #define ED_BOOT_PIN        0
#elif defined(BOARD_NMTV154)
  #define ED_BOARD           "NM-TV-154"
  #define ED_HAS_TFT         1
  #define ED_TFT_W           240
  #define ED_TFT_H           240
  #define ED_BL_ACTIVE_LOW   1     // Backlight GPIO19, LOW = an
  #define ED_HAS_XPT         0
  #define ED_HAS_CAP_TOUCH   1     // GPIO32: Beruehren schaltet das Licht an/aus
  #define ED_HAS_LDR         0
  #define ED_HAS_ROTATION    0
  #define ED_FIXED_ROTATION  2
  #define ED_TFT_INVERT_OFF  0     // Panel braucht die Standard-Invertierung
  #define ED_TFT_SPI_MODE    SPI_MODE0
  #define ED_BOOT_PIN        0
#elif defined(BOARD_C2_DUCO)
  #define ED_BOARD           "MiniTV C2"
  #define ED_HAS_TFT         1
  #define ED_TFT_W           240
  #define ED_TFT_H           240
  #define ED_BL_ACTIVE_LOW   1     // Backlight GPIO18, LOW = an
  #define ED_HAS_XPT         0
  #define ED_HAS_CAP_TOUCH   0
  #define ED_HAS_LDR         0
  #define ED_HAS_ROTATION    0
  #define ED_FIXED_ROTATION  2
  #define ED_TFT_INVERT_OFF  0
  #define ED_TFT_SPI_MODE    SPI_MODE3  // CS liegt fest auf GND
  #define ED_BOOT_PIN        -1    // kein BOOT-Knopf erreichbar: Portal nur bei WLAN-Ausfall/Erststart
#else
  #ifndef BOARD_CYD
    #define BOARD_CYD
  #endif
  #define ED_BOARD           "CYD 2.8"
  #define ED_HAS_TFT         1
  #define ED_TFT_W           240
  #define ED_TFT_H           320
  #define ED_BL_ACTIVE_LOW   0     // Backlight GPIO21, HIGH = an
  #define ED_HAS_XPT         1
  #define ED_HAS_CAP_TOUCH   0
  #define ED_HAS_LDR         1
  #define ED_HAS_ROTATION    1
  #define ED_FIXED_ROTATION  1
  #define ED_TFT_INVERT_OFF  1     // CYD-Panel: Inversion aus, sonst Negativfarben
  #define ED_TFT_SPI_MODE    SPI_MODE0
  #define ED_BOOT_PIN        0
#endif

// Status-LED nur auf Boards ohne Display (bei CYD/NM-TV ist GPIO2 = TFT_DC!)
#ifndef ED_LED_PIN
  #define ED_LED_PIN -1
#endif

// Hochkant-/Quadrat-Layout ist 240x240; auf hoeheren Panels mittig.
#define ED_PORTRAIT_Y0  ((ED_TFT_H - 240) / 2)

// Backlight-Pegel und PWM unabhaengig von der Polaritaet
#define ED_BL_ON_LEVEL   (ED_BL_ACTIVE_LOW ? LOW : HIGH)
#define ED_BL_OFF_LEVEL  (ED_BL_ACTIVE_LOW ? HIGH : LOW)
#define ED_BL_PWM(duty)  (ED_BL_ACTIVE_LOW ? (255 - (int)(duty)) : (int)(duty))
