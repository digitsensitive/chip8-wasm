#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <functional>
#include <unordered_map>

#include "chip8_types.h"
#include "display.h"
#include "keypad.h"
#include "random.h"

class Chip8 {
 public:
  Chip8();
  ~Chip8();

  void save_rom(const void* source);
  void cycle();
  void reset();

 public:
  void set_key(u8 key, bool state) { this->keypad.set_key(key, state); }
  bool get_draw_flag() { return this->draw_flag; }
  void deactivate_draw_flag() { this->draw_flag = false; }
  Display& get_display() { return *this->display; }

 private:
  typedef std::unordered_map<u8, std::function<void()> > instruction_subset;
  std::unordered_map<u8, instruction_subset> instructions;

  bool draw_flag;

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
  Keypad keypad;
  Random rand;

  void bind_instructions();

  // Enable the Interpreter class to access
  // private and protected members of the Chip8 class
  friend class Interpreter;
};

#endif