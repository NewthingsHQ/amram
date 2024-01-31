// Snapshot Jan21a

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "amrled.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix(); 

void drawGlyph(const byte* glyph, int startDraw) {
    matrix.clear();

    for (int row = 0; row < 8; ++row) {
        byte glyphByte = pgm_read_byte_near(glyph + row);
        for (int col = 0; col < 3; ++col) {
            if (bitRead(glyphByte, col) == 1) {
                matrix.drawPixel(7 - row, 9 - (col + startDraw), LED_ON);
            }
        }
    }

    matrix.writeDisplay();
}
void setup() {
  matrix.begin(0x70); 
}

void loop() {
  drawGlyph(char5, 2);
  delay(1000);
}