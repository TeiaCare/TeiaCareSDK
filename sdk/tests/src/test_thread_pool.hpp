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

#include <gtest/gtest.h>
#include <teiacare/sdk/thread_pool.hpp>

namespace tc::sdk::tests
{
class test_thread_pool : public ::testing::Test
{
protected:
    explicit test_thread_pool()
        : tp { std::make_unique<tc::sdk::thread_pool>() }
    {
    }

    ~test_thread_pool() override
    {
        tp->stop();
    }

    const unsigned int num_threads = 4;
    const unsigned int max_threads_count = std::thread::hardware_concurrency();
    std::unique_ptr<tc::sdk::thread_pool> tp;
};

}
