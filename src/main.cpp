/* TODO: Add Support for WASM
#include <emscripten.h>
#include <emscripten/bind.h>
*/

#include <iostream>

#include "virtual-machine.h"

VirtualMachine virtual_machine;

/* TODO: Add Support for WASM
void main_loop() { virtual_machine.run(); }

extern "C" {
// Wrap in extern C to prevent C++ name mangling
void load_game(char* data) { virtual_machine.flash_program(data); }
void change_game_color(u8 red, u8 green, u8 blue) {
  virtual_machine.change_game_color(red, green, blue);
}
}*/

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: chip-8 chip8application\n\n");
    return 1;
  }

  bool initSucceeded = virtual_machine.boot();

  if (!virtual_machine.load_program(argv[1])) {
    return 1;
  };

  if (initSucceeded) {
    virtual_machine.run();
  }

  virtual_machine.shutdown_systems();

  /* TODO: Add Support for WASM
  if (virtual_machine.boot()) {
    //
  https://emscripten.org/docs/api_reference/emscripten.h.html#c.emscripten_set_main_loop
    emscripten_set_main_loop(main_loop, -1, true);
  } else {
    std::cerr << "Failed to boot the Chip-8 Virtual Machine. " << '\n';
    return 1;
  }*/

  return 0;
}
