cmake_minimum_required(VERSION 3.28.1)
project(Goku)

add_subdirectory(soul)
add_subdirectory(soulsfml)
add_subdirectory(game)
add_subdirectory(tests)
add_subdirectory(benchmarks)
add_subdirectory(test_sprite)
add_subdirectory(test_spriteanim)
add_subdirectory(test_animable)
add_subdirectory(test_state_animable)
add_subdirectory(test_entity_states)

# Enable Doxygen
find_package(Doxygen REQUIRED)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
        COMMAND Doxygen::doxygen ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
    
    # cmake --build build --target clean-doc
    add_custom_target(clean-doc
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/docs
        COMMENT "Cleaning Doxygen-generated documentation"
    )
endif()