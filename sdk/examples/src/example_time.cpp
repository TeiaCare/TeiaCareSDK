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

#include <teiacare/sdk/datetime/time.hpp>

#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        auto t1 = tc::sdk::Time();
        std::cout << std::boolalpha << "\nTime() is_valid(): " << t1.is_valid() << std::endl;

        auto t2 = tc::sdk::Time(42min);
        std::cout << std::boolalpha << "Time(42min) is_valid(): " << t2.is_valid() << std::endl;
    }

    {
        auto t1 = tc::sdk::Time::now(); // same as Time::now<std::chrono::nanoseconds>();
        std::cout << "\nTime::now(): " << t1.to_string() << std::endl;

        auto t2 = tc::sdk::Time::now<std::chrono::seconds>(); // DurationT is seconds
        std::cout << "Time::now<std::chrono::seconds>(): " << t2.to_string() << std::endl;

        auto t3 = tc::sdk::Time::now();
        std::cout << "Time::now(): " << t3.iso_string() << std::endl;
    }

    {
        auto t = tc::sdk::Time(std::chrono::hours(1), date::days(1), std::chrono::minutes(121), std::chrono::milliseconds(1999));
        std::cout
            << "\noperator<<: " << t << std::endl
            << "iso_string: " << t.iso_string() << std::endl
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
        auto t1 = tc::sdk::Time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(59));
        auto t2 = tc::sdk::Time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(60));
        std::cout
            << "\nTime(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(59)): " << t1 << std::endl
            << "Time(std::chrono::hours(23), std::chrono::minutes(59), std::chrono::seconds(60)): " << t2 << std::endl;
    }

    {
        auto t1 = tc::sdk::Time(1h);
        auto t2 = tc::sdk::Time(60min);
        auto t3 = tc::sdk::Time(58min, 181s);

        std::cout << "\nTime(1h): " << t1 << std::endl;
        std::cout << "Time(60min): " << t2 << std::endl;
        std::cout << "Time(58min, 181s): " << t3 << std::endl;

        std::cout << std::boolalpha << "\nTime(1h)==Time(60min): " << (t1 == t2) << std::endl;
        std::cout << std::boolalpha << "Time(1h)==Time(58min, 181s): " << (t1 == t3) << std::endl;

        std::cout << "\nTime(58min, 181s) - Time(1h): " << t3 - t1 << std::endl;

        auto delta = tc::sdk::TimeDelta(2min);
        std::cout << "\nTimeDelta(2min) + Time(60min): " << delta + t2 << std::endl;
        std::cout << "Time(60min) + TimeDelta(2min): " << t2 + delta << std::endl;
        std::cout << "Time(60min) - TimeDelta(2min): " << t2 - delta << std::endl;
    }

    return 0;
}