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

/**
 * @example example_thread_pool.cpp
 * @brief Simple example of tc::sdk::thread_pool
 */

#include <teiacare/sdk/thread_pool.hpp>

#include "utils/callback_timer.hpp"
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

/**
 * @cond SKIP_DOXYGEN
 * This section won't be documented.
 */
class Foo
{
public:
    explicit Foo(int i)
        : x{i}
    {
    }
    void update()
    {
        ++x;
    }
    int get()
    {
        return x;
    }

private:
    int x;
};
/** @endcond */

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    auto tick_callback = [](unsigned tick) { spdlog::info("[{}]", tick); };
    tc::sdk::examples::utils::callback_timer t(1s, tick_callback);

    tc::sdk::thread_pool tp;
    tp.start();

    {
        spdlog::info("Is running: {}", tp.is_running());
        spdlog::info("Threads count: {}", tp.threads_count());
    }

    // Run lambda
    {
        tp.run([] { spdlog::info("This is a lambda function!"); });
    }

    // Run lambda with return value
    {
        auto f = tp.run([] {
            spdlog::info("I return 42");
            return 42;
        });

        spdlog::info("Fourty-two: {}", f.get());
    }

    // Bind and run a class member function
    {
        Foo foo(0);
        tp.run(std::bind(&Foo::update, &foo));
        std::this_thread::sleep_for(3s);

        spdlog::info("foo: {}", foo.get());
    }

    return 0;
}
