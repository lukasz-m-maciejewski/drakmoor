cmake_minimum_required (VERSION 3.0.0)
project(drakmoor)

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
  )

add_executable(drakmoor-test
  )

target_link_libraries(drakmoor-test drakmoor)
