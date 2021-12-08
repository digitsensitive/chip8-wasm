#pragma once

#include <array>
#include <cstring>

#include "chip8_types.h"

class Display {
 public:
  const int get_width() { return this->WIDTH; }
  const int get_height() { return this->HEIGHT; }
  const int get_scale() { return this->SCALE; }

  void clear_screen() { std::memset(pixels.data(), 0, WIDTH * HEIGHT); }

  u8& operator[](int index) {
    if (index > pixels.size()) {
      throw std::out_of_range("index out of range");
    }
    return pixels[index];
  }

 private:
  static const int HEIGHT = 32;
  static const int SCALE = 10;
  static const int WIDTH = 64;

  std::array<u8, WIDTH * HEIGHT> pixels;
};
