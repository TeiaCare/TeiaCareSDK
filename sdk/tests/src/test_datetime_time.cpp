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

#include "test_datetime_time.hpp"

namespace tc::sdk::tests
{
TEST_F(test_datetime_time, ctor_empty)
{
    const auto t1 = tc::sdk::time();
    const auto t2 = tc::sdk::time();

    EXPECT_EQ(t1, t2);
    EXPECT_FALSE(t1.is_valid());
    EXPECT_FALSE(t2.is_valid());
}

TEST_F(test_datetime_time, ctor_durations_ref)
{
    const auto h = std::chrono::hours(23);
    const auto m = std::chrono::minutes(59);
    const auto s = std::chrono::seconds(59);

    const auto t1 = tc::sdk::time(h, m, s);         // 23:59:59
    const auto t2 = tc::sdk::time(23h, 59min, 59s); // 23:59:59
    const auto t3 = tc::sdk::time(12s);             // 00:00:12

    EXPECT_EQ(t1, t2);
    EXPECT_NE(t1, t3);

    EXPECT_TRUE(t1.is_valid());
    EXPECT_TRUE(t2.is_valid());
    EXPECT_TRUE(t3.is_valid());
}

TEST_F(test_datetime_time, ctor_durations_move)
{
    const auto t1 = tc::sdk::time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(59)); // 23:59:59
    const auto t2 = tc::sdk::time(23h, 59min, 59s);                                                            // 23:59:59
    const auto t3 = tc::sdk::time(12s);                                                                        // 00:00:12

    EXPECT_EQ(t1, t2);
    EXPECT_NE(t1, t3);

    EXPECT_TRUE(t1.is_valid());
    EXPECT_TRUE(t2.is_valid());
    EXPECT_TRUE(t3.is_valid());
}

TEST_F(test_datetime_time, ctor_durations_unordered)
{
    const auto t1 = tc::sdk::time(6ns, 5us, 4ms, 3s, 2min, 1h); // 01:02:03.004005006
    const auto t2 = tc::sdk::time(1h, 2min, 3s, 4ms, 5us, 6ns); // 01:02:03.004005006

    EXPECT_EQ(t1, t2);

    EXPECT_TRUE(t1.is_valid());
    EXPECT_TRUE(t2.is_valid());
}

TEST_F(test_datetime_time, ctor_durations_overflow)
{
    const auto t1 = tc::sdk::time(1h, 2min, 3000ms, 4000us, 5006ns); // 01:02:03.004005006
    const auto t2 = tc::sdk::time(1h, 2min, 3s, 4ms, 5us, 6ns);      // 01:02:03.004005006

    EXPECT_EQ(t1, t2);

    EXPECT_TRUE(t1.is_valid());
    EXPECT_TRUE(t2.is_valid());
}

TEST_F(test_datetime_time, ctor_durations_sum)
{
    const auto t1 = tc::sdk::time(2min);                   // 00:02:00
    const auto t2 = tc::sdk::time(80s, 20s, 1'000ms, 19s); // 00:02:00

    EXPECT_EQ(t1, t2);

    EXPECT_TRUE(t1.is_valid());
    EXPECT_TRUE(t2.is_valid());
}

TEST_F(test_datetime_time, not_valid)
{
    const auto t1 = tc::sdk::time();
    EXPECT_FALSE(t1.is_valid());

    const auto t2 = tc::sdk::time(std::chrono::nanoseconds::min());
    EXPECT_TRUE(t2.is_valid()); // it is valid since the duration is clamped to 24h

    const auto t3 = tc::sdk::time(std::chrono::nanoseconds::max());
    EXPECT_TRUE(t3.is_valid()); // it is valid since the duration is clamped to 24h
}

TEST_F(test_datetime_time, getters)
{
    const auto t1 = tc::sdk::time(11h, 22min, 33s, 44ms, 55us, 66ns);
    const auto t2 = tc::sdk::time(12h, 23min, 34s, 45ms, 56us, 67ns);

    EXPECT_EQ(t1.hours(), 11h);
    EXPECT_EQ(t1.minutes(), 22min);
    EXPECT_EQ(t1.seconds(), 33s);
    EXPECT_EQ(t1.milliseconds(), 44ms);
    EXPECT_EQ(t1.microseconds(), t1.milliseconds() + 55us);
    EXPECT_EQ(t1.nanoseconds(), t1.microseconds() + 66ns);

    EXPECT_EQ(t1.hours() + 1h, t2.hours());
    EXPECT_EQ(t1.minutes() + 1min, t2.minutes());
    EXPECT_EQ(t1.seconds() + 1s, t2.seconds());
    EXPECT_EQ(t1.milliseconds() + 1ms, t2.milliseconds());
    EXPECT_EQ(t1.microseconds() + 1ms + 1us, t2.microseconds());
    EXPECT_EQ(t1.nanoseconds() + 1ms + 1us + 1ns, t2.nanoseconds());
}

TEST_F(test_datetime_time, to_duration)
{
    const auto t1 = tc::sdk::time(11h, 22min, 33s, 44ms, 55us, 66ns);
    EXPECT_EQ(t1.hours() + t1.minutes() + t1.seconds() + t1.nanoseconds(), t1.to_duration());

    const auto t2 = tc::sdk::time(25h, 61min, 61s);
    EXPECT_EQ(t2.hours() + t2.minutes() + t2.seconds() + t2.nanoseconds(), t2.to_duration());
}

TEST_F(test_datetime_time, operator_equal)
{
    const auto t1 = tc::sdk::time(1min);             // 00:01:00
    const auto t2 = tc::sdk::time(60s);              // 00:01:00
    const auto t3 = tc::sdk::time(60'000ms);         // 00:01:00
    const auto t4 = tc::sdk::time(60'000'000us);     // 00:01:00
    const auto t5 = tc::sdk::time(60'000'000'000ns); // 00:01:00
    const auto t6 = tc::sdk::time(1min, 1ms);        // 00:01:00.001

    EXPECT_TRUE(t1 == t2);
    EXPECT_TRUE(t1 == t3);
    EXPECT_TRUE(t1 == t4);
    EXPECT_TRUE(t1 == t5);
    EXPECT_TRUE(t1 == t6);

    EXPECT_TRUE(t1 != t6);
    EXPECT_TRUE(t2 != t6);
    EXPECT_TRUE(t3 != t6);
}

TEST_F(test_datetime_time, operator_plus_timedelta)
{
    const auto delta = tc::sdk::timedelta(1h, 1s);
    const auto t1 = tc::sdk::time(2min, 2s);     // 00:01:02
    const auto t2 = tc::sdk::time(1h, 2min, 3s); // 01:02:03

    EXPECT_EQ(t1 + delta, t2);
    EXPECT_EQ(t2 + delta, tc::sdk::time(2h, 2min, 4s));
}

TEST_F(test_datetime_time, operator_plus_timedelta_long)
{
    const auto t1 = tc::sdk::time(1h, 2min, 3s); // 01:02:03
    EXPECT_EQ(t1 + tc::sdk::timedelta(std::chrono::days(1)), t1);
}

TEST_F(test_datetime_time, operator_minus_timedelta)
{
    const auto delta = tc::sdk::timedelta(1h, 1s);
    const auto t1 = tc::sdk::time(1h, 1min, 2s); // 01:01:02
    const auto t2 = tc::sdk::time(1min, 1s);     // 00:01:01

    EXPECT_EQ(t1 - delta, t2);
    EXPECT_EQ(t2 - delta, tc::sdk::time(-59min));
}

TEST_F(test_datetime_time, operator_minus_timedelta_long)
{
    const auto t1 = tc::sdk::time(1h, 1min, 1s); // 01:01:01
    EXPECT_EQ(t1 - tc::sdk::timedelta(std::chrono::days(1)), tc::sdk::time(-22h, -58min, -59s));
}

TEST_F(test_datetime_time, operator_minus)
{
    const auto t1 = tc::sdk::time(1h, 1min, 1s);    // 01:01:01
    const auto t2 = tc::sdk::time(-1h, -1min, -1s); // -01:01:01

    EXPECT_EQ(t2 - t1, tc::sdk::timedelta(-2h, -2min, -2s));
    EXPECT_EQ(t1 - t2, tc::sdk::timedelta(2h, 2min, 2s));
}

TEST_F(test_datetime_time, operator_plus)
{
    const auto t1 = tc::sdk::time(1h, 1min, 1s);    // 01:01:01
    const auto t2 = tc::sdk::time(-1h, -1min, -1s); // -01:01:01

    EXPECT_EQ(t1 + t1, tc::sdk::timedelta(2h, 2min, 2s));
    EXPECT_EQ(t1 + t2, tc::sdk::timedelta(0s));
}

TEST_F(test_datetime_time, operator_comparison)
{
    const auto t1 = tc::sdk::time(3'662s);               // 01:01:02
    const auto t2 = tc::sdk::time(1h, 1min, 1s);         // 01:01:01
    const auto t3 = tc::sdk::time(std::chrono::days(1)); // 00:00:00

    EXPECT_TRUE(t1 > t2);
    EXPECT_TRUE(t2 > t3);

    EXPECT_TRUE(t3 < t1);
    EXPECT_TRUE(t3 < t2);
}

TEST_F(test_datetime_time, ostream)
{
    {
        const auto t = tc::sdk::time(std::chrono::days(1)); // 00:00:00
        std::stringstream stream;
        stream << t;
        EXPECT_STREQ(stream.str().c_str(), t.to_string().c_str());
    }

    {
        std::stringstream stream;
        stream << tc::sdk::time(1h, 1min, 1s);
        EXPECT_STREQ(stream.str().c_str(), tc::sdk::time(3'661s).to_string().c_str());
    }
}

TEST_F(test_datetime_time, to_string)
{
    EXPECT_EQ(tc::sdk::time(std::chrono::days(1)).to_string(), "00:00:00.000");
    EXPECT_EQ(tc::sdk::time(std::chrono::hours(24)).to_string(), "00:00:00.000");
    EXPECT_EQ(tc::sdk::time(std::chrono::minutes(86'400)).to_string(), "00:00:00.000");

    EXPECT_EQ(tc::sdk::time(3'662s).to_string(), "01:01:02.000");
    EXPECT_EQ(tc::sdk::time(-1h, -1min, -1s).to_string(), "-01:01:01.000");
    EXPECT_EQ(tc::sdk::time(1h, 2min, 3000ms, 4000us, 5006ns).to_string(), "01:02:03.004");
    EXPECT_EQ(tc::sdk::time(6ns, 5us, 4ms, 3s, 2min, 1h).to_string(), "01:02:03.004");
    EXPECT_EQ(tc::sdk::time(1h, 2min, 3s, 4ms, 5us, 6ns).to_string(), "01:02:03.004");
}

TEST_F(test_datetime_time, to_string_resolution)
{
    const auto t = tc::sdk::time(1h, 2min, 3000ms, 4000us, 5006ns);

    EXPECT_EQ(t.to_string<std::chrono::seconds>(), "01:02:03");
    EXPECT_EQ(t.to_string<std::chrono::milliseconds>(), "01:02:03.004");
    EXPECT_EQ(t.to_string<std::chrono::microseconds>(), "01:02:03.004005");
    EXPECT_EQ(t.to_string<std::chrono::nanoseconds>(), "01:02:03.004005006");
}

TEST_F(test_datetime_time, to_string_format)
{
    const auto t = tc::sdk::time(1h, 2min, 3000ms, 4000us, 5006ns);

    EXPECT_EQ(t.to_string(), "01:02:03.004");
    EXPECT_EQ(t.to_string("%T"), "01:02:03.004");
    EXPECT_EQ(t.to_string("%R"), "01:02");
    EXPECT_EQ(t.to_string("%H"), "01");
    EXPECT_EQ(t.to_string("%M"), "02");
    EXPECT_EQ(t.to_string("%S"), "03.004");
}

TEST_F(test_datetime_time, from_string)
{
    EXPECT_EQ(tc::sdk::time::from_string<std::chrono::seconds>("01:02:03"), tc::sdk::time(1h, 2min, 3s));
    EXPECT_EQ(tc::sdk::time::from_string<std::chrono::milliseconds>("01:02:03.004"), tc::sdk::time(1h, 2min, 3s, 4ms));
    EXPECT_EQ(tc::sdk::time::from_string<std::chrono::microseconds>("01:02:03.004005"), tc::sdk::time(1h, 2min, 3s, 4ms, 5us));
    EXPECT_EQ(tc::sdk::time::from_string<std::chrono::nanoseconds>("01:02:03.004005006"), tc::sdk::time(1h, 2min, 3s, 4ms, 5us, 6ns));

    const auto t_hours = tc::sdk::time(1h);
    EXPECT_EQ(t_hours, tc::sdk::time::from_string<std::chrono::hours>("01:02:03.004"));

    const auto t_minutes = tc::sdk::time(1h, 2min);
    EXPECT_EQ(t_minutes, tc::sdk::time::from_string<std::chrono::minutes>("01:02:03.004"));

    const auto t_seconds = tc::sdk::time(1h, 2min, 3s);
    EXPECT_EQ(t_seconds, tc::sdk::time::from_string<std::chrono::seconds>("01:02:03.004"));
    EXPECT_EQ(t_seconds, tc::sdk::time::from_string<std::chrono::seconds>("01:02:03.004005"));
    EXPECT_EQ(t_seconds, tc::sdk::time::from_string<std::chrono::seconds>("01:02:03.004005006"));

    const auto t_milliseconds = tc::sdk::time(1h, 2min, 3s, 4ms);
    EXPECT_GT(t_milliseconds, tc::sdk::time::from_string<std::chrono::milliseconds>("01:02:03"));
    EXPECT_EQ(t_milliseconds, tc::sdk::time::from_string<std::chrono::milliseconds>("01:02:03.004"));
    EXPECT_EQ(t_milliseconds, tc::sdk::time::from_string<std::chrono::milliseconds>("01:02:03.004005"));
    EXPECT_EQ(t_milliseconds, tc::sdk::time::from_string<std::chrono::milliseconds>("01:02:03.004005006"));
    EXPECT_LT(t_milliseconds, tc::sdk::time::from_string<std::chrono::milliseconds>("01:02:03.005"));

    const auto t_microseconds = tc::sdk::time(1h, 2min, 3s, 4ms, 5us);
    EXPECT_GT(t_microseconds, tc::sdk::time::from_string<std::chrono::microseconds>("01:02:03"));
    EXPECT_GT(t_microseconds, tc::sdk::time::from_string<std::chrono::microseconds>("01:02:03.004"));
    EXPECT_EQ(t_microseconds, tc::sdk::time::from_string<std::chrono::microseconds>("01:02:03.004005"));
    EXPECT_EQ(t_microseconds, tc::sdk::time::from_string<std::chrono::microseconds>("01:02:03.004005006"));
    EXPECT_LT(t_microseconds, tc::sdk::time::from_string<std::chrono::microseconds>("01:02:03.004006"));

    const auto t_nanoseconds = tc::sdk::time(1h, 2min, 3s, 4ms, 5us, 6ns);
    EXPECT_GT(t_nanoseconds, tc::sdk::time::from_string<std::chrono::nanoseconds>("01:02:03"));
    EXPECT_GT(t_nanoseconds, tc::sdk::time::from_string<std::chrono::nanoseconds>("01:02:03.004"));
    EXPECT_GT(t_nanoseconds, tc::sdk::time::from_string<std::chrono::nanoseconds>("01:02:03.004005"));
    EXPECT_EQ(t_nanoseconds, tc::sdk::time::from_string<std::chrono::nanoseconds>("01:02:03.004005006"));
    EXPECT_LT(t_nanoseconds, tc::sdk::time::from_string<std::chrono::nanoseconds>("01:02:03.004005007"));

    EXPECT_THROW(tc::sdk::time::from_string("BOOM!"), std::runtime_error);
    EXPECT_THROW(tc::sdk::time::from_string("01-02-03-04-05"), std::runtime_error);
}

TEST_F(test_datetime_time, utc_now)
{
    const auto utc_now = tc::sdk::time::utc_now();
    const auto current_time = std::chrono::system_clock::now() - std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto current_time_minus_1_minute = current_time - 1min;

    EXPECT_GT(current_time, utc_now.to_duration());
    EXPECT_LT(current_time_minus_1_minute, utc_now.to_duration());
}

}
