import { wasmBrowserInstantiate } from "./instantiateWasm.js";

/*const WIDTH = 64;
const HEIGHT = 32;
const SCALE = 15;

const canvas = document.getElementById("canvas");
canvas.width = WIDTH * SCALE;
canvas.height = HEIGHT * SCALE;

const ctx = canvas.getContext("2d");
ctx.fillStyle = "black";
ctx.fillRect(0, 0, WIDTH * SCALE, HEIGHT * SCALE);

const input = document.getElementById("fileinput");*/

const runWasmAdd = async () => {
  // Instantiate our wasm module
  const wasmModule = await wasmBrowserInstantiate("./build/optimized.wasm");

  // Get our exports object, with all of our exported Wasm Properties
  const exports = wasmModule.instance.exports;
};

/*function mainloop(chip8) {
  // Only draw every few ticks
  for (let i = 0; i < TICKS_PER_FRAME; i++) {
    chip8.tick();
  }
  chip8.tick_timers();

  // Clear the canvas before drawing
  ctx.fillStyle = "black";
  ctx.fillRect(0, 0, WIDTH * SCALE, HEIGHT * SCALE);
  // Set the draw color back to white before we render our frame
  ctx.fillStyle = "white";
  chip8.draw_screen(SCALE);

  anim_frame = window.requestAnimationFrame(() => {
    mainloop(chip8);
  });
}*/

runWasmAdd();
