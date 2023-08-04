function(add_sanitizer_thread TARGET)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Address sanitizer enabled")
        target_compile_options(${TARGET} PRIVATE -g
            -fsanitize=thread
            -fno-sanitize=signed-integer-overflow
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer
        )
        target_link_libraries(${TARGET} PRIVATE -fsanitize=thread)
    elseif(MSVC)
        message("Thread sanitizer not supported.")
    endif()
endfunction()
