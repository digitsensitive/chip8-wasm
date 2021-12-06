# A CHIP-8 virtual machine written in C++ with WebAssembly

This project was created to deal with the creation of an interpreter in more detail
and to use [WebAssembly](https://webassembly.org) for the first time.

I have documented and described the development of the project in detail on the
following [article on Medium](https://medium.com/p/2e7c8749031d/edit).

Have fun reading through and coding!

# Prerequisites

- Install [Visual Studio Code](https://code.visualstudio.com)
- Install the [C++ extension for VS Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- Install the [Clang-Format extension for VS Code](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
- Install [Clang](https://clang.llvm.org) and ensure it is installed `clang --version`
- Get the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)

# Code formatter
Use of [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html). The style options are defined in the `.clang-format` file. For more details have a look at the official [Style Options](https://clang.llvm.org/docs/ClangFormatStyleOptions.html).

# Emscripten Compiler

The Emscripten Compiler Frontend (em++ für C++) is used to call the
Emscripten compiler from the command line.
For more details about the arguments look at the official
[Emscripten Compiler Frontend Reference](https://emscripten.org/docs/tools_reference/emcc.html#emcc-o2)

## To do list

- [x] Setup basic repository (C++, WebAssembly)
- [x] Setup basic VSCode configuration (C++, Clang)
- [x] Medium: Create history section
- [x] CHIP-8: Add memory
- [x] CHIP-8: Add registers
- [x] CHIP-8: Add stack
- [x] Medium: Add references section
- [x] CHIP-8: Add delay and sound registers
- [x] CHIP-8: Add program counter (PC)
- [x] CHIP-8: Add input (keypad)
- [x] CHIP-8: Add display
- [x] CHIP-8: Add fontset
- [x] Add roms
- [x] Medium: Write instructions section
- [] CHIP-8: Add code to load program into memory
- [] CHIP-8: Add code to emulate each cycle
- [] Add Prettier to package.json with prettier configurationa file
- [] Write disassembler (https://aimechanics.tech/2020/09/03/chip8-emulation-rom-disassembler)

- Read the following ressource: http://vanbeveren.byethost13.com/stuff/CHIP8.pdf?i=1
