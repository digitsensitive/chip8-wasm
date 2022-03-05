
#include "renderer.h"

#include "chip8/display.h"

Renderer::Renderer(WindowProperties const &properties)
    : window_properties(properties), window(nullptr), renderer(nullptr) {}

Renderer::~Renderer() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

bool Renderer::is_current_display_mode_valid() {
  // Returns 0 on success or a negative error code on failure
  int currentDisplayModeReturnValue = SDL_GetCurrentDisplayMode(0, &current);

  return currentDisplayModeReturnValue != 0 ? false : true;
}

bool Renderer::initialize() {
  // Returns zero on success or a negative error code on failure
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    return false;
  }

  if (!is_current_display_mode_valid()) {
    printf("Validation of current display mode failed: %s\n", SDL_GetError());
    return false;
  };

  // Create our window with title, position, size and flags
  this->window = SDL_CreateWindow(
      window_properties.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      window_properties.width, window_properties.height, SDL_WINDOW_SHOWN);

  // Check if we have a window
  if (this->window == nullptr) {
    return false;
  }

  // Check if we have a renderer
  if (this->renderer == nullptr) {
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
  }

  return true;
}

void Renderer::draw(Display &display) {
  // Clear the screen
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0xff);
  SDL_RenderClear(this->renderer);

  // Loop through the display and draw pixels
  for (int y = 0; y < display.get_height(); y++) {
    for (int x = 0; x < display.get_width(); x++) {
      if (display[x + (y * display.get_width())]) {
        draw_pixel(x, y, display.get_scale());
      }
    }
  }

  // Draw Grid
  SDL_SetRenderDrawColor(this->renderer, this->grid_line_color.r,
                         this->grid_line_color.g, this->grid_line_color.b,
                         this->grid_line_color.a);

  for (int x = 0; x < 1 + display.get_width() * display.get_scale();
       x += display.get_scale()) {
    SDL_RenderDrawLine(this->renderer, x, 0, x, window_properties.height);
  }

  for (int y = 0; y < 1 + display.get_height() * display.get_scale();
       y += display.get_scale()) {
    SDL_RenderDrawLine(this->renderer, 0, y, window_properties.width, y);
  }

  SDL_RenderPresent(this->renderer);
}

void Renderer::draw_pixel(int x, int y, int scale) {
  SDL_SetRenderDrawColor(this->renderer, this->pixel_color.r,
                         this->pixel_color.g, this->pixel_color.b,
                         this->pixel_color.a);

  SDL_Rect rect{x * scale, y * scale, scale, scale};
  SDL_RenderDrawRect(this->renderer, &rect);
  SDL_RenderFillRect(this->renderer, &rect);
}
