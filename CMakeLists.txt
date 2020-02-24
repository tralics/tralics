cmake_minimum_required (VERSION 3.0)
project (Tralics CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}/")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES spdlog/1.5.0
                BASIC_SETUP NO_OUTPUT_DIRS
                BUILD missing)

add_compile_options(-fdiagnostics-color=always -pedantic -Wall -Wextra -Wpedantic -O3)

file (GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
add_executable(tralics ${sources})
target_link_libraries (tralics ${CONAN_LIBS})

add_custom_target(alltests ALL 
    ../test/alltests ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
    DEPENDS tralics
    COMMENT "Running tests")