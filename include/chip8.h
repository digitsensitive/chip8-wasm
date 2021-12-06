#pragma once

#include <array>
#include <functional>
#include <unordered_map>

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
  typedef std::unordered_map<uint8_t, std::function<void()> > operations_set;

  std::array<uint8_t, 0x1000> memory;
  std::array<uint8_t, 0x10> registers;
  std::array<uint16_t, 0x10> stack;

  std::unordered_map<uint8_t, operations_set> operations;
  Display& display;
  Input& input;
  Rand rand;

  uint8_t t_delay;
  uint8_t t_sound;
  uint16_t I;
  uint16_t pc;
  uint16_t sp;
  uint16_t opcode;

  void BindOperations();

  friend class Interpreter;
};
