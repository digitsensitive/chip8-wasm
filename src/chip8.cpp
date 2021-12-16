#include "chip8.h"

#include <cstring>
#include <iostream>

#include "fontset.h"

const int START_LOCATION_IN_MEMORY = 0x200;

Chip8::Chip8() {
  this->display = new Display();
  this->keypad = new Keypad();

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
}

Chip8::~Chip8() {
  // free up memories
  delete this->display;
  delete this->keypad;
}

void Chip8::save_rom(const void* source) {
  // 0x200 (512) Start of most Chip-8 programs
  memcpy(this->memory.data() + START_LOCATION_IN_MEMORY, source,
         this->memory.size() - START_LOCATION_IN_MEMORY);
}

void Chip8::reset() {
  // Clear out rom data from memory
  memset(this->memory.data() + START_LOCATION_IN_MEMORY, 0,
         this->memory.size() - START_LOCATION_IN_MEMORY);
  this->general_purpose_variable_registers.fill(0);
  this->stack.fill(0);
  this->index_register = 0;
  this->program_counter = 0x200;
  this->stack_pointer = 0;
  this->delay_timer = 0;
  this->sound_timer = 0;

  display->clear_screen();
}

void Chip8::execute_instructions() {
  // Fetch the current operation code
  this->current_opcode = this->memory[this->program_counter] << 8 |
                         this->memory[this->program_counter + 1];

  // Increment program counter before execution
  this->program_counter += 2;
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
