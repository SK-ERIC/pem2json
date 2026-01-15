cmake_minimum_required(VERSION 3.20)
project(pem2json VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project version for C++ code
add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}")

include_directories(${CMAKE_SOURCE_DIR})

add_executable(pem2json main.cpp
        include/pem2json.hpp)

install(TARGETS pem2json
        RUNTIME DESTINATION bin)

# Testing
enable_testing()
add_executable(pem2json_tests
        tests/test_pem2json.cpp
        include/pem2json.hpp)

add_test(NAME UnitTests COMMAND pem2json_tests)
