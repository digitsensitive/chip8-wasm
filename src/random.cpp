#include "random.h"

Random::Random() {}

Random::~Random() {}

int Random::get_random_number() {
  srand(time(0));
  return rand() % 256;
}