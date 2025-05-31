cmake_minimum_required(VERSION 3.28.1)
set(TESTS_NAME gokutests)
project(${TESTS_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -Wextra -pedantic") # -Werror
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

# Use google test for unit tests    
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories((BEFORE "../debug/_deps/googletest-src/googletest/include/gtest"))
include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")
include_directories(BEFORE "${CMAKE_SOURCE_DIR}/soul")
include_directories(BEFORE "${CMAKE_SOURCE_DIR}/soulsfml")
include_directories(BEFORE "../debug/_deps/sfml-src/include")
include_directories(BEFORE "../debug/_deps/imgui-src/include")
include_directories(BEFORE "../debug/_deps/imgui-src")
include_directories(BEFORE "../debug/_deps/imgui-sfml-src")

set(SOURCES_TEST 
    logger_test.cpp
    datetime_test.cpp
    entity_test.cpp
    time_elapsed_test.cpp
    texture_test.cpp
    sprite_test.cpp
    safe_numeric_test.cpp
    singleton_test.cpp
    fireball_test.cpp
    hashset_test.cpp
    collision_test.cpp
)

# Testing with google test
enable_testing()
add_executable(${TESTS_NAME} ${SOURCES_TEST})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${TESTS_NAME} PRIVATE DEBUG_MODE)
    target_compile_definitions(${TESTS_NAME} PRIVATE DEBUG_MODE_STATES)
    target_compile_options(${TESTS_NAME} PRIVATE -g -O0)
    link_directories("${CMAKE_SOURCE_DIR}/debug/soul")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${TESTS_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
    link_directories("${CMAKE_SOURCE_DIR}/release/soul")
endif()

target_link_libraries(${TESTS_NAME} GTest::gtest_main libsoul libsoulsfml sfml-system sfml-window sfml-graphics)

include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})