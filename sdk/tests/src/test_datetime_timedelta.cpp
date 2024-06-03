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

#include "test_datetime_timedelta.hpp"

namespace tc::sdk::tests
{
TEST_F(test_datetime_timedelta, ctor_empty)
{
    const auto td1 = tc::sdk::timedelta();
    const auto td2 = tc::sdk::timedelta();

    EXPECT_EQ(td1, td2);
    EXPECT_TRUE(td1.is_null());
    EXPECT_TRUE(td2.is_null());
    EXPECT_FALSE(tc::sdk::timedelta(3s).is_null());
}

TEST_F(test_datetime_timedelta, ctor_duration_ref)
{
    constexpr auto one_hour = std::chrono::hours(1);
    constexpr auto one_hour_in_seconds = std::chrono::seconds(3'600);
    EXPECT_EQ(tc::sdk::timedelta(one_hour), tc::sdk::timedelta(one_hour_in_seconds));
}

TEST_F(test_datetime_timedelta, ctor_duration_move)
{
    EXPECT_EQ(tc::sdk::timedelta(1'000ns), tc::sdk::timedelta(1us));
    EXPECT_EQ(tc::sdk::timedelta(1'000us), tc::sdk::timedelta(1ms));
    EXPECT_EQ(tc::sdk::timedelta(1'000ms), tc::sdk::timedelta(1s));
    EXPECT_EQ(tc::sdk::timedelta(60s), tc::sdk::timedelta(1min));
    EXPECT_EQ(tc::sdk::timedelta(60min), tc::sdk::timedelta(1h));
    EXPECT_EQ(tc::sdk::timedelta(24h), tc::sdk::timedelta(std::chrono::days(1)));
}

TEST_F(test_datetime_timedelta, ctor_durations_unordered)
{
    const auto td1 = tc::sdk::timedelta(std::chrono::days(1), 6ns, 5us, 4ms, 3s, 2min, 1h); // 1 day + 01:02:03.004005006
    const auto td2 = tc::sdk::timedelta(25h, 2min, 3s, 4ms, 5us, 6ns); // 25:02:03.004005006

    EXPECT_EQ(td1, td2);
}

TEST_F(test_datetime_timedelta, ctor_durations_overflow)
{
    const auto td1 = tc::sdk::timedelta(std::chrono::days(1), 1h, 2min, 3000ms, 4000us, 5006ns); // 01:02:03.004005006
    const auto td2 = tc::sdk::timedelta(25h, 2min, 3s, 4ms, 5us, 6ns);      // 01:02:03.004005006

    EXPECT_EQ(td1, td2);
}

TEST_F(test_datetime_timedelta, getters)
{
    const auto td1 = tc::sdk::timedelta(11h, 22min, 33s, 44ms, 55us, 66ns);
    const auto td2 = tc::sdk::timedelta(std::chrono::days(1), 12h, 23min, 34s, 45ms, 56us, 67ns);

    EXPECT_EQ(td1.hours(), 11h);
    EXPECT_EQ(td1.minutes(), 22min);
    EXPECT_EQ(td1.seconds(), 33s);
    EXPECT_EQ(td1.milliseconds(), 44ms);
    EXPECT_EQ(td1.microseconds(), 55us);
    EXPECT_EQ(td1.nanoseconds(), 66ns);

    EXPECT_EQ(td1.hours() + 1h + std::chrono::days(1), td2.hours());
    EXPECT_EQ(td1.minutes() + 1min, td2.minutes());
    EXPECT_EQ(td1.seconds() + 1s, td2.seconds());
    EXPECT_EQ(td1.milliseconds() + 1ms, td2.milliseconds());
    EXPECT_EQ(td1.microseconds() + 1us, td2.microseconds());
    EXPECT_EQ(td1.nanoseconds() + 1ns, td2.nanoseconds());
}

TEST_F(test_datetime_timedelta, durations)
{
    const auto td = tc::sdk::timedelta(std::chrono::days(367), 11h, 22min, 33s, 44ms, 55us, 66ns);

    EXPECT_EQ(td.total_years(), std::chrono::years(1));
    EXPECT_EQ(td.total_months(), std::chrono::months(12));
    EXPECT_EQ(td.total_days(), std::chrono::days(367));
    EXPECT_EQ(td.total_hours(), 11h + std::chrono::days(367));
    EXPECT_EQ(td.total_minutes(), 22min + td.total_hours());
    EXPECT_EQ(td.total_seconds(), 33s + td.total_minutes());
    EXPECT_EQ(td.total_milliseconds(), 44ms + td.total_seconds());
    EXPECT_EQ(td.total_microseconds(), 55us + td.total_milliseconds());
    EXPECT_EQ(td.total_nanoseconds(), 66ns + td.total_microseconds());
}

TEST_F(test_datetime_timedelta, operator_equal)
{
    const auto td1 = tc::sdk::timedelta(1min);             // 00:01:00
    const auto td2 = tc::sdk::timedelta(60s);              // 00:01:00
    const auto td3 = tc::sdk::timedelta(60'000ms);         // 00:01:00
    const auto td4 = tc::sdk::timedelta(60'000'000us);     // 00:01:00
    const auto td5 = tc::sdk::timedelta(60'000'000'000ns); // 00:01:00
    const auto td6 = tc::sdk::timedelta(1min, 1ms);        // 00:01:00.001

    EXPECT_TRUE(td1 == td2);
    EXPECT_TRUE(td1 == td3);
    EXPECT_TRUE(td1 == td4);
    EXPECT_TRUE(td1 == td5);

    EXPECT_TRUE(td1 != td6);
    EXPECT_TRUE(td2 != td6);
    EXPECT_TRUE(td3 != td6);
}

TEST_F(test_datetime_timedelta, operator_plus)
{
    const auto delta = tc::sdk::timedelta(1h, 1s);
    const auto t1 = tc::sdk::timedelta(2min, 2s);     // 00:01:02
    const auto t2 = tc::sdk::timedelta(1h, 2min, 3s); // 01:02:03

    EXPECT_EQ(t1 + delta, t2);
    EXPECT_EQ(t2 + delta, tc::sdk::timedelta(2h, 2min, 4s));
}


TEST_F(test_datetime_timedelta, operator_minus)
{
    const auto delta = tc::sdk::timedelta(1h, 1s);
    const auto t1 = tc::sdk::timedelta(1h, 1min, 2s); // 01:01:02
    const auto t2 = tc::sdk::timedelta(1min, 1s);     // 00:01:01

    EXPECT_EQ(t1 - delta, t2);
    EXPECT_EQ(t2 - delta, tc::sdk::timedelta(-59min));
}

TEST_F(test_datetime_timedelta, operator_mul)
{
    const int64_t multiplier = 4;
    const auto t1 = tc::sdk::timedelta(30min);
    const auto t2 = tc::sdk::timedelta(2h);
    const auto t3 = tc::sdk::timedelta(7'200'000ms);

    EXPECT_EQ(t1 * multiplier, t2);
    EXPECT_EQ(t1 * multiplier, t3);
}

TEST_F(test_datetime_timedelta, operator_div)
{
    const int64_t divider = 5;
    const auto t1 = tc::sdk::timedelta(1h);
    const auto t2 = tc::sdk::timedelta(12min);
    const auto t3 = tc::sdk::timedelta(720'000ms);

    EXPECT_EQ(t1 / divider, t2);
    EXPECT_EQ(t1 / divider, t3);
}

TEST_F(test_datetime_timedelta, ostream)
{
    {
        const auto t = tc::sdk::timedelta(std::chrono::days(1)); // 00:00:00
        std::stringstream stream;
        stream << t;
        EXPECT_STREQ(stream.str().c_str(), t.to_string().c_str());
    }

    {
        std::stringstream stream;
        stream << tc::sdk::timedelta(1h, 1min, 1s);
        EXPECT_STREQ(stream.str().c_str(), tc::sdk::timedelta(3'661s).to_string().c_str());
    }
}

TEST_F(test_datetime_timedelta, to_string)
{
    EXPECT_EQ(tc::sdk::timedelta(std::chrono::days(2)).to_string(), "48:00:00.000");
    EXPECT_EQ(tc::sdk::timedelta(std::chrono::days(1)).to_string(), "24:00:00.000");
    EXPECT_EQ(tc::sdk::timedelta(std::chrono::hours(24)).to_string(), "24:00:00.000");
    EXPECT_EQ(tc::sdk::timedelta(std::chrono::minutes(86'400)).to_string(), "1440:00:00.000");

    EXPECT_EQ(tc::sdk::timedelta(3'662s).to_string(), "01:01:02.000");
    EXPECT_EQ(tc::sdk::timedelta(-1h, -1min, -1s).to_string(), "-01:01:01.000");
    EXPECT_EQ(tc::sdk::timedelta(1h, 2min, 3000ms, 4000us, 5006ns).to_string(), "01:02:03.004");
    EXPECT_EQ(tc::sdk::timedelta(6ns, 5us, 4ms, 3s, 2min, 1h).to_string(), "01:02:03.004");
    EXPECT_EQ(tc::sdk::timedelta(1h, 2min, 3s, 4ms, 5us, 6ns).to_string(), "01:02:03.004");
}

TEST_F(test_datetime_timedelta, to_string_resolution)
{
    const auto t = tc::sdk::timedelta(1h, 2min, 3000ms, 4000us, 5006ns);

    EXPECT_EQ(t.to_string<std::chrono::seconds>(), "01:02:03");
    EXPECT_EQ(t.to_string<std::chrono::milliseconds>(), "01:02:03.004");
    EXPECT_EQ(t.to_string<std::chrono::microseconds>(), "01:02:03.004005");
    EXPECT_EQ(t.to_string<std::chrono::nanoseconds>(), "01:02:03.004005006");
}

}
