cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(TeiaCareSDK_PackageTest CXX)

find_package(teiacare_sdk CONFIG REQUIRED)

add_executable(teiacare_sdk_package_test src/test.cpp)
target_link_libraries(teiacare_sdk_package_test teiacare::sdk)
