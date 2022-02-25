#pragma once

#include <SDL.h>

#include "display.h"

struct WindowProperties {
  const char* title;
  int width;
  int height;
};

class Renderer {
 public:
  Renderer(WindowProperties const& properties);
  ~Renderer();

  bool is_current_display_mode_valid();

  bool initialize();
  void draw(Display& display);

  void set_color(u8 red, u8 green, u8 blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
  }

 private:
  u8 red;
  u8 green;
  u8 blue;

  SDL_Window* window;
  SDL_Renderer* renderer;

  // Declare a structure with a description of a display mode:
  // Fields: SDL_PixelFormatEnum values, width, height, refresh rate in Hz
  // and driver-specific data
  SDL_DisplayMode current;

  WindowProperties window_properties;

  void draw_pixel(int x, int y, int scale);
};
