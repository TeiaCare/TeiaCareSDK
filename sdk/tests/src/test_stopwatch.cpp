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

#include "test_stopwatch.hpp"

#include <thread>

using namespace std::chrono_literals;

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_stopwatch, start_time_range)
{
    ASSERT_TRUE(s.start_time() > tc::sdk::time_point::min());
    ASSERT_TRUE(s.start_time() < tc::sdk::time_point::max());
    ASSERT_TRUE(s.start_time() < tc::sdk::clock::now());
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, start_time_updated_on_reset)
{
    constexpr int total_count = 1'000;
    for (auto i = 0; i < total_count; ++i)
    {
        auto initial_timepoint = tc::sdk::clock::now();

        auto start_before_reset = s.start_time();
        ASSERT_LT(start_before_reset, initial_timepoint);
        s.reset();
        auto start_sfter_reset = s.start_time();
        ASSERT_GT(start_after_reset, initial_timepoint);
    }
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, start_time_increases_on_reset)
{
    constexpr int total_count = 1'000;
    for (auto i = 0; i < total_count; ++i)
    {
        auto start_before_reset = s.start_time();
        s.reset();
        auto start_sfter_reset = s.start_time();
        ASSERT_GT(start_after_reset, start_before_reset);
    }
}

// NOLINTNEXTLINE
TEST_F(test_stopwatch, elapsed_monotonically_increases)
{
    constexpr int total_count = 10;
    auto elapsed = s.elapsed();
    for (auto i = 0; i < total_count; ++i)
    {
        EXPECT_GT(s.elapsed(), elapsed);
        elapsed = s.elapsed();
        std::this_thread::sleep_for(1ms);
    }

    EXPECT_GT(s.elapsed(), elapsed);
}

using duration_types = testing::Types<std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds>;
TYPED_TEST_SUITE(test_stopwatch_duration_t, duration_types);

TYPED_TEST(test_stopwatch_duration_t, elapsed_duration_types)
{
    using duration_t = TypeParam;
    constexpr auto max_error = 10ms;
    constexpr auto abs_error = std::chrono::duration_cast<duration_t>(max_error).count();

    ASSERT_TRUE(this->s.elapsed() > tc::sdk::time_duration::min());
    ASSERT_TRUE(this->s.elapsed() > std::chrono::duration_cast<duration_t>(tc::sdk::time_duration::min()));
    EXPECT_NEAR(
        static_cast<double>(std::chrono::duration_cast<duration_t>(this->s.elapsed()).count()),
        static_cast<double>((this->s.template elapsed<duration_t>()).count()),
        static_cast<double>(abs_error));

    // see why the template syntax this->s.template elapsed<duration_t>() must be used here:
    // https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords
}

TYPED_TEST(test_stopwatch_duration_t, elapsed_reset_duration_types)
{
    constexpr int total_count = 1'000;

    for (auto i = 0; i < total_count; ++i)
    {
        this->s.reset();
        std::this_thread::sleep_for(1ms);

        const auto start = this->s.start_time();
        const auto elapsed = this->s.elapsed();
        EXPECT_GT(tc::sdk::clock::now(), start + elapsed);
    }
}

}
