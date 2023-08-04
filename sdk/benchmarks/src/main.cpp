#include <benchmark/benchmark.h>

auto main(int argc, char** argv) -> int
{
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;

    benchmark::SetDefaultTimeUnit(benchmark::kMillisecond);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
