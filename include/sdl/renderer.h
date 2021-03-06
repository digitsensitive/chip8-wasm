#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

#include "chip8/display.h"

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
    this->pixel_color.r = red;
    this->pixel_color.g = green;
    this->pixel_color.b = blue;
  }

 private:
  SDL_Color pixel_color = {0xD2, 0xA2, 0x4C, 0xFF};
  SDL_Color grid_line_color = {0x1C, 0x1C, 0x1C, 0xFF};

  SDL_Window* window;
  SDL_Renderer* renderer;

  // Declare a structure with a description of a display mode:
  // Fields: SDL_PixelFormatEnum values, width, height, refresh rate in Hz
  // and driver-specific data
  SDL_DisplayMode current;

  WindowProperties window_properties;

  void draw_pixel(int x, int y, int scale);
};

#endif