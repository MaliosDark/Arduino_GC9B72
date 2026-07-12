/*
 * Arduino_GC9B72 - Arduino_GFX display driver for the GalaxyCore GC9B72,
 * the controller on 2.1" 360x360 round SPI TFT panels (silkscreen
 * "Driver IC: GC9B72", brand EstarDyn / "M17 2.1 TFT") that have no other
 * public Arduino driver.
 *
 * Copyright (c) 2026 MaliosDark
 * Released under the MIT License (see LICENSE).
 *
 * The GC9B72 register init sequence below is ported from the xboot project's
 * fb-gc9b72.c reference driver (the only known-good public GC9B72 init).
 */
#ifndef _ARDUINO_GC9B72_H_
#define _ARDUINO_GC9B72_H_

#include <Arduino_GFX_Library.h>

#define GC9B72_TFTWIDTH 360
#define GC9B72_TFTHEIGHT 360

#define GC9B72_RST_DELAY 120
#define GC9B72_SLPIN_DELAY 120
#define GC9B72_SLPOUT_DELAY 120

#define GC9B72_SWRESET 0x01
#define GC9B72_SLPIN 0x10
#define GC9B72_SLPOUT 0x11
#define GC9B72_INVOFF 0x20
#define GC9B72_INVON 0x21
#define GC9B72_DISPOFF 0x28
#define GC9B72_DISPON 0x29
#define GC9B72_CASET 0x2A
#define GC9B72_RASET 0x2B
#define GC9B72_RAMWR 0x2C
#define GC9B72_COLMOD 0x3A
#define GC9B72_MADCTL 0x36

#define GC9B72_MADCTL_MY 0x80
#define GC9B72_MADCTL_MX 0x40
#define GC9B72_MADCTL_MV 0x20
#define GC9B72_MADCTL_ML 0x10
#define GC9B72_MADCTL_BGR 0x08
#define GC9B72_MADCTL_RGB 0x00

// GalaxyCore GC9B72 initialization sequence, ported verbatim from the xboot
// project's fb-gc9b72.c reference driver (the only known-good GC9B72 init).
// This is the 360x360 round panel init; COLMOD 0x05 = 16-bit RGB565.
static const uint8_t gc9b72_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xFE,
    WRITE_COMMAND_8, 0xEF,
    WRITE_C8_D8, 0x80, 0x19,
    WRITE_C8_D8, 0x82, 0x09,
    WRITE_C8_D8, 0x83, 0x03,
    WRITE_C8_D8, 0x88, 0x00,
    WRITE_C8_D8, 0x89, 0x38,
    WRITE_C8_D8, 0x8A, 0x40,
    WRITE_C8_D8, 0x8B, 0x0A,
    WRITE_C8_D8, 0x8C, 0x00,
    WRITE_C8_D8, 0x81, 0xFF,
    WRITE_C8_D8, 0x84, 0xFF,
    WRITE_C8_D8, 0x85, 0xFF,
    WRITE_C8_D8, 0x86, 0xFF,
    WRITE_C8_D8, 0x87, 0xFF,
    WRITE_C8_D8, 0x8E, 0xFF,
    WRITE_C8_D8, 0x8F, 0xFF,
    WRITE_C8_D8, 0x98, 0x3E,
    WRITE_C8_D8, 0x99, 0x3E,
    WRITE_C8_D8, 0x7D, 0x72,

    WRITE_COMMAND_8, 0x70,
    WRITE_BYTES, 10, 0x02, 0x03, 0x03, 0x06, 0x03, 0x03, 0x09, 0x07, 0x09, 0x03,

    WRITE_COMMAND_8, 0x90,
    WRITE_BYTES, 4, 0x06, 0x06, 0x01, 0x01,

    WRITE_COMMAND_8, 0x93,
    WRITE_BYTES, 3, 0x02, 0xFF, 0x00,

    WRITE_C8_D8, 0xCB, 0x02,

    WRITE_COMMAND_8, 0xFB,
    WRITE_BYTES, 2, 0x00, 0x00,

    WRITE_C8_D8, 0xF6, 0xC0,

    WRITE_COMMAND_8, 0x6C,
    WRITE_BYTES, 7, 0x00, 0x00, 0x22, 0x00, 0xCC, 0x04, 0x58,

    WRITE_COMMAND_8, 0xAA,
    WRITE_BYTES, 2, 0x0B, 0x00,

    WRITE_C8_D8, 0xEC, 0x07,
    WRITE_C8_D8, 0xF9, 0x40,

    WRITE_COMMAND_8, 0xEB,
    WRITE_BYTES, 2, 0x01, 0x67,

    WRITE_COMMAND_8, 0x74,
    WRITE_BYTES, 6, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xB5,
    WRITE_BYTES, 3, 0x14, 0x14, 0x14,

    WRITE_COMMAND_8, 0x6E,
    WRITE_BYTES, 32,
    0x0B, 0x0B, 0x09, 0x09, 0x13, 0x13, 0x11, 0x11,
    0x16, 0x15, 0x01, 0x04, 0x00, 0x0D, 0x1D, 0x00,
    0x00, 0x1D, 0x0D, 0x00, 0x04, 0x08, 0x15, 0x16,
    0x12, 0x12, 0x14, 0x14, 0x0A, 0x0A, 0x0C, 0x0C,

    WRITE_COMMAND_8, 0x60,
    WRITE_BYTES, 4, 0x38, 0x1C, 0x13, 0x56,
    WRITE_COMMAND_8, 0x61,
    WRITE_BYTES, 4, 0xF8, 0x0A, 0x13, 0x56,
    WRITE_COMMAND_8, 0x62,
    WRITE_BYTES, 4, 0xF8, 0x0B, 0x13, 0x56,
    WRITE_COMMAND_8, 0x63,
    WRITE_BYTES, 4, 0x38, 0x1C, 0x13, 0x56,
    WRITE_COMMAND_8, 0x64,
    WRITE_BYTES, 6, 0x38, 0x20, 0x72, 0xF8, 0x13, 0x56,
    WRITE_COMMAND_8, 0x65,
    WRITE_BYTES, 6, 0x78, 0x1A, 0x70, 0x0B, 0x56, 0x13,
    WRITE_COMMAND_8, 0x66,
    WRITE_BYTES, 6, 0x38, 0x24, 0x72, 0xFC, 0x13, 0x56,
    WRITE_COMMAND_8, 0x68,
    WRITE_BYTES, 7, 0xB3, 0x08, 0x0E, 0x08, 0x0E, 0x0A, 0x0A,
    WRITE_COMMAND_8, 0x69,
    WRITE_BYTES, 7, 0xB3, 0x08, 0x0E, 0x08, 0x0E, 0x0A, 0x0A,

    WRITE_COMMAND_8, 0x6A,
    WRITE_BYTES, 2, 0x00, 0x00,

    WRITE_C8_D8, 0x3A, 0x05, // COLMOD: 16 bits / pixel (RGB565)
    WRITE_C8_D8, 0x36, 0x00, // MADCTL

    WRITE_COMMAND_8, 0x7C,
    WRITE_BYTES, 2, 0xB6, 0x29,

    WRITE_C8_D8, 0xAC, 0x40,
    WRITE_C8_D8, 0xC3, 0x1A,
    WRITE_C8_D8, 0xC4, 0x24,
    WRITE_C8_D8, 0xC9, 0x2F,

    WRITE_COMMAND_8, 0xF0,
    WRITE_BYTES, 6, 0x11, 0x17, 0x08, 0x06, 0x05, 0x38,
    WRITE_COMMAND_8, 0xF1,
    WRITE_BYTES, 6, 0x4D, 0x72, 0x72, 0x2D, 0x34, 0x8F,
    WRITE_COMMAND_8, 0xF2,
    WRITE_BYTES, 6, 0x11, 0x17, 0x08, 0x06, 0x05, 0x38,
    WRITE_COMMAND_8, 0xF3,
    WRITE_BYTES, 6, 0x4D, 0x72, 0x72, 0x2D, 0x34, 0x8F,

    WRITE_C8_D8, 0xB4, 0x0A,
    WRITE_C8_D8, 0x35, 0x00,

    WRITE_COMMAND_8, 0xFE,
    WRITE_COMMAND_8, 0xEE,

    WRITE_COMMAND_8, GC9B72_SLPOUT, // sleep out
    END_WRITE,
    DELAY, GC9B72_SLPOUT_DELAY,

    BEGIN_WRITE,
    WRITE_COMMAND_8, GC9B72_DISPON, // display on
    END_WRITE,
    DELAY, 20};

class Arduino_GC9B72 : public Arduino_TFT
{
public:
  Arduino_GC9B72(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = GC9B72_TFTWIDTH, int16_t h = GC9B72_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;
  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;
  void setRotation(uint8_t r) override;
  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

protected:
  void tftInit() override;

private:
};

#endif // _ARDUINO_GC9B72_H_
