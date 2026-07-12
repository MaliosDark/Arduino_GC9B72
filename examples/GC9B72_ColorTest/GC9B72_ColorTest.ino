/*
 * Arduino_GC9B72 - color test.
 *
 * Drives a GC9B72 360x360 round SPI TFT (e.g. the EstarDyn/"M17" 2.1" round
 * panel) using the Arduino_GFX library. Shows a blue screen with white/red/
 * green squares, then cycles solid colors.
 *
 * Wiring (change to match yours):
 *   Panel  ->  ESP32-S3
 *   VCC    ->  3V3        (3.3V ONLY - never 5V)
 *   GND    ->  GND
 *   SCL    ->  GPIO 12
 *   SDA    ->  GPIO 11
 *   RST    ->  GPIO 8
 *   DC     ->  GPIO 9
 *   CS     ->  GPIO 10
 *   BL     ->  GPIO 14
 *   SDO/TE ->  not connected
 *
 * Needs: "GFX Library for Arduino" (moononournation) + this Arduino_GC9B72.
 */
#include <Arduino_GFX_Library.h>
#include <Arduino_GC9B72.h>

#define TFT_SCLK 12
#define TFT_MOSI 11
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define TFT_BL   14

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_GC9B72(bus, TFT_RST, 0 /*rotation*/, false /*IPS*/, 360, 360);

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  gfx->begin(20000000);   // GC9B72 handles fast SPI; lower it if wiring is long
  gfx->fillScreen(BLUE);
  gfx->fillRect(120, 120, 120, 120, WHITE);
  gfx->fillRect(10, 10, 50, 50, RED);
  gfx->fillRect(360 - 60, 360 - 60, 50, 50, GREEN);
  delay(2000);
}

void loop() {
  gfx->fillScreen(RED);   delay(1000);
  gfx->fillScreen(GREEN); delay(1000);
  gfx->fillScreen(BLUE);  delay(1000);
  gfx->setCursor(90, 170);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(4);
  gfx->fillScreen(BLACK);
  gfx->println("GC9B72");
  delay(2000);
}
