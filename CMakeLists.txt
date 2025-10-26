cmake_minimum_required(VERSION 4.1.1)

project("rhyper")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("dep/zydis")

add_executable(${PROJECT_NAME} "src/main.cpp" "src/code_analysis/instruction.cpp" "src/code_analysis/basic_block.cpp" "src/code_analysis/code_analysis.cpp" "src/obfuscation/obfuscation.cpp" "src/obfuscation/mutation.cpp" "src/code_analysis/function.cpp" "src/code_analysis/assembler.cpp" "src/cryptography/cryptography.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE "Zydis")
