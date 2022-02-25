#include "disassembler.h"

#include <fstream>
#include <iostream>

Disassembler::Disassembler() : PROGRAM_STARTING_LOCATION(0x0200) {
  this->program_counter = 0;
  this->maximum_address_count = 0;
}

Disassembler::~Disassembler() {}

void Disassembler::load_program(std::string filename) {
  // create variable of type ifstream and connect it to our input file
  std::ifstream program_file(filename,
                             std::ios::in | std::ios::binary | std::ios::ate);

  // check if the file is open and associated with this stream object
  if (!program_file.is_open()) {
    throw std::string("Unable to load program. ");
  }

  // get size of the program
  this->program_size = program_file.tellg();
  this->maximum_address_count = PROGRAM_STARTING_LOCATION + this->program_size;

  std::cout << ";------------------------------------" << std::endl;
  std::cout << "; Program: " << filename << std::endl;
  std::cout << "; Size: " << this->program_size << " Bytes" << std::endl;
  std::cout << ";------------------------------------" << std::endl;

  program_file.close();
}
