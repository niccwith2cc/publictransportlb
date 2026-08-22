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
    src/Map.cpp
    src/Route.cpp
)

add_executable(sim ${SOURCES})
target_include_directories(sim PRIVATE headers)

option(BUILD_TESTS "Build and run unit tests" OFF)
option(BUILD_MAP_TOOLS "Build OSM PBF converter" OFF)

if(BUILD_MAP_TOOLS)
    message(STATUS "Map tools are ENABLED - Fetching libosmium and protozero...")
    include(FetchContent)

    FetchContent_Declare(
        protozero
        GIT_REPOSITORY https://github.com/mapbox/protozero.git
        GIT_TAG v1.8.1
    )
    FetchContent_Declare(
        libosmium
        GIT_REPOSITORY https://github.com/osmcode/libosmium.git
        GIT_TAG v2.23.1
    )

    FetchContent_GetProperties(protozero)
    if(NOT protozero_POPULATED)
        FetchContent_Populate(protozero)
    endif()

    FetchContent_GetProperties(libosmium)
    if(NOT libosmium_POPULATED)
        FetchContent_Populate(libosmium)
    endif()

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    set(PROTOZERO_INCLUDE_DIR "${protozero_SOURCE_DIR}/include" CACHE PATH "" FORCE)
    set(OSMIUM_INCLUDE_DIR "${libosmium_SOURCE_DIR}/include" CACHE PATH "" FORCE)

    find_package(Osmium 2.13 REQUIRED COMPONENTS pbf)
    find_package(Threads REQUIRED)
    find_package(ZLIB REQUIRED)

    add_executable(pbf_converter tools/pbf_converter.cpp src/Map.cpp)
    target_include_directories(pbf_converter PRIVATE headers)
    target_include_directories(pbf_converter SYSTEM PRIVATE ${OSMIUM_INCLUDE_DIRS})
    target_link_libraries(pbf_converter PRIVATE ${OSMIUM_PBF_LIBRARIES} ZLIB::ZLIB Threads::Threads)
    set_pthread_on_target(pbf_converter)
else()
    message(STATUS "Map tools are DISABLED")
endif()

if(BUILD_TESTS)
    message(STATUS "Unit tests are ENABLED - Fetching Catch2...")
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.2
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(unit_tests
        tests/test_transport.cpp
        tests/test_map.cpp
        tests/test_route.cpp
        src/Transport.cpp
        src/Map.cpp
        src/Route.cpp
    )
    target_include_directories(unit_tests PRIVATE headers)
    target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

    include(Catch)
    catch_discover_tests(unit_tests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
    message(STATUS "Unit tests are DISABLED")
endif()
