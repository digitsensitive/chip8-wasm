#include "chip8.h"

#include <cstring>
#include <iostream>

#include "fontset.h"

const int START_LOCATION_IN_MEMORY = 0x200;

Chip8::Chip8() {
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

void Chip8::execute_instructions(bool logging) {
  // Fetch the current operation code
  this->current_opcode = this->memory[this->program_counter] << 8 |
                         this->memory[this->program_counter + 1];

  // Increment program counter before execution
  this->program_counter += 2;

  // Decode and execute the current operation code
  switch (this->current_opcode & 0xF000) {
    case 0x0000:
      switch (this->current_opcode) {
        case 0x00E0:
          if (logging) {
            printf("[00E0, Display]: CLS - Clear display. \n");
          }
          this->clear_screen();
          break;

        case 0x00EE:
          if (logging) {
            printf("[00EE, Flow]: RET - Return from a subroutine. \n");
          }
          this->return_from_subroutine();
          break;

        default:
          printf("[ERROR]: Unrecognized opcode 0x%X. \n", this->current_opcode);
          exit(EXIT_FAILURE);
      }
      break;

    case 0x1000:
      if (logging) {
        printf("[1nnn, Flow]: JP addr - Jump to location nnn. \n");
      }
      this->jump_to_location();
      break;

    case 0x2000:
      if (logging) {
        printf("[2nnn, Flow]: CALL addr - Call subroutine at nnn. \n");
      }
      this->call_subroutine();
      break;

    case 0x3000:
      if (logging) {
        printf(
            "[3xnn, Cond]: SE Vx, byte - Skip next instruction if Vx = nn. \n");
      }
      this->skip_next_instruction_if_equal();
      break;

    case 0x4000:
      if (logging) {
        printf(
            "[4xnn, Cond]: SNE Vx, byte - Skip next instruction if Vx != nn. "
            "\n");
      }
      this->skip_next_instruction_if_not_equal();
      break;

    case 0x5000:
      if (logging) {
        printf(
            "[5xy0, Cond]: SE Vx, Vy - Skip next instruction if Vx = Vy. "
            "\n");
      }
      this->skip_next_instruction_if_vx_equal_vy();
      break;

    case 0x6000:
      if (logging) {
        printf("[6xnn, Const]: LD Vx, byte - Set Vx = nn. \n");
      }
      this->set_general_purpose_variable_registers();
      break;

    case 0x7000:
      if (logging) {
        printf("[7xnn, Const]: ADD Vx, byte - Add nn to Vx. \n");
      }
      this->add_to_general_purpose_variable_registers();
      break;

    case 0x8000:
      switch (this->current_opcode & 0x000F) {
        case 0x0000:
          if (logging) {
            printf("[8xy0, Assig]: LD Vx, Vy - Set Vx = Vy. \n");
          }
          this->load_vy_in_vx();
          break;

        case 0x0001:
          if (logging) {
            printf(
                "[8xy1, BitOp]: OR Vx, Vy - Set VX to VX or VY (Bitwise OR "
                "operation). \n");
          }
          this->set_vx_to_bitwise_or_of_vx_and_vy();
          break;

        default:
          printf("[ERROR]: Unrecognized opcode 0x%X. \n", this->current_opcode);
          exit(EXIT_FAILURE);
      }
      break;

    case 0xA000:
      if (logging) {
        printf("Instruction Annn - LD I, addr Set I = nnn.\n");
      }
      this->set_index_register();
      break;

    case 0xD000:
      if (logging) {
        printf("Instruction Dxyn - DRW Vx, Vy, nibble.\n");
      }
      this->draw_sprite();
      break;

    case 0xE000:
      switch (this->current_opcode & 0x00FF) {
        case 0x009E:
          if (logging) {
            printf("Instruction Ex9E - SKP Vx.\n");
          }
          this->skip_instruction_if_key_pressed();
          break;

        case 0x00A1:
          if (logging) {
            printf("Instruction ExA1 - SKNP Vx.\n");
          }
          this->skip_instruction_if_key_is_not_pressed();
          break;
      }
      break;

    case 0xF000:
      switch (this->current_opcode & 0x00FF) {
        case 0x000A:
          if (logging) {
            printf("Instruction Fx0A - LD Vx, K.\n");
          }
          this->wait_for_key_pressed();
          break;
      }

    default:
      printf("ERROR: Unrecognized opcode: 0x%X\n", this->current_opcode);
      exit(EXIT_FAILURE);
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

void Chip8::clear_screen() { this->display->clear_screen(); }

void Chip8::return_from_subroutine() {
  this->program_counter = this->stack[--this->stack_pointer];
}

void Chip8::jump_to_location() {
  const u16 address = this->current_opcode & 0x0FFF;
  this->program_counter = address;
}

void Chip8::call_subroutine() {
  const u16 address = this->current_opcode & 0x0FFF;
  this->stack[this->stack_pointer++] = this->program_counter;
  this->program_counter = address;
}

void Chip8::skip_next_instruction_if_equal() {
  const u8 Vx = this->get_x();
  const u8 byte = this->current_opcode & 0x00FF;
  if (this->general_purpose_variable_registers[Vx] == byte) {
    this->program_counter += 2;
  }
}

void Chip8::skip_next_instruction_if_not_equal() {
  const u8 Vx = this->get_x();
  const u8 byte = this->current_opcode & 0x00FF;
  if (this->general_purpose_variable_registers[Vx] != byte) {
    this->program_counter += 2;
  }
}

void Chip8::skip_next_instruction_if_vx_equal_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  if (this->general_purpose_variable_registers[Vx] ==
      this->general_purpose_variable_registers[Vy]) {
    this->program_counter += 2;
  }
}

void Chip8::set_general_purpose_variable_registers() {
  const u8 Vx = this->get_x();
  const u8 byte = this->current_opcode & 0x00FF;
  this->general_purpose_variable_registers[Vx] = byte;
}

void Chip8::add_to_general_purpose_variable_registers() {
  const u8 Vx = this->get_x();
  const u8 byte = this->current_opcode & 0x00FF;
  this->general_purpose_variable_registers[Vx] += byte;
}

void Chip8::load_vy_in_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  this->general_purpose_variable_registers[Vx] =
      this->general_purpose_variable_registers[Vy];
}

void Chip8::set_vx_to_bitwise_or_of_vx_and_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  this->general_purpose_variable_registers[Vx] |=
      this->general_purpose_variable_registers[Vy];
}

void Chip8::set_index_register() {
  const u16 address = this->current_opcode & 0x0FFF;
  this->index_register = address;
}

void Chip8::draw_sprite() {
  const u16 Vx = this->general_purpose_variable_registers[this->get_x()];
  const u16 Vy = this->general_purpose_variable_registers[this->get_y()];
  const u16 height = this->current_opcode & 0x000F;
  u16 pixel;

  this->general_purpose_variable_registers[0xF] = 0;

  for (size_t y = 0; y < height; y++) {
    pixel = this->memory[this->index_register + y];
    for (size_t x = 0; x < 8; x++) {
      if (pixel & (0x80 >> x)) {
        u32 loc = x + Vx + (y + Vy) * this->display->get_width();
        if (this->display->operator[](loc)) {
          this->general_purpose_variable_registers[0xF] = 1;
        }
        this->display->operator[](loc) ^= 1;
      }
    }
  }
}

void Chip8::skip_instruction_if_key_pressed() {
  const u8 Vx = this->get_x();
  if (this->keypad.is_pressed(this->general_purpose_variable_registers[Vx])) {
    this->program_counter += 2;
  }
}

void Chip8::skip_instruction_if_key_is_not_pressed() {
  const u8 Vx = this->get_x();
  if (!this->keypad.is_pressed(this->general_purpose_variable_registers[Vx])) {
    this->program_counter += 2;
  }
}

void Chip8::wait_for_key_pressed() {
  bool key_pressed = false;
  const u8 Vx = this->get_x();
  for (u8 i = 0; i < this->keypad.size(); i++) {
    if (this->keypad.is_pressed(i)) {
      this->general_purpose_variable_registers[Vx] = i;
      key_pressed = true;
    }
  }

  if (!key_pressed) {
    this->program_counter -= 2;
  }
}

u8 Chip8::get_x() { return (this->current_opcode & 0x0F00) >> 8; }
u8 Chip8::get_y() { return (this->current_opcode & 0x00F0) >> 4; }
