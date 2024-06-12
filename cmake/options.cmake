set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CONAN_CMAKE_SILENT_OUTPUT ON)

option(TC_ENABLE_UNIT_TESTS "Enable Unit Tests" True)
cmake_print_variables(TC_ENABLE_UNIT_TESTS)

option(TC_ENABLE_UNIT_TESTS_COVERAGE "Enable Unit Tests Coverage" False)
cmake_print_variables(TC_ENABLE_UNIT_TESTS_COVERAGE)

option(TC_ENABLE_BENCHMARKS "Enable Benchmarks" False)
cmake_print_variables(TC_ENABLE_BENCHMARKS)

option(TC_ENABLE_EXAMPLES "Enable Examples" True)
cmake_print_variables(TC_ENABLE_EXAMPLES)

option(TC_ENABLE_DOCS "Enable Doxygen docuentation" True)
cmake_print_variables(TC_ENABLE_DOCS)

option(TC_ENABLE_WARNINGS_ERROR "Enable treat Warnings as Errors" True)
cmake_print_variables(TC_ENABLE_WARNINGS_ERROR)

option(TC_ENABLE_SANITIZER_ADDRESS "Enable Address and Leak Sanitizers" False)
cmake_print_variables(TC_ENABLE_SANITIZER_ADDRESS)

option(TC_ENABLE_SANITIZER_THREAD "Enable Thread Sanitizer" True)
cmake_print_variables(TC_ENABLE_SANITIZER_THREAD)

option(TC_ENABLE_CLANG_FORMAT "Enable Clang Format" True)
cmake_print_variables(TC_ENABLE_CLANG_FORMAT)

option(TC_ENABLE_CLANG_TIDY "Enable Clang Tidy" True)
cmake_print_variables(TC_ENABLE_CLANG_TIDY)

option(TC_ENABLE_CPPCHECK "Enable Cppcheck" True)
cmake_print_variables(TC_ENABLE_CPPCHECK)

option(TC_ENABLE_CPPLINT "Enable Cpplint" False)
cmake_print_variables(TC_ENABLE_CPPLINT)

function(validate_project_options)
    if(TC_ENABLE_SANITIZER_ADDRESS AND TC_ENABLE_SANITIZER_THREAD)
        message(FATAL_ERROR "It's not possible to set both Address and Thread sanitizers simultaneously.")
    endif()

    if(TC_ENABLE_UNIT_TESTS_COVERAGE AND NOT TC_ENABLE_UNIT_TESTS)
        message(FATAL_ERROR "Unit Tests must be enabled in order to run Code Coverage")
    endif()

    if(TC_ENABLE_UNIT_TESTS_COVERAGE AND TC_ENABLE_BENCHMARKS)
        message(FATAL_ERROR "Code Coverage cannot be enabled with Benchmarks")
    endif()
endfunction()
