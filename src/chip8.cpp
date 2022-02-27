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
  this->program_counter = START_LOCATION_IN_MEMORY;
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

        case 0x0002:
          if (logging) {
            printf(
                "[8xy2, BitOp]: AND Vx, Vy - Set Vx to Vx and Vy (Bitwise AND "
                "operation). \n");
          }
          this->set_vx_to_bitwise_and_of_vx_and_vy();
          break;

        case 0x0003:
          if (logging) {
            printf(
                "[8xy3, BitOp]: XOR Vx, Vy - Set Vx to Vx xor Vy (Bitwise XOR "
                "operation). \n");
          }
          this->set_vx_to_bitwise_xor_of_vx_and_vy();
          break;

        case 0x0004:
          if (logging) {
            printf("[8xy4, Math]: ADD Vx, Vy - Add Vy to Vx. \n");
          }
          this->add_vy_to_vx();
          break;

        case 0x0005:
          if (logging) {
            printf("[8xy5, Math]: SUB Vx, Vy - Subtract Vy from Vx. \n");
          }
          this->subtract_vy_from_vx();
          break;

        case 0x0006:
          if (logging) {
            printf("[8xy6, BitOp]: SHR Vx {, Vy} - Set Vx = Vx SHR 1. \n");
          }
          this->shift_vx_by_one_to_right();
          break;

        case 0x0007:
          if (logging) {
            printf("[8xy7, Math]: SUBN Vx, Vy - Set Vx = Vy - Vx. \n");
          }
          this->set_vx_to_vy_minus_vx();
          break;

        case 0x000E:
          if (logging) {
            printf("[8xyE, BitOp]: SHL Vx {, Vy} - Set Vx = Vx SHL 1. \n");
          }
          this->shift_vx_by_one_to_left();
          break;

        default:
          printf("[ERROR]: Unrecognized opcode 0x%X. \n", this->current_opcode);
          exit(EXIT_FAILURE);
      }
      break;

    case 0x9000:
      if (logging) {
        printf(
            "[9xy0, Cond]: SNE Vx, Vy - Skip next instruction if Vx != Vy. \n");
      }
      this->skip_next_instruction_if_vx_not_equal_vy();
      break;

    case 0xA000:
      if (logging) {
        printf("[Annn, MEM]: LD I, addr - Set I to the location nnn. \n");
      }
      this->set_index_register();
      break;

    case 0xB000:
      if (logging) {
        printf(
            "[Bnnn, Flow]: JP V0, addr - Jump to the location nnn plus V0. \n");
      }
      this->jump_to_extended_v0_location();
      break;

    case 0xC000:
      if (logging) {
        printf(
            "[Cxnn, Rand]: RND Vx, byte - Set Vx to the result of a bitwise "
            "AND operation on a random number. \n");
      }
      this->generate_random_number();
      break;

    case 0xD000:
      if (logging) {
        printf(
            "[Dxyn, Display]: DRW Vx, Vy, nibble - Draw a sprite at coordinate "
            "(Vx, Vy) .\n");
      }
      this->draw_sprite();
      break;

    case 0xE000:
      switch (this->current_opcode & 0x00FF) {
        case 0x009E:
          if (logging) {
            printf(
                "[Ex9E, KeyOp]: SKP Vx - Skip next instruction if key "
                "with value Vx is pressed. \n");
          }
          this->skip_instruction_if_key_pressed();
          break;

        case 0x00A1:
          if (logging) {
            printf(
                "[ExA1, KeyOp]: SKNP Vx - Skip next instruction if key with "
                "value Vx is not pressed. \n");
          }
          this->skip_instruction_if_key_is_not_pressed();
          break;

        default:
          printf("[ERROR]: Unrecognized opcode 0x%X. \n", this->current_opcode);
          exit(EXIT_FAILURE);
      }
      break;

    case 0xF000:
      switch (this->current_opcode & 0x00FF) {
        case 0x0007:
          if (logging) {
            printf(
                "[Fx07, Timer]: LD Vx, DT - Set Vx to the value of the delay "
                "timer. \n");
          }
          this->set_vx_to_delay_timer();
          break;

        case 0x000A:
          if (logging) {
            printf(
                "[Fx0A, KeyOp]: LD Vx, K - Wait for a key press, store the key "
                "value in Vx. \n");
          }
          this->wait_for_key_pressed();
          break;

        case 0x0015:
          if (logging) {
            printf("[Fx15, Timer]: LD DT, Vx - Set the delay timer to Vx. \n");
          }
          this->set_delay_timer_to_vx();
          break;

        case 0x0018:
          if (logging) {
            printf("[Fx18, Sound]: LD ST, Vx - Set the sound timer to Vx. \n");
          }
          this->set_sound_timer_to_vx();
          break;

        case 0x001E:
          if (logging) {
            printf("[Fx1E, MEM]: ADD I, Vx - Add Vx to I. \n");
          }
          this->add_i_to_vx();
          break;

        case 0x0029:
          if (logging) {
            printf(
                "[Fx29, MEM]: LD F, Vx - Set I to the location of the sprite "
                "character in Vx. \n");
          }
          this->set_i_to_sprite_character_in_vx();
          break;

        case 0x0033:
          if (logging) {
            printf(
                "[FX33, BCD]: LD B, Vx - Store binary-coded decimal "
                "representation of Vx in memory locations I, I+1, and I+2. \n");
          }
          this->store_binary_coded_decimal_of_vx();
          break;

        case 0x0055:
          if (logging) {
            printf(
                "[FX55, MEM]: LD [I], Vx - Store from V0 to Vx with values "
                "from memory @ I. \n");
          }
          this->store_registers_at_i();
          break;

        case 0x0065:
          if (logging) {
            printf(
                "[FX65, MEM]: LD Vx, [I] - Fill from V0 to Vx with values from "
                "memory @ I. \n");
          }
          this->load_registers_from_i();
          break;

        default:
          printf("[ERROR]: Unrecognized opcode 0x%X. \n", this->current_opcode);
          exit(EXIT_FAILURE);
      }

    default:
      printf("[ERROR]: Unrecognized opcode 0x%X. \n", this->current_opcode);
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

void Chip8::set_vx_to_bitwise_and_of_vx_and_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  this->general_purpose_variable_registers[Vx] &=
      this->general_purpose_variable_registers[Vy];
}

void Chip8::set_vx_to_bitwise_xor_of_vx_and_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  this->general_purpose_variable_registers[Vx] ^=
      this->general_purpose_variable_registers[Vy];
}

void Chip8::add_vy_to_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  const u16 sum = this->general_purpose_variable_registers[Vy] +
                  this->general_purpose_variable_registers[Vx];

  this->general_purpose_variable_registers[0xF] = (sum > 255) ? 1 : 0;
  this->general_purpose_variable_registers[Vx] = sum & 0xFF;
}

void Chip8::subtract_vy_from_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();

  this->general_purpose_variable_registers[0xF] =
      (this->general_purpose_variable_registers[Vx] >
       this->general_purpose_variable_registers[Vy])
          ? 1
          : 0;

  this->general_purpose_variable_registers[Vx] -=
      this->general_purpose_variable_registers[Vy];
}

void Chip8::shift_vx_by_one_to_right() {
  const u8 Vx = get_x();

  // get the least-significant bit of Vx
  this->general_purpose_variable_registers[0xF] =
      (this->general_purpose_variable_registers[Vx] & 0x1u);

  // shift Vx one to the right
  this->general_purpose_variable_registers[Vx] >>= 1;
}

void Chip8::set_vx_to_vy_minus_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();

  this->general_purpose_variable_registers[0xF] =
      (this->general_purpose_variable_registers[Vy] >
       this->general_purpose_variable_registers[Vx])
          ? 1
          : 0;

  this->general_purpose_variable_registers[Vx] =
      this->general_purpose_variable_registers[Vy] -
      this->general_purpose_variable_registers[Vx];
}

void Chip8::shift_vx_by_one_to_left() {
  const u8 Vx = get_x();

  // get the most-significant bit of Vx
  this->general_purpose_variable_registers[0xF] =
      (this->general_purpose_variable_registers[Vx] & 0x80u) >> 7u;

  // shift Vx one to the left
  this->general_purpose_variable_registers[Vx] <<= 1;
}

void Chip8::skip_next_instruction_if_vx_not_equal_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  if (this->general_purpose_variable_registers[Vx] !=
      this->general_purpose_variable_registers[Vy]) {
    this->program_counter += 2;
  }
}

void Chip8::set_index_register() {
  const u16 address = this->current_opcode & 0x0FFFu;
  this->index_register = address;
}

void Chip8::jump_to_extended_v0_location() {
  const u16 address = this->current_opcode & 0x0FFFu;
  this->program_counter = this->general_purpose_variable_registers[0] + address;
}

void Chip8::generate_random_number() {
  const u8 Vx = this->get_x();
  const u8 byte = this->current_opcode & 0x00FFu;
  this->general_purpose_variable_registers[Vx] =
      this->rand.get_random_number() & byte;
}

void Chip8::draw_sprite() {
  const u16 Vx = this->general_purpose_variable_registers[this->get_x()];
  const u16 Vy = this->general_purpose_variable_registers[this->get_y()];
  const u16 height = this->current_opcode & 0x000Fu;
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

void Chip8::set_vx_to_delay_timer() {
  const u8 Vx = this->get_x();
  this->general_purpose_variable_registers[Vx] = this->delay_timer;
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

void Chip8::set_delay_timer_to_vx() {
  const u8 Vx = this->get_x();
  this->delay_timer = this->general_purpose_variable_registers[Vx];
}

void Chip8::set_sound_timer_to_vx() {
  const u8 Vx = this->get_x();
  this->sound_timer = this->general_purpose_variable_registers[Vx];
}

void Chip8::add_i_to_vx() {
  const u8 Vx = this->get_x();
  this->index_register += this->general_purpose_variable_registers[Vx];
}

void Chip8::set_i_to_sprite_character_in_vx() {
  const u8 Vx = this->get_x();
  this->index_register += this->general_purpose_variable_registers[Vx] * 5;
}

void Chip8::store_binary_coded_decimal_of_vx() {
  const u8 Vx = this->get_x();
  this->memory[this->index_register] =
      this->general_purpose_variable_registers[Vx] / 100;
  this->memory[this->index_register + 1] =
      (this->general_purpose_variable_registers[Vx] / 10) % 10;
  this->memory[this->index_register + 2] =
      this->general_purpose_variable_registers[Vx] % 10;
}

void Chip8::store_registers_at_i() {
  const u8 Vx = this->get_x();
  for (u8 i = 0; i <= Vx; ++i) {
    this->memory[this->index_register + i] =
        this->general_purpose_variable_registers[i];
  }
}

void Chip8::load_registers_from_i() {
  const u8 Vx = this->get_x();
  for (u8 i = 0; i <= Vx; ++i) {
    this->general_purpose_variable_registers[i] =
        this->memory[this->index_register + i];
  }
}

u8 Chip8::get_x() { return (this->current_opcode & 0x0F00) >> 8; }
u8 Chip8::get_y() { return (this->current_opcode & 0x00F0) >> 4; }
