cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
file(STRINGS "VERSION" VERSION_STR)
project(teiacare_sdk
    VERSION ${VERSION_STR}
    LANGUAGES CXX
    HOMEPAGE_URL "https://dev.azure.com/teiacare/Ancelia/_git/TeiaCareSDK"
    DESCRIPTION "TeiaCareSDK is a collection of reusable C++ components"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)

include(GNUInstallDirs)
include(CMakePrintHelpers)
# include(CheckIPOSupported)
# check_ipo_supported(RESULT ipo_supported OUTPUT error)
# if(ipo_supported)
#     set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)
# endif()

cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_GENERATOR)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_CXX_COMPILER_VERSION)

include(compiler_info)
include(options)
validate_project_options()

enable_testing()
add_subdirectory(sdk)

# add_executable(uuid_v2 sdk/main_uuid.cpp)
# target_compile_features(uuid_v2 PUBLIC cxx_std_20)

# include(warnings)
# add_warnings(uuid_v2)
# add_warnings_as_errors(uuid_v2)

# include(sanitizer_address)
# add_sanitizer_address(uuid_v2)