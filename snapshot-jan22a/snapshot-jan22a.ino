// Snapshot Jan22a

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Timelib.h>

#include "amrled.h"

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); 

void draw(const byte* glyph, int startDraw) {
  matrix.clear();

  for (int row = 0; row < 8; ++row) {
    byte glyphByte = pgm_read_byte_near(glyph + row);
    for (int col = 0; col < 3; ++col) {
      if (bitRead(glyphByte, col) == 1) {
        matrix.drawPixel(row, (col + 13 - startDraw), LED_ON);
      }
    }
  }

  matrix.writeDisplay();
}

void setup() {
  matrix.begin(0x70);
  Serial.begin(9600);
  setTime(08, 00, 00, 01, 01, 2024);
}

void loop() {
  time_t t = now();
  int h1 = hour(t) / 10;
  int h2 = hour(t) % 10;
  int m1 = minute(t) / 10;
  int m2 = minute(t) % 10;

  draw(h1, 0);
  draw(h2, 3);
  draw(m1, 9);
  draw(m2, 12);
}