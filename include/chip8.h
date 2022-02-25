#ifndef CHIP8_H
#define CHIP8_H

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
  void execute_instructions(bool logging);
  void update_timers();

 public:
  void set_key(u8 key, bool state) { this->keypad.set_key(key, state); }

  Display& get_display() { return *this->display; }

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
  Keypad keypad;

  // Instructions

  // 00E0 - CLS Clear the display.
  void clear_screen();

  // 1nnn - JP addr Jump to location nnn.
  void jump_to_location();

  // 6xkk - LD Vx, byte Set Vx = kk.
  void set_general_purpose_variable_registers();

  // 7xkk - ADD Vx, byte Set Vx = Vx + kk.
  void add_to_general_purpose_variable_registers();

  // Annn - LD I, addr Set I = nnn.
  void set_index_register();

  // Dxyn - DRW Vx, Vy, nibble
  void draw_sprite();

  // Ex9E - SKP Vx
  void skip_instruction_if_key_pressed();

  // ExA1 - SKNP Vx
  void skip_instruction_if_key_is_not_pressed();

  // Helpers
  u8 get_x();
  u8 get_y();
};

#endif