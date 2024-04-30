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
 * @example example_time.cpp
 * @brief Simple example of tc::sdk::time
 */

#include <teiacare/sdk/datetime/time.hpp>

#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        auto t1 = tc::sdk::time();
        std::cout << std::boolalpha << "\nTime() is_valid(): " << t1.is_valid() << std::endl;

        auto t2 = tc::sdk::time(42min);
        std::cout << std::boolalpha << "time(42min) is_valid(): " << t2.is_valid() << std::endl;
    }

    {
        std::cout << "\nTime::utc_now():                            "
            << tc::sdk::time::utc_now().to_string() << std::endl;

        std::cout << "time::utc_now<std::chrono::seconds>():      "
            << tc::sdk::time::utc_now<std::chrono::seconds>().to_string<std::chrono::milliseconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::seconds>():      "
            << tc::sdk::time::utc_now<std::chrono::seconds>().to_string() << std::endl;

        std::cout << "time::utc_now<std::chrono::seconds>():      "
            << tc::sdk::time::utc_now<std::chrono::seconds>().to_string<std::chrono::microseconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::seconds>():      "
            << tc::sdk::time::utc_now<std::chrono::seconds>().to_string<std::chrono::nanoseconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::milliseconds>(): "
            << tc::sdk::time::utc_now<std::chrono::milliseconds>().to_string<std::chrono::milliseconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::microseconds>(): "
            << tc::sdk::time::utc_now<std::chrono::microseconds>().to_string<std::chrono::microseconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::nanoseconds>():  "
            << tc::sdk::time::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::seconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::nanoseconds>():  "
            << tc::sdk::time::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::milliseconds>() << std::endl;

        std::cout << "time::utc_now<std::chrono::nanoseconds>():  "
            << tc::sdk::time::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::microseconds>() << std::endl;
                
        std::cout << "time::utc_now<std::chrono::nanoseconds>():  "
            << tc::sdk::time::utc_now<std::chrono::nanoseconds>().to_string<std::chrono::nanoseconds>() << std::endl;
    }

    {
        auto t = tc::sdk::time(std::chrono::hours(1), date::days(1), std::chrono::minutes(121), std::chrono::milliseconds(1999));
        std::cout
            << "\noperator << time(std::chrono::hours(1), date::days(1), std::chrono::minutes(121), std::chrono::milliseconds(1999): " << t << std::endl
            << "to_string: " << t.to_string() << std::endl
            << "to_string(%T): " << t.to_string("%T") << std::endl
            << "to_string(%H:%M:%S): " << t.to_string("%H:%M:%S") << std::endl
            << "to_string<std::chrono::seconds>(%T): " << t.to_string<std::chrono::seconds>("%T") << std::endl
            << "to_string(%Hh:%Mm:%Ss): " << t.to_string("%Hh:%Mm:%Ss") << std::endl
            << "to_string<std::chrono::hours>(%Hh:%Mm:%Ss): " << t.to_string<std::chrono::hours>("%Hh:%Mm:%Ss") << std::endl;

        std::cout
            << "\nhours: " << t.hours() << std::endl
            << "seconds: " << t.seconds() << std::endl
            << "minutes: " << t.minutes() << std::endl
            << "milliseconds: " << t.milliseconds() << std::endl
            << "microseconds: " << t.microseconds() << std::endl
            << "nanoseconds: " << t.nanoseconds() << std::endl;
    }

    {
        auto t1 = tc::sdk::time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(59));
        auto t2 = tc::sdk::time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(60));
        std::cout
            << "\nTime(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(59)): " << t1 << std::endl
            << "time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(60)): " << t2 << std::endl;
    }

    {
        auto t1 = tc::sdk::time(1h);
        auto t2 = tc::sdk::time(60min);
        auto t3 = tc::sdk::time(58min, 181s);

        std::cout << "\nTime(1h): " << t1 << std::endl;
        std::cout << "time(60min): " << t2 << std::endl;
        std::cout << "time(58min, 181s): " << t3 << std::endl;

        std::cout << std::boolalpha << "\nTime(1h)==time(60min): " << (t1 == t2) << std::endl;
        std::cout << std::boolalpha << "time(1h)==time(58min, 181s): " << (t1 == t3) << std::endl;

        std::cout << std::boolalpha << "\nTime(1h)>time(60min): " << (t1 > t2) << std::endl;
        std::cout << std::boolalpha << "time(1h)<time(58min, 181s): " << (t1 < t3) << std::endl;

        std::cout << "\nTime(58min, 181s) - time(1h): " << t3 - t1 << std::endl;

        auto delta = tc::sdk::timedelta(2min);
        std::cout << "time(60min) + timedelta(2min): " << t2 + delta << std::endl;
        std::cout << "time(60min) - timedelta(2min): " << t2 - delta << std::endl;
    }

    return 0;
}