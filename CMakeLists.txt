cmake_minimum_required(VERSION 3.10)
project(publictransportlb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
    src/sim.cpp
    src/Tram.cpp
    src/Transport.cpp
    src/Metro.cpp
)

add_executable(sim ${SOURCES})

target_include_directories(sim PRIVATE headers)

# target_link_libraries(main)