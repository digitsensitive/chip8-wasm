import { Display } from "./display";
import { FONTSET } from "./font";
import { Keypad } from "./keypad";

export class Chip8 {
  private delayTimer: u8;
  private display: Display;
  private generalPurposeVariableRegisters: StaticArray<u8> =
    new StaticArray<u8>(16);
  private indexRegister: u16;
  private keypad: Keypad;
  private memory: StaticArray<u8> = new StaticArray<u8>(4096);
  private programCounter: u16;
  private soundTimer: u8;
  private stack: StaticArray<u16> = new StaticArray<u16>(16);

  constructor() {
    this.init();
    this.loadFontIntoMemory();
  }

  private init(): void {
    // Reset delay timer
    this.delayTimer = 0;

    // Init Display
    this.display = new Display();

    // Init general purpose variable registers V0 - VF
    const GPVR_LENGTH = this.generalPurposeVariableRegisters.length;
    for (let i = 0; i < GPVR_LENGTH; i++) {
      this.generalPurposeVariableRegisters[i] = 0;
    }

    // Reset index (address) register I to zero
    this.indexRegister = 0;

    // Init Keypad
    this.keypad = new Keypad();

    // Init Memory
    const MEMORY_LENGTH = this.memory.length;
    for (let i = 0; i < MEMORY_LENGTH; i++) {
      this.memory[i] = 0;
    }

    // Program counter starts at 0x200 (Start adress program)
    this.programCounter = 0x200;

    // Reset sound timer
    this.soundTimer = 0;

    // Init stack array
    const STACK_LENGTH = this.stack.length;
    for (let i = 0; i < STACK_LENGTH; i++) {
      this.stack[i] = 0;
    }
  }

  private loadFontIntoMemory(): void {
    const FONTSET_LENGTH = FONTSET.length;
    for (let i = 0; i < FONTSET_LENGTH; i++) {
      this.memory[i] = FONTSET[i];
    }
  }
}
