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

#include "test_datetime_datetime.hpp"

namespace tc::sdk::tests
{
TEST_F(test_datetime_datetime, ctor_empty)
{
    const auto dt1 = tc::sdk::datetime();
    const auto dt2 = tc::sdk::datetime();

    EXPECT_EQ(dt1, dt2);
    EXPECT_FALSE(dt1.is_valid());
    EXPECT_FALSE(dt2.is_valid());
}

TEST_F(test_datetime_datetime, ctor_timepoint_ref)
{
    tc::sdk::sys_time_point tp1{1'714'588'424'000ms}; // 01 May 2024 18:33:44.000 UTC
    const auto dt1 = tc::sdk::datetime(tp1);

    tc::sdk::sys_time_point tp2{1'714'588'424'000'000us}; // 01 May 2024 18:33:44.000 UTC
    const auto dt2 = tc::sdk::datetime(tp2);

    tc::sdk::sys_time_point tp3{1'714'521'600s}; // 01 May 2024 00:00:00.000 UTC
    const auto dt3 = tc::sdk::datetime(tp3);

    tc::sdk::sys_time_point tp4{1'714'674'824s}; // 02 May 2024 18:33:44.000 UTC
    const auto dt4 = tc::sdk::datetime(tp4);

    EXPECT_EQ(dt1, dt2);
    EXPECT_NE(dt1, dt3);
    EXPECT_NE(dt1, dt4);
    EXPECT_NE(dt3, dt4);

    EXPECT_TRUE(dt1.is_valid());
    EXPECT_TRUE(dt2.is_valid());
    EXPECT_TRUE(dt3.is_valid());
    EXPECT_TRUE(dt4.is_valid());
}

TEST_F(test_datetime_datetime, ctor_timepoint_move)
{
    tc::sdk::sys_time_point tp1{1'714'588'424'000ms}; // 01 May 2024 18:33:44.000 UTC
    const auto dt1 = tc::sdk::datetime(std::move(tp1));

    tc::sdk::sys_time_point tp2{1'714'588'424'000'000us}; // 01 May 2024 18:33:44.000 UTC
    const auto dt2 = tc::sdk::datetime(std::move(tp2));

    tc::sdk::sys_time_point tp3{1'714'521'600s}; // 01 May 2024 00:00:00.000 UTC
    const auto dt3 = tc::sdk::datetime(std::move(tp3));

    tc::sdk::sys_time_point tp4{1'714'674'824s}; // 02 May 2024 18:33:44.000 UTC
    const auto dt4 = tc::sdk::datetime(std::move(tp4));

    EXPECT_EQ(dt1, dt2);
    EXPECT_NE(dt1, dt3);
    EXPECT_NE(dt1, dt4);
    EXPECT_NE(dt3, dt4);

    EXPECT_TRUE(dt1.is_valid());
    EXPECT_TRUE(dt2.is_valid());
    EXPECT_TRUE(dt3.is_valid());
    EXPECT_TRUE(dt4.is_valid());
}

TEST_F(test_datetime_datetime, ctor_date)
{
    const auto date = tc::sdk::date(2024y / std::chrono::May / 1d); // 01 May 2024
    const auto dt1 = tc::sdk::datetime(date);

    EXPECT_TRUE(dt1.is_valid());
    EXPECT_EQ(dt1.date(), date);
    EXPECT_EQ(dt1.to_time_point(), date.to_timepoint());
}

TEST_F(test_datetime_datetime, ctor_time)
{
    const auto time = tc::sdk::time(18h, 33min, 44s); // 18:33:44.000
    const auto dt1 = tc::sdk::datetime(time);

    EXPECT_TRUE(dt1.is_valid());
    EXPECT_EQ(dt1.time(), time);
    EXPECT_EQ(dt1.to_time_point(), tc::sdk::sys_time_point(time.to_duration()));
}

TEST_F(test_datetime_datetime, ctor_date_time)
{
    const auto date = tc::sdk::date(2024y / std::chrono::May / 1d); // 01 May 2024
    const auto time = tc::sdk::time(18h, 33min, 44s);               // 18:33:44
    const auto dt1 = tc::sdk::datetime(date, time);

    EXPECT_TRUE(dt1.is_valid());
    EXPECT_EQ(dt1.date(), date);
    EXPECT_EQ(dt1.time(), time);

    const auto dt2 = tc::sdk::datetime(tc::sdk::sys_time_point(1'714'588'424s)); // 01 May 2024 18:33:44 UTC
    EXPECT_EQ(dt1, dt2);
}

TEST_F(test_datetime_datetime, ctor_parameters)
{
    const auto dt1 = tc::sdk::datetime(2024y, std::chrono::May, 1d, 18h, 33min, 44s, 555ms); // 01 May 2024 18:33:44.555 UTC
    const auto dt2 = tc::sdk::datetime(tc::sdk::sys_time_point(1'714'588'424'555ms));        // 01 May 2024 18:33:44.555 UTC

    EXPECT_TRUE(dt1.is_valid());
    EXPECT_TRUE(dt2.is_valid());
    EXPECT_EQ(dt1.date(), dt2.date());
    EXPECT_EQ(dt1.time(), dt2.time());
    EXPECT_EQ(dt1, dt2);
}

TEST_F(test_datetime_datetime, not_valid)
{
    const auto dt1 = tc::sdk::datetime();
    EXPECT_FALSE(dt1.is_valid());

    const auto dt2 = tc::sdk::datetime(tc::sdk::sys_time_point::min());
    EXPECT_FALSE(dt2.is_valid());

    const auto dt3 = tc::sdk::datetime(tc::sdk::sys_time_point::max());
    EXPECT_FALSE(dt3.is_valid());
}

TEST_F(test_datetime_datetime, to_timepoint)
{
    tc::sdk::sys_time_point tp1{1'714'588'424'000ms}; // 01 May 2024 18:33:44.000 UTC
    const auto dt1 = tc::sdk::datetime(tp1);

    tc::sdk::sys_time_point tp2{1'714'588'424'000'000us}; // 01 May 2024 18:33:44.000 UTC
    const auto dt2 = tc::sdk::datetime(tp2);

    EXPECT_EQ(tp1, dt1.to_time_point());
    EXPECT_EQ(tp2, dt2.to_time_point());
    EXPECT_EQ(dt1.to_time_point(), dt2.to_time_point());
}

TEST_F(test_datetime_datetime, operator_equal)
{
    const auto date = tc::sdk::date(2024y / std::chrono::May / 1d); // 01 May 2024
    const auto time = tc::sdk::time(18h, 33min, 44s);               // 18:33:44
    const auto dt1 = tc::sdk::datetime(date, time);

    const auto dt2 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'588'424'000ms)}; // 01 May 2024 18:33:44.000 UTC
    const auto dt3 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'588'424'001ms)}; // 01 May 2024 18:33:44.001 UTC

    EXPECT_TRUE(dt1 == dt2);
    EXPECT_TRUE(dt1 != dt3);
    EXPECT_TRUE(dt2 != dt3);
}

TEST_F(test_datetime_datetime, operator_plus_timedelta)
{
    const auto delta = tc::sdk::timedelta(1h, 1s, 1ms);
    const auto dt1 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'588'424'000ms)}; // 01 May 2024 18:33:44.000 UTC
    const auto dt2 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'592'025'001ms)}; // 01 May 2024 19:33:45.001 UTC

    EXPECT_EQ(dt1 + delta, dt2);
    EXPECT_EQ(dt2 + delta, tc::sdk::datetime(2024y, std::chrono::May, 1d, 20h, 33min, 46s, 2ms));
}

TEST_F(test_datetime_datetime, operator_minus_timedelta)
{
    const auto delta = tc::sdk::timedelta(1h, 1s, 1ms);
    const auto dt1 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'588'424'000ms)}; // 01 May 2024 18:33:44.000 UTC
    const auto dt2 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'592'025'001ms)}; // 01 May 2024 19:33:45.001 UTC

    EXPECT_EQ(dt2 - delta, dt1);
    EXPECT_EQ(dt2, tc::sdk::datetime(2024y, std::chrono::May, 1d, 20h, 33min, 46s, 2ms) - delta);
}
TEST_F(test_datetime_datetime, operator_minus)
{
    const auto dt1 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'588'424'000ms)}; // 01 May 2024 18:33:44.000 UTC
    const auto dt2 = tc::sdk::datetime{tc::sdk::sys_time_point(1'714'592'025'001ms)}; // 01 May 2024 19:33:45.001 UTC

    EXPECT_EQ(dt2 - dt1, tc::sdk::timedelta(1h, 1s, 1ms));
    EXPECT_EQ(dt1 - dt2, tc::sdk::timedelta(-1h, -1s, -1ms));
}

TEST_F(test_datetime_datetime, operator_comparison)
{
    const auto dt1 = tc::sdk::datetime(2024y, std::chrono::May, 1d, 18h, 33min, 44s, 555ms, 666us); // 01 May 2024 18:33:44.555666 UTC
    const auto dt2 = tc::sdk::datetime(2024y, std::chrono::May, 1d, 18h, 33min, 44s, 555ms);        // 01 May 2024 18:33:44.555000 UTC
    const auto dt3 = tc::sdk::datetime(2024y, std::chrono::May, 1d, 18h, 33min, 44s);               // 01 May 2024 18:33:44.000000 UTC

    EXPECT_TRUE(dt1 > dt2);
    EXPECT_TRUE(dt2 > dt3);

    EXPECT_TRUE(dt3 < dt1);
    EXPECT_TRUE(dt3 < dt2);
}

TEST_F(test_datetime_datetime, ostream)
{
    {
        const auto dt = tc::sdk::datetime(2024y, std::chrono::May, 1d, 18h, 33min, 44s);
        std::stringstream stream;
        stream << dt;
        EXPECT_STREQ(stream.str().c_str(), "2024-05-01T18:33:44.000");
    }

    {
        std::stringstream stream;
        stream << tc::sdk::datetime(2024y, std::chrono::May, 1d, 18h, 33min, 44s);
        EXPECT_STREQ(stream.str().c_str(), "2024-05-01T18:33:44.000");
    }
}

TEST_F(test_datetime_datetime, to_string)
{
    EXPECT_EQ(tc::sdk::datetime(2024y, std::chrono::May, 31d, 18h, 33min, 44s, 555ms, 666us, 777ns).to_string(), "2024-05-31T18:33:44.555"); // 01 May 2024 18:33:44.555666 UTC
    EXPECT_EQ(tc::sdk::datetime(2024y, std::chrono::May, 31d, 18h, 33min, 44s, 555ms, 666us).to_string(), "2024-05-31T18:33:44.555");        // 01 May 2024 18:33:44.555666 UTC
    EXPECT_EQ(tc::sdk::datetime(2024y, std::chrono::May, 31d, 18h, 33min, 44s, 555ms).to_string(), "2024-05-31T18:33:44.555");               // 01 May 2024 18:33:44.555000 UTC
    EXPECT_EQ(tc::sdk::datetime(2024y, std::chrono::May, 31d, 18h, 33min, 44s).to_string(), "2024-05-31T18:33:44.000");                      // 01 May 2024 18:33:44.000000 UTC

    EXPECT_EQ(tc::sdk::datetime(tc::sdk::date(2024y, std::chrono::May, 31d), tc::sdk::time(18h, 33min, 44s)).to_string(), "2024-05-31T18:33:44.000"); // 01 May 2024 18:33:44.000000 UTC

    EXPECT_EQ(tc::sdk::datetime(tc::sdk::date(2024y, std::chrono::December, 31d)).to_string(), "2024-12-31T00:00:00.000");
    EXPECT_EQ(tc::sdk::datetime(tc::sdk::time(1h, 2min, 3s)).to_string(), "1970-01-01T01:02:03.000");
}

TEST_F(test_datetime_datetime, to_string_resolution)
{
    {
        const auto dt = tc::sdk::datetime(2024y, std::chrono::May, 31d, 18h, 33min, 1s, 3000ms, 4000us, 5006ns);
        EXPECT_EQ(dt.to_string<std::chrono::seconds>(), "2024-05-31T18:33:04");
        EXPECT_EQ(dt.to_string<std::chrono::milliseconds>(), "2024-05-31T18:33:04.004");
        EXPECT_EQ(dt.to_string<std::chrono::microseconds>(), "2024-05-31T18:33:04.004005");
        EXPECT_EQ(dt.to_string<std::chrono::nanoseconds>(), "2024-05-31T18:33:04.004005006");
    }
    {
        const auto dt = tc::sdk::datetime(2024y, std::chrono::May, 31d, 18h, 33min, 44s, 555ms, 666us, 777ns);
        EXPECT_EQ(dt.to_string<std::chrono::seconds>(), "2024-05-31T18:33:44");
        EXPECT_EQ(dt.to_string<std::chrono::milliseconds>(), "2024-05-31T18:33:44.555");
        EXPECT_EQ(dt.to_string<std::chrono::microseconds>(), "2024-05-31T18:33:44.555666");
        EXPECT_EQ(dt.to_string<std::chrono::nanoseconds>(), "2024-05-31T18:33:44.555666777");
    }
}

TEST_F(test_datetime_datetime, to_string_format)
{
    const auto dt = tc::sdk::datetime(2024y, std::chrono::May, 31d, 1h, 2min, 3s, 4ms, 5us, 6ns);

    // date
    EXPECT_EQ(dt.to_string("%F"), "2024-05-31");
    EXPECT_EQ(dt.to_string("%D"), "05/31/24");
    EXPECT_EQ(dt.to_string("%m/%d/%y"), "05/31/24");
    EXPECT_EQ(dt.to_string("%d/%m/%Y"), "31/05/2024");
    EXPECT_EQ(dt.to_string("%Y/%b/%d"), "2024/May/31");
    EXPECT_EQ(dt.to_string("%Y-%B-%d"), "2024-May-31");
    EXPECT_EQ(dt.to_string("%Y-%B-%d %a"), "2024-May-31 Fri");
    EXPECT_EQ(dt.to_string("%Y-%B-%d %A"), "2024-May-31 Friday");

    // time
    EXPECT_EQ(dt.to_string("%T"), "01:02:03.004");
    EXPECT_EQ(dt.to_string("%R"), "01:02");
    EXPECT_EQ(dt.to_string("%H"), "01");
    EXPECT_EQ(dt.to_string("%M"), "02");
    EXPECT_EQ(dt.to_string("%S"), "03.004");
}

TEST_F(test_datetime_datetime, from_string)
{
    /*
    EXPECT_EQ(tc::sdk::datetime::from_string<std::chrono::seconds>("01:02:03"), tc::sdk::datetime(1h, 2min, 3s));
    EXPECT_EQ(tc::sdk::datetime::from_string<std::chrono::milliseconds>("01:02:03.004"), tc::sdk::datetime(1h, 2min, 3s, 4ms));
    EXPECT_EQ(tc::sdk::datetime::from_string<std::chrono::microseconds>("01:02:03.004005"), tc::sdk::datetime(1h, 2min, 3s, 4ms, 5us));
    EXPECT_EQ(tc::sdk::datetime::from_string<std::chrono::nanoseconds>("01:02:03.004005006"), tc::sdk::datetime(1h, 2min, 3s, 4ms, 5us, 6ns));

    const auto t_hours = tc::sdk::datetime(1h);
    EXPECT_EQ(t_hours, tc::sdk::datetime::from_string<std::chrono::hours>("01:02:03.004"));

    const auto t_minutes = tc::sdk::datetime(1h, 2min);
    EXPECT_EQ(t_minutes, tc::sdk::datetime::from_string<std::chrono::minutes>("01:02:03.004"));

    const auto t_seconds = tc::sdk::datetime(1h, 2min, 3s);
    EXPECT_EQ(t_seconds, tc::sdk::datetime::from_string<std::chrono::seconds>("01:02:03.004"));
    EXPECT_EQ(t_seconds, tc::sdk::datetime::from_string<std::chrono::seconds>("01:02:03.004005"));
    EXPECT_EQ(t_seconds, tc::sdk::datetime::from_string<std::chrono::seconds>("01:02:03.004005006"));

    const auto t_milliseconds = tc::sdk::datetime(1h, 2min, 3s, 4ms);
    EXPECT_GT(t_milliseconds, tc::sdk::datetime::from_string<std::chrono::milliseconds>("01:02:03"));
    EXPECT_EQ(t_milliseconds, tc::sdk::datetime::from_string<std::chrono::milliseconds>("01:02:03.004"));
    EXPECT_EQ(t_milliseconds, tc::sdk::datetime::from_string<std::chrono::milliseconds>("01:02:03.004005"));
    EXPECT_EQ(t_milliseconds, tc::sdk::datetime::from_string<std::chrono::milliseconds>("01:02:03.004005006"));
    EXPECT_LT(t_milliseconds, tc::sdk::datetime::from_string<std::chrono::milliseconds>("01:02:03.005"));

    const auto t_microseconds = tc::sdk::datetime(1h, 2min, 3s, 4ms, 5us);
    EXPECT_GT(t_microseconds, tc::sdk::datetime::from_string<std::chrono::microseconds>("01:02:03"));
    EXPECT_GT(t_microseconds, tc::sdk::datetime::from_string<std::chrono::microseconds>("01:02:03.004"));
    EXPECT_EQ(t_microseconds, tc::sdk::datetime::from_string<std::chrono::microseconds>("01:02:03.004005"));
    EXPECT_EQ(t_microseconds, tc::sdk::datetime::from_string<std::chrono::microseconds>("01:02:03.004005006"));
    EXPECT_LT(t_microseconds, tc::sdk::datetime::from_string<std::chrono::microseconds>("01:02:03.004006"));

    const auto t_nanoseconds = tc::sdk::datetime(1h, 2min, 3s, 4ms, 5us, 6ns);
    EXPECT_GT(t_nanoseconds, tc::sdk::datetime::from_string<std::chrono::nanoseconds>("01:02:03"));
    EXPECT_GT(t_nanoseconds, tc::sdk::datetime::from_string<std::chrono::nanoseconds>("01:02:03.004"));
    EXPECT_GT(t_nanoseconds, tc::sdk::datetime::from_string<std::chrono::nanoseconds>("01:02:03.004005"));
    EXPECT_EQ(t_nanoseconds, tc::sdk::datetime::from_string<std::chrono::nanoseconds>("01:02:03.004005006"));
    EXPECT_LT(t_nanoseconds, tc::sdk::datetime::from_string<std::chrono::nanoseconds>("01:02:03.004005007"));

    EXPECT_THROW(tc::sdk::datetime::from_string("BOOM!"), std::runtime_error);
    EXPECT_THROW(tc::sdk::datetime::from_string("01-02-03-04-05"), std::runtime_error);
*/

    // date
    const auto dt_date_only = tc::sdk::datetime(tc::sdk::date(2024y, std::chrono::June, 1d));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-06-01T00:00:00.000000000"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-06-01T00:00:00.000000"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-06-01T00:00:00.000"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-06-01T00:00:00"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-06-01", "%F"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("06/01/24", "%D"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024/06/01", "%Y/%m/%d"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("06/01/24", "%m/%d/%y"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("01/06/2024", "%d/%m/%Y"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024/June/01", "%Y/%b/%d"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-June-01", "%Y-%B-%d"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-June-01 Sat", "%Y-%B-%d %a"));
    EXPECT_EQ(dt_date_only, tc::sdk::datetime::from_string("2024-June-01 Saturday", "%Y-%B-%d %A"));

    // time
    EXPECT_EQ(tc::sdk::datetime(tc::sdk::time(1h, 2min, 3s)), tc::sdk::datetime::from_string<std::chrono::seconds>("1970-01-01T01:02:03"));
    EXPECT_EQ(tc::sdk::datetime(tc::sdk::time(1h, 2min, 3s, 4ms)), tc::sdk::datetime::from_string<std::chrono::milliseconds>("1970-01-01T01:02:03.004"));
    EXPECT_EQ(tc::sdk::datetime(tc::sdk::time(1h, 2min, 3s, 4ms, 5us)), tc::sdk::datetime::from_string<std::chrono::microseconds>("1970-01-01T01:02:03.004005"));
    EXPECT_EQ(tc::sdk::datetime(tc::sdk::time(1h, 2min, 3s, 4ms, 5us, 6ns)), tc::sdk::datetime::from_string<std::chrono::nanoseconds>("1970-01-01T01:02:03.004005006"));

    // Exceptions
    EXPECT_THROW(tc::sdk::date::from_string("BOOM!"), std::runtime_error);
    EXPECT_THROW(tc::sdk::date::from_string("2024/12/31T00:00:00.000"), std::runtime_error); // The default format is with the "-" separator instead of "/"
    EXPECT_THROW(tc::sdk::date::from_string("2024-12-32", "%F"), std::runtime_error);        // invalida date (32 December)
    EXPECT_THROW(tc::sdk::date::from_string("25:00:00", "%T"), std::runtime_error);          // invalida time (25:00:00)
    EXPECT_THROW(tc::sdk::datetime::from_string("2024-12-32T00:00:00"), std::runtime_error); // invalida date (32 December)
    EXPECT_THROW(tc::sdk::datetime::from_string("2024-12-31T25:00:00"), std::runtime_error); // invalid time (25:00:00)
}

TEST_F(test_datetime_datetime, utc_now)
{
    // nanoseconds
    {
        const auto now = std::chrono::floor<std::chrono::microseconds>(std::chrono::system_clock::now());
        const auto now_minus_1 = now - std::chrono::nanoseconds(1);
        const auto utc_now = tc::sdk::datetime::utc_now<std::chrono::nanoseconds>();

        EXPECT_LT(now, utc_now.to_time_point());
        EXPECT_LT(now_minus_1, utc_now.to_time_point());
    }

    // microseconds
    {
        const auto now = std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
        const auto now_minus_1 = now - std::chrono::microseconds(1);
        const auto utc_now = tc::sdk::datetime::utc_now<std::chrono::microseconds>();

        EXPECT_LT(now, utc_now.to_time_point());
        EXPECT_LT(now_minus_1, utc_now.to_time_point());
    }

    // milliseconds
    {
        const auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
        const auto now_minus_1 = now - std::chrono::milliseconds(1);
        const auto utc_now = tc::sdk::datetime::utc_now<std::chrono::milliseconds>();

        EXPECT_LT(now, utc_now.to_time_point());
        EXPECT_LT(now_minus_1, utc_now.to_time_point());
    }

    // seconds
    {
        const auto now = std::chrono::floor<std::chrono::minutes>(std::chrono::system_clock::now());
        const auto now_minus_1 = now - std::chrono::seconds(1);
        const auto utc_now = tc::sdk::datetime::utc_now<std::chrono::seconds>();

        EXPECT_LT(now, utc_now.to_time_point());
        EXPECT_LT(now_minus_1, utc_now.to_time_point());
    }
}

TEST_F(test_datetime_datetime, copy_assignment)
{
    tc::sdk::datetime dt1;
    tc::sdk::datetime dt2 = tc::sdk::datetime::from_string("2024-11-22T10:01:12.789");
    dt1 = dt2;

    EXPECT_EQ(dt1, dt2);
    EXPECT_EQ(dt1, tc::sdk::datetime::from_string("2024-11-22T10:01:12.789"));
}

TEST_F(test_datetime_datetime, move_assignment)
{
    tc::sdk::datetime dt;
    dt = tc::sdk::datetime::from_string("2024-11-22T10:01:12.789");

    EXPECT_EQ(dt, tc::sdk::datetime::from_string("2024-11-22T10:01:12.789"));
}

}
