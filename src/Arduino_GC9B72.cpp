/*
 * Arduino_GC9B72 - Arduino_GFX display driver for the GalaxyCore GC9B72
 * (2.1" 360x360 round SPI TFT).
 *
 * Copyright (c) 2026 MaliosDark. MIT License (see LICENSE).
 */
#include "Arduino_GC9B72.h"

Arduino_GC9B72::Arduino_GC9B72(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

bool Arduino_GC9B72::begin(int32_t speed)
{
  _override_datamode = SPI_MODE0; // GC9B72 uses SPI mode 0
  return Arduino_TFT::begin(speed);
}

void Arduino_GC9B72::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW) || (y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(GC9B72_CASET, x + _xStart, x + w - 1 + _xStart);
    _bus->writeC8D16D16(GC9B72_RASET, y + _yStart, y + h - 1 + _yStart);

    _currentX = x;
    _currentY = y;
    _currentW = w;
    _currentH = h;
  }

  _bus->writeCommand(GC9B72_RAMWR); // write to RAM
}

void Arduino_GC9B72::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = GC9B72_MADCTL_MV | GC9B72_MADCTL_MX | GC9B72_MADCTL_RGB;
    break;
  case 2:
    r = GC9B72_MADCTL_MX | GC9B72_MADCTL_MY | GC9B72_MADCTL_RGB;
    break;
  case 3:
    r = GC9B72_MADCTL_MV | GC9B72_MADCTL_MY | GC9B72_MADCTL_RGB;
    break;
  default: // case 0:
    r = GC9B72_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeCommand(GC9B72_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_GC9B72::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? GC9B72_INVOFF : GC9B72_INVON) : (i ? GC9B72_INVON : GC9B72_INVOFF));
}

void Arduino_GC9B72::displayOn(void)
{
  _bus->sendCommand(GC9B72_SLPOUT);
  delay(GC9B72_SLPOUT_DELAY);
}

void Arduino_GC9B72::displayOff(void)
{
  _bus->sendCommand(GC9B72_SLPIN);
  delay(GC9B72_SLPIN_DELAY);
}

void Arduino_GC9B72::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(GC9B72_RST_DELAY);
    digitalWrite(_rst, LOW);
    delay(GC9B72_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(GC9B72_RST_DELAY);
  }

  _bus->batchOperation(gc9b72_init_operations, sizeof(gc9b72_init_operations));
}
