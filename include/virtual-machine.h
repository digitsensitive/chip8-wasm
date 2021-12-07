#pragma once

#include <string>

#include "chip8.h"
#include "window.h"

enum VirtualMachineState { kRomLoading = 1 << 0, kRomLoaded = 1 << 1 };

class VirtualMachine {
 public:
  bool Boot();
  bool LoadRom(const std::string& kFile);
  void FlashRom(char* data);
  void Run();

  VirtualMachine()
      : display(Display::Instance()),
        input(Input::Instance()),
        window(display.width() * display.scale(),
               display.height() * display.scale()) {}

 private:
  uint8_t emu_state_{0};

  inline void ToggleState(uint8_t state) { emu_state_ ^= state; }

  inline bool CheckState(uint8_t state) { return emu_state_ & state; }

  Display& display;
  Input& input;
  Window window;
  Chip8 chip8;
};
