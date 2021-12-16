#pragma once

#include <array>
#include <functional>
#include <unordered_map>

#include "chip8_types.h"
#include "display.h"
#include "keypad.h"

class Chip8 {
 public:
  Chip8();
  ~Chip8();

  void save_rom(const void* source);
  void reset();
  void execute_instructions();
  void update_timers();

  Display& get_display() { return *this->display; }
  Keypad& get_keypad() { return *this->keypad; }

 private:
  u16 current_opcode;
  u8 delay_timer;
  u16 index_register;
  u16 program_counter;
  u8 sound_timer;
  u16 stack_pointer;

  std::array<u8, 16> general_purpose_variable_registers;
  std::array<u8, 4096> memory;
  std::array<u16, 16> stack;

  Display* display;
  Keypad* keypad;
};
