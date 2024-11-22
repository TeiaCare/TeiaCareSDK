# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # GCC options
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    # Clang options
    add_compile_options(-fstandalone-debug) # Add support for std::string debug
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # MSVC options
endif()

function(get_compiler_versions)
    string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    set(COMPILER_VERSION_MAJOR ${CMAKE_MATCH_1} PARENT_SCOPE)
    set(COMPILER_VERSION_MINOR ${CMAKE_MATCH_2} PARENT_SCOPE)
    set(COMPILER_VERSION_PATCH ${CMAKE_MATCH_3} PARENT_SCOPE)
endfunction()

cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_GENERATOR)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_CXX_COMPILER_VERSION)
