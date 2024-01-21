// Snapshot Jan22c

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <TimeLib.h>

#include "amrled.h"

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); // Matrix

const int sw1 = 16;
const int sw2 = 14; 
byte cnt = 0; 

void drawGlyph(const byte* glyph, int startDraw, bool clear = false) {
  if (clear) {
    matrix.clear();
  }

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

const byte* chars[10] = {char0, char1, char2, char3, char4, char5, char6, char7, char8, char9};

void drawDigit(int digit, int startDraw, bool clear = false) {
  if ((0 <= digit) && (digit <= 9)) {
    const byte* charId = chars[digit];
    drawGlyph(charId, startDraw, clear);
  } else {
    matrix.clear();
  }
}

void resetTime(bool doReset = false) {
  if (doReset) {
    setTime(8,0,0,1,1,2024);
  }
}

void setup() {
  matrix.begin(0x70);
  Serial.begin(115200);
  resetTime(false); // DEFAULT SHOULD BE FALSE; set to true if just plugged in
}

void loop() {
  time_t t = now();
  int h1 = hour(t) / 10;
  int h2 = hour(t) % 10;
  int m1 = minute(t) / 10;
  int m2 = minute(t) % 10;

  drawDigit(h1, 0, true); // Should only be true for this one
  drawDigit(h2, 4);
  drawDigit(m1, 9);
  drawDigit(m2, 13);

  delay(1000);
}