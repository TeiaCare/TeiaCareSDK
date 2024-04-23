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
 * @example example_date.cpp
 * @brief Simple example of tc::sdk::date and tc::sdk::datetime
 */

#include <teiacare/sdk/datetime/datetime.hpp>

#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        std::chrono::sys_days t;
        std::string str = "2024-04-19";
        std::stringstream ss{str};
        ss >> date::parse("%F", t);
        std::cout << date::format("%F", t) << std::endl;

        auto tt = tc::sdk::Date(t);
        std::cout << tt << std::endl;
    }

    {
        using T = std::chrono::milliseconds;
        T t;
        std::string str = "18:09:12.123456789";
        std::stringstream ss{str};
        ss >> date::parse("%T", t);
        std::cout << date::format("%T", t) << std::endl;

        auto tt = tc::sdk::Time(t);
        std::cout << tt << std::endl;
    }

    {
        std::chrono::sys_time<std::chrono::milliseconds> t;
        std::string str = "2024-04-19T18:09:12.123456789";
        std::stringstream ss{str};
        ss >> date::parse("%FT%T", t);
        std::cout << date::format("%FT%T", t) << std::endl;
    }

    {
        std::chrono::sys_time<std::chrono::microseconds> t;
        std::string str = "2024-04-19T18:09:12.123456789";
        std::stringstream ss{str};
        ss >> date::parse("%FT%T", t);
        std::cout << date::format("%FT%T", t) << std::endl;
    }

    {
        std::chrono::sys_time<std::chrono::nanoseconds> t;
        std::string str = "2024-04-19T18:09:12.123456789";
        std::stringstream ss{str};
        ss >> date::parse("%FT%T", t);
        if (ss.fail())
            throw std::runtime_error("Failed to parse " + str);

        std::cout << date::format("%FT%T", t) << std::endl;
        auto dt = tc::sdk::DateTime(t);
        std::cout << dt << std::endl;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
        // "default" use millis precision (no template specialization defined, so std::chrono::milliseconds is used as a default template specialization)
        std::string str = "2024-04-19T18:09:12.123";
        auto dt = tc::sdk::DateTime::from_string(str);
        std::cout << dt << std::endl;
    }

    {
        // again "default" use millis precision (no template specialization defined, so std::chrono::milliseconds is used as a default template specialization)
        // Note that here there will be trailing zeros in the milliseconds representation (i.e.: 2024-04-19T18:09:12.123000000) since milliseconds precision is used.
        std::string str = "2024-04-19T18:09:12.123456789";
        auto dt = tc::sdk::DateTime::from_string(str);
        std::cout << dt << std::endl;
    }

    {
        // use nanoseconds precision (now template specialization is defined, so std::chrono::nanoseconds is used as the template specialization)
        // Note that here there will not be trailing zeros in the nanoseconds representation (i.e.: 2024-04-19T18:09:12.123456789) since nanoseconds precision is high enough to cover the given input value.
        std::string str = "2024-04-19T18:09:12.123456789";
        auto dt = tc::sdk::DateTime::from_string<std::chrono::nanoseconds>(str);
        std::cout << dt << std::endl;
    }

    {
        std::string str = "2024-04-19T18:09:12.123456789";
        auto dt = tc::sdk::DateTime::from_string<std::chrono::microseconds>(str);
        std::cout << dt << std::endl;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
        std::string str = "2024-04-19T18:09:12.123";
        auto dt = tc::sdk::DateTime::from_string(str);
        std::cout << dt.date() << std::endl;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
        std::string str = "2024-04-19T18:09:12.123";
        auto dt = tc::sdk::DateTime::from_string(str);
        std::cout << dt.time() << std::endl;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::cout << "now.time_since_epoch(): " << now.time_since_epoch() << std::endl;

    auto day = std::chrono::floor<std::chrono::days>(now);
    std::cout << "day.time_since_epoch(): " << day.time_since_epoch() << std::endl;

    std::chrono::year_month_day date = day;
    std::cout
        << "date: " << date.day()
        << " - month: " << date.month()
        << " - year: " << date.year()
        << std::endl;

    // std::chrono::hh_mm_ss<std::chrono::milliseconds>

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