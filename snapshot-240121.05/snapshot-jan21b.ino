// Snapshot Jan21b

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "amrled.h"

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); 

void drawGlyph(const byte* glyph, int startDraw) {
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
}

void loop() {
  int i;

  for (i = 0; i < 16; i++) {
    drawGlyph(char5, i);
    delay(300);
  }
}