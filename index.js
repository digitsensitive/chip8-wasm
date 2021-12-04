import { wasmBrowserInstantiate } from "./instantiateWasm.js";

const runWasm = async () => {
  // Instantiate our wasm module
  const wasmModule = await wasmBrowserInstantiate("./build/optimized.wasm");

  // Get our exports object, with all of our exported Wasm Properties
  const exports = wasmModule.instance.exports;
};

runWasm();
