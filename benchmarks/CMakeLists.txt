cmake_minimum_required(VERSION 3.28.1)
set(TESTS_NAME gokubenchmarks)
project(${TESTS_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

# Use google test for unit tests
include(FetchContent)

# Add Google Benchmark
FetchContent_Declare(
    benchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
)
FetchContent_MakeAvailable(benchmark)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories((BEFORE "../debug/_deps/benchmark-src/include/benchmark"))
include_directories(BEFORE "/Users/soulseeker/Projects/GitHub/Goku/soul")
include_directories(BEFORE "/Users/soulseeker/Projects/GitHub/Goku/soulsfml")
include_directories(BEFORE "../debug/_deps/sfml-src/include")
include_directories(BEFORE "../debug/_deps/imgui-src/include")
include_directories(BEFORE "../debug/_deps/imgui-src")
include_directories(BEFORE "../debug/_deps/imgui-sfml-src")

set(SOURCES_TEST 
    safe_numeric_benchmark.cpp
    hashset_benchmark.cpp
)

# Testing with google test
enable_testing()
add_executable(${TESTS_NAME} ${SOURCES_TEST})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${TESTS_NAME} PRIVATE DEBUG_MODE)
    target_compile_definitions(${TESTS_NAME} PRIVATE DEBUG_MODE_STATES)
    target_compile_options(${TESTS_NAME} PRIVATE -g -O0)
    link_directories("/Users/soulseeker/Projects/GitHub/Goku/debug/soul")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${TESTS_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
    link_directories("/Users/soulseeker/Projects/GitHub/Goku/release/soul")
endif()

target_link_libraries(${TESTS_NAME} benchmark::benchmark libsoul libsoulsfml sfml-system sfml-window sfml-graphics)