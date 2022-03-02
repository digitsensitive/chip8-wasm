#include "virtual-machine.h"

/* TODO: Add WASM Support
#include <emscripten.h>
*/

#include <fstream>
#include <iostream>
#include <vector>

VirtualMachine::VirtualMachine() : is_running(false) {
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

bool VirtualMachine::load_program(const std::string& program_file) {
  ToggleState(kRomLoading);  // Turn on rom loading state

  std::ifstream rom(program_file);
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
  this->is_running = true;

  while (this->is_running && (kRomLoaded) && !CheckState(kRomLoading)) {
    this->frame_start = SDL_GetTicks();

    this->chip8.execute_instructions(true);
    this->chip8.update_timers();
    if (this->chip8.get_draw_flag()) {
      renderer->draw(this->chip8.get_display());
    }

    this->process_input();
    this->chip8.set_draw_flag(false);

    this->frame_time = SDL_GetTicks() - this->frame_start;

    if (this->frame_delay > this->frame_time) {
      SDL_Delay(this->frame_delay - this->frame_time);
    }
  }
}

void VirtualMachine::process_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    const u8 key = event.key.keysym.sym;
    switch (event.type) {
      case SDL_QUIT:
        this->is_running = false;
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

void VirtualMachine::shutdown_systems() {
  // shutdown here
}
