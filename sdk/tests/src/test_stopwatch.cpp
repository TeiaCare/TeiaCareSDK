#include "test_stopwatch.hpp"

using namespace std::chrono_literals;

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_stopwatch, start_time)
{
    ASSERT_TRUE(s.start_time() > tc::sdk::time_point::min());
    ASSERT_TRUE(s.start_time() < tc::sdk::time_point::max());
    ASSERT_TRUE(s.start_time() < tc::sdk::clock::now());
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, elapsed_seconds)
{
    using duration_t = std::chrono::seconds;

    ASSERT_TRUE(s.elapsed() > tc::sdk::time_duration::min());
    ASSERT_TRUE(s.elapsed() > std::chrono::duration_cast<duration_t>(tc::sdk::time_duration::min()));

    auto elapsed = s.elapsed<duration_t>();
    EXPECT_EQ(std::chrono::duration_cast<duration_t>(s.elapsed()), elapsed);
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, elapsed_milliseconds)
{
    using duration_t = std::chrono::milliseconds;

    ASSERT_TRUE(s.elapsed() > tc::sdk::time_duration::min());
    ASSERT_TRUE(s.elapsed() > std::chrono::duration_cast<duration_t>(tc::sdk::time_duration::min()));

    auto elapsed = s.elapsed<duration_t>();
    EXPECT_EQ(std::chrono::duration_cast<duration_t>(s.elapsed()), elapsed);
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, elapsed_multiple)
{
    const int total_count = 10;
    auto elapsed = s.elapsed();
    for (auto i = 0; i < total_count; ++i)
    {
        EXPECT_GT(s.elapsed(), elapsed);
        elapsed = s.elapsed();
        std::this_thread::sleep_for(1ms);
    }
}


// NOLINTNEXTLINE
TEST_F(test_stopwatch, elapsed_reset_multiple)
{
    const auto variance_microseconds = 500;
    const int total_count = 10;
    auto prev_elapsed = s.elapsed<std::chrono::microseconds>().count();
    for (auto i = 0; i < total_count; ++i)
    {
        auto elapsed = s.elapsed<std::chrono::microseconds>().count();
        EXPECT_NEAR(elapsed, prev_elapsed, variance_microseconds);
        prev_elapsed = elapsed;
        s.reset();
    }
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, reset)
{
    auto start = s.start_time();
    std::this_thread::sleep_for(5ms);
    s.reset();

    ASSERT_GT(s.start_time(), start);
}

}