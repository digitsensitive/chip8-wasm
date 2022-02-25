#include <emscripten.h>
#include <emscripten/bind.h>

#include <iostream>

#include "virtual-machine.h"

VirtualMachine virtual_machine;

void do_error(const std::string& kErrorMessage) {
  std::cerr << "ERROR: " << kErrorMessage << '\n';
}

void main_loop() { virtual_machine.run(); }

extern "C" {
// Wrap in extern C to prevent C++ name mangling
void load_game(char* data) { virtual_machine.flash_rom(data); }
void change_game_color(u8 red, u8 green, u8 blue) {
  virtual_machine.change_game_color(red, green, blue);
}
// TODO: void disassemble(char* data) {
// virtual_machine.disassemble_program(data); }
}

int main() {
  if (virtual_machine.boot()) {
    emscripten_set_main_loop(main_loop, /* fps */ -1, /* infinite loop */ true);
  } else {
    do_error("Failed to boot emulator");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
