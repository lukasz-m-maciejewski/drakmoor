add_library(funexpr-parser
  src/funexpr-parser.cpp)

target_include_directories(funexpr-parser
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(funexpr-parser.test
  test/catch.main.cpp
  test/funexpr-parser.test.cpp)

target_link_libraries(funexpr-parser.test
  funexpr-parser
  ${CONAN_LIBS})
