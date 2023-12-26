#include "test_high_precision_timer.hpp"
#include "utils.hpp"
#include <gmock/gmock.h>

#include <semaphore>
// #include <chrono>
// #include <future>
// #include <latch>
// #include <barrier>

using namespace std::chrono_literals;

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, start_without_callback)
{
    const auto interval = 250ms;
    EXPECT_FALSE(t.start(interval));
    EXPECT_FALSE(t.start(interval));
    EXPECT_FALSE(t.start(interval));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, start)
{
    const auto interval = 250ms;
    EXPECT_FALSE(t.start(interval));

    auto callback = [](){};
    t.set_callback(callback);

    EXPECT_TRUE(t.start(interval));
    EXPECT_TRUE(t.start(interval));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, stop_before_start_without_callback)
{
    const auto interval = 250ms;

    t.stop();
    EXPECT_FALSE(t.start(interval));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, start_multiple_times)
{
    auto callback = [](){};
    t.set_callback(callback);

    const auto interval = 250ms;
    EXPECT_TRUE(t.start(interval));
    EXPECT_TRUE(t.start(interval));
    EXPECT_TRUE(t.start(interval));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, start_callback_with_simple_arguments)
{
    const auto interval = 250ms;
    
    std::binary_semaphore sync(0);
    auto callback_count = 0;
    auto callback = [&sync, &callback_count](float f, const std::string& s){
        EXPECT_EQ(f, 1.2345f);
        EXPECT_EQ(s, "__hello__");
        ++callback_count;
        sync.release();
    };

    t.set_callback(callback, 1.2345f, "__hello__"); //NOLINT

    EXPECT_TRUE(t.start(interval));
    sync.acquire();
    EXPECT_EQ(callback_count, 1);

    EXPECT_TRUE(t.start(interval));
    sync.acquire();
    EXPECT_EQ(callback_count, 2);
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, start_callback_with_custom_arguments)
{
    const auto interval = 250ms;

    struct foo_t
    {
        int i = 98765;
    };

    std::binary_semaphore sync(0);
    auto callback_count = 0;
    auto callback = [&sync, &callback_count](const foo_t& foo1, foo_t foo2, foo_t foo3){
        EXPECT_EQ(foo1.i, 98765);
        EXPECT_EQ(foo2.i, 98765);
        EXPECT_EQ(foo3.i, 1);
        ++callback_count;
        sync.release();
    };

    foo_t foo;
    foo_t another_foo;
    another_foo.i = 1;
    t.set_callback(callback, foo, foo_t{}, std::move(another_foo));

    EXPECT_TRUE(t.start(interval));
    sync.acquire();
    EXPECT_EQ(callback_count, 1);

    EXPECT_TRUE(t.start(interval));
    sync.acquire();
    EXPECT_EQ(callback_count, 2);
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, callback_count_delay)
{
    uint64_t callback_count = 0;
    auto callback = [&callback_count]{
        const auto execution_duration = 25ms;
        ++callback_count;
        std::this_thread::sleep_for(execution_duration * callback_count);
    };

    t.set_callback(callback);

    const auto interval = 100ms;
    t.start(interval);

    const auto test_duration_time = 500ms;
    std::this_thread::sleep_for(test_duration_time);
    t.stop();

    EXPECT_EQ(callback_count, t.invoked_callback_count());
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, callback_invoked_count)
{
/*
    execution_duration < execution_interval

    - invoked_callback_count == 5
        Every callback must be executed and completed before the next one so each of the 5 callbacks is invoked.

    - missed_callback_count == 0
        No scheduled callback is missed since the execution_duration (10ms) of a single callback is less than the execution_interval (100ms)
*/

    const auto execution_duration = 10ms;
    t.set_callback([&]{ std::this_thread::sleep_for(execution_duration); });
    
    const auto execution_interval = 100ms;
    t.start(execution_interval);
    
    const auto test_duration_time = 500ms;
    std::this_thread::sleep_for(test_duration_time);
    t.stop();

    EXPECT_THAT(t.invoked_callback_count(), testing::Eq(5));
    EXPECT_THAT(t.missed_callback_count(), testing::Eq(0));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, callback_missed_count)
{
/*
    execution_duration > test_duration_time

    - invoked_callback_count == 1 
        Only the first callback can be invoked because of the execution_duration longer than the 
        test_duration_time (i.e. when the test finishes the first callback is still running)

    - missed_callback_count == 10
        Since the execution_duration of a single callback is 1s it is possible to execute one
        callback (with execution_interval=100ms) every 10 callbacks.
*/

    const auto execution_duration = 1s;
    t.set_callback([&]{ std::this_thread::sleep_for(execution_duration); });
    
    const auto execution_interval = 100ms;
    t.start(execution_interval);

    const auto test_duration_time = 500ms;
    std::this_thread::sleep_for(test_duration_time);
    t.stop();

    EXPECT_THAT(t.invoked_callback_count(), testing::Eq(1));
    EXPECT_THAT(t.missed_callback_count(), testing::Eq(10));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, callback_invoked_missed_count)
{
/*
[timeline]:     0...1...2...3...4...5...6...7...8...9...
(millis)        |   |   |   |   |   |   |   |   |   |
                |   |   |   |   |   |   |   |   |   |
Required Ticks: T---T---T---T---T---T---T---T---T---T---
(@10ms=10 ticks)|   |   |   |   |   |   |   |   |   |
                |   |   |   |   |   |   |   |   |   |   
Executed Ticks: OOOOOOO-OOOOOOO-OOOOOOO-OOOOOOO-OOOOOOO-
(due to delay)  |   |   |   |   |   |   |   |   |   |
                |   |   |   |   |   |   |   |   |   |   
Missed Ticks:   ----X-------X-------X-------X-------X---
*/

    const auto interval = 100ms;

    auto callback_count = 0;
    auto callback = [&callback_count]{
        std::this_thread::sleep_for(150ms); //NOLINT
        ++callback_count;
    };

    t.set_callback(callback);
    t.start(interval);

    const auto test_duration_time = 1s;
    std::this_thread::sleep_for(test_duration_time);
    t.stop();

    EXPECT_THAT(callback_count, RANGE(4, 5)); 
    EXPECT_THAT(t.invoked_callback_count(), RANGE(4, 5));
    EXPECT_THAT(t.missed_callback_count(), RANGE(4, 5));
}

// NOLINTNEXTLINE
TEST_F(test_high_precision_timer, reset_callback)
{
    std::binary_semaphore sync(0);
    const int total_count = 10;

    auto callback_count_1 = 0;
    auto callback_1 = [&sync, &callback_count_1]{
        ++callback_count_1;
        sync.release();
    };

    t.set_callback(callback_1);
    const auto interval1 = 100ms;
    EXPECT_TRUE(t.start(interval1));
    for(int i = 1; i < total_count; ++i)
    {
        sync.acquire();
        EXPECT_EQ(callback_count_1, i);
    }

    auto callback_count_2 = 0;
    auto callback_2 = [&sync, &callback_count_2]{
        ++callback_count_2;
        sync.release();
    };

    const auto interval2 = 200ms;
    t.set_callback(callback_2);
    EXPECT_TRUE(t.start(interval2));
    for(int i = 1; i < total_count; ++i)
    {
        sync.acquire();
        EXPECT_EQ(callback_count_2, i);
    }
}

}