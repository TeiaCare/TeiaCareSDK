include(benchmarks)
set(BENCHMARKS_SRC
    src/benchmark_event_dispatcher.cpp
    src/benchmark_event_dispatcher.hpp
    src/main.cpp
)
setup_benchmarks(${TARGET_NAME} ${BENCHMARKS_SRC})