set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")  # replace "g++-11" with your version
cmake_minimum_required(VERSION 3.26)
project(paradigms_4)

set(CMAKE_CXX_STANDARD 20)

add_executable(paradigms_4 main.cpp)
