cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
file(STRINGS "VERSION" VERSION_STR)
project(TeiaCoreSDK
    VERSION ${VERSION_STR}
    LANGUAGES CXX
    HOMEPAGE_URL "https://dev.azure.com/teiacare/Ancelia/_git/TeiaCoreSDK"
    DESCRIPTION "TeiaCoreSDK is a collection of reusable C++ components"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LINK_WHAT_YOU_USE True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)
set(WINDOWS_EXPORT_ALL_SYMBOLS ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_GENERATOR)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_CXX_COMPILER_VERSION)

option(TEIACORE_ENABLE_INTEGRATION_TESTS "Enable Integration Tests" False)
cmake_print_variables(TEIACORE_ENABLE_INTEGRATION_TESTS)

option(TEIACORE_ENABLE_UNIT_TESTS "Enable Unit Tests" True)
cmake_print_variables(TEIACORE_ENABLE_UNIT_TESTS)

option(TEIACORE_ENABLE_UNIT_TESTS_COVERAGE "Enable Unit Tests Coverage" False)
cmake_print_variables(TEIACORE_ENABLE_UNIT_TESTS_COVERAGE)

option(TEIACORE_ENABLE_BENCHMARKS "Enable Benchmarks" False)
cmake_print_variables(TEIACORE_ENABLE_BENCHMARKS)

option(TEIACORE_ENABLE_EXAMPLES "Enable Examples" False)
cmake_print_variables(TEIACORE_ENABLE_EXAMPLES)

option(TEIACORE_ENABLE_WARNINGS_ERROR "Enable treat Warnings as Errors" True)
cmake_print_variables(TEIACORE_ENABLE_WARNINGS_ERROR)

option(TEIACORE_ENABLE_SANITIZER_ADDRESS "Enable Address and Leak Sanitizers" False)
cmake_print_variables(TEIACORE_ENABLE_SANITIZER_ADDRESS)

option(TEIACORE_ENABLE_SANITIZER_THREAD "Enable Thread Sanitizer" False)
cmake_print_variables(TEIACORE_ENABLE_SANITIZER_THREAD)

option(TEIACORE_ENABLE_CLANG_FORMAT "Enable Clang Format" False)
cmake_print_variables(TEIACORE_ENABLE_CLANG_FORMAT)

option(TEIACORE_ENABLE_CLANG_TIDY "Enable Clang Tidy" False)
cmake_print_variables(TEIACORE_ENABLE_CLANG_TIDY)

option(TEIACORE_ENABLE_CPPCHECK "Enable Cppcheck" False)
cmake_print_variables(TEIACORE_ENABLE_CPPCHECK)

option(TEIACORE_ENABLE_CPPLINT "Enable Cpplint" False)
cmake_print_variables(TEIACORE_ENABLE_CPPLINT)

option(TEIACORE_ENABLE_DOCS "Enable Documentation" False)
cmake_print_variables(TEIACORE_ENABLE_DOCS)

option(TEIACORE_ENABLE_DEPENDENCIES_SETUP "Enable automatic dependencies setup (tests, examples and benchmarks)" False)
cmake_print_variables(TEIACORE_ENABLE_DEPENDENCIES_SETUP)

if(TEIACORE_ENABLE_SANITIZER_ADDRESS AND TEIACORE_ENABLE_SANITIZER_THREAD)
    message(FATAL_ERROR "It's not possible to set both Address and Thread sanitizers simultaneously.")
endif()

if(TEIACORE_ENABLE_UNIT_TESTS_COVERAGE AND NOT TEIACORE_ENABLE_UNIT_TESTS)
    message(FATAL_ERROR "Unit Tests must be enabled in order to run Code Coverage")
endif()

if(TEIACORE_ENABLE_UNIT_TESTS_COVERAGE AND TEIACORE_ENABLE_BENCHMARKS)
    message(FATAL_ERROR "Code Coverage cannot be enabled with Benchmarks")
endif()

if(TEIACORE_ENABLE_UNIT_TESTS_COVERAGE AND TEIACORE_ENABLE_EXAMPLES)
    message(FATAL_ERROR "Code Coverage cannot be enabled with Examples")
endif()

enable_testing()
add_subdirectory(sdk)

if(TEIACORE_ENABLE_INTEGRATION_TESTS)
    add_subdirectory(sdk_integration_test)
endif()
