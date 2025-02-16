cmake_minimum_required(VERSION 3.28)
set(MY_LIB_NAME libsoulsfml)
project(${MY_LIB_NAME} LANGUAGES CXX)

# Set the C and C++ compilers to Clang 19
# set(CMAKE_C_COMPILER /opt/homebrew/Cellar/llvm/19.1.7/bin/clang)
# set(CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/19.1.7/bin/clang++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_LIBRARIES "-stdlib=libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20 -g -O0 -Wall -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1 -fprofile-instr-generate -fcoverage-mapping")

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG MASTER)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(IMGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8)
FetchContent_MakeAvailable(IMGUI)

FetchContent_Declare(imgui-sfml
    # GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    # GIT_TAG 82dc2033e51b8323857c3ae1cf1f458b3a933c35
    # GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    # GIT_TAG 2.6.x)
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG master)
message(STATUS "Fetching ImGui-SFML...")
set(GL_SILENCE_DEPRECATION "")
set(IMGUI_DIR "/Users/soulseeker/Projects/GitHub/Goku/debug/_deps/imgui-src")
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
FetchContent_MakeAvailable(imgui-sfml)

include_directories(BEFORE "/Users/soulseeker/Projects/GitHub/Goku/soul")
include_directories(BEFORE "/Users/soulseeker/Projects/GitHub/Goku/soulsfml")
include_directories(BEFORE "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")


set(SOURCES
    gamewindow.cpp
    texture2d.cpp
    sprite2d.cpp
    spriteanimation.cpp
    assetmanager.cpp
    shader.cpp
    animable.cpp
    state.cpp
    gui.cpp
    entitymanager.cpp
    player.cpp
    fireball.cpp
)

add_library(${MY_LIB_NAME} SHARED ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")

    include_directories(BEFORE "../debug/_deps/sfml-src/include")
    include_directories(BEFORE "../debug/_deps/imgui-src/include")
    include_directories(BEFORE "../debug/_deps/imgui-src")
    include_directories(BEFORE "../debug/_deps/imgui-sfml-src")
    include_directories(BEFORE "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

    target_compile_definitions(${MY_LIB_NAME} PRIVATE DEBUG_MODE)
    # target_compile_definitions(${MY_LIB_NAME} PRIVATE DEBUG_MODE_STATES)
    target_compile_options(${MY_LIB_NAME} PRIVATE -g -O0 -Wreturn-stack-address -fprofile-instr-generate -fcoverage-mapping -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")

    include_directories(BEFORE "../release/_deps/sfml-src/include")
    include_directories(BEFORE "../release/_deps/imgui-src/include")
    include_directories(BEFORE "../release/_deps/imgui-src")
    include_directories(BEFORE "../release/_deps/imgui-sfml-src")
    include_directories(BEFORE "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

    target_compile_options(${MY_LIB_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
endif()

find_package(OpenGL REQUIRED)

# target_link_libraries(${MY_LIB_NAME} PRIVATE libsoul sfml-system sfml-window sfml-graphics ImGui-SFML::ImGui-SFML OpenGL::GL)
target_link_libraries(${MY_LIB_NAME} PRIVATE libsoul SFML::System SFML::Window SFML::Graphics ImGui-SFML::ImGui-SFML OpenGL::GL)
target_compile_features(${MY_LIB_NAME} PRIVATE cxx_std_20)