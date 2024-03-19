set(TARGET_NAME teiacare_sdk)
add_library(${TARGET_NAME})
add_library(teiacare::sdk ALIAS ${TARGET_NAME})

configure_file(
    src/version.cpp.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp
)

set(TARGET_HEADERS
    include/teiacare/sdk/datetime/date.hpp
    include/teiacare/sdk/datetime/datetime.hpp
    include/teiacare/sdk/datetime/time.hpp
    include/teiacare/sdk/datetime/timedelta.hpp

    include/teiacare/sdk/geometry/line.hpp
    include/teiacare/sdk/geometry/point.hpp
    include/teiacare/sdk/geometry/range.hpp
    include/teiacare/sdk/geometry/rectangle.hpp
    include/teiacare/sdk/geometry/size.hpp

    include/teiacare/sdk/blocking_queue.hpp
    include/teiacare/sdk/clock.hpp
    include/teiacare/sdk/event_dispatcher.hpp
    include/teiacare/sdk/function_traits.hpp
    include/teiacare/sdk/high_precision_timer.hpp
    include/teiacare/sdk/non_copyable.hpp
    include/teiacare/sdk/non_moveable.hpp
    include/teiacare/sdk/observable.hpp
    include/teiacare/sdk/rate_limiter.hpp
    include/teiacare/sdk/service_locator.hpp
    include/teiacare/sdk/signal_handler.hpp
    include/teiacare/sdk/singleton.hpp
    include/teiacare/sdk/stopwatch.hpp
    include/teiacare/sdk/task_scheduler.hpp
    include/teiacare/sdk/task.hpp
    include/teiacare/sdk/thread_pool.hpp
    include/teiacare/sdk/unreachable.hpp
    include/teiacare/sdk/uuid_generator.hpp
    include/teiacare/sdk/uuid.hpp
    include/teiacare/sdk/version.hpp
)

set(TARGET_SOURCES
    include/teiacare/sdk/datetime/date/tz.cpp # TODO: fix me
    
    src/event_dispatcher.cpp
    src/high_precision_timer.cpp
    src/rate_limiter.cpp
    src/service_locator.cpp
    src/signal_handler.cpp
    src/task_scheduler.cpp
    src/task.cpp
    src/thread_pool.cpp
    src/uuid_generator.cpp
    src/uuid.cpp
    src/version.cpp
)

list(APPEND ALL_SOURCES ${TARGET_HEADERS} ${TARGET_SOURCES})
list(TRANSFORM ALL_SOURCES_ABS_PATH PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_sources(${TARGET_NAME} PUBLIC ${TARGET_HEADERS} PRIVATE ${TARGET_SOURCES})
target_include_directories(${TARGET_NAME} 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>
)
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${TARGET_HEADERS}")
install(TARGETS ${TARGET_NAME} PUBLIC_HEADER DESTINATION include/teiacare/sdk)

if(TC_ENABLE_WARNINGS_ERROR)
    include(warnings)
    add_warnings(${TARGET_NAME})
    add_warnings_as_errors(${TARGET_NAME})
endif()

if(TC_ENABLE_SANITIZER_ADDRESS)
    include(sanitizer_address)
    add_sanitizer_address(${TARGET_NAME})
endif()

if(TC_ENABLE_SANITIZER_THREAD)
    include(sanitizer_thread)
    add_sanitizer_thread(${TARGET_NAME})
endif()

if(TC_ENABLE_CLANG_FORMAT)
    include(clang_format)
    setup_target_clang_format(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(TC_ENABLE_CLANG_TIDY)
    include(clang_tidy)
    setup_target_clang_tidy(${TARGET_NAME})
endif()

if(TC_ENABLE_CPPCHECK)
    include(cppcheck)
    setup_target_cppcheck(${TARGET_NAME})
endif()

if(TC_ENABLE_CPPLINT)
    include(cpplint)
    setup_target_cpplint(${TARGET_NAME} ${TARGET_SRC})
    set(CMAKE_CXX_CPPLINT cpplint)
endif()

if(TC_ENABLE_UNIT_TESTS)
    add_subdirectory(tests)
endif()

if(TC_ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(TC_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TC_ENABLE_DOCS)
    include(doxygen)
    setup_docs(
        ${TARGET_NAME} 
        "${TARGET_HEADERS}"
        ${CMAKE_CURRENT_SOURCE_DIR}/include/teiacare/sdk 
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
    )
endif()
