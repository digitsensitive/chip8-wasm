#pragma once

#include <SDL.h>

#include "display.h"
#include "input.h"

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
  bool get_running() { return running; }

  bool initialize();
  void PollEvents(Input& input);
  void draw(Display& display);

 private:
  bool running;

  SDL_Window* window;
  SDL_Renderer* renderer;

  // Declare a structure with a description of a display mode:
  // Fields: SDL_PixelFormatEnum values, width, height, refresh rate in Hz
  // and driver-specific data
  SDL_DisplayMode current;

  WindowProperties window_properties;

  void DrawPixel(int x, int y, int scale);
};
