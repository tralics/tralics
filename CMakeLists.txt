cmake_minimum_required (VERSION 3.0)
project (Tralics CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}/")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/conan.cmake)
conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
conan_cmake_run(REQUIRES 
                spdlog/1.5.0
                BASIC_SETUP NO_OUTPUT_DIRS BUILD missing)

add_compile_options(-fdiagnostics-color=always -pedantic -Wall -Wextra -Wpedantic -O3)

find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "Using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

file (GLOB_RECURSE sources CONFIGURE_DEPENDS *.C)
file (GLOB_RECURSE headers CONFIGURE_DEPENDS *.h)

add_executable(tralics ${sources})
target_link_libraries (tralics ${CONAN_LIBS})

find_package(Doxygen REQUIRED dot)
if (${DOXYGEN_FOUND})
  add_custom_target (docs ALL doxygen WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Building documentation")
endif()

add_custom_target(alltests ALL 
    ../test/alltests ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
    DEPENDS tralics
    COMMENT "Running tests")