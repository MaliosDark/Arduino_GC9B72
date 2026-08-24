# Arduino_GC9B72

An **Arduino_GFX** display driver for the **GalaxyCore GC9B72**, the controller
on **2.1-inch 360×360 round SPI TFT panels** that ship with **no public driver**
anywhere. If you bought one of these round color displays and only got a blank
screen, garbage, or noise with every "similar" driver (GC9A01, GC9C01, ST7789,
ST77916), this is why: the GC9B72 needs its own specific initialization
sequence, and until now that sequence wasn't available in any Arduino library.

This library adds a proper `Arduino_GC9B72` class to
[GFX Library for Arduino](https://github.com/moononournation/Arduino_GFX) so the
panel just works: solid colors, shapes, text, the full GFX API.

## The panel this is for

These are sold on AliExpress/Amazon under generic names, but the way to
identify yours for certain is the **silkscreen printed on the back of the panel
PCB**, which reads:

```
VER:TFT 2.1 0_10
Driver IC: GC9B72
Resolution: 360x360
```

- **Controller IC:** GalaxyCore **GC9B72**
- **Size / resolution:** 2.1" round, **360 × 360**
- **Brand seen on these:** **EstarDyn** (also listed as "GoldenMorning",
  "M17 2.1 TFT YuanXing", or just "2.1 inch round display")
- **Interface:** 4-wire SPI (SCL, SDA, DC, CS, RST) + backlight
- **Power:** **3.3 V logic, do NOT feed 5 V to VCC**

If the back of your board says `GC9B72`, this is your driver.

## Panel pinout

The 10-pad breakout exposes: `GND VCC SCL SDA RST DC CS BL SDO TE`.
Only 8 are needed; `SDO` (read-back) and `TE` (frame-sync) are left unconnected.

| Panel pad | Meaning              | Notes                              |
|-----------|----------------------|------------------------------------|
| VCC       | Power                | **3.3 V only** (no onboard reg)    |
| GND       | Ground               |                                    |
| SCL       | SPI clock            | -> your `TFT_SCLK`                 |
| SDA       | SPI data (MOSI)      | -> your `TFT_MOSI`                 |
| RST       | Reset                | -> your `TFT_RST`                  |
| DC        | Data / command       | -> your `TFT_DC`                   |
| CS        | Chip select          | -> your `TFT_CS`                   |
| BL        | Backlight            | -> a GPIO (drive HIGH to turn on)  |
| SDO       | Data out (read)      | leave unconnected                  |
| TE        | Tearing effect       | leave unconnected                  |

## Wiring used in the bundled example (ESP32-S3)

| Panel | ESP32-S3 |
|-------|----------|
| VCC   | 3V3      |
| GND   | GND      |
| SCL   | GPIO 12  |
| SDA   | GPIO 11  |
| RST   | GPIO 8   |
| DC    | GPIO 9   |
| CS    | GPIO 10  |
| BL    | GPIO 14  |

Change the `#define`s in the sketch to match your own wiring.

## Install

1. Install **GFX Library for Arduino** (Library Manager → search
   "GFX Library for Arduino" by moononournation). This is a required dependency.
2. Install this library (copy the folder into your Arduino `libraries/`, or add
   it as a ZIP).

## Usage

```cpp
#include <Arduino_GFX_Library.h>
#include <Arduino_GC9B72.h>

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_GC9B72(bus, TFT_RST, 0 /*rotation*/, false /*IPS*/, 360, 360);

void setup() {
  pinMode(TFT_BL, OUTPUT);      // turn the backlight on, or the screen stays black
  digitalWrite(TFT_BL, HIGH);

  gfx->begin();                 // or gfx->begin(spiHz) to set the SPI clock
  gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(4);
  gfx->setCursor(90, 170);
  gfx->println("GC9B72");
}
```

If the screen stays black, check the backlight first: `TFT_BL` has to be driven
HIGH. Jumper the `BL` pad to 3V3 to test the panel independently of any code.

From here it's the standard Arduino_GFX API: `drawLine`, `fillRect`,
`drawCircle`, fonts, `Arduino_Canvas`, etc.

See `examples/GC9B72_ColorTest`.

## Notes

- SPI mode is **0** (set automatically). The controller handles fast SPI
  (tested up to ~20 MHz on short leads); if you use long/breadboard jumpers and
  see speckle, lower the clock via `gfx->begin(<hz>)`.
- Color format is 16-bit **RGB565**, MADCTL RGB order (rotation 0). Rotations
  1-3 are supported via `setRotation()`.
- The panel has a hardware reset line; wire `RST` for reliable startup.

## Credits

The GC9B72 initialization register sequence is ported from the
[xboot](https://github.com/xboot/xstar) project's `fb-gc9b72.c` reference
driver, the only known-good public GC9B72 init. This library repackages it as
an Arduino_GFX display class.

## License

MIT. See `LICENSE`.
