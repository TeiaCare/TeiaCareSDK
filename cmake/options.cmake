set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CONAN_CMAKE_SILENT_OUTPUT ON)

function(validate_project_options)
    if(TEIACORE_SDK_ENABLE_SANITIZER_ADDRESS AND TEIACORE_SDK_ENABLE_SANITIZER_THREAD)
        message(FATAL_ERROR "It's not possible to set both Address and Thread sanitizers simultaneously.")
    endif()
    
    if(TEIACORE_SDK_ENABLE_UNIT_TESTS_COVERAGE AND NOT TEIACORE_SDK_ENABLE_UNIT_TESTS)
        message(FATAL_ERROR "Unit Tests must be enabled in order to run Code Coverage")
    endif()
    
    if(TEIACORE_SDK_ENABLE_UNIT_TESTS_COVERAGE AND TEIACORE_SDK_ENABLE_BENCHMARKS)
        message(FATAL_ERROR "Code Coverage cannot be enabled with Benchmarks")
    endif()
    
    if(TEIACORE_SDK_ENABLE_UNIT_TESTS_COVERAGE AND TEIACORE_SDK_ENABLE_EXAMPLES)
        message(FATAL_ERROR "Code Coverage cannot be enabled with Examples")
    endif()
endfunction()
