#ifndef CHIP8_H
#define CHIP8_H

#include <array>

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

  /*
    Standard Chip-8 Instructions:
    [Opcode, Type]: Syntax for assembly language - Explanation
    Be aware: There is no official syntax for the assembly language

    Not supported:
    - [0nnn, Call]: SYS addr - Jump to a machine code routine at nnn.
    - Super Chip-48 Instructions
  */

  // [00E0, Display]: CLS - Clear display
  void clear_screen();

  // [00EE, Flow]: RET - Return from a subroutine
  void return_from_subroutine();

  // [1nnn, Flow]: JP addr - Jump to location nnn
  void jump_to_location();

  // [2nnn, Flow]: CALL addr - Call subroutine at nnn
  void call_subroutine();

  // [3xnn, Cond]: SE Vx, byte - Skip next instruction if Vx = nn
  void skip_instruction_if_equal();

  // [4xnn, Cond]: SNE Vx, byte - Skip next instruction if Vx != nn
  void skip_instruction_if_not_equal();

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

  // Fx0A - LD Vx, K
  void wait_for_key_pressed();

  // Helpers
  u8 get_x();
  u8 get_y();
};

#endif