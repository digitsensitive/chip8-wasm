#pragma once

#include <array>
#include <cstring>

#include "chip8_types.h"

class Display {
 public:
  Display(){};
  ~Display(){};

  const int get_width() { return this->WIDTH; }
  const int get_height() { return this->HEIGHT; }
  const int get_scale() { return this->SCALE; }

  u8& operator[](int index) {
    if (index > display.size()) {
      throw std::out_of_range("display[] : index out of range");
    }
    return display[index];
  }

  void clear_screen() { std::memset(display.data(), 0, WIDTH * HEIGHT); }

 private:
  static const int WIDTH = 64;
  static const int HEIGHT = 32;
  static const int SCALE = 10;

  std::array<u8, WIDTH * HEIGHT> display;
};
