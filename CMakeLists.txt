cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(network-simulator)

add_library(network-simulator-lib INTERFACE
    "include/packet.hpp"
    "include/packet_pull.hpp"
    "include/client.hpp"
    "include/ip_client.hpp"
)
target_include_directories(network-simulator-lib INTERFACE "include/")
target_compile_features(network-simulator-lib INTERFACE cxx_std_17)

include(FetchContent)
FetchContent_Declare(betterstring
    GIT_REPOSITORY https://github.com/NUCLEAR-BOMB/betterstring.git
    GIT_TAG eca85b0140151d3c13322b27bc9ad9b969cd3581
)
FetchContent_MakeAvailable(betterstring)
target_link_libraries(network-simulator-lib INTERFACE betterstring)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    target_sources(network-simulator-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vis.natvis")
endif()

add_executable(network-simulator
    "src/main.cpp"
)
target_link_libraries(network-simulator PRIVATE network-simulator-lib)

add_subdirectory(test)