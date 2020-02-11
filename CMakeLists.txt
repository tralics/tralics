cmake_minimum_required (VERSION 3.0)
project (Tralics CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}/")

add_compile_options(-fdiagnostics-color=always -pedantic -Wall -Wextra -Wpedantic -O3)

file (GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
add_executable(tralics ${sources})

add_custom_target(alltests ALL 
    ../test/alltests ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
    DEPENDS tralics
    COMMENT "Running tests")