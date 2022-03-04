#include "interpreter.h"

u8 Interpreter::get_x() { return (chip8.current_opcode & 0x0F00u) >> 8; }
u8 Interpreter::get_y() { return (chip8.current_opcode & 0x00F0u) >> 4; }

void Interpreter::clear_screen() {
  chip8.display->clear_screen();
  chip8.draw_flag = true;
}

void Interpreter::return_from_subroutine() {
  // TODO: Why --chip8.stack_pointer and not chip8.stack_pointer--
  chip8.program_counter = chip8.stack[--chip8.stack_pointer];
}

void Interpreter::jump_to_location() {
  const u16 address = chip8.current_opcode & 0x0FFFu;
  chip8.program_counter = address;
}

void Interpreter::call_subroutine() {
  const u16 address = chip8.current_opcode & 0x0FFFu;
  chip8.stack[chip8.stack_pointer++] = chip8.program_counter;
  chip8.program_counter = address;
}

void Interpreter::skip_next_instruction_if_equal() {
  const u8 Vx = this->get_x();
  const u8 byte = chip8.current_opcode & 0x00FFu;
  if (chip8.general_purpose_variable_registers[Vx] == byte) {
    chip8.program_counter += 2;
  }
}

void Interpreter::skip_next_instruction_if_not_equal() {
  const u8 Vx = this->get_x();
  const u8 byte = chip8.current_opcode & 0x00FFu;
  if (chip8.general_purpose_variable_registers[Vx] != byte) {
    chip8.program_counter += 2;
  }
}

void Interpreter::skip_next_instruction_if_vx_equal_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  if (chip8.general_purpose_variable_registers[Vx] ==
      chip8.general_purpose_variable_registers[Vy]) {
    chip8.program_counter += 2;
  }
}

void Interpreter::set_general_purpose_variable_registers() {
  const u8 Vx = this->get_x();
  const u8 byte = chip8.current_opcode & 0x00FFu;
  chip8.general_purpose_variable_registers[Vx] = byte;
}

void Interpreter::add_to_general_purpose_variable_registers() {
  const u8 Vx = this->get_x();
  const u8 byte = chip8.current_opcode & 0x00FFu;
  chip8.general_purpose_variable_registers[Vx] += byte;
}

void Interpreter::load_vy_in_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  chip8.general_purpose_variable_registers[Vx] =
      chip8.general_purpose_variable_registers[Vy];
}

void Interpreter::set_vx_to_bitwise_or_of_vx_and_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  chip8.general_purpose_variable_registers[Vx] |=
      chip8.general_purpose_variable_registers[Vy];
}

void Interpreter::set_vx_to_bitwise_and_of_vx_and_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  chip8.general_purpose_variable_registers[Vx] &=
      chip8.general_purpose_variable_registers[Vy];
}

void Interpreter::set_vx_to_bitwise_xor_of_vx_and_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  chip8.general_purpose_variable_registers[Vx] ^=
      chip8.general_purpose_variable_registers[Vy];
}

void Interpreter::add_vy_to_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  const u16 sum = chip8.general_purpose_variable_registers[Vy] +
                  chip8.general_purpose_variable_registers[Vx];

  chip8.general_purpose_variable_registers[0xF] = (sum > 255) ? 1 : 0;
  chip8.general_purpose_variable_registers[Vx] = sum & 0xFF;
}

void Interpreter::subtract_vy_from_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();

  chip8.general_purpose_variable_registers[0xF] =
      (chip8.general_purpose_variable_registers[Vx] >
       chip8.general_purpose_variable_registers[Vy])
          ? 1
          : 0;

  chip8.general_purpose_variable_registers[Vx] -=
      chip8.general_purpose_variable_registers[Vy];
}

void Interpreter::shift_vx_by_one_to_right() {
  const u8 Vx = get_x();

  // get the least-significant bit of Vx
  chip8.general_purpose_variable_registers[0xF] =
      (chip8.general_purpose_variable_registers[Vx] & 0x1u);

  // shift Vx one to the right
  chip8.general_purpose_variable_registers[Vx] >>= 1;
}

void Interpreter::set_vx_to_vy_minus_vx() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();

  chip8.general_purpose_variable_registers[0xF] =
      (chip8.general_purpose_variable_registers[Vy] >
       chip8.general_purpose_variable_registers[Vx])
          ? 1
          : 0;

  chip8.general_purpose_variable_registers[Vx] =
      chip8.general_purpose_variable_registers[Vy] -
      chip8.general_purpose_variable_registers[Vx];
}

void Interpreter::shift_vx_by_one_to_left() {
  const u8 Vx = get_x();

  // get the most-significant bit of Vx
  chip8.general_purpose_variable_registers[0xF] =
      (chip8.general_purpose_variable_registers[Vx] & 0x80u) >> 7u;

  // shift Vx one to the left
  chip8.general_purpose_variable_registers[Vx] <<= 1;
}

void Interpreter::skip_next_instruction_if_vx_not_equal_vy() {
  const u8 Vx = this->get_x();
  const u8 Vy = this->get_y();
  if (chip8.general_purpose_variable_registers[Vx] !=
      chip8.general_purpose_variable_registers[Vy]) {
    chip8.program_counter += 2;
  }
}

void Interpreter::set_index_register() {
  const u16 address = chip8.current_opcode & 0x0FFFu;
  chip8.index_register = address;
}

void Interpreter::jump_to_extended_v0_location() {
  const u16 address = chip8.current_opcode & 0x0FFFu;
  chip8.program_counter = chip8.general_purpose_variable_registers[0] + address;
}

void Interpreter::generate_random_number() {
  const u8 Vx = this->get_x();
  const u8 byte = chip8.current_opcode & 0x00FFu;
  chip8.general_purpose_variable_registers[Vx] =
      chip8.rand.get_random_number() & byte;
}

void Interpreter::draw_sprite() {
  const u16 Vx = chip8.general_purpose_variable_registers[this->get_x()];
  const u16 Vy = chip8.general_purpose_variable_registers[this->get_y()];
  const u16 height = chip8.current_opcode & 0x000F;
  u16 pixel;

  chip8.general_purpose_variable_registers[0xF] = 0;

  for (size_t y = 0; y < height; y++) {
    pixel = chip8.memory[chip8.index_register + y];
    for (size_t x = 0; x < 8; x++) {
      if (pixel & (0x80u >> x)) {
        u32 loc = x + Vx + (y + Vy) * chip8.display->get_width();
        if (chip8.display->operator[](loc)) {
          chip8.general_purpose_variable_registers[0xF] = 1;
        }
        chip8.display->operator[](loc) ^= 1;
      }
    }
  }
  chip8.draw_flag = true;
}

void Interpreter::skip_instruction_if_key_pressed() {
  const u8 Vx = this->get_x();
  if (chip8.keypad.is_pressed(chip8.general_purpose_variable_registers[Vx])) {
    chip8.program_counter += 2;
  }
}

void Interpreter::skip_instruction_if_key_is_not_pressed() {
  const u8 Vx = this->get_x();
  if (!chip8.keypad.is_pressed(chip8.general_purpose_variable_registers[Vx])) {
    chip8.program_counter += 2;
  }
}

void Interpreter::set_vx_to_delay_timer() {
  const u8 Vx = this->get_x();
  chip8.general_purpose_variable_registers[Vx] = chip8.delay_timer;
}

void Interpreter::wait_for_key_pressed() {
  bool key_pressed = false;
  const u8 Vx = this->get_x();
  for (u8 i = 0; i < chip8.keypad.size(); i++) {
    if (chip8.keypad.is_pressed(i)) {
      chip8.general_purpose_variable_registers[Vx] = i;
      key_pressed = true;
    }
  }

  if (!key_pressed) {
    chip8.program_counter -= 2;
  }
}

void Interpreter::set_delay_timer_to_vx() {
  const u8 Vx = this->get_x();
  chip8.delay_timer = chip8.general_purpose_variable_registers[Vx];
}

void Interpreter::set_sound_timer_to_vx() {
  const u8 Vx = this->get_x();
  chip8.sound_timer = chip8.general_purpose_variable_registers[Vx];
}

void Interpreter::add_i_to_vx() {
  const u8 Vx = this->get_x();
  chip8.index_register += chip8.general_purpose_variable_registers[Vx];
}

void Interpreter::set_i_to_sprite_character_in_vx() {
  const u8 Vx = this->get_x();
  chip8.index_register = chip8.general_purpose_variable_registers[Vx] * 5;
}

void Interpreter::store_binary_coded_decimal_of_vx() {
  const u8 Vx = this->get_x();
  chip8.memory[chip8.index_register] =
      chip8.general_purpose_variable_registers[Vx] / 100;
  chip8.memory[chip8.index_register + 1] =
      (chip8.general_purpose_variable_registers[Vx] / 10) % 10;
  chip8.memory[chip8.index_register + 2] =
      (chip8.general_purpose_variable_registers[Vx] % 100) % 10;
}

void Interpreter::store_registers_at_i() {
  const u8 Vx = this->get_x();
  for (u8 i = 0; i <= Vx; ++i) {
    chip8.memory[chip8.index_register + i] =
        chip8.general_purpose_variable_registers[i];
  }
}

void Interpreter::load_registers_from_i() {
  const u8 Vx = this->get_x();
  for (u8 i = 0; i <= Vx; ++i) {
    chip8.general_purpose_variable_registers[i] =
        chip8.memory[chip8.index_register + i];
  }
}