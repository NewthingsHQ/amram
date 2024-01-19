#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();

void setup() {
  matrix.begin(0x70);
}

void loop() {
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  for (int8_t x=7; x>=-100; x — ) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print(“Hello World!”);
    matrix.writeDisplay();
    delay(100);
  }
}