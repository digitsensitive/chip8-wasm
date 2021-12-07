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
  void SaveRom(const void* source);
  void Reset();
  void Cycle();
  void UpdateTimers();

 private:
  typedef std::unordered_map<u8, std::function<void()> > operations_set;

  std::array<u8, 4096> memory;
  std::array<u8, 16> general_purpose_variable_registers;
  std::array<u16, 16> stack;

  std::unordered_map<u8, operations_set> operations;
  Display& display;
  Input& input;
  Rand rand;

  u8 t_delay;
  u8 t_sound;
  u16 I;
  u16 pc;
  u16 sp;
  u16 opcode;

  void BindOperations();

  friend class Interpreter;
};
