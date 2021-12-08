#pragma once

#include <array>
#include <functional>
#include <unordered_map>

#include "chip8_types.h"
#include "display.h"
#include "input.h"
#include "rand.h"

class Chip8 {
 public:
  Chip8();
  ~Chip8();

  void save_rom(const void* source);
  void reset();
  void cycle();
  void update_timers();

 private:
  typedef std::unordered_map<u8, std::function<void()> > operations_set;

  u16 current_opcode;
  u8 delay_timer;
  u16 index_register;
  u16 program_counter;
  u8 sound_timer;
  u16 stack_pointer;

  std::array<u8, 16> general_purpose_variable_registers;
  std::array<u8, 4096> memory;
  std::array<u16, 16> stack;

  std::unordered_map<u8, operations_set> operations;
  Display& display;
  Input& input;
  Rand rand;

  void bind_operations();

  friend class Interpreter;
};
