cmake_minimum_required(VERSION 3.28)
set(BINARY_NAME goku)
project(${BINARY_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/soul")
include_directories(BEFORE "${CMAKE_SOURCE_DIR}/soulsfml")

set(SOURCES 
    main.cpp 
    game.cpp
)

add_executable(${BINARY_NAME} ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    
    link_directories("${CMAKE_SOURCE_DIR}/debug/soul")
    link_directories("${CMAKE_SOURCE_DIR}/debug/soulsfml")
    target_compile_definitions(${BINARY_NAME} PRIVATE DEBUG_MODE)
    # target_compile_definitions(${BINARY_NAME} PRIVATE DEBUG_MODE_STATES)
    target_compile_definitions(${BINARY_NAME} PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_options(${BINARY_NAME} PRIVATE -g -O0 -Wdeprecated-declarations)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")

    link_directories("${CMAKE_SOURCE_DIR}/release/soul")
    link_directories("${CMAKE_SOURCE_DIR}/release/soulsfml")
    target_compile_options(${BINARY_NAME} PRIVATE -O3)  # Example flags for GCC/Clang

endif()

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${BINARY_NAME} PRIVATE sfml-system sfml-window sfml-graphics ImGui-SFML::ImGui-SFML OpenGL::GL GLEW::GLEW)

target_compile_features(${BINARY_NAME} PRIVATE cxx_std_20)
target_link_libraries(${BINARY_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(${BINARY_NAME} PRIVATE libsoul)
target_link_libraries(${BINARY_NAME} PRIVATE libsoulsfml)

install(TARGETS ${BINARY_NAME})