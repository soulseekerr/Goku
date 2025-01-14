cmake_minimum_required(VERSION 3.28.1)
set(TESTS_NAME test_spriteanim)
project(${TESTS_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories((BEFORE "../debug/_deps/googletest-src/googletest/include/gtest"))
include_directories(BEFORE "/Users/soulseeker/Projects/GitHub/Goku/soul")
include_directories(BEFORE "/Users/soulseeker/Projects/GitHub/Goku/soulsfml")

include_directories(BEFORE "../debug/_deps/sfml-src/include")
include_directories(BEFORE "../debug/_deps/imgui-src/include")
include_directories(BEFORE "../debug/_deps/imgui-src")
include_directories(BEFORE "../debug/_deps/imgui-sfml-src")

set(SOURCES main.cpp)

add_executable(${TESTS_NAME} ${SOURCES})

message("Building in Debug mode")
target_compile_definitions(${TESTS_NAME} PRIVATE DEBUG_MODE)
target_compile_options(${TESTS_NAME} PRIVATE -g -O0)
link_directories("/Users/soulseeker/Projects/GitHub/Goku/debug/soul")

target_link_libraries(${TESTS_NAME} GTest::gtest_main libsoul libsoulsfml sfml-system sfml-window sfml-graphics)