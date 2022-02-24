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

  bool isCurrentDisplayModeValid();

  bool initialize();
  void draw(Display& display);

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  // Declare a structure with a description of a display mode:
  // Fields: SDL_PixelFormatEnum values, width, height, refresh rate in Hz
  // and driver-specific data
  SDL_DisplayMode current;

  WindowProperties window_properties;

  void DrawPixel(int x, int y, int scale);
};
