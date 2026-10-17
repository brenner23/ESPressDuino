#ifndef PINS_H
#define PINS_H

#include "Board.h"

#if defined(BOARD_ESP32_HEADLESS)
  // Kein Display: Pins nur als Platzhalter, es wird nie etwas angesteuert.
  #define TFT_MOSI       -1
  #define TFT_MISO       -1
  #define TFT_SCK        -1
  #define TFT_CS         -1
  #define TFT_DC         -1
  #define TFT_RST        -1
  #define TFT_BACKLIGHT  -1
  #define SCREEN_W       240
  #define SCREEN_H       240

#elif defined(BOARD_NMTV154)
  // NM-TV-154 - ST7789 240x240
  #define TFT_MOSI       13
  #define TFT_MISO       12
  #define TFT_SCK        14
  #define TFT_CS         15
  #define TFT_DC          2      // GPIO2 = Data/Command (darum keine LED auf GPIO2!)
  #define TFT_RST         4
  #define TFT_EN_PIN     21      // Display-Versorgung, LOW = an
  #define TFT_BACKLIGHT  19      // LOW-aktiv
  #define SCREEN_W       240
  #define SCREEN_H       240
  #define TOUCH_CAP_PIN  32      // kapazitive Taste
  #define TOUCH_THRESH   90
  #define TOUCH_CONFIRM_MS 80

#elif defined(BOARD_C2_DUCO)
  // MiniTV ESP32-C2 - ST7789V 240x240, CS fest auf GND, kein MISO
  #define TFT_SCK         4
  #define TFT_MOSI        6
  #define TFT_MISO       -1
  #define TFT_CS         -1
  #define TFT_DC          5
  #define TFT_RST         1
  #define TFT_BACKLIGHT  18      // LOW-aktiv
  #define SCREEN_W       240
  #define SCREEN_H       240

#else
  // CYD - ST7789 320x240 (FIX01; Pinbelegung aus FIX00 beibehalten)
  #define TFT_MOSI       13
  #define TFT_MISO       12
  #define TFT_SCK        14
  #define TFT_CS         15
  #define TFT_DC          2      // GPIO2 = Data/Command (darum keine LED auf GPIO2!)
  #define TFT_RST        -1
  #define TFT_BACKLIGHT  21      // CYD: HIGH-aktiv
  #define SCREEN_W       320
  #define SCREEN_H       240

  // XPT2046 Touch - eigener SPI-Bus (funktionierendes CYD-Projekt)
  #define TOUCH_SCK      25
  #define TOUCH_MOSI     32
  #define TOUCH_MISO     39
  #define TOUCH_CS       33
  #define TOUCH_IRQ      36

  // Onboard CdS/LDR des CYD
  #define LDR_PIN        34
#endif

#endif
