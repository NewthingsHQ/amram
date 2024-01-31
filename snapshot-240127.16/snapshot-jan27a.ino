// Snapshot Jan27a

/*
  
  DEVELOPER CORNER

  ~~~ TODO LIST ~~~

  / URGENT: Fix Time Adjust Mechanism
  X MEDIUM: Figure out how to actually add new medicine
  - OTHERS: -

  ~~~ LEGEND ~~~

  / : Done
  ? : Done, but not tested
  O : In progress
  X : Not done
  - : N/A

*/

// --- INITIALIZE --- //

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <TimeLib.h>
#include <RTClib.h>

#include <iostream>
using namespace std;

#include "amrled.h"

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); // Matrix

const int sw1 = 16;
const int sw2 = 14;
byte cnt = 0;
byte mde = 0;

time_t t = now();
int h1 = hour(t) / 10;
int h2 = hour(t) % 10;
int m1 = minute(t) / 10;
int m2 = minute(t) % 10;

long pressTime = 0;

// --- DRAWING FUNCTIONS --- //

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


// --- TIME FUNCTIONS --- //

string addZeros(string s, int length) {
  while (s.length() < length) {
    s = "0" + s;
  }
  return s;
}


void resetTime(bool doReset = false) {
  if (doReset) {
    setTime(8,0,0,1,1,2024);
  }
}

// --- SETUP --- //

void setup() {
  matrix.begin(0x70);
  Serial.begin(115200);
  resetTime(false); // DEFAULT SHOULD BE FALSE; set to true if just plugged in
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
}

int val1 = digitalRead(sw1);
int val2 = digitalRead(sw2);

// --- TIME CHANGE MECHANISM --- //

void changeTime() {
  // Setup
  bool changing = false;
  unsigned long blinkTime = millis();

  // Hour
  while (true) {
    int val1 = digitalRead(sw1);
    int val2 = digitalRead(sw2);
    t = now();
    h1 = hour(t) / 10;
    h2 = hour(t) % 10;
    m1 = minute(t) / 10;
    m2 = minute(t) % 10;

    drawDigit(h1, 0, true);
    drawDigit(h2, 4);
    
    if (val1 == LOW) {
      adjustTime(3600);
      delay(200);
    } else if (val2 == LOW) {
      while (val2 == LOW) { 
        val2 = digitalRead(sw2);
      };
      break;
    }
  }

  // Minute
  while (true) {
    int val1 = digitalRead(sw1);
    int val2 = digitalRead(sw2);

    t = now();
    h1 = hour(t) / 10;
    h2 = hour(t) % 10;
    m1 = minute(t) / 10;
    m2 = minute(t) % 10;

    drawDigit(m1, 9, true);
    drawDigit(m2, 13);

    if (val1 == LOW) {
      adjustTime(60);
      delay(200); 
    } else if (val2 == LOW) {
      break;
    }
  }
}

// --- MAIN LOOP --- //

void loop() { 
  t = now();
  h1 = hour(t) / 10;
  h2 = hour(t) % 10;
  m1 = minute(t) / 10;
  m2 = minute(t) % 10;

  int val1 = digitalRead(sw1);
  int val2 = digitalRead(sw2);

  long presstime = 0;

  drawDigit(h1, 0, true); // Should only be true for this one
  drawDigit(h2, 4);
  drawDigit(m1, 9);
  drawDigit(m2, 13);
  
  static unsigned long pressTime = 0;
  if (val1 == LOW && val2 == LOW) {
    if (pressTime == 0) {
      pressTime = millis();
    } else if (millis() - pressTime >= 1000) {
      // Wait until both switches are released
      while (digitalRead(sw1) == LOW || digitalRead(sw2) == LOW) {
        // Do nothing, just wait
      }
      changeTime();
      pressTime = 0;
    }
  } else {
    pressTime = 0;
  }

  delay(200);
}