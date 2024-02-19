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

#include <teiacare/sdk/blocking_queue.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

using namespace std::chrono_literals;

template<typename T>
struct fmt::formatter<tc::sdk::blocking_queue<T>>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(tc::sdk::blocking_queue<T>& q, format_context &ctx) const -> decltype(ctx.out())
    {
        std::stringstream os;
        while(q.size() > 0)
        {
            os << q.pop();
            
            if (q.size() > 0)
                os << ", ";
        }
        
        return fmt::format_to(ctx.out(), "[{}]", os.str());
    }
};

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::set_level(spdlog::level::trace);

    {
        auto q = tc::sdk::blocking_queue<int>(4);
        spdlog::info("capacity: {}, size: {}", q.capacity(), q.size()); // capacity: 4, size: 0
        spdlog::info("queue: {}", q);

        q.push(1);
        q.push(2);
        q.push(3);
        q.pop();
        q.push(4);

        spdlog::info("capacity: {}, size: {}", q.capacity(), q.size()); // capacity: 4, size: 3
        spdlog::info("queue: {}", q); // [2, 3, 4]
    }

    {
        auto q = tc::sdk::blocking_queue<const char*>(4);
        q.push("a");
        q.push("b");
        q.push("c");
        q.pop();
        q.push("d");

        spdlog::info("capacity: {}, size: {}", q.capacity(), q.size()); // capacity: 4, size: 3
        spdlog::info("queue: {}", q); // [b, c, d]
    }

    {
        auto q = tc::sdk::blocking_queue<int>(4);
        
        auto consumer_thread = std::jthread([&]{
            for(int i = 0; i < 10; ++i)
            {
                auto item = q.pop();
                spdlog::debug("consumed: {}", item);
            }

            spdlog::warn("consumer finished!");
        });

        auto producer_thread = std::jthread([&]{
            for(int i = 0; i < 10; ++i)
            {
                q.push(i+1);
                spdlog::debug("produced: {}", i);
            }

            spdlog::warn("producer finished!");
        });

        producer_thread.join();
        consumer_thread.join();
        spdlog::info("finished!");
    }

    return 0;
}
