include(FindPythonInterp)
find_program(CLANG_FORMAT NAMES "clang-format")

if(NOT (CLANG_FORMAT OR PYTHON_FOUND))
    message(WARNING "clang-format not found!")
else()
	message(STATUS "clang-format: " ${CLANG_FORMAT})
	execute_process(COMMAND ${CLANG_FORMAT} --version)
    message(STATUS "\n")

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.clang-format")
        message(FATAL_ERROR "'${CMAKE_SOURCE_DIR}/.clang-format' configuration file not found!")
    endif()
endif()

function(setup_target_clang_format TARGET)
    if(NOT (CLANG_FORMAT OR PYTHON_FOUND))
        message(WARNING "clang-format not found!")
        return()
    endif()
    add_custom_target(${TARGET}_clang_format ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/tools/run_clang_format.py -i ${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running ${CLANG_FORMAT} on ${TARGET}"
    )
endfunction()
