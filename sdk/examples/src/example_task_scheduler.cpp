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
 * @example example_task_scheduler.cpp
 * @brief Simple example of tc::sdk::task_scheduler
 */

#include <teiacare/sdk/task_scheduler.hpp>

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

void recursive_lambda(tc::sdk::task_scheduler& s, const std::string& str)
{
    spdlog::info(str);
    s.in("recursive_lambda", 1s, [&s, str] { recursive_lambda(s, str); });
}
/** @endcond */

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    auto tick_callback = [](unsigned tick) { spdlog::info("[{}]", tick); };
    tc::sdk::examples::utils::callback_timer t(1s, tick_callback);

    tc::sdk::task_scheduler s;
    s.start();

    // Bind class member
    {
        Foo foo(0);
        s.every("task_id", 1s, std::bind(&Foo::update, &foo));
        std::this_thread::sleep_for(3s);
        s.remove_task("task_id");

        spdlog::info("foo: {}", foo.get());
    }

    // In
    {
        auto future = s.in(1s, [] { spdlog::info("in"); });
        future->wait();
    }

    // At
    {
        auto future = s.at(tc::sdk::clock::now() + 1s, [] { spdlog::info("at"); });
        future->wait();
    }

    // Every
    {
        bool is_scheduled = s.every("task_id", 100ms, [] {
            spdlog::info("Every 100ms");
            std::this_thread::sleep_for(50ms);
        });
        std::this_thread::sleep_for(1s);

        s.update_interval("task_id", 400ms);
        std::this_thread::sleep_for(1s);

        s.remove_task("task_id");
        std::this_thread::sleep_for(1s);
    }

    // Nested
    {
        s.in(1s, [&s] {
            spdlog::info("First!");
            s.in(1s, [] {
                spdlog::info("Second!");
            });
        });
    }

    // Recursive
    {
        recursive_lambda(s, "recursive payload");
        std::this_thread::sleep_for(3s);
    }

    // Return value
    {
        std::optional<std::future<std::string>> f = s.in(1s, [] {
            return std::string("HEY!");
        });

        spdlog::info(f.value().get());
    }

    // Parameters
    {
        auto f = s.in(
            1s, [](int i, std::string s, float f) {
                spdlog::info("int: {}, std::string: {}, float: {}", i, s, f);
                return i + 1;
            },
            123, std::string("something!"), 9.87654321);

        spdlog::info(f.value().get());
    }

    return 0;
}
