#ifndef CHIP8_H
#define CHIP8_H

#include <array>

#include "chip8_types.h"
#include "display.h"
#include "keypad.h"
#include "random.h"

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
  Random rand;

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

  // [3xnn, Cond]: SE Vx, byte - Skip next instruction if Vx == nn
  void skip_next_instruction_if_equal();

  // [4xnn, Cond]: SNE Vx, byte - Skip next instruction if Vx != nn
  void skip_next_instruction_if_not_equal();

  // [5xy0, Cond]: SE Vx, Vy - Skip next instruction if Vx == Vy
  void skip_next_instruction_if_vx_equal_vy();

  // [6xnn, Const]: LD Vx, byte - Set Vx = nn
  void set_general_purpose_variable_registers();

  // [7xnn, Const]: ADD Vx, byte - Add nn to Vx
  void add_to_general_purpose_variable_registers();

  // [8xy0, Assig]: LD Vx, Vy - Set Vx = Vy
  void load_vy_in_vx();

  // [8xy1, BitOp]: OR Vx, Vy - Set Vx to Vx or Vy (Bitwise OR operation)
  void set_vx_to_bitwise_or_of_vx_and_vy();

  // [8xy2, BitOp]: AND Vx, Vy - Set Vx to Vx and Vy (Bitwise AND operation)
  void set_vx_to_bitwise_and_of_vx_and_vy();

  // [8xy3, BitOp]: XOR Vx, Vy - Set Vx to Vx xor Vy (Bitwise XOR operation)
  void set_vx_to_bitwise_xor_of_vx_and_vy();

  // [8xy4, Math]: ADD Vx, Vy - Add Vy to Vx
  // If Vx is larger than 255 we have a overflow and Vf is set to 1
  void add_vy_to_vx();

  // [8xy5, Math]: SUB Vx, Vy - Subtract Vy from Vx
  // If Vx is larger than Vy, then Vf is set to 1, otherwise 0
  void subtract_vy_from_vx();

  // [8xy6, BitOp]: SHR Vx {, Vy} - Set Vx = Vx SHR 1
  void shift_vx_by_one_to_right();

  // [8xy7, Math]: SUBN Vx, Vy - Set Vx = Vy - Vx
  // If Vy is larger than Vx, then Vf is set to 1, otherwise 0
  void set_vx_to_vy_minus_vx();

  // [8xyE, BitOp]: SHL Vx {, Vy} - Set Vx = Vx SHL 1
  void shift_vx_by_one_to_left();

  // [9xy0, Cond]: SNE Vx, Vy - Skip next instruction if Vx != Vy
  void skip_next_instruction_if_vx_not_equal_vy();

  // [Annn, MEM]: LD I, addr - Set I to the location nnn
  void set_index_register();

  // [Bnnn, Flow]: JP V0, addr - Jump to the location nnn plus V0
  void jump_to_extended_v0_location();

  // [Cxnn, Rand]: RND Vx, byte - Set Vx to the result of a bitwise AND
  // operation on a random number
  void generate_random_number();

  // [Dxyn, Display]: DRW Vx, Vy, nibble - Draw a sprite at coordinate (Vx, Vy)
  // Sprite has a width of 8 pixels and a height of n pixels
  void draw_sprite();

  // [Ex9E, KeyOp]: SKP Vx - Skip next instruction if key with value Vx is
  // pressed
  void skip_instruction_if_key_pressed();

  // [ExA1, KeyOp]: SKNP Vx - Skip next instruction if key with value Vx is not
  // pressed
  void skip_instruction_if_key_is_not_pressed();

  // [Fx07, Timer]: LD Vx, DT - Set Vx to the value of the delay timer
  void set_vx_to_delay_timer();

  // [Fx0A, KeyOp]: LD Vx, K - Wait for a key press, store the key value in Vx
  // All execution stops until a key is pressed!
  void wait_for_key_pressed();

  // [Fx15, Timer]: LD DT, Vx - Set the delay timer to Vx
  void set_delay_timer_to_vx();

  // [Fx18, Sound]: LD ST, Vx - Set the sound timer to Vx
  void set_sound_timer_to_vx();

  // [Fx1E, MEM]: ADD I, Vx - Add Vx to I
  void add_i_to_vx();

  // [Fx29, MEM]: LD F, Vx - Set I to the location of the sprite character in Vx
  void set_i_to_sprite_character_in_vx();

  // Helpers
  u8 get_x();
  u8 get_y();
};

#endif