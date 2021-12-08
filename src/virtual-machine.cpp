#include "virtual-machine.h"

#include <emscripten.h>

#include <fstream>
#include <iostream>
#include <vector>

const auto kCyclePerSecond = 10;  // Arbitrary, works best for most roms

VirtualMachine::VirtualMachine() : input(Input::Instance()) {
  this->chip8 = new Chip8();
  this->window = new Window(this->chip8->get_display().get_width() *
                                this->chip8->get_display().get_scale(),
                            this->chip8->get_display().get_height() *
                                this->chip8->get_display().get_scale());
}

VirtualMachine::~VirtualMachine() {
  // free up memories
  delete this->chip8;
  delete this->window;
}

bool VirtualMachine::Boot() { return window->Initialize(); }

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
    window->PollEvents(input);
    for (int i = 0; i < kCyclePerSecond; ++i) {
      chip8->cycle();
    }
    window->Draw(this->chip8->get_display());
  }
}
