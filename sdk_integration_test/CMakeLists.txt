cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(TeiaCoreSDK_IntegrationTest CXX)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(teiacore_sdk CONFIG REQUIRED)

add_executable(sdk_integration_test src/integration_test.cpp)
target_link_libraries(sdk_integration_test teiacore_sdk::teiacore_sdk)
install(TARGETS sdk_integration_test DESTINATION .)
