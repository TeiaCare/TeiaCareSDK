if(TC_ENABLE_UNIT_TESTS_COVERAGE)
    include(coverage)
    add_coverage(${TARGET_NAME})
endif()

include(unit_tests)
set(UNIT_TESTS_SRC
    src/main.cpp
    src/test_blocking_queue.cpp
    src/test_blocking_queue.hpp
    src/test_event_dispatcher.cpp
    src/test_event_dispatcher.hpp    
    src/test_geometry_line.cpp
    src/test_geometry_line.hpp
    src/test_geometry_point.cpp
    src/test_geometry_point.hpp
    src/test_geometry_range.cpp
    src/test_geometry_range.hpp
    src/test_geometry_rectangle.cpp
    src/test_geometry_rectangle.hpp
    src/test_geometry_size.cpp
    src/test_geometry_size.hpp
    src/test_high_precision_timer.cpp
    src/test_high_precision_timer.hpp
    src/test_observable.cpp
    src/test_observable.hpp
    src/test_rate_limiter.cpp
    src/test_rate_limiter.hpp        
    src/test_service_locator.cpp
    src/test_service_locator.hpp
    src/test_stopwatch.cpp
    src/test_stopwatch.hpp
    src/test_task_scheduler.hpp
    src/test_task_scheduler.cpp
    src/test_task.cpp
    src/test_task.hpp
    src/test_thread_pool.cpp
    src/test_thread_pool.hpp
    src/test_uuid_generator.cpp
    src/test_uuid_generator.hpp
    src/test_uuid.cpp
    src/test_uuid.hpp
)

# If Address or Thread Sanitizer is enabled signal handlers tests halt the program: do not include their unit tests in such cases.
if(NOT TC_ENABLE_SANITIZER_ADDRESS AND NOT TC_ENABLE_SANITIZER_THREAD)
    set(UNIT_TESTS_SRC ${UNIT_TESTS_SRC}
        src/test_signal_handler.cpp
        src/test_signal_handler.hpp
    )
endif()
setup_unit_tests(${TARGET_NAME} ${UNIT_TESTS_SRC})

# Disable warnings on GCC (-Wall compiler flag), due to a bug in GTest 1.14.0 in Release with GCC 12 and std=c++20
# https://github.com/google/googletest/issues/4108
if(TC_ENABLE_WARNINGS_ERROR )
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message(STATUS "Unit Tests warning disabled for GCC/Release (see GoogleTest issue: https://github.com/google/googletest/issues/4108)")
        return()
    endif()

    include(warnings)
    add_warnings(${TARGET_NAME}_unit_tests)
    add_warnings_as_errors(${TARGET_NAME}_unit_tests)
endif()