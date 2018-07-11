cmake_minimum_required (VERSION 3.0.0)
execute_process(COMMAND /usr/bin/conan install ${CMAKE_SOURCE_DIR}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

project(drakmoor)
set(CONAN_SYSTEM_INCLUDES "On")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wall \
-Wextra \
-Wconversion \
-Wfloat-equal \
-Wunreachable-code \
-Wswitch-enum \
-Wshadow \
-Wnon-virtual-dtor \
-Wsuggest-override \
-std=c++17 \
-pedantic \
-g \
-Werror")

add_library(drakmoor
  src/function_expression.cpp
  )

add_executable(drakmoor-test
  src/catch.main.cpp
  src/function_expression.test.cpp
  )

target_link_libraries(drakmoor-test drakmoor)

add_test(NAME drakmoor-basic
  COMMAND bin/drakmoor-test
  )

add_executable(x3_roman_numeral_example
  src/x3_roman_numeral_example.cpp)

add_subdirectory(src/x3-minimal-ast)
