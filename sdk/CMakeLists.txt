set(TARGET_NAME teiacore_sdk)
add_library(${TARGET_NAME})
add_library(teiacore_sdk::teiacore_sdk ALIAS ${TARGET_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

configure_file(
    src/version.hpp.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sdk/version.hpp
)

set(TARGET_HEADERS
    include/sdk/datetime/date.hpp
    include/sdk/datetime/datetime.hpp
    include/sdk/datetime/time.hpp
    include/sdk/datetime/timedelta.hpp

    include/sdk/concurrent_queue.hpp
    include/sdk/data_types.hpp
    include/sdk/event_dispatcher.hpp
    include/sdk/function_traits.hpp
    include/sdk/high_precision_timer.hpp
    include/sdk/macros.hpp
    include/sdk/observable.hpp
    include/sdk/rate_limiter.hpp
    include/sdk/service_locator.hpp
    include/sdk/shutdown_signal_handler.hpp
    include/sdk/stopwatch.hpp
    include/sdk/task_scheduler.hpp
    include/sdk/task.hpp
    include/sdk/thread_pool.hpp
    include/sdk/uuid_generator.hpp
    include/sdk/uuid.hpp
    include/sdk/version.hpp
)

set(TARGET_SOURCES
    include/sdk/datetime/date/tz.cpp # TODO: fix me
    
    src/event_dispatcher.cpp
    src/high_precision_timer.cpp
    src/rate_limiter.cpp
    src/service_locator.cpp
    src/shutdown_signal_handler.cpp
    src/task_scheduler.cpp
    src/task.cpp
    src/thread_pool.cpp
    src/uuid_generator.cpp
    src/uuid.cpp
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_sources(${TARGET_NAME} PUBLIC ${TARGET_HEADERS} PRIVATE ${TARGET_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> $<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${TARGET_HEADERS}")
install(TARGETS ${TARGET_NAME} PUBLIC_HEADER DESTINATION include/sdk)

if(TEIACORE_ENABLE_UNIT_TESTS 
    OR TEIACORE_ENABLE_UNIT_TESTS_COVERAGE
	OR TEIACORE_ENABLE_BENCHMARKS 
	OR TEIACORE_ENABLE_SANITIZER_ADDRESS
    OR TEIACORE_ENABLE_SANITIZER_THREAD
	OR TEIACORE_ENABLE_CLANG_TIDY 
	OR TEIACORE_ENABLE_CPPCHECK
	OR TEIACORE_ENABLE_CPPLINT)
	set(CMAKE_DISABLE_PRECOMPILE_HEADERS True)
else()
    target_precompile_headers(${TARGET_NAME}
        PRIVATE
            <string>
            <csignal>
            <filesystem>
            <chrono>
            <tuple>
            <future>
            <fstream>
            <atomic>
            <condition_variable>
            <functional>
            <map>
            <memory>
            <mutex>
            <queue>
            <thread>
            <barrier>
            <vector>
    )
endif()

if(TEIACORE_ENABLE_WARNINGS_ERROR)
    include(warnings)
    add_warnings(${TARGET_NAME})
    add_warnings_as_errors(${TARGET_NAME})
endif()

if(TEIACORE_ENABLE_SANITIZER_ADDRESS)
    include(sanitizer_address)
    add_sanitizer_address(${TARGET_NAME})
endif()

if(TEIACORE_ENABLE_SANITIZER_THREAD)
    include(sanitizer_thread)
    add_sanitizer_thread(${TARGET_NAME})
endif()

if(TEIACORE_ENABLE_CLANG_FORMAT)
    include(clang_format)
    setup_target_clang_format(${TARGET_NAME} ${TARGET_SRC})
endif()

if(TEIACORE_ENABLE_CLANG_TIDY)
    include(clang_tidy)
    setup_target_clang_tidy(${TARGET_NAME} ${TARGET_SRC})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

if(TEIACORE_ENABLE_CPPCHECK)
    include(cppcheck)
    setup_target_cppcheck(${TARGET_NAME} ${TARGET_SRC})
    set(CMAKE_CXX_CPPCHECK cppcheck) # --enable=all --suppress=missingIncludeSystem
endif()

if(TEIACORE_ENABLE_CPPLINT)
    include(cpplint)
    setup_target_cpplint(${TARGET_NAME} ${TARGET_SRC})
    set(CMAKE_CXX_CPPLINT cpplint)
endif()

####################
## UNIT_TESTS
if(TEIACORE_ENABLE_UNIT_TESTS)
    if(TEIACORE_ENABLE_DEPENDENCIES_SETUP)
        include(compiler_info)
        get_compiler_versions()
        get_compiler_name()
        include(packages)
        setup_packages(${COMPILER_NAME_C} ${COMPILER_VERSION_MAJOR} "sdk/tests")
    endif()

    if(TEIACORE_ENABLE_UNIT_TESTS_COVERAGE)
        include(coverage)
        add_coverage(${TARGET_NAME})
    endif()

    include(unit_tests)
    set(TEIACORE_UNIT_TESTS_SRC
        tests/src/main.cpp
        tests/src/test_event_dispatcher.cpp
        tests/src/test_event_dispatcher.hpp
        tests/src/test_high_precision_timer.cpp
        tests/src/test_high_precision_timer.cpp
        tests/src/test_observable.cpp
        tests/src/test_observable.hpp
        tests/src/test_rate_limiter.cpp
        tests/src/test_rate_limiter.hpp        
        tests/src/test_service_locator.cpp
        tests/src/test_service_locator.hpp
        tests/src/test_stopwatch.cpp
        tests/src/test_stopwatch.hpp
        tests/src/test_task.cpp
        tests/src/test_task.hpp
        tests/src/test_thread_pool.cpp
        tests/src/test_thread_pool.hpp
    )
    setup_unit_tests(${TARGET_NAME} "${TEIACORE_UNIT_TESTS_SRC}")
endif()

####################
## BENCHMARKS
if(TEIACORE_ENABLE_BENCHMARKS)
    if(TEIACORE_ENABLE_DEPENDENCIES_SETUP)
        include(compiler_info)
        get_compiler_versions()
        get_compiler_name()
        include(packages)
        setup_packages(${COMPILER_NAME_C} ${COMPILER_VERSION_MAJOR} "sdk/benchmarks")
    endif()

    include(benchmarks)
    set(BENCHMARKS_SRC
        benchmarks/src/benchmark_event_dispatcher.cpp
        benchmarks/src/benchmark_event_dispatcher.hpp
        benchmarks/src/main.cpp
    )
    setup_benchmarks(${TARGET_NAME} "${BENCHMARKS_SRC}")
endif()

####################
## EXAMPLES
if(TEIACORE_ENABLE_EXAMPLES)
    if(TEIACORE_ENABLE_DEPENDENCIES_SETUP)
        include(compiler_info)
        get_compiler_versions()
        get_compiler_name()
        include(packages)
        setup_packages(${COMPILER_NAME_C} ${COMPILER_VERSION_MAJOR} "sdk/examples")
    endif()

    include(examples)
    add_example(${TARGET_NAME} example_date)
    add_example(${TARGET_NAME} example_event_dispatcher)
    add_example(${TARGET_NAME} example_high_precision_timer)
    add_example(${TARGET_NAME} example_observable)
    add_example(${TARGET_NAME} example_rate_limiter)
    add_example(${TARGET_NAME} example_service_locator)
    add_example(${TARGET_NAME} example_time)
    add_example(${TARGET_NAME} example_timedelta)
    add_example(${TARGET_NAME} example_uuid)
endif()

####################
## DOCS
if(TEIACORE_ENABLE_DOCS)
    include(docs)
    setup_docs(
        ${TARGET_NAME} 
        "${TARGET_HEADERS}"
        ${PROJECT_SOURCE_DIR}/sdk/include/sdk 
        ${PROJECT_SOURCE_DIR}/docs/Doxyfile
    )
endif()