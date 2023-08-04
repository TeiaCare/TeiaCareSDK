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
TEST_F(test_thread_pool, start_twice)
{
    EXPECT_TRUE(tp->start(num_threads));
    EXPECT_FALSE(tp->start(num_threads+1));
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, start_twice_after_run)
{
    EXPECT_TRUE(tp->start(num_threads));

    std::binary_semaphore sync(0);
    auto task = [&sync]{ sync.release(); };
    tp->run(task);
    sync.acquire();

    EXPECT_FALSE(tp->start(num_threads+1));
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
    auto task = [&sync]{ sync.release(); };
    tp->run(task);
    sync.acquire();

    EXPECT_TRUE(tp->stop());
    EXPECT_FALSE(tp->stop());
}

// NOLINTNEXTLINE
TEST_F(test_thread_pool, run)
{
    EXPECT_TRUE(tp->start(num_threads));

    constexpr unsigned int task_count = 10;
    std::counting_semaphore<task_count> sync(0);

    std::atomic_int counter = 0;
    auto task = [&sync, &counter]{
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);
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

}
