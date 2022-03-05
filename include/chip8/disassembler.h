#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <string>

#include "chip8_types.h"

class Disassembler {
 public:
  Disassembler();
  ~Disassembler();

  void load_program(std::string filename);

 private:
  u16 program_counter;
  u16 maximum_address_count;
  std::streampos program_size;

  const int PROGRAM_STARTING_LOCATION;
};

#endif