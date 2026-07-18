cmake_minimum_required(VERSION 3.14)
project(publictransportlb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(SOURCES
src/sim.cpp
src/Tram.cpp
src/Transport.cpp
src/Metro.cpp
)

add_executable(sim ${SOURCES})

target_include_directories(sim PRIVATE headers)

# option for testing
option(BUILD_TESTS "Build and run unit tests" OFF)
# for testing
if(BUILD_TESTS)
    message(STATUS "Unit tests are ENABLED - Fetching Catch2...")
    include(FetchContent)
    FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.15.2 # Stable version
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(unit_tests
        tests/test_transport.cpp
        src/Transport.cpp
    )
    target_include_directories(unit_tests PRIVATE headers)
    target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

    # ---- Automatically register tests with CMake's CTest ----
    include(Catch)
    catch_discover_tests(unit_tests)
else()
    message(STATUS "Unit tests are DISABLED")
endif()

# target_link_libraries(main)