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

/**
 * @example example_datetime_datetime.cpp
 * @brief Simple example of tc::sdk::datetime
 */

#include <teiacare/sdk/datetime/datetime.hpp>

#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        std::cout << tc::sdk::datetime::utc_now().to_string<std::chrono::seconds>() << std::endl;
        std::cout << tc::sdk::datetime::utc_now().to_string<std::chrono::milliseconds>() << std::endl;
        std::cout << tc::sdk::datetime::utc_now().to_string<std::chrono::microseconds>() << std::endl;
        std::cout << tc::sdk::datetime::utc_now().to_string<std::chrono::nanoseconds>() << std::endl;

        std::cout << tc::sdk::datetime::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::seconds>() << std::endl;
        std::cout << tc::sdk::datetime::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::milliseconds>() << std::endl;
        std::cout << tc::sdk::datetime::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::microseconds>() << std::endl;
        std::cout << tc::sdk::datetime::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::nanoseconds>() << std::endl;
    }

    {
        // "default" use millis precision (no template specialization defined, so std::chrono::milliseconds is used as a default template specialization)
        std::string str = "2024-04-19T18:09:12.123";
        tc::sdk::datetime dt = tc::sdk::datetime::from_string(str);
        std::cout << dt << std::endl;
        std::cout << dt.to_string() << std::endl;
    }

    {
        // again "default" use millis precision (no template specialization defined, so std::chrono::milliseconds is used as a default template specialization)
        // Note that here there will be trailing zeros in the milliseconds representation (i.e.: 2024-04-19T18:09:12.123000000) since milliseconds precision is used.
        std::string str = "2024-04-19T18:09:12.123456789";
        tc::sdk::datetime dt = tc::sdk::datetime::from_string(str);
        std::cout << dt << std::endl;
        std::cout << dt.to_string<std::chrono::nanoseconds>() << std::endl;
    }

    {
        std::string str = "2024-04-19T18:09:12.123456789";
        tc::sdk::datetime dt = tc::sdk::datetime::from_string<std::chrono::microseconds>(str);
        std::cout << dt << std::endl;
        std::cout << dt.to_string<std::chrono::nanoseconds>() << std::endl;
    }
    {
        // use nanoseconds precision (now template specialization is defined, so std::chrono::nanoseconds is used as the template specialization)
        // Note that here there will not be trailing zeros in the nanoseconds representation (i.e.: 2024-04-19T18:09:12.123456789) since nanoseconds precision is high enough to cover the given input value.
        std::string str = "2024-04-19T18:09:12.123456789";
        tc::sdk::datetime dt = tc::sdk::datetime::from_string<std::chrono::nanoseconds>(str);
        std::cout << dt << std::endl;
        std::cout << dt.to_string<std::chrono::nanoseconds>() << std::endl;
        std::cout << dt.to_string<std::chrono::nanoseconds>("%FT%H:%M:%S") << std::endl;
    }

    {
        std::string str = "2024-04-19T18:09:12.123";
        tc::sdk::datetime dt = tc::sdk::datetime::from_string(str);
        tc::sdk::date date = dt.date();
        std::cout << date << std::endl;
    }

    {
        std::string str = "2024-04-19T18:09:12.123";
        tc::sdk::datetime dt = tc::sdk::datetime::from_string<std::chrono::nanoseconds>(str);
        tc::sdk::time time = dt.time();
        std::cout << time << std::endl;
    }

    {
        tc::sdk::datetime dt = tc::sdk::datetime(2024y, std::chrono::April, 24d, 22h, 28min, 0s);
        std::cout << dt << std::endl;
    }

    {
        tc::sdk::date date = tc::sdk::date(2024y, std::chrono::April, 24d);
        tc::sdk::datetime dt = tc::sdk::datetime(date);
        std::cout << dt << std::endl;
    }

    {
        tc::sdk::time time = tc::sdk::time(22h, 28min, 0s);
        tc::sdk::datetime dt = tc::sdk::datetime(time);
        std::cout << dt << std::endl;
    }

    {
        tc::sdk::date date = tc::sdk::date(2024y, std::chrono::April, 24d);
        tc::sdk::time time = tc::sdk::time(22h, 28min, 0s);

        tc::sdk::datetime dt = tc::sdk::datetime(date, time);
        std::cout << dt << std::endl;
    }

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::cout << "now.time_since_epoch(): " << now.time_since_epoch() << std::endl;

    auto day = std::chrono::floor<std::chrono::days>(now);
    std::cout << "day.time_since_epoch(): " << day.time_since_epoch() << std::endl;

    std::chrono::year_month_day date = day;
    std::cout
        << "day: " << date.day() << std::endl
        << "month: " << date.month() << std::endl
        << "year: " << date.year() << std::endl;

    std::chrono::system_clock::duration dur = now - day;
    std::cout << "now - day: " << dur.count() << std::endl;

    std::chrono::hh_mm_ss time = std::chrono::hh_mm_ss(dur);
    std::cout
        << "hours: " << time.hours()
        << " - minutes: " << time.minutes()
        << " - seconds: " << time.seconds()
        << " - subseconds: " << time.subseconds()
        << std::endl;

    return 0;
}