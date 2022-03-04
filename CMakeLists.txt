# Set the minimum required version of cmake for this project
cmake_minimum_required(VERSION 3.18.1)

# Define the C++ standard whose features we request to build this project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the project version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_REVISION 0)

# Set the project name with the corresponding version
project(chip8-wasm VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION})

# Add SDL2 Library
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

# Add main executable to our project
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/chip8.cpp
  src/virtual-machine.cpp
  src/renderer.cpp
  src/disassembler.cpp
  src/interpreter.cpp
  src/random.cpp)

# Add the include directories (= our header files) to our target
target_include_directories(${PROJECT_NAME} PRIVATE 
  include
  ${SDL2_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIBRARIES}
)