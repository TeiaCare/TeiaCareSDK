#include <sdk/datetime/date.hpp>
#include <iostream>

using namespace std::chrono_literals;

int main() 
{
    auto now = std::chrono::system_clock::now();

    {
        auto d1 = tc::sdk::Date();
        std::cout << std::boolalpha << "\nDate() is_valid(): " << d1.is_valid() << std::endl;
        
        auto d2 = tc::sdk::Date(2023y/8/3d); // 3 August 2023
        std::cout << std::boolalpha << "Date(2023y/8/3d) is_valid(): " << d2.is_valid() << std::endl;
        
        auto d3 = tc::sdk::Date(2023y/std::chrono::August/3); // 3 August 2023
        std::cout << std::boolalpha << "Date(2023y/std::chrono::August/3) is_valid(): " << d3.is_valid() << std::endl;
    }

    {
        auto d1 = tc::sdk::Date::from_duration(27876240min);
        std::cout << "\nDate::from_duration(27876240min): " << d1 << std::endl;

        auto d2 = tc::sdk::Date::from_duration(1672574401s);
        std::cout << "Date::from_duration(1672574401s): " << d2 << std::endl;

        auto d3 = tc::sdk::Date::from_duration(1672574401000ms);
        std::cout << "Date::from_duration(1672574401000ms): " << d3 << std::endl;

        auto d4 = tc::sdk::Date::from_duration(now.time_since_epoch());
        std::cout << "Date::from_duration(now.time_since_epoch()): = " << d4 << std::endl;
    }

    {
        auto d = tc::sdk::Date::from_timepoint(now);
        std::cout << "\nDate::from_timepoint(now): " << d << std::endl;
    }

    {
        auto d = tc::sdk::Date::today();
        std::cout 
            << "\ntoday(): " << d << std::endl
            << "operator<<: " << d << std::endl
            << "to_string(): " << d.to_string() << std::endl
            << "to_string(%D): " << d.to_string("%D") << std::endl
            << "iso_string: " << d.iso_string() << std::endl
            << "year: " << d.year() << std::endl
            << "month: " << d.month() << std::endl
            << "day: " << d.day() << std::endl
            << "iso_weekday: " << d.iso_weekday() << std::endl;
    }

    {
        auto d1 = tc::sdk::Date(2023y/8/3d); // 3 August 2023
        auto d2 = tc::sdk::Date(2023y/8/1d); // 1 August 2023

        std::cout << "\nDate(2023y/8/3d): "<< d1 << std::endl;
        std::cout << "Date(2023y/8/1d): "<< d2 << std::endl;

        std::cout << std::boolalpha << "\nDate(2023y/8/3d) == Date(2022y/8/1d): " << (d1 == d2) << std::endl;
        std::cout << std::boolalpha << "Date(2023y/8/3d) == Date(2023y/std::chrono::August/3): " << (d1 == tc::sdk::Date(2023y/std::chrono::August/3)) << std::endl;

        std::cout << "\nDate(2023y/8/3d) - Date(2023y/8/1d): " << d1 - d2 << std::endl;

        auto delta = tc::sdk::TimeDelta(std::chrono::days(2));
        std::cout << "\nTimeDelta(std::chrono::days(2)) + Date(2023y/8/3d): " << delta + d1 << std::endl;
        std::cout << "Date(2023y/8/3d) + TimeDelta(std::chrono::days(2)): " << d1 + delta << std::endl;
        std::cout << "Date(2023y/8/3d) - TimeDelta(std::chrono::days(2)): " << d1 - delta << std::endl;
    }

    return 0;
}