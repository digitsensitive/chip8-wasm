#include "virtual-machine.h"

#include <emscripten.h>

#include <fstream>
#include <iostream>
#include <vector>

const auto kCyclePerSecond = 10;  // Arbitrary, works best for most roms

VirtualMachine::VirtualMachine() : input(Input::Instance()) {
  this->chip8 = new Chip8();

  Display display = this->chip8->get_display();
  const int DISPLAY_WIDTH = display.get_width() * display.get_scale();
  const int DISPLAY_HEIGHT = display.get_height() * display.get_scale();

  this->renderer =
      new Renderer({"CHIP-8 GREAT!", DISPLAY_WIDTH, DISPLAY_HEIGHT});
}

VirtualMachine::~VirtualMachine() {
  // free up memories
  delete this->chip8;
  delete this->renderer;
}

bool VirtualMachine::Boot() { return renderer->initialize(); }

bool VirtualMachine::LoadRom(const std::string& kFile) {
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
  FlashRom(buffer.data());

  return true;
}

void VirtualMachine::FlashRom(char* data) {
  if (CheckState(kRomLoaded)) {
    // Rom is already loaded, reset state
    ToggleState(kRomLoaded);
    chip8->reset();
  }

  chip8->save_rom(data);
  ToggleState(kRomLoaded);
}

void VirtualMachine::Run() {
  if (CheckState(kRomLoaded) && !CheckState(kRomLoading)) {
    chip8->update_timers();
    renderer->PollEvents(input);
    for (int i = 0; i < kCyclePerSecond; ++i) {
      chip8->cycle();
    }
    renderer->draw(this->chip8->get_display());
  }
}
