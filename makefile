chip8-wasm:
	em++ -O2 \
	./src/main.cpp \
	./src/chip8.cpp \
	./src/emulator.cpp \
	./src/window.cpp \
	./src/interpreter.cpp \
	-s --std=c++17 \
	-s WASM=1 \
	-s USE_SDL=2 \
	-s EXPORTED_FUNCTIONS='['_main', '_load_game']' \
	-s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
	-o ./build/chip8.js


