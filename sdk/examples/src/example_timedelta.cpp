#include <sdk/datetime/timedelta.hpp>
#include <iostream>

using namespace std::chrono_literals;

int main()
{
    {
        auto t1 = tc::sdk::TimeDelta();
        std::cout << "\nTimeDelta(): " << t1 << std::endl;

        auto t2 = tc::sdk::TimeDelta(1h, 2min, 3s);
        std::cout << std::boolalpha << "TimeDelta(1h, 2min, 3s): " << t2 << std::endl;
    }

    {
        auto t = tc::sdk::TimeDelta(std::chrono::days(1), 25h, 61min, 61s, 1001ms, 1001us
    #if defined (_MSC_VER)
            ); // Visual Studio seems to not support nanoseconds (!?)
    #else
            , 1001ns);
    #endif

        std::cout
            << "\nTimeDelta(std::chrono::days(1), 25h, 61min, 61s, 1001ms, 1001us, 1001ns): " << t << std::endl
            
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
            << (t1==t2)
            << std::endl;
    }

    {
        auto t1 = tc::sdk::TimeDelta(5min, 5s);
        auto t2 = t1 * 2;

        std::cout
            << std::boolalpha
            << "\nTimeDelta(5min, 5s) * 2: " << t2 << std::endl
            << "3 * TimeDelta(5min, 5s): " << 3 * t1 << std::endl
            << "TimeDelta(5min, 5s) + TimeDelta(10min, 5s): " << tc::sdk::TimeDelta(5min, 5s) + tc::sdk::TimeDelta(10min, 5s) << std::endl
            << "TimeDelta(5min, 5s) - TimeDelta(10min, 5s): " << tc::sdk::TimeDelta(5min, 5s) - tc::sdk::TimeDelta(10min, 5s) << std::endl;
    }

    return 0;
}