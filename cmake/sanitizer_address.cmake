function(add_sanitizer_address TARGET)
    message(STATUS "Address sanitizer enabled")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET} PRIVATE -g
            -fsanitize=address
            -fno-sanitize=signed-integer-overflow
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer
        )
        target_link_libraries(${TARGET} PRIVATE -fsanitize=address)
    elseif(MSVC)
        target_compile_options(${TARGET} PRIVATE -fsanitize=address)
        target_link_libraries(${TARGET} PRIVATE -fsanitize=address)
    endif()
endfunction()
