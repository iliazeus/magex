find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCES
    nfa.cc
    parse.cc
)

add_executable(test_magex ${SOURCES})
target_link_libraries(test_magex
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    libmagex
)
add_test(NAME test_magex COMMAND test_magex)
