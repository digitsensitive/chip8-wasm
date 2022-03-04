#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <string>

#include "chip8.h"
#include "disassembler.h"
#include "keypad.h"
#include "renderer.h"

enum VirtualMachineState { kRomLoading = 1 << 0, kRomLoaded = 1 << 1 };

class VirtualMachine {
 public:
  VirtualMachine();
  ~VirtualMachine();

  bool boot();
  bool load_program(const std::string& program_file);
  void flash_program(char* data);
  void disassemble_program(char* data);
  void run();
  void process_input();
  void shutdown_systems();

  void change_game_color(u8 red, u8 green, u8 blue) {
    this->renderer->set_color(red, green, blue);
  }

 private:
  bool is_running;
  u8 emu_state_{0};
  inline void ToggleState(u8 state) { emu_state_ ^= state; }
  inline bool CheckState(u8 state) { return emu_state_ & state; }

  // Frame Rate
  const int FPS = 500;
  const int frame_delay = 1000 / FPS;
  Uint32 frame_start;
  int frame_time;

  Renderer* renderer;
  Chip8 chip8;
  Disassembler disassembler;
};

#endif