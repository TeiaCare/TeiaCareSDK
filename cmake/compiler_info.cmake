# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # GCC options
    add_compile_options(-march=native) # Add support for compiler intrinsics (SIMD)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    # Clang options
    add_compile_options(-fstandalone-debug) # Add support for std::string debug
    add_compile_options(-march=native) # Add support for compiler intrinsics (SIMD)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # MSVC options
endif()

function(get_compiler_versions)
    string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
    set(COMPILER_VERSION_MAJOR ${CMAKE_MATCH_1} PARENT_SCOPE)
    set(COMPILER_VERSION_MINOR ${CMAKE_MATCH_2} PARENT_SCOPE)
    set(COMPILER_VERSION_PATCH ${CMAKE_MATCH_3} PARENT_SCOPE)
endfunction()
