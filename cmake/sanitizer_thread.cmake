function(add_sanitizer_thread TARGET)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Address sanitizer enabled")
        target_compile_options(${TARGET} PRIVATE
            -g
            -O1
            -fsanitize=thread
            -fno-omit-frame-pointer
        )
        target_link_libraries(${TARGET} PRIVATE -fsanitize=thread)
    elseif(MSVC)
        message("Thread sanitizer not supported.")
    endif()
endfunction()

# NOTE:
# Thread Sanitizer is currently broken on several Linux distributions (will be fixed with clang18/Ubuntu24.04).
# This is the error that is printed when a program compiled with -fsanitize=thread is executed:
# FATAL: ThreadSanitizer: unexpected memory mapping 0x58509008b000-0x58509008d000
# There is currently a suggested workaround outlined here: https://github.com/google/sanitizers/issues/1716
# This is the suggested workaround that fixes Thread Sanitizer error:
# sudo sysctl vm.mmap_rnd_bits=28
