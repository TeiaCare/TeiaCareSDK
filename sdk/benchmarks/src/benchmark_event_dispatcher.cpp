// Copyright 2024 TeiaCare
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "benchmark_event_dispatcher.hpp"
#include <chrono>
#include <iostream>
#include <thread>

namespace tc::sdk::benchmarks
{
// NOLINTNEXTLINE
BENCHMARK_DEFINE_F(benchmark_event_dispatcher, one_event_N_handlers)(benchmark::State& state)
{
    constexpr auto event_name = "Event_1";

    for (auto _ : state)
    {
        state.PauseTiming();
        
        const auto threads_count = state.range(0);
        const auto handlers_count = state.range(1);
        
        e->start(threads_count);
        unsigned int call_counter = 0;
        
        for(auto i = 0; i < handlers_count; ++i)
        {
            e->add_handler<std::string>(event_name, [&call_counter](const std::string& s){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                ++call_counter;
            });
        }

        state.ResumeTiming();
        bool is_call_scheduled = e->emit(event_name, std::string("THE EVENT PAYLOAD!"));
        benchmark::DoNotOptimize(is_call_scheduled);
    }
}

// NOLINTNEXTLINE
BENCHMARK_REGISTER_F(benchmark_event_dispatcher, one_event_N_handlers)
    ->ArgsProduct({{1,2,3,4},{5,10,15,20}})
    // ->ThreadRange(1, 8)
    ->Threads(1)
    ->ArgNames({"dispatcher_threads", "dispatcher_handlers"})
    ->UseRealTime()
    ->ReportAggregatesOnly(false)
    ->DisplayAggregatesOnly(false)
    ;
    
}