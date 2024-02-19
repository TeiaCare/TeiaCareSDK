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

#include <teiacare/sdk/high_precision_timer.hpp>
#include <spdlog/spdlog.h>
#include "utils/callback_timer.hpp"

using namespace std::chrono_literals;

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    tc::sdk::examples::utils::callback_timer debug_timer(1s, [](unsigned i){spdlog::info("{}", i);});

    {
        tc::sdk::high_precision_timer timer;
        spdlog::info("Timer @ 10ms --- Start");

        timer.set_callback([]{ spdlog::info("@"); });
        timer.start(10ms);
        std::this_thread::sleep_for(2s);

        spdlog::info("Timer @ 10ms --- Stop");
        timer.stop();
    }

    {
        tc::sdk::high_precision_timer timer;
        spdlog::info("Timer @ 250ms --- Start");

        timer.set_callback([]{ spdlog::info("@"); });
        timer.start(250ms);
        std::this_thread::sleep_for(2s);

        spdlog::info("Timer @ 250ms --- Stop");
        timer.stop();
    }

    return 0;
}
