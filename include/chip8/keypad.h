#ifndef KEYPAD_H
#define KEYPAD_H

#include <array>

/*
    Keypad class:
    Chip-8 hexadecimal keypad with 16 keys, ranging from 0 to F.

    +----------------------+-----------------------+
    |   Chip-8 Keyboard    |    Keyboard Map       |
    +----------------------+-----------------------+
    |   1    2    3    C   |    1    2    3    4   |
    |   4    5    6    D   |    Q    W    E    R   |
    |   7    8    9    E   |    A    S    D    F   |
    |   A    0    B    F   |    Z    X    C    V   |
    +----------------------+-----------------------+
*/

class Keypad {
 public:
  u8& operator[](u8 index) {
    if (index > keys.size()) {
      throw std::out_of_range("keys[] : index out of range");
    }
    return keys[index];
  }

  bool is_pressed(u8 key) {
    if (key > keys.size()) {
      throw std::out_of_range("IsPressed : keys[] : index out of range");
    }
    return keys[kKeyMap[key]] == 1;
  }

  void set_key(u8 key, bool state) { this->keys[key] = state; }

  u8 size() { return 16; }

 private:
  std::array<u8, 256> keys{};

  std::array<u8, 16> kKeyMap = {
      0x31,  // 1
      0x32,  // 2
      0x33,  // 3
      0x34,  // 4
      0x71,  // Q
      0x77,  // W
      0x65,  // E
      0x72,  // R
      0x61,  // A
      0x73,  // S
      0x64,  // D
      0x66,  // F
      0x7a,  // Z
      0x78,  // X
      0x63,  // C
      0x76,  // V
  };
};

#endif