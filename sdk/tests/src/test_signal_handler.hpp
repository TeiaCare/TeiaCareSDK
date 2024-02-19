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
#include <sdk/signal_handler.hpp>
#include <thread>

namespace tc::sdk::tests
{
struct signal_handler_params
{
    int signal;
    const char* name;
    std::function<void(const char*, int)> callback;
};

class test_signal_handler_shutdown : public testing::Test
{
protected:
    explicit test_signal_handler_shutdown()
    {
    }

    ~test_signal_handler_shutdown() override
    {
    }

    std::jthread signal_thread;
};

class test_signal_handler 
    : public test_signal_handler_shutdown
    , public testing::WithParamInterface<signal_handler_params>
{
};

}
