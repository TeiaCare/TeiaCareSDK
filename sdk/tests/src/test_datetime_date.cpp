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

#include "test_datetime_date.hpp"

namespace tc::sdk::tests
{
TEST_F(test_datetime_date, ctor_empty)
{
    const auto d1 = tc::sdk::date();
    const auto d2 = tc::sdk::date();

    EXPECT_EQ(d1, d2);
    EXPECT_FALSE(d1.is_valid());
    EXPECT_FALSE(d2.is_valid());
}

TEST_F(test_datetime_date, ctor_ymd_ref)
{
    std::chrono::year_month_day ymd1 = 2024y / std::chrono::May / 1d; // 2024 May 1st
    const auto d1 = tc::sdk::date(ymd1);

    std::chrono::year_month_day ymd2(2024y, std::chrono::May, 1d); // 2024 May 1st
    const auto d2 = tc::sdk::date(ymd2);

    const auto ymd3 = ymd1 + std::chrono::months(1);
    const auto d3 = tc::sdk::date(ymd3); // 2024 June 1st

    EXPECT_EQ(d1, d2);
    EXPECT_NE(d1, d3);

    EXPECT_TRUE(d1.is_valid());
    EXPECT_TRUE(d2.is_valid());
    EXPECT_TRUE(d3.is_valid());
}

TEST_F(test_datetime_date, ctor_ymd_move)
{
    std::chrono::year_month_day ymd1 = 2024y / std::chrono::May / 1d; // 2024 May 1st
    const auto d1 = tc::sdk::date(std::move(ymd1));
    const auto d2 = tc::sdk::date(2024y / std::chrono::May / 1d);  // 2024 May 1st
    const auto d3 = tc::sdk::date(2024y / std::chrono::June / 1d); // 2024 June 1st

    EXPECT_EQ(d1, d2);
    EXPECT_NE(d1, d3);

    EXPECT_TRUE(d1.is_valid());
    EXPECT_TRUE(d2.is_valid());
    EXPECT_TRUE(d3.is_valid());
}

TEST_F(test_datetime_date, ctor_year_month_day_ref)
{
    std::chrono::year y = 2024y;
    std::chrono::month m = std::chrono::May;
    std::chrono::day d = 1d;

    const auto d1 = tc::sdk::date(y, m, d);
    const auto d2 = tc::sdk::date(2024y, std::chrono::May, 1d);  // 2024 May 1st
    const auto d3 = tc::sdk::date(2024y, std::chrono::June, 1d); // 2024 June 1st

    EXPECT_EQ(d1, d2);
    EXPECT_NE(d1, d3);

    EXPECT_TRUE(d1.is_valid());
    EXPECT_TRUE(d2.is_valid());
    EXPECT_TRUE(d3.is_valid());
}

TEST_F(test_datetime_date, ctor_year_month_day_move)
{
    std::chrono::year y = 2024y;
    std::chrono::month m = std::chrono::May;
    std::chrono::day d = 1d;

    const auto d1 = tc::sdk::date(std::move(y), std::move(m), std::move(d));
    const auto d2 = tc::sdk::date(2024y, std::chrono::May, 1d);  // 2024 May 1st
    const auto d3 = tc::sdk::date(2024y, std::chrono::June, 1d); // 2024 June 1st

    EXPECT_EQ(d1, d2);
    EXPECT_NE(d1, d3);

    EXPECT_TRUE(d1.is_valid());
    EXPECT_TRUE(d2.is_valid());
    EXPECT_TRUE(d3.is_valid());
}

TEST_F(test_datetime_date, ctor_sys_timepoint)
{
    const tc::sdk::sys_time_point may_1st_2024_time_point(1'714'521'600'000'000'000ns); // 2024 May 1st, in nanoseconds since epoch
    const auto d1 = tc::sdk::date(may_1st_2024_time_point);
    const auto d2 = tc::sdk::date(2024y / std::chrono::May / 1d); // 2024 May 1st

    EXPECT_EQ(d1, d2);

    EXPECT_TRUE(d1.is_valid());
    EXPECT_TRUE(d2.is_valid());
}

TEST_F(test_datetime_date, ctor_duration)
{
    const std::chrono::nanoseconds may_1st_2024_ns(1'714'521'600'000'000'000ns); // 2024 May 1st, in nanoseconds since epoch
    const auto d1 = tc::sdk::date(may_1st_2024_ns);
    const auto d2 = tc::sdk::date(2024y / std::chrono::May / 1d); // 2024 May 1st

    EXPECT_EQ(d1, d2);

    EXPECT_TRUE(d1.is_valid());
    EXPECT_TRUE(d2.is_valid());
}

TEST_F(test_datetime_date, not_valid)
{
    const auto d1 = tc::sdk::date(2024y / std::chrono::May / 99d);
    EXPECT_FALSE(d1.is_valid());
}

TEST_F(test_datetime_date, year_month_day_getters)
{
    const auto d1 = tc::sdk::date(2024y / std::chrono::May / 1d);
    const auto d2 = tc::sdk::date(2025y / std::chrono::June / 2d);

    EXPECT_EQ(d1.year(), std::chrono::year(2024));
    EXPECT_EQ(d1.month(), std::chrono::month(5));
    EXPECT_EQ(d1.day(), std::chrono::day(1));

    EXPECT_EQ(d1.year() + std::chrono::years(1), d2.year());
    EXPECT_EQ(d1.month() + std::chrono::months(1), d2.month());
    EXPECT_EQ(d1.day() + std::chrono::days(1), d2.day());
}

TEST_F(test_datetime_date, weekday_getters)
{
    const auto d1 = tc::sdk::date(2024y / std::chrono::May / 1d);

    EXPECT_EQ(d1.weekday(), std::chrono::Wednesday);
    EXPECT_EQ(d1.iso_weekday(), 3);
}

TEST_F(test_datetime_date, to_duration)
{
    const std::chrono::nanoseconds may_1st_2024_ns(1'714'521'600'000'000'000ns); // 2024 May 1st, in nanoseconds since epoch
    const auto d1 = tc::sdk::date(may_1st_2024_ns);

    std::chrono::sys_days d{std::chrono::floor<std::chrono::days>(may_1st_2024_ns)};

    EXPECT_EQ(d1.to_duration(), d);
}

TEST_F(test_datetime_date, operator_equal)
{
    const auto d1 = tc::sdk::date(2024y / 5 / 1);                // 2024 May 1st
    const auto d2 = tc::sdk::date(2024y, std::chrono::May, 1d);  // 2024 May 1st
    const auto d3 = tc::sdk::date(2024y, std::chrono::June, 1d); // 2024 June 1st

    EXPECT_TRUE(d1 == d2);
    EXPECT_TRUE(d1 != d3);
}

TEST_F(test_datetime_date, operator_plus_timedelta)
{
    const auto delta = tc::sdk::timedelta(48h);
    const auto d1 = tc::sdk::date(2024y, std::chrono::April, 30d); // 2024 May 1st
    const auto d2 = tc::sdk::date(2024y, std::chrono::May, 2d);    // 2024 May 2nd

    EXPECT_EQ(d1 + delta, d2);
    EXPECT_EQ(d2 + delta, tc::sdk::date(2024y, std::chrono::May, 4d));
}

TEST_F(test_datetime_date, operator_plus_timedelta_short)
{
    const auto d1 = tc::sdk::date(2024y, std::chrono::April, 30d);

    EXPECT_EQ(d1 + tc::sdk::timedelta(1s), d1);   // still the very same day since we added just 1 second
    EXPECT_EQ(d1 + tc::sdk::timedelta(1min), d1); // still the very same day since we added just 1 minute
    EXPECT_EQ(d1 + tc::sdk::timedelta(23h), d1);  // still the very same day since we added just 23 hours
}

TEST_F(test_datetime_date, operator_minus_timedelta)
{
    const auto delta = tc::sdk::timedelta(24h);
    const auto d1 = tc::sdk::date(2024y, std::chrono::January, 1d);   // 2024 Jan 1st
    const auto d2 = tc::sdk::date(2023y, std::chrono::December, 31d); // 2023 Dec 31th

    EXPECT_EQ(d1 - delta, d2);
    EXPECT_EQ(d2 - delta, tc::sdk::date(2023y, std::chrono::December, 30d));
}

TEST_F(test_datetime_date, operator_minus_timedelta_short)
{
    const auto today = tc::sdk::date(2024y, std::chrono::April, 30d);
    const auto yesterday = tc::sdk::date(2024y, std::chrono::April, 29d);
    const auto two_days_ago = tc::sdk::date(2024y, std::chrono::April, 28d);

    EXPECT_EQ(today - tc::sdk::timedelta(1s), yesterday);   // it results in the previous day since if we subtracted 1 second
    EXPECT_EQ(today - tc::sdk::timedelta(1min), yesterday); // it results in the previous day since if we subtracted 1 minute
    EXPECT_EQ(today - tc::sdk::timedelta(23h), yesterday);  // it results in the previous day since if we subtracted 23 hours

    EXPECT_EQ(today - tc::sdk::timedelta(24h, 1ms), two_days_ago); // it results in two_days_ago since we subtracted 24 hours and 1 milliseconds (i.e. more than one entire day)
}

TEST_F(test_datetime_date, operator_minus)
{
    const auto d1 = tc::sdk::date(2024y, std::chrono::January, 1d);   // 2024 Jan 1st
    const auto d2 = tc::sdk::date(2023y, std::chrono::December, 31d); // 2023 Dec 31th
    const auto d3 = tc::sdk::date(2023y, std::chrono::December, 1d);  // 2023 Dec 1st

    EXPECT_EQ(d1 - d2, tc::sdk::timedelta(24h));
    EXPECT_EQ(d1 - d2, tc::sdk::timedelta(std::chrono::days(1)));
    EXPECT_EQ(d2 - d3, tc::sdk::timedelta(std::chrono::days(30)));
}

TEST_F(test_datetime_date, operator_comparison)
{
    const auto d1 = tc::sdk::date(2024y, std::chrono::January, 1d);   // 2024 Jan 1st
    const auto d2 = tc::sdk::date(2023y, std::chrono::December, 31d); // 2023 Dec 31th
    const auto d3 = tc::sdk::date(2023y, std::chrono::December, 1d);  // 2023 Dec 1st

    EXPECT_TRUE(d1 > d2);
    EXPECT_TRUE(d2 > d3);

    EXPECT_TRUE(d3 < d1);
    EXPECT_TRUE(d3 < d2);
}

TEST_F(test_datetime_date, ostream)
{
    {
        const auto d = tc::sdk::date(2024y, std::chrono::May, 2d);
        std::stringstream stream;
        stream << d;
        EXPECT_STREQ(stream.str().c_str(), d.to_string().c_str());
    }

    {
        std::stringstream stream;
        stream << tc::sdk::date(2024y, std::chrono::May, 2d);
        EXPECT_STREQ(stream.str().c_str(), tc::sdk::date(2024y, std::chrono::May, 2d).to_string().c_str());
    }
}

TEST_F(test_datetime_date, to_string)
{
    const auto d = tc::sdk::date(2024y, std::chrono::May, 26d);
    EXPECT_EQ(d.to_string(), "2024-05-26");
    EXPECT_EQ(d.to_string("%F"), "2024-05-26");
    EXPECT_EQ(d.to_string("%D"), "05/26/24");
    EXPECT_EQ(d.to_string("%m/%d/%y"), "05/26/24");
    EXPECT_EQ(d.to_string("%d/%m/%Y"), "26/05/2024");
    EXPECT_EQ(d.to_string("%Y/%b/%d"), "2024/May/26");
    EXPECT_EQ(d.to_string("%Y-%B-%d"), "2024-May-26");
    EXPECT_EQ(d.to_string("%Y-%B-%d %a"), "2024-May-26 Sun");
    EXPECT_EQ(d.to_string("%Y-%B-%d %A"), "2024-May-26 Sunday");
}

TEST_F(test_datetime_date, from_string)
{
    const auto d1 = tc::sdk::date(2024y, std::chrono::May, 26d);

    EXPECT_EQ(d1, tc::sdk::date::from_string("2024-05-26"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("2024-05-26", "%F"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("05/26/24", "%D"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("2024/05/26", "%Y/%m/%d"));

    EXPECT_EQ(d1, tc::sdk::date::from_string("05/26/24", "%m/%d/%y"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("26/05/2024", "%d/%m/%Y"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("2024/May/26", "%Y/%b/%d"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("2024-May-26", "%Y-%B-%d"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("2024-May-26 Sun", "%Y-%B-%d %a"));
    EXPECT_EQ(d1, tc::sdk::date::from_string("2024-May-26 Sunday", "%Y-%B-%d %A"));

    EXPECT_THROW(tc::sdk::date::from_string("BOOM!"), std::runtime_error);
    EXPECT_THROW(tc::sdk::date::from_string("2024/05/26"), std::runtime_error);
}

TEST_F(test_datetime_date, today)
{
    const auto today = tc::sdk::date::today();
    const auto now = std::chrono::system_clock::now();
    const auto now_minus_24_hours = now - 24h;

    // These two asserts only fail if "now" is between midnight and midnight + 1nanoseconds (oh, come on!)
    // So, it is quite safe to avoid a more relaxed comparison with greate/lower_OR_equal and keep only a strict lower/higher comparison.
    EXPECT_GT(now, today.to_duration());
    EXPECT_LT(now_minus_24_hours, today.to_duration());
}

}