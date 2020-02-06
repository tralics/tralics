cmake_minimum_required (VERSION 3.0)
project (Tralics CXX)

set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_options(-fdiagnostics-color=always -Wall -Wextra -Wpedantic)

file (GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
add_executable(tralics ${sources})