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
 * @example example_datetime_date.cpp
 * @brief Simple example of tc::sdk::date
 */

#include <teiacare/sdk/datetime/date.hpp>

#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        std::cout << std::boolalpha << "\ndate()               is_valid(): "
                  << tc::sdk::date().is_valid() << std::endl;

        std::cout << std::boolalpha << "date(2023y/8/3d)     is_valid(): "
                  << tc::sdk::date(2023y / 8 / 3d).is_valid() << std::endl; // 3 August 2023

        std::cout << std::boolalpha << "date(2023y/August/3) is_valid(): "
                  << tc::sdk::date(2023y / std::chrono::August / 3).is_valid() << std::endl; // 3 August 2023
    }

    {
        std::cout << "\ndate(27876240min):     "
                  << tc::sdk::date(27876240min).to_string() << std::endl;

        std::cout << "date(1672574401s):     "
                  << tc::sdk::date(1672574401s).to_string() << std::endl;

        std::cout << "date(1672574401000ms): "
                  << tc::sdk::date(1672574401000ms).to_string() << std::endl;
    }

    {
        const auto now = std::chrono::system_clock::now();

        std::cout << "\ndate(now):                    "
                  << tc::sdk::date(now) << std::endl;

        std::cout << "date(now.time_since_epoch()): "
                  << tc::sdk::date(now.time_since_epoch()) << std::endl;
    }

    {
        auto today = tc::sdk::date::today();
        std::cout
            << "\ntoday():       " << today << std::endl
            << "to_string(%D): " << today.to_string("%D") << std::endl
            << "year:  " << today.year() << std::endl
            << "month: " << today.month() << std::endl
            << "day:   " << today.day() << std::endl
            << "iso_weekday: " << today.iso_weekday() << std::endl;
    }

    {
        auto d1 = tc::sdk::date(2023y / 8 / 3d); // 3 August 2023
        auto d2 = tc::sdk::date(2023y / 8 / 1d); // 1 August 2023

        std::cout << "\ndate(2023y/8/3d): " << d1 << std::endl;
        std::cout << "date(2023y/8/1d): " << d2 << std::endl;

        std::cout << std::boolalpha << "\ndate(2023y/8/3d) == date(2022y/8/1d): " << (d1 == d2) << std::endl;
        std::cout << std::boolalpha << "date(2023y/8/3d) == date(2023y/std::chrono::August/3): " << (d1 == tc::sdk::date(2023y / std::chrono::August / 3)) << std::endl;

        std::cout << "\ndate(2023y/8/3d) - date(2023y/8/1d): " << d1 - d2 << std::endl;

        auto delta = tc::sdk::timedelta(std::chrono::days(2));
        std::cout << "date(2023y/8/3d) + timedelta(std::chrono::days(2)): " << d1 + delta << std::endl;
        std::cout << "date(2023y/8/3d) - timedelta(std::chrono::days(2)): " << d1 - delta << std::endl;
    }

    return 0;
}