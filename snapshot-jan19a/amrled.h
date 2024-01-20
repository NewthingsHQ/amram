#ifndef AMRLED_H
#define AMRLED_H

#include <Arduino.h>

const byte char0[] PROGMEM = {
  0b000,
  0b010,
  0b101,
  0b101,
  0b101,
  0b101,
  0b010,
  0b000
};

const byte char1[] PROGMEM = {
  0b000,
  0b010,
  0b110,
  0b010,
  0b010,
  0b010,
  0b111,
  0b000
};

const byte char2[] PROGMEM = {
  0b000,
  0b010,
  0b101,
  0b001,
  0b010,
  0b100,
  0b111,
  0b000
};

const byte char3[] PROGMEM = {
  0b000,
  0b110,
  0b001,
  0b010,
  0b001,
  0b101,
  0b010,
  0b000
};

const byte char4[] PROGMEM = {
  0b000,
  0b001,
  0b011,
  0b101,
  0b111,
  0b001,
  0b001,
  0b000
};

const byte char5[] PROGMEM = {
  0b000,
  0b111,
  0b100,
  0b110,
  0b001,
  0b101,
  0b010,
  0b000
};

const byte char6[] PROGMEM = {
  0b000,
  0b011,
  0b100,
  0b110,
  0b101,
  0b101,
  0b010,
  0b000
};

const byte char7[] PROGMEM = {
  0b000,
  0b111,
  0b001,
  0b001,
  0b010,
  0b100,
  0b100,
  0b000
};

const byte char8[] PROGMEM = {
  0b000,
  0b010,
  0b101,
  0b010,
  0b101,
  0b101,
  0b010,
  0b000
};

const byte char9[] PROGMEM = {
  0b000,
  0b010,
  0b101,
  0b101,
  0b011,
  0b001,
  0b110,
};

#endif