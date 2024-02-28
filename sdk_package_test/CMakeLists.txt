cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(TeiaCareSDK_IntegrationTest CXX)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(teiacare_sdk CONFIG REQUIRED)

add_executable(teiacare_sdk_integration_test src/main.cpp)
target_compile_features(teiacare_sdk_integration_test PUBLIC cxx_std_20)
target_link_libraries(teiacare_sdk_integration_test teiacare::sdk)
install(TARGETS teiacare_sdk_integration_test DESTINATION .)
