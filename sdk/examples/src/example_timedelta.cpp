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
 * @example example_timedelta.cpp
 * @brief Simple example of tc::sdk::timedelta
 */

#include <teiacare/sdk/datetime/timedelta.hpp>

#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        auto t1 = tc::sdk::TimeDelta();
        std::cout << std::boolalpha << "\nTimeDelta(): " << t1 << " - is_null: " << t1.is_null() << std::endl;
    }

    {
        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456ms):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456ms).to_string<std::chrono::nanoseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456us):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456us).to_string<std::chrono::nanoseconds>() << std::endl;
        
        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456ns):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456ns).to_string<std::chrono::nanoseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456ms):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456ms).to_string<std::chrono::microseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456us):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456us).to_string<std::chrono::microseconds>() << std::endl;
        
        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456ns):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456ns).to_string<std::chrono::microseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456ms):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456ms).to_string<std::chrono::milliseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456us):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456us).to_string<std::chrono::milliseconds>() << std::endl;
        
        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 123456ns):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 123456ns).to_string<std::chrono::milliseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 1ns):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 1ns).to_string<std::chrono::nanoseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 1us):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 1us).to_string<std::chrono::microseconds>() << std::endl;
        
        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 1ms):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 1ms).to_string<std::chrono::milliseconds>() << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 1ms):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 1ms).to_string<std::chrono::seconds>() << std::endl;
    }

    {
        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 4ms):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 4ms) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 44ms):    " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 44ms) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 444ms):   " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 444ms) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 4'444ms): " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 4444ms) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 4us):     " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 4us) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 44us):    " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 44us) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 444us):   " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 444us) << std::endl;

        std::cout << "TimeDelta(1d, 1h, 2min, 3s, 4444us):  " 
            << tc::sdk::TimeDelta(std::chrono::days(1), 1h, 2min, 3s, 4444us) << std::endl;
    }

    {
        auto t = tc::sdk::TimeDelta(std::chrono::days(1), 25h, 61min, 61s, 1001ms, 1001us
#if defined(_MSC_VER)
        ); // Visual Studio seems to not support nanoseconds (!?)
#else
                                    ,
                                    1001ns);
#endif

        std::cout
            << "\nTimeDelta(std::chrono::days(1), 25h, 61min, 61s, 1001ms, 1001us, 1001ns): " << t.to_string<std::chrono::nanoseconds>() << std::endl

            << "\nhours(): " << t.hours() << std::endl
            << "minutes(): " << t.minutes() << std::endl
            << "seconds(): " << t.seconds() << std::endl
            << "milliseconds(): " << t.milliseconds() << std::endl
            << "microseconds(): " << t.microseconds() << std::endl
            << "nanoseconds(): " << t.nanoseconds() << std::endl

            << "\ntotal_hours(): " << t.total_hours() << std::endl
            << "total_minutes(): " << t.total_minutes() << std::endl
            << "total_seconds(): " << t.total_seconds() << std::endl
            << "total_milliseconds(): " << t.total_milliseconds() << std::endl
            << "total_microseconds(): " << t.total_microseconds() << std::endl
            << "total_nanoseconds(): " << t.total_nanoseconds() << std::endl;
    }

    {
        auto t1 = tc::sdk::TimeDelta(std::chrono::days(1), 25h, 61min, 61s, 1001ms, 1002us);
        auto t2 = tc::sdk::TimeDelta(std::chrono::days(2), 2h, 2min, 2s, 2ms, 2us);

        std::cout
            << std::boolalpha
            << "\nTimeDelta(std::chrono::days(1), 25h, 61min, 61s, 1001ms, 1002us)"
            << "\n is equal to: "
            << "\nTimeDelta(std::chrono::days(2),  2h,  2min,  2s,    2ms,    2us)\n"
            << (t1 == t2)
            << std::endl;
    }

    {
        auto t1 = tc::sdk::TimeDelta(5min, 5s);
        auto t2 = t1 * 2;

        std::cout
            << std::boolalpha
            << "\nTimeDelta(5min, 5s) * 2: " << t2 << std::endl
            << "TimeDelta(5min, 5s) * 3: " << t1 * 3 << std::endl
            << "TimeDelta(5min, 5s) + TimeDelta(10min, 5s): " << tc::sdk::TimeDelta(5min, 5s) + tc::sdk::TimeDelta(10min, 5s) << std::endl
            << "TimeDelta(5min, 5s) - TimeDelta(10min, 5s): " << tc::sdk::TimeDelta(5min, 5s) - tc::sdk::TimeDelta(10min, 5s) << std::endl;
    }

    return 0;
}