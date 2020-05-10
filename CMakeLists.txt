cmake_minimum_required (VERSION 3.0)
project (Tralics CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}/")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/conan.cmake)
conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
set(CONAN_SYSTEM_INCLUDES ON)
conan_cmake_run(REQUIRES
                fmt/6.2.0
                ms-gsl/3.0.1
                spdlog/1.5.0
                utfcpp/3.1
                BASIC_SETUP NO_OUTPUT_DIRS BUILD missing)

add_compile_options(-fdiagnostics-color=always -pedantic -Wall -Wextra -Wshadow
                    -Wpedantic -Wold-style-cast -Wunused -Wconversion
                    -Wsign-conversion -Wnull-dereference)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("-- Debug mode")
	add_compile_options(-g -O0)
else()
  message("-- Release mode")
  add_compile_options(-O3 -DNDEBUG)
endif()

find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "Using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

file (GLOB_RECURSE sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(tralics ${sources})
target_link_libraries (tralics ${CONAN_LIBS})

add_custom_target(alltests ALL 
    ../test/alltests ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
    DEPENDS tralics
    COMMENT "Running tests")
