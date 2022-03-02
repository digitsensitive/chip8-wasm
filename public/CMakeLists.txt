# Set the minimum required version of cmake for this project
cmake_minimum_required(VERSION 3.18.1)

# Set the default inform CMake to use the right C++11 flag for the current compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_REVISION 0)

# Set the project name with the corresponding version
project(chip8-wasm VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION})

################################
# SDL2
################################
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

################################
# Add the executable
################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public")
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/chip8.cpp
  src/virtual-machine.cpp
  src/renderer.cpp
  src/disassembler.cpp
  src/random.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
  include
  ${SDL2_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIBRARIES}
)


set(CMAKE_EXECUTABLE_SUFFIX ".js")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='['_main', '_load_game', '_change_game_color']' -s EXPORTED_RUNTIME_METHODS='[ccall, cwrap]'")

