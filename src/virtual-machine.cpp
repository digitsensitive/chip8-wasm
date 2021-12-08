#include "virtual-machine.h"

#include <emscripten.h>

#include <fstream>
#include <iostream>
#include <vector>

const auto kCyclePerSecond = 10;  // Arbitrary, works best for most roms

bool VirtualMachine::Boot() { return window.Initialize(); }

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
    chip8.Reset();
  }

  chip8.SaveRom(data);
  ToggleState(kRomLoaded);
}

void VirtualMachine::Run() {
  if (CheckState(kRomLoaded) && !CheckState(kRomLoading)) {
    chip8.UpdateTimers();
    window.PollEvents(input);
    for (int i = 0; i < kCyclePerSecond; ++i) {
      chip8.Cycle();
    }
    window.Draw(display);
  }
}