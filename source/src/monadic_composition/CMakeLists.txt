add_library(chain
  chain.cpp
  chain.hpp)

add_executable(chain.test
  catch_main.cpp
  chain.test.cpp)

target_link_libraries(chain.test
  chain)
