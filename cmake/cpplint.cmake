include(FindPythonInterp)
find_program(CPPLINT NAMES "cpplint")

if(NOT CPPLINT)
    message(WARNING "cpplint not found!")
else()
	message(STATUS "cpplint: " ${CPPLINT})
    execute_process(COMMAND ${CPPLINT} --version)
    message(STATUS "\n")

endif()

function(setup_target_cpplint TARGET)
    if(NOT CPPLINT)
        message(WARNING "cpplint not found!")
        return()
    endif()
    add_custom_target(${TARGET}_cpplint ALL
        COMMAND ${CPPLINT} "--extensions=cpp,hpp --root=${CMAKE_CURRENT_SOURCE_DIR}" ${ARGN}
        DEPENDS ${TARGET_SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running ${CPPLINT} on ${TARGET}"
    )
endfunction()
