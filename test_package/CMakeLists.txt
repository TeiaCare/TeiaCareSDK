cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(TeiaCoreSDK_PackageTest CXX)

find_package(teiacore_sdk CONFIG REQUIRED)

add_executable(teiacore_sdk_package_test src/test.cpp)
target_link_libraries(teiacore_sdk_package_test teiacore_sdk::teiacore_sdk)
