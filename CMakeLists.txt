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
                fmt/7.1.2
                ms-gsl/3.1.0
                spdlog/1.8.1
                ctre/3.3.2
                BASIC_SETUP NO_OUTPUT_DIRS BUILD missing)

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external/utfcpp-3.1.2/source)

add_compile_options(-fdiagnostics-color=always -pedantic -Wall)

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

file (GLOB_RECURSE sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/packages/*.cpp)

add_executable(tralics ${sources})
target_link_libraries (tralics ${CONAN_LIBS})

add_custom_target(alltests ALL 
    ../test/alltests ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
    DEPENDS tralics
    COMMENT "Running tests")
