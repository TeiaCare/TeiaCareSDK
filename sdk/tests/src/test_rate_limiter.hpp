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
#include <teiacare/sdk/rate_limiter.hpp>
#include <functional>
#include <chrono>

namespace tc::sdk::tests
{
class elapsed_timer
{
public:
    template<typename Rep, typename Dur>
    explicit elapsed_timer(const std::chrono::duration<Rep, Dur> duration)
        : _deadline { std::chrono::steady_clock::now() + duration }
    {
    }

    bool is_elapsed() const
    {
        return std::chrono::steady_clock::now() >= _deadline; 
    }
private:
    const std::chrono::steady_clock::time_point _deadline;
};

}
