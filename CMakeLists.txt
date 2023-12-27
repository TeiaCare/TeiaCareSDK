cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
file(STRINGS "VERSION" VERSION_STR)
project(teiacore_sdk
    VERSION ${VERSION_STR}
    LANGUAGES CXX
    HOMEPAGE_URL "https://dev.azure.com/teiacare/Ancelia/_git/TeiaCoreSDK"
    DESCRIPTION "TeiaCoreSDK is a collection of reusable C++ components"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)
endif()

cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_GENERATOR)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_CXX_COMPILER_VERSION)

option(TEIACORE_SDK_ENABLE_UNIT_TESTS "Enable Unit Tests" True)
cmake_print_variables(TEIACORE_SDK_ENABLE_UNIT_TESTS)

option(TEIACORE_SDK_ENABLE_UNIT_TESTS_COVERAGE "Enable Unit Tests Coverage" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_UNIT_TESTS_COVERAGE)

option(TEIACORE_SDK_ENABLE_BENCHMARKS "Enable Benchmarks" True)
cmake_print_variables(TEIACORE_SDK_ENABLE_BENCHMARKS)

option(TEIACORE_SDK_ENABLE_EXAMPLES "Enable Examples" True)
cmake_print_variables(TEIACORE_SDK_ENABLE_EXAMPLES)

option(TEIACORE_SDK_ENABLE_DOCS "Enable Doxygen docuentation" True)
cmake_print_variables(TEIACORE_SDK_ENABLE_DOCS)

option(TEIACORE_SDK_ENABLE_WARNINGS_ERROR "Enable treat Warnings as Errors" True)
cmake_print_variables(TEIACORE_SDK_ENABLE_WARNINGS_ERROR)

option(TEIACORE_SDK_ENABLE_SANITIZER_ADDRESS "Enable Address and Leak Sanitizers" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_SANITIZER_ADDRESS)

option(TEIACORE_SDK_ENABLE_SANITIZER_THREAD "Enable Thread Sanitizer" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_SANITIZER_THREAD)

option(TEIACORE_SDK_ENABLE_CLANG_FORMAT "Enable Clang Format" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_CLANG_FORMAT)

option(TEIACORE_SDK_ENABLE_CLANG_TIDY "Enable Clang Tidy" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_CLANG_TIDY)

option(TEIACORE_SDK_ENABLE_CPPCHECK "Enable Cppcheck" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_CPPCHECK)

option(TEIACORE_SDK_ENABLE_CPPLINT "Enable Cpplint" False)
cmake_print_variables(TEIACORE_SDK_ENABLE_CPPLINT)

include(options)
validate_project_options()

enable_testing()
add_subdirectory(sdk)
