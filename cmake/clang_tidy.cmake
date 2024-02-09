include(FindPythonInterp)
find_program(CLANG_TIDY NAMES "clang-tidy" HINTS "${CMAKE_SOURCE_DIR}/.venv/bin")

if(NOT CLANG_TIDY)
    message(WARNING "clang-tidy not found!")
else()
	message(STATUS "clang-tidy: " ${CLANG_TIDY})
	execute_process(COMMAND ${CLANG_TIDY} --version)
    message(STATUS "\n")

    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.clang-tidy")
        message(FATAL_ERROR "'${CMAKE_SOURCE_DIR}/.clang-tidy' configuration file not found!")
    endif()
endif()

function(setup_target_clang_tidy TARGET_NAME TARGET_FOLDERS)
    if(NOT CLANG_TIDY)
        message(WARNING "clang-tidy not found!")
        return()
    endif()

    if(NOT ${PYTHON_FOUND})
        message(WARNING "python3 not found!")
        return()
    endif()

    add_custom_target(${TARGET_NAME}_clang_tidy ALL
        COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/scripts/tools/run_clang_tidy.py
            -clang-tidy-binary ${CLANG_TIDY}
            -header-filter=.*
            ${TARGET_FOLDERS}
            # ${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running ${CLANG_TIDY} on ${TARGET_NAME}"
    )
endfunction()
