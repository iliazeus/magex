cmake_minimum_required(VERSION 3.0)


project(libmagex
    VERSION 0.0.1
    LANGUAGES C CXX
)


include_directories(src)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")


set(CMAKE_CONFIGURATION_TYPES Debug Profile Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_C_FLAGS "-std=c99")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -Wall")

set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} -g -pg")
set(CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} -g -pg")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2")


enable_testing()


add_subdirectory(src)
add_subdirectory(tests)
