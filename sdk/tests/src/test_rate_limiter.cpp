#include "test_rate_limiter.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace tc::sdk::tests
{
//NOLINTNEXTLINE
TEST(test_rate_limiter, sync)
{
    const auto rate = 4;
    tc::sdk::rate_limiter limiter(rate);

    const auto timing_duration = 1;
    auto timer = elapsed_timer(std::chrono::seconds(timing_duration));

    auto tick_counter = 0;
    const auto expected_ticks = rate * timing_duration;
    while(!timer.is_elapsed())
    {
        ++tick_counter;
        limiter.sync();
    }
    
    EXPECT_EQ(tick_counter, expected_ticks);
}

//NOLINTNEXTLINE
TEST(test_rate_limiter, sync_same_rate)
{
    const auto rate = 1;
    tc::sdk::rate_limiter limiter(rate);

    const auto timing_duration = 1;
    auto timer = elapsed_timer(std::chrono::seconds(timing_duration));

    auto tick_counter = 0;
    const auto expected_ticks = rate * timing_duration;
    while(!timer.is_elapsed())
    {
        ++tick_counter;
        limiter.sync();
    }
    
    EXPECT_EQ(tick_counter, expected_ticks);
}

//NOLINTNEXTLINE
TEST(test_rate_limiter, sync_fractional_duration)
{
    const auto rate = 1;
    tc::sdk::rate_limiter limiter(rate);

    const auto timing_duration = 0.25; // 1/4 second
    auto timer = elapsed_timer(std::chrono::milliseconds(250)); // NOLINT

    auto tick_counter = 0;
    /* expected_ticks is 1 since the rate is 1 tick per second but the timing_duration is 250 millis, so
     * only the initial tick is performed
     */
    const auto expected_ticks = 1; 
    while(!timer.is_elapsed())
    {
        ++tick_counter;
        limiter.sync();
    }
    
    EXPECT_EQ(tick_counter, expected_ticks);
}

//NOLINTNEXTLINE
TEST(test_rate_limiter, sync_lower_delay)
{
    const auto rate = 10;
    tc::sdk::rate_limiter limiter(rate);

    const auto timing_duration = 1;
    auto timer = elapsed_timer(std::chrono::seconds(timing_duration));

    auto tick_counter = 0;

    /* expected_ticks is 10 even if a delay is introduced in the loop because
     * the duration of the delay (50 millis) is lower than a tick (100 mills)
     */
    const auto expected_ticks = rate * timing_duration;
    while(!timer.is_elapsed())
    {
        ++tick_counter;
        std::this_thread::sleep_for(50ms);
        limiter.sync();
    }
    
    EXPECT_EQ(tick_counter, expected_ticks);
}

//NOLINTNEXTLINE
TEST(test_rate_limiter, sync_higher_delay)
{
    const auto rate = 10;
    tc::sdk::rate_limiter limiter(rate);

    const auto timing_duration = 1;
    auto timer = elapsed_timer(std::chrono::seconds(timing_duration));

    auto tick_counter = 0;

    /* expected_ticks is 4 because of the delay introduced in the loop.
     * Since the timing_duration is 1 seconds and the delay is 250 millis, the ticks performed are only 4.
     */
    const auto expected_ticks = 4;
    while(!timer.is_elapsed())
    {
        ++tick_counter;
        std::this_thread::sleep_for(250ms);
        limiter.sync();
    }
    
    EXPECT_EQ(tick_counter, expected_ticks);
}

}
