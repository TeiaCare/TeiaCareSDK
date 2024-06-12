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

#include "test_thread_pool.hpp"

#include <limits>
#include <semaphore>
#include <thread>

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_thread_pool, threads_count_range_min)
{
    EXPECT_TRUE(tp->start(std::numeric_limits<unsigned int>::min()));
    EXPECT_EQ(tp->threads_count(), 1);
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, threads_count_range_max)
{
    EXPECT_TRUE(tp->start(std::numeric_limits<unsigned int>::max()));
    EXPECT_EQ(tp->threads_count(), max_threads_count);
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, threads_count_before_after_start)
{
    EXPECT_EQ(tp->threads_count(), 0);

    EXPECT_TRUE(tp->start(1));
    EXPECT_EQ(tp->threads_count(), 1);

    EXPECT_FALSE(tp->start(2));
    EXPECT_EQ(tp->threads_count(), 1);
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, threads_count_start_stop)
{
    for (unsigned int i = 1; i < max_threads_count; ++i)
    {
        EXPECT_TRUE(tp->start(i));
        EXPECT_EQ(tp->threads_count(), i);
        EXPECT_TRUE(tp->stop());
    }
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, start_stop_lock)
{
    for (unsigned int i = 1; i < 100; ++i)
    {
        EXPECT_TRUE(tp->start(max_threads_count));
        EXPECT_TRUE(tp->is_running());

        EXPECT_TRUE(tp->stop());
        EXPECT_FALSE(tp->is_running());
    }
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, start_stop_run_lock)
{
    for (unsigned int i = 1; i < 100; ++i)
    {
        EXPECT_TRUE(tp->start(max_threads_count));

        auto result = tp->run([i] { return i; });
        EXPECT_EQ(result.get(), i);

        EXPECT_TRUE(tp->stop());
    }
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, start_twice)
{
    EXPECT_TRUE(tp->start(num_threads));
    EXPECT_FALSE(tp->start(num_threads + 1));
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, start_twice_after_run)
{
    EXPECT_TRUE(tp->start(num_threads));

    std::binary_semaphore sync(0);
    auto task = [&sync] { sync.release(); };
    tp->run(task);
    sync.acquire();

    EXPECT_FALSE(tp->start(num_threads + 1));
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, stop_without_start)
{
    EXPECT_FALSE(tp->stop());
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, stop_twice)
{
    EXPECT_TRUE(tp->start(num_threads));
    EXPECT_TRUE(tp->stop());
    EXPECT_FALSE(tp->stop());
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, stop_twice_after_run)
{
    EXPECT_TRUE(tp->start(num_threads));

    std::binary_semaphore sync(0);
    auto task = [&sync] { sync.release(); };
    tp->run(task);
    sync.acquire();

    EXPECT_TRUE(tp->stop());
    EXPECT_FALSE(tp->stop());
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, run)
{
    using namespace std::chrono_literals;

    EXPECT_TRUE(tp->start(max_threads_count));
    constexpr int task_count = 64;
    std::counting_semaphore<task_count> sync(0);

    std::atomic_int counter = 0;
    std::function<void()> task = [&sync, &counter] {
        ++counter;
        sync.release();
    };

    for (auto i = 0; i < task_count; ++i)
    {
        tp->run(task);
    }

    for (auto i = 0; i < task_count; ++i)
    {
        sync.acquire();
    }

    EXPECT_EQ(counter, task_count);
    EXPECT_EQ(counter, sync.max());
}

TEST_F(test_thread_pool, run_with_return)
{
    using namespace std::chrono_literals;

    EXPECT_TRUE(tp->start(max_threads_count));
    constexpr int task_count = 64;
    std::counting_semaphore<task_count> sync(0);

    std::atomic_int counter = 0;
    std::function<int()> task = [&sync, &counter] {
        ++counter;
        sync.release();
        return counter.load();
    };

    for (auto i = 0; i < task_count; ++i)
    {
        auto result = tp->run(task);
        EXPECT_EQ(result.get(), i + 1);
    }

    for (auto i = 0; i < task_count; ++i)
    {
        sync.acquire();
    }

    EXPECT_EQ(counter, task_count);
    EXPECT_EQ(counter, sync.max());
}

TEST_F(test_thread_pool, run_with_return_and_args)
{
    using namespace std::chrono_literals;

    EXPECT_TRUE(tp->start(max_threads_count));
    constexpr int task_count = 64;
    std::counting_semaphore<task_count> sync(0);

    std::atomic_int counter = 0;
    std::function<double(const std::string& s, float f)> task = [&sync, &counter](const std::string& s, float f) {
        (void)s;
        ++counter;
        sync.release();
        return static_cast<double>(f + counter.load());
    };

    for (auto i = 0; i < task_count; ++i)
    {
        constexpr float float_k = 1.234;
        auto result = tp->run(task, "param", float_k);
        EXPECT_EQ(result.get(), static_cast<double>(1 + i + float_k));
    }

    for (auto i = 0; i < task_count; ++i)
    {
        sync.acquire();
    }

    EXPECT_EQ(counter, task_count);
    EXPECT_EQ(counter, sync.max());
}

}
