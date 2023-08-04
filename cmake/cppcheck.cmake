include(FindPythonInterp)
find_program(CPPCHECK NAMES "cppcheck")

if(NOT CPPCHECK)
    message(WARNING "cppcheck not found!")
else()
	message(STATUS "cppcheck: " ${CPPCHECK})
    execute_process(COMMAND ${CPPCHECK} --version)
    message(STATUS "\n")

endif()

function(setup_target_cppcheck TARGET)
    if(NOT CPPCHECK)
        message(WARNING "cppcheck not found!")
        return()
    endif()
    add_custom_target(${TARGET}_cppcheck ALL
        COMMAND ${CPPCHECK} ${ARGN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running ${CPPCHECK} on ${TARGET}"
    )
endfunction()
