set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_POLICY_DEFAULT_CMP0048 OLD)
add_subdirectory(../gtest ${CMAKE_BINARY_DIR}/gtest)

set(SOURCES
    nfa.cc
    parse.cc
)

add_executable(test_magex ${SOURCES})
target_link_libraries(test_magex
    gtest gtest_main
    libmagex
)
set_target_properties(test_magex PROPERTIES
  RUNTIME_OUTPUT_DIR ../bin
)
add_test(NAME test_magex COMMAND test_magex)
