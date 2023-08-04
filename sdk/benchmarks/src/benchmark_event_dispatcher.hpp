#pragma once

#include <benchmark/benchmark.h>
#include <sdk/event_dispatcher.hpp>

namespace tc::sdk::benchmarks
{
class benchmark_event_dispatcher : public benchmark::Fixture
{
public:
    explicit benchmark_event_dispatcher() : e { std::make_unique<tc::sdk::event_dispatcher>() } { }
    void SetUp(benchmark::State& st) override { }
    void TearDown(benchmark::State& st) override { e->stop(); }

protected:
    std::unique_ptr<tc::sdk::event_dispatcher> e;
};

}