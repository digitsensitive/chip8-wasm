#pragma once

#include <SDL.h>

#include "display.h"
#include "input.h"

class Window {
 public:
  Window(int width, int height)
      : width(width),
        height(height),
        window(nullptr),
        renderer(nullptr),
        running(false) {}
  ~Window();

  bool get_running() { return running; }

  bool Initialize();
  void PollEvents(Input& input);
  void Draw(Display& display);

 private:
  int width;
  int height;
  bool running;

  SDL_Window* window;
  SDL_Renderer* renderer;

  void DrawPixel(int x, int y, int scale);
};
