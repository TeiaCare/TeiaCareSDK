set(VIRTUAL_ENV_BINARY_DIR "${CMAKE_SOURCE_DIR}/.venv/bin")
find_program(CLANG_FORMAT NAMES "clang-format" HINTS ${VIRTUAL_ENV_BINARY_DIR})
find_package(Python3)

if(NOT CLANG_FORMAT)
    message(WARNING "clang-format not found!")
else()
	message(STATUS "clang-format: " ${CLANG_FORMAT})
	execute_process(COMMAND ${CLANG_FORMAT} --version)

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.clang-format")
        message(FATAL_ERROR "'${CMAKE_SOURCE_DIR}/.clang-format' configuration file not found!")
    endif()
endif()

function(setup_target_clang_format TARGET_NAME TARGET_FOLDERS)
    if(NOT CLANG_FORMAT)
        message(WARNING "clang-format not found!")
        return()
    endif()

    if(NOT ${PYTHON_FOUND})
        message(WARNING "python3 not found!")
        return()
    endif()

    add_custom_target(${TARGET_NAME}_clang_format ALL
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/scripts/tools/run_clang_format.py
            --clang-format-executable ${CLANG_FORMAT}
            --in-place
            --recursive
            ${TARGET_FOLDERS}
        COMMENT "Running ${CLANG_FORMAT} on ${TARGET_NAME}"
    )
endfunction()
