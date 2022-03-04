#include "chip8.h"

#include <cstring>
#include <iostream>

#include "fontset.h"
#include "interpreter.h"

const int START_LOCATION_IN_MEMORY = 0x200;

Chip8::Chip8() {
  this->draw_flag = true;
  this->display = new Display();

  this->current_opcode = 0;
  this->delay_timer = 0;
  this->index_register = 0;

  // Program counter starts at 0x200 (Start adress program)
  this->program_counter = START_LOCATION_IN_MEMORY;

  this->sound_timer = 0;
  this->stack_pointer = 0;

  // Apply zero to all elements in the containers
  this->general_purpose_variable_registers.fill(0);
  this->memory.fill(0);
  this->stack.fill(0);

  // Load and store fontset (= 80 bytes)
  // @ memory locations 0x00 (location 0) to 0x4F (location 79)
  memcpy(this->memory.data(), FONTSET.data(), FONTSET.size());

  this->bind_instructions();
}

Chip8::~Chip8() {
  // free up memories
  delete this->display;
}

void Chip8::save_rom(const void* source) {
  // 0x200 (512) Start of most Chip-8 programs
  memcpy(this->memory.data() + START_LOCATION_IN_MEMORY, source,
         this->memory.size() - START_LOCATION_IN_MEMORY);
}

void Chip8::cycle() {
  // Fetch the current operation code
  this->current_opcode = this->memory[this->program_counter] << 8 |
                         this->memory[this->program_counter + 1];

  // Increment program counter before execution
  this->program_counter += 2;

  // Decode and execute the current operation code:
  // We can declare four instruction groups, that share the same first nibble
  // and therefore can be distinguished with a switch statement (0x0, 0x8, 0xE
  // and 0xF). These four groups have more than one instruction and get further
  // distinguished with the last nibble (0x0, 0x8 and 0xE) or the last byte
  // (0xF). This last distinction is our instruction_subset.
  // If the main switch statement does not match any of the upper four cases we
  // call the default case, which will select the last instruction group. This
  // group is a collective of instructions, that only have one instruction each
  // which gets distinguished by the first nibble.
  // Here the label 0x1 is used for this last group, although one of the
  // following labels could just as well be used: 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
  // 0x9, 0xA, 0xB, 0xC or 0xD.
  switch ((this->current_opcode & 0xF000u) >> 12u) {
    case 0x0:
      instructions[0x0][(this->current_opcode & 0x000Fu)]();
      break;
    case 0x8:
      instructions[0x8][(this->current_opcode & 0x000Fu)]();
      break;
    case 0xE:
      instructions[0xE][(this->current_opcode & 0x000Fu)]();
      break;
    case 0xF:
      instructions[0xF][(this->current_opcode & 0x00FFu)]();
      break;
    default: {
      instructions[0x1][((this->current_opcode & 0xF000u) >> 12u)]();
    }
  }

  // Update timers
  // Decrement the delay timer if it's been set
  if (this->delay_timer > 0) {
    --this->delay_timer;
  }

  // Decrement the sound timer if it's been set
  if (this->sound_timer > 0) {
    --this->sound_timer;
  }
}

void Chip8::reset() {
  // Clear out rom data from memory
  memset(this->memory.data() + START_LOCATION_IN_MEMORY, 0,
         this->memory.size() - START_LOCATION_IN_MEMORY);
  this->general_purpose_variable_registers.fill(0);
  this->stack.fill(0);
  this->index_register = 0;
  this->program_counter = START_LOCATION_IN_MEMORY;
  this->stack_pointer = 0;
  this->delay_timer = 0;
  this->sound_timer = 0;

  display->clear_screen();
}

void Chip8::bind_instructions() {
  Interpreter i(*this);

  instructions[0x0][0x00] = std::bind(&Interpreter::clear_screen, i);
  instructions[0x0][0x0E] = std::bind(&Interpreter::return_from_subroutine, i);
  instructions[0x1][0x01] = std::bind(&Interpreter::jump_to_location, i);
  instructions[0x1][0x02] = std::bind(&Interpreter::call_subroutine, i);
  instructions[0x1][0x03] =
      std::bind(&Interpreter::skip_next_instruction_if_equal, i);
  instructions[0x1][0x04] =
      std::bind(&Interpreter::skip_next_instruction_if_not_equal, i);
  instructions[0x1][0x05] =
      std::bind(&Interpreter::skip_next_instruction_if_vx_equal_vy, i);
  instructions[0x1][0x06] =
      std::bind(&Interpreter::set_general_purpose_variable_registers, i);
  instructions[0x1][0x07] =
      std::bind(&Interpreter::add_to_general_purpose_variable_registers, i);
  instructions[0x1][0x09] =
      std::bind(&Interpreter::skip_next_instruction_if_vx_not_equal_vy, i);
  instructions[0x1][0x0A] = std::bind(&Interpreter::set_index_register, i);
  instructions[0x1][0x0B] =
      std::bind(&Interpreter::jump_to_extended_v0_location, i);
  instructions[0x1][0x0C] = std::bind(&Interpreter::generate_random_number, i);
  instructions[0x1][0x0D] = std::bind(&Interpreter::draw_sprite, i);
  instructions[0x8][0x00] = std::bind(&Interpreter::load_vy_in_vx, i);
  instructions[0x8][0x01] =
      std::bind(&Interpreter::set_vx_to_bitwise_or_of_vx_and_vy, i);
  instructions[0x8][0x02] =
      std::bind(&Interpreter::set_vx_to_bitwise_and_of_vx_and_vy, i);
  instructions[0x8][0x03] =
      std::bind(&Interpreter::set_vx_to_bitwise_xor_of_vx_and_vy, i);
  instructions[0x8][0x04] = std::bind(&Interpreter::add_vy_to_vx, i);
  instructions[0x8][0x05] = std::bind(&Interpreter::subtract_vy_from_vx, i);
  instructions[0x8][0x06] =
      std::bind(&Interpreter::shift_vx_by_one_to_right, i);
  instructions[0x8][0x07] = std::bind(&Interpreter::set_vx_to_vy_minus_vx, i);
  instructions[0x8][0x0E] = std::bind(&Interpreter::shift_vx_by_one_to_left, i);
  instructions[0xE][0x01] =
      std::bind(&Interpreter::skip_instruction_if_key_is_not_pressed, i);
  instructions[0xE][0x0E] =
      std::bind(&Interpreter::skip_instruction_if_key_pressed, i);
  instructions[0xF][0x07] = std::bind(&Interpreter::set_vx_to_delay_timer, i);
  instructions[0xF][0x0A] = std::bind(&Interpreter::wait_for_key_pressed, i);
  instructions[0xF][0x15] = std::bind(&Interpreter::set_delay_timer_to_vx, i);
  instructions[0xF][0x18] = std::bind(&Interpreter::set_sound_timer_to_vx, i);
  instructions[0xF][0x1E] = std::bind(&Interpreter::add_i_to_vx, i);
  instructions[0xF][0x29] =
      std::bind(&Interpreter::set_i_to_sprite_character_in_vx, i);
  instructions[0xF][0x33] =
      std::bind(&Interpreter::store_binary_coded_decimal_of_vx, i);
  instructions[0xF][0x55] = std::bind(&Interpreter::store_registers_at_i, i);
  instructions[0xF][0x65] = std::bind(&Interpreter::load_registers_from_i, i);
}
