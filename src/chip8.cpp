#include "chip8.h"

#include <cstring>
#include <iostream>

#include "fontset.h"
#include "interpreter.h"

const int START_LOCATION_IN_MEMORY = 0x200;

const int kOp1 = 0x1;  // Default operations set
const int kOp0 = 0x0;
const int kOp8 = 0x8;
const int kOpE = 0xE;
const int kOpF = 0xF;

Chip8::Chip8() : operations(), input(Input::Instance()), rand(0, 255) {
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
  memcpy(memory.data(), FONTSET.data(), FONTSET.size());

  this->bind_operations();
}

Chip8::~Chip8() {
  // free up memories
  delete this->display;
}

void Chip8::save_rom(const void* source) {
  // 0x200 (512) Start of most Chip-8 programs
  memcpy(memory.data() + START_LOCATION_IN_MEMORY, source,
         memory.size() - START_LOCATION_IN_MEMORY);
}

void Chip8::reset() {
  // Clear out rom data from memory
  memset(memory.data() + START_LOCATION_IN_MEMORY, 0,
         memory.size() - START_LOCATION_IN_MEMORY);
  this->general_purpose_variable_registers.fill(0);
  this->stack.fill(0);
  this->index_register = 0;
  this->program_counter = 0x200;
  this->stack_pointer = 0;
  this->delay_timer = 0;
  this->sound_timer = 0;

  display->clear_screen();
}

void Chip8::cycle() {
  // Fetch opcode
  this->current_opcode =
      memory[this->program_counter] << 8 | memory[this->program_counter + 1];

  // Increment PC before execution
  this->program_counter += 2;

  try {
    switch ((this->current_opcode & 0xF000u) >> 12u) {
      case kOp0:
        operations[kOp0][(this->current_opcode & 0x000Fu)]();
        break;
      case kOp8:
        operations[kOp8][(this->current_opcode & 0x000Fu)]();
        break;
      case kOpE:
        operations[kOpE][(this->current_opcode & 0x000Fu)]();
        break;
      case kOpF:
        operations[kOpF][(this->current_opcode & 0x00FFu)]();
        break;
      default: {
        operations[kOp1][((this->current_opcode & 0xF000u) >> 12u)]();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n'
              << "Failed to execute opcode: " << std::hex
              << this->current_opcode << '\n';
  }
}

void Chip8::update_timers() {
  // Decrement the delay timer if it's been set
  if (this->delay_timer > 0) {
    --this->delay_timer;
  }

  // Decrement the sound timer if it's been set
  if (this->sound_timer > 0) {
    --this->sound_timer;
  }
}

void Chip8::bind_operations() {
  Interpreter d(*this);

  operations[kOp0][0x00] = std::bind(&Interpreter::OP_00E0, d);
  operations[kOp0][0x0E] = std::bind(&Interpreter::OP_00EE, d);
  operations[kOp1][0x01] = std::bind(&Interpreter::OP_1nnn, d);
  operations[kOp1][0x02] = std::bind(&Interpreter::OP_2nnn, d);
  operations[kOp1][0x03] = std::bind(&Interpreter::OP_3xkk, d);
  operations[kOp1][0x04] = std::bind(&Interpreter::OP_4xkk, d);
  operations[kOp1][0x05] = std::bind(&Interpreter::OP_5xy0, d);
  operations[kOp1][0x06] = std::bind(&Interpreter::OP_6xkk, d);
  operations[kOp1][0x07] = std::bind(&Interpreter::OP_7xkk, d);
  operations[kOp1][0x09] = std::bind(&Interpreter::OP_9xy0, d);
  operations[kOp1][0x0A] = std::bind(&Interpreter::OP_Annn, d);
  operations[kOp1][0x0B] = std::bind(&Interpreter::OP_Bnnn, d);
  operations[kOp1][0x0C] = std::bind(&Interpreter::OP_Cxkk, d);
  operations[kOp1][0x0D] = std::bind(&Interpreter::OP_Dxyn, d);
  operations[kOp8][0x00] = std::bind(&Interpreter::OP_8xy0, d);
  operations[kOp8][0x01] = std::bind(&Interpreter::OP_8xy1, d);
  operations[kOp8][0x02] = std::bind(&Interpreter::OP_8xy2, d);
  operations[kOp8][0x03] = std::bind(&Interpreter::OP_8xy3, d);
  operations[kOp8][0x04] = std::bind(&Interpreter::OP_8xy4, d);
  operations[kOp8][0x05] = std::bind(&Interpreter::OP_8xy5, d);
  operations[kOp8][0x06] = std::bind(&Interpreter::OP_8xy6, d);
  operations[kOp8][0x07] = std::bind(&Interpreter::OP_8xy7, d);
  operations[kOp8][0x0E] = std::bind(&Interpreter::OP_8xyE, d);
  operations[kOpE][0x01] = std::bind(&Interpreter::OP_ExA1, d);
  operations[kOpE][0x0E] = std::bind(&Interpreter::OP_Ex9E, d);
  operations[kOpF][0x07] = std::bind(&Interpreter::OP_Fx07, d);
  operations[kOpF][0x0A] = std::bind(&Interpreter::OP_Fx0A, d);
  operations[kOpF][0x15] = std::bind(&Interpreter::OP_Fx15, d);
  operations[kOpF][0x18] = std::bind(&Interpreter::OP_Fx18, d);
  operations[kOpF][0x1E] = std::bind(&Interpreter::OP_Fx1E, d);
  operations[kOpF][0x29] = std::bind(&Interpreter::OP_Fx29, d);
  operations[kOpF][0x33] = std::bind(&Interpreter::OP_Fx33, d);
  operations[kOpF][0x55] = std::bind(&Interpreter::OP_Fx55, d);
  operations[kOpF][0x65] = std::bind(&Interpreter::OP_Fx65, d);
}
