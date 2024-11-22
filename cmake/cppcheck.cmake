find_program(CPPCHECK NAMES "cppcheck")
find_package(Python3)

if(NOT CPPCHECK)
    message(WARNING "cppcheck not found!")
else()
	message(STATUS "cppcheck: " ${CPPCHECK})
    execute_process(COMMAND ${CPPCHECK} --version)
endif()

function(setup_target_cppcheck TARGET)
    if(NOT CPPCHECK)
        message(WARNING "cppcheck not found!")
        return()
    endif()

    if(NOT ${PYTHON_FOUND})
        message(WARNING "python3 not found!")
        return()
    endif()

    add_custom_target(${TARGET_NAME}_cppcheck ALL
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/scripts/tools/run_cppcheck.py
            ${CMAKE_BUILD_TYPE}
        DEPENDS ${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running ${CPPCHECK} on ${TARGET_NAME}"
    )
endfunction()
