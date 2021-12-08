#pragma once

#include <string>

#include "chip8.h"
#include "window.h"

enum VirtualMachineState { kRomLoading = 1 << 0, kRomLoaded = 1 << 1 };

class VirtualMachine {
 public:
  VirtualMachine();
  ~VirtualMachine();

  bool Boot();
  bool LoadRom(const std::string& kFile);
  void FlashRom(char* data);
  void Run();

 private:
  uint8_t emu_state_{0};

  inline void ToggleState(uint8_t state) { emu_state_ ^= state; }

  inline bool CheckState(uint8_t state) { return emu_state_ & state; }

  Input& input;
  Window* window;
  Chip8* chip8;
};
