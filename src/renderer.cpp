
#include "renderer.h"

#include "display.h"

Renderer::~Renderer() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

bool Renderer::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return false;
  }

  SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
  if (window == nullptr || renderer == nullptr) return false;
  SDL_SetWindowTitle(window, "CHIP-8 Emulator");

  running = true;
  return running;
}

void Renderer::PollEvents(Input &input) {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      running = false;
    }
    if (event.type == SDL_KEYDOWN) {
      const uint8_t key = event.key.keysym.sym;
      input[key] = 1;
    }
    if (event.type == SDL_KEYUP) {
      const uint8_t &key = event.key.keysym.sym;
      input[key] = 0;
    }
  }
}

void Renderer::draw(Display &display) {
  // Clear the screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
  SDL_RenderClear(renderer);

  // Loop through the display and draw pixels
  for (int y = 0; y < display.get_height(); y++) {
    for (int x = 0; x < display.get_width(); x++) {
      if (display[x + (y * display.get_width())]) {
        DrawPixel(x, y, display.get_scale());
      }
    }
  }

  SDL_RenderPresent(renderer);
}

void Renderer::DrawPixel(int x, int y, int scale) {
  SDL_SetRenderDrawColor(renderer, 0xEF, 0xB3, 0xEE, 0xFF);

  SDL_Rect rect{x * scale, y * scale, scale, scale};
  SDL_RenderDrawRect(renderer, &rect);
  SDL_RenderFillRect(renderer, &rect);
}
