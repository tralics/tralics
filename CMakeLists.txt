cmake_minimum_required (VERSION 3.12)
project (Tralics CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}/")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external/ctre-3.10.0/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external/fmt-12.1.0/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external/spdlog-1.17.0/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external/utfcpp-4.0.9/source)

add_compile_definitions(FMT_HEADER_ONLY SPDLOG_FMT_EXTERNAL)
add_compile_options(-fdiagnostics-color=always -pedantic -Wall)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Debug mode")
	add_compile_options(-g -O0)
else()
  message(STATUS "Release mode")
  add_compile_options(-O3 -DNDEBUG)
endif()

find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "Using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

file (GLOB_RECURSE sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/packages/*.cpp)

add_executable(tralics ${sources})

add_custom_target(alltests ALL
    ../test/alltests ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
    DEPENDS tralics
    COMMENT "Running tests")
