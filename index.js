import { wasmBrowserInstantiate } from "./instantiateWasm.js";

const runWasmAdd = async () => {
  // Instantiate our wasm module
  const wasmModule = await wasmBrowserInstantiate("./build/optimized.wasm");

  // Get our exports object, with all of our exported Wasm Properties
  const exports = wasmModule.instance.exports;

  console.log(exports.add(24, 24));
};
runWasmAdd();
