cmake_minimum_required(VERSION 3.12)

project(BitVaram LANGUAGES CXX)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.12)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include_directories(include)

add_executable(BitVaram
    src/BitVaram.cpp
 "include/lexer.h" "src/lexer.cpp" "src/parser.cpp" "include/preprocessor.h" "src/preprocessor.cpp" "include/IRcompiler.h" "src/IRcompiler.cpp")

# TODO: Добавьте тесты и цели, если нужно
