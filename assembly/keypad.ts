/*
const KEYCODES_MAPPED_TO_HEXADECIMAL = {
  49: 0x1, // 1
  50: 0x2, // 2
  51: 0x3, // 3
  52: 0xc, // 4
  81: 0x4, // Q
  87: 0x5, // W
  69: 0x6, // E
  82: 0xd, // R
  65: 0x7, // A
  83: 0x8, // S
  68: 0x9, // D
  70: 0xe, // F
  90: 0xa, // Z
  89: 0x0, // Y -> German keyboard
  88: 0x0, // X -> English keyboard
  67: 0xb, // C
  86: 0xf, // V
};
*/
export class Keypad {
  private keys: StaticArray<boolean> = new StaticArray<boolean>(16);

  constructor() {
    this.init();
  }

  private init(): void {
    const keysLength = this.keys.length;
    for (let i = 0; i < keysLength; i++) {
      this.keys[i] = false;
    }
  }

  private keyDown(key: number): void {
    this.keys[key] = true;
  }

  private keyUp(key: number): void {
    this.keys[key] = false;
  }

  private isKeyDown(key: number): boolean {
    return this.keys[key];
  }
}
