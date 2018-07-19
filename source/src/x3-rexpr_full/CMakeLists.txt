add_library(rexpr
  src/rexpr.cpp)

target_include_directories(rexpr PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(rexpr.test
  test/parse_rexpr_test.cpp)
target_link_libraries(rexpr.test
  rexpr
  ${CONAN_LIBS})
