cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(teiacare_sdk_test_package CXX)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)
find_package(teiacare_sdk CONFIG REQUIRED)

add_executable(teiacare_sdk_test_package src/test.cpp)
target_link_libraries(teiacare_sdk_test_package PRIVATE teiacare::sdk)
target_compile_features(teiacare_sdk_test_package PRIVATE cxx_std_20)
install(TARGETS teiacare_sdk_test_package DESTINATION .)
