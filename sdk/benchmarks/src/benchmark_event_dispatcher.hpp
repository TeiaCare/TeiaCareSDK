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

#pragma once

#include <teiacare/sdk/event_dispatcher.hpp>

#include <benchmark/benchmark.h>

namespace tc::sdk::benchmarks
{
class benchmark_event_dispatcher : public benchmark::Fixture
{
public:
    explicit benchmark_event_dispatcher()
        : e{std::make_unique<tc::sdk::event_dispatcher>()}
    {
    }
    void SetUp(benchmark::State& st) override
    {
    }
    void TearDown(benchmark::State& st) override
    {
        e->stop();
    }

protected:
    std::unique_ptr<tc::sdk::event_dispatcher> e;
};

}