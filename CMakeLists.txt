cmake_minimum_required(VERSION 3.20)
project(pem2json VERSION 0.1.0 LANGUAGES CXX)

option(PEM2JSON_BUILD_TESTS "Build pem2json tests" ON)
option(PEM2JSON_BUILD_CLI "Build pem2json CLI executable" ON)
option(PEM2JSON_STRICT_BUILD "Enable strict warnings as errors" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (PEM2JSON_STRICT_BUILD)
    if (MSVC)
        add_compile_options(/W4 /permissive- /WX)
    else ()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif ()
else ()
    if (MSVC)
        add_compile_options(/W4 /permissive-)
    else ()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif ()
endif ()

set(PEM2JSON_VERSION "${PROJECT_VERSION}" CACHE STRING "Version string for pem2json")
add_compile_definitions(PROJECT_VERSION="${PEM2JSON_VERSION}")

include(GNUInstallDirs)

include_directories(${CMAKE_SOURCE_DIR})

add_library(pem2json
        pem2json.cpp
        include/pem2json.hpp)

if (PEM2JSON_BUILD_CLI)
    add_executable(pem2json-cli
            main.cpp)

    target_link_libraries(pem2json-cli PRIVATE pem2json)

    set_target_properties(pem2json-cli PROPERTIES OUTPUT_NAME pem2json)

    install(TARGETS pem2json-cli
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif ()

install(TARGETS pem2json
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES include/pem2json.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pem2json)

if (PEM2JSON_BUILD_TESTS)
    enable_testing()

    add_executable(pem2json_tests
            tests/test_pem2json.cpp)

    target_link_libraries(pem2json_tests PRIVATE pem2json)

    add_test(NAME UnitTests COMMAND pem2json_tests)
endif ()

set(CPACK_PACKAGE_NAME "pem2json")
set(CPACK_PACKAGE_VERSION "${PEM2JSON_VERSION}")
set(CPACK_PACKAGE_CONTACT "pem2json")

if (WIN32)
    set(CPACK_GENERATOR "ZIP")
else ()
    set(CPACK_GENERATOR "TGZ")
endif ()

include(CPack)
