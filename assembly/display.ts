const WIDTH = 64;
const HEIGHT = 32;

export class Display {
  private pixels: StaticArray<boolean> = new StaticArray<boolean>(
    WIDTH * HEIGHT
  );

  constructor() {
    this.clearScreen();
  }

  private clearScreen(): void {
    const length = this.pixels.length;
    for (let i = 0; i < length; i++) {
      this.pixels[i] = false;
    }
  }
}
