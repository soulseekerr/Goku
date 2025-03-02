cmake_minimum_required(VERSION 3.28)
set(MY_LIB_NAME libsoul)
project(${MY_LIB_NAME} LANGUAGES CXX)

# Set the C and C++ compilers to Clang 19
# set(CMAKE_C_COMPILER /opt/homebrew/Cellar/llvm/19.1.7/bin/clang)
# set(CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/19.1.7/bin/clang++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/soul")

set(SOURCES 
    core/datetime.cpp
    core/logger.cpp
    ecs/entity.cpp
)

add_library(libfmt SHARED IMPORTED)
add_library(${MY_LIB_NAME} SHARED ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${MY_LIB_NAME} PRIVATE DEBUG_MODE)
    # target_compile_definitions(${MY_LIB_NAME} PRIVATE DEBUG_MODE_STATES)
    target_compile_options(${MY_LIB_NAME} PRIVATE -g -O0 -Wreturn-stack-address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${MY_LIB_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
endif()

find_package(fmt)

target_link_libraries(${MY_LIB_NAME} PRIVATE fmt::fmt)
target_compile_features(${MY_LIB_NAME} PRIVATE cxx_std_20)