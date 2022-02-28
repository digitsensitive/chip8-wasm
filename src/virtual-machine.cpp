#include "virtual-machine.h"

#include <emscripten.h>

#include <fstream>
#include <iostream>
#include <vector>

const auto kCyclePerSecond = 10;  // Arbitrary, works best for most roms

VirtualMachine::VirtualMachine() {
  Display display = this->chip8.get_display();
  const int DISPLAY_WIDTH = display.get_width() * display.get_scale();
  const int DISPLAY_HEIGHT = display.get_height() * display.get_scale();

  this->renderer =
      new Renderer({"CHIP-8 interpreter", DISPLAY_WIDTH, DISPLAY_HEIGHT});
}

VirtualMachine::~VirtualMachine() {
  // free up memories
  delete this->renderer;
}

bool VirtualMachine::boot() { return renderer->initialize(); }

bool VirtualMachine::load_rom(const std::string& kFile) {
  ToggleState(kRomLoading);  // Turn on rom loading state

  std::ifstream rom(kFile);
  if (!rom) return false;

  rom.seekg(0, std::ios::end);
  const int kSize = rom.tellg();
  rom.seekg(0, std::ios::beg);

  // Read rom data into a temporary buffer
  std::vector<char> buffer;
  buffer.reserve(kSize);
  buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(rom),
                std::istreambuf_iterator<char>());

  ToggleState(kRomLoading);  // Turn off rom loading state
  flash_program(buffer.data());

  return true;
}

void VirtualMachine::flash_program(char* data) {
  if (CheckState(kRomLoaded)) {
    // Rom is already loaded, reset state
    ToggleState(kRomLoaded);
    this->chip8.reset();
  }

  this->chip8.save_rom(data);
  ToggleState(kRomLoaded);
}

void VirtualMachine::disassemble_program(char* data) {
  this->disassembler.load_program(data);
}

void VirtualMachine::run() {
  if (CheckState(kRomLoaded) && !CheckState(kRomLoading)) {
    this->chip8.update_timers();
    this->poll_events();
    for (int i = 0; i < kCyclePerSecond; ++i) {
      this->chip8.execute_instructions(true);
    }
    renderer->draw(this->chip8.get_display());
  }
}

void VirtualMachine::poll_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    const u8 key = event.key.keysym.sym;
    switch (event.type) {
      case SDL_QUIT:
        break;
      case SDL_KEYDOWN:
        this->chip8.set_key(key, true);
        break;
      case SDL_KEYUP:
        this->chip8.set_key(key, false);
        break;
    }
  }
}
