#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <string>

#include "chip8.h"
#include "disassembler.h"
#include "keypad.h"
#include "renderer.h"

enum VirtualMachineState { kRomLoading = 1 << 0, kRomLoaded = 1 << 1 };

class VirtualMachine {
 public:
  VirtualMachine();
  ~VirtualMachine();

  bool boot();
  bool load_rom(const std::string& kFile);
  void flash_program(char* data);
  void disassemble_program(char* data);
  void run();
  void poll_events();

  void change_game_color(u8 red, u8 green, u8 blue) {
    this->renderer->set_color(red, green, blue);
  }

 private:
  uint8_t emu_state_{0};

  inline void ToggleState(uint8_t state) { emu_state_ ^= state; }

  inline bool CheckState(uint8_t state) { return emu_state_ & state; }

  Renderer* renderer;
  Chip8 chip8;
  Disassembler disassembler;
};

#endif