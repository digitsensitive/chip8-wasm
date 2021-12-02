export class Chip8 {
  private delayTimer: u8;
  private generalPurposeVariableRegisters: StaticArray<u8> =
    new StaticArray<u8>(16);
  private indexRegister: u16;
  private memory: StaticArray<u8> = new StaticArray<u8>(4096);
  private programCounter: u16;
  private soundTimer: u8;
  private stack: StaticArray<u16> = new StaticArray<u16>(16);

  constructor() {
    this.init();
  }

  private init(): void {
    // Reset delay timer
    this.delayTimer = 0;

    // Init general purpose variable registers V0 - VF
    const gpvrLength = this.generalPurposeVariableRegisters.length;
    for (let i = 0; i < gpvrLength; i++) {
      this.generalPurposeVariableRegisters[i] = 0;
    }

    // Reset index (address) register I to zero
    this.indexRegister = 0;

    // Init Memory
    const memoryLength = this.memory.length;
    for (let i = 0; i < memoryLength; i++) {
      this.memory[i] = 0;
    }

    // Program counter starts at 0x200 (Start adress program)
    this.programCounter = 0x200;

    // Reset sound timer
    this.soundTimer = 0;

    // Init stack array
    const stackLength = this.stack.length;
    for (let i = 0; i < stackLength; i++) {
      this.stack[i] = 0;
    }
  }
}
