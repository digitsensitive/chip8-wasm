#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>

#include "SDL_ttf.h"

class Text {
 public:
  Text();
  ~Text();

  void set(SDL_Point position);

 private:
  SDL_Point position = {2, 2};
};

#endif