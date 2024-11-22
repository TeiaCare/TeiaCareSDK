function(setup_docs TARGET_NAME TARGET_HEADERS TARGET_HEADERS_DIRECTORY DOXYFILE_IN)
    find_package(Doxygen REQUIRED)

    if (NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found. Unable to generate docs")
        return()
    endif()

    set(DOXYGEN_INPUT_DIR
        ${CMAKE_SOURCE_DIR}/sdk/examples/src
        ${TARGET_HEADERS_DIRECTORY}
        ${CMAKE_SOURCE_DIR}/README.md
    )
    string(REPLACE ";" " " DOXYGEN_INPUT_DIR "${DOXYGEN_INPUT_DIR}")

    set(DOXYGEN_EXAMPLE_DIR ${CMAKE_SOURCE_DIR}/sdk/examples/src)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    set(DOXYFILE_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/docs/logo/logo-small.png)
    set(DOXYFILE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/header.html)
    set(DOXYFILE_FOOTER ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/footer.html)

    set(DOXYFILE_EXTRA_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/doxygen-awesome-darkmode-toggle.js
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/doxygen-awesome-fragment-copy-button.js
    )
    string(REPLACE ";" " " DOXYFILE_EXTRA_FILES "${DOXYFILE_EXTRA_FILES}")

    set(DOXYFILE_EXTRA_STYLESHEET
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/doxygen-awesome.css
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/doxygen-awesome-sidebar-only.css
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/style/doxygen-awesome-sidebar-only-darkmode-toggle.css
    )
    string(REPLACE ";" " " DOXYFILE_EXTRA_STYLESHEET "${DOXYFILE_EXTRA_STYLESHEET}")

    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    add_custom_command(
        OUTPUT ${DOXYGEN_INDEX_FILE}
        DEPENDS ${TARGET_HEADERS}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating docs"
    )
    add_custom_target(${TARGET_NAME}_docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION .)
endfunction()
