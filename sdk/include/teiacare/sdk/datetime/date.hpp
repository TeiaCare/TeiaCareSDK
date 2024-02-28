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

#pragma once

#include <teiacare/sdk/datetime/timedelta.hpp>

#include "date/date.h"
#include <ostream>

namespace tc::sdk
{
class Date
{
    const std::chrono::year_month_day _ymd;

public:
    explicit Date();
    explicit Date(const std::chrono::year_month_day& ymd);
    explicit Date(std::chrono::year_month_day&& ymd);
    explicit Date(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d);
    explicit Date(std::chrono::year&& y, std::chrono::month&& m, std::chrono::day&& d);

    constexpr bool is_valid() const;

    std::chrono::year year() const;
    std::chrono::month month() const;
    std::chrono::day day() const;
    std::chrono::weekday weekday() const;
    unsigned iso_weekday() const;

    std::string to_string(const char* fmt = "%F") const; // equivalent to "%Y-%m-%d"
    std::string iso_string() const;

    static Date today();
    static Date from_duration(const std::chrono::system_clock::duration& duration);
    static Date from_timepoint(const std::chrono::system_clock::time_point& timepoint);

    // TODO!
    static Date from_string(const char* str);
    static Date from_string(const std::string& str);

protected:
    const std::chrono::year_month_day& year_month_day() const;

public:
    friend bool operator==(const Date& d1, const Date& d2);
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend Date operator+(const Date& date, const TimeDelta& time_delta);
    friend Date operator+(const TimeDelta& time_delta, const Date& date);
    friend Date operator-(const Date& date, const TimeDelta& time_delta);
    friend TimeDelta operator-(const Date& date1, const Date& date2);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Date impl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Date::Date()
    : _ymd{}
{
}

Date::Date(const std::chrono::year_month_day& ymd)
    : _ymd{ymd}
{
}

Date::Date(std::chrono::year_month_day&& ymd)
    : _ymd{std::move(ymd)}
{
}

Date::Date(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d)
    : _ymd{y, m, d}
{
}

Date::Date(std::chrono::year&& y, std::chrono::month&& m, std::chrono::day&& d)
    : _ymd{std::move(y), std::move(m), std::move(d)}
{
}

constexpr bool Date::is_valid() const
{
    return _ymd.ok();
}

Date Date::today()
{
    return Date{std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
}

const std::chrono::year_month_day& Date::year_month_day() const
{
    return _ymd;
}

std::chrono::year Date::year() const
{
    return _ymd.year();
}

std::chrono::month Date::month() const
{
    return _ymd.month();
}

std::chrono::day Date::day() const
{
    return _ymd.day();
}

std::chrono::weekday Date::weekday() const
{
    return std::chrono::weekday(_ymd);
}

unsigned Date::iso_weekday() const
{
    return weekday().iso_encoding();
}

std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& ymd)
{
    os.fill('0');
    os.flags(std::ios::dec | std::ios::right);
    os.imbue(std::locale::classic());
    os << static_cast<int>(ymd.year()) << '-';
    os.width(2);
    os << static_cast<unsigned>(ymd.month()) << '-';
    os.width(2);
    os << static_cast<unsigned>(ymd.day());

    if (!ymd.ok())
        os << " is not a valid year_month_day";

    return os;
}

std::string Date::to_string(const char* fmt) const
{
    return date::format(fmt, std::chrono::sys_days(_ymd));
}

std::string Date::iso_string() const
{
    return to_string("%F");
}

Date Date::from_duration(const std::chrono::system_clock::duration& d)
{
    return from_timepoint(std::chrono::time_point<std::chrono::system_clock>(d));
}

Date Date::from_timepoint(const std::chrono::system_clock::time_point& tp)
{
    return Date{std::chrono::floor<std::chrono::days>(tp)};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// friend operators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool operator==(const Date& d1, const Date& d2)
{
    return d1.year_month_day() == d2.year_month_day();
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    return os << date.iso_string();
}

Date operator+(const Date& date, const TimeDelta& time_delta)
{
    const auto time_point = std::chrono::sys_days(date.year_month_day()) + time_delta.duration();
    return Date{std::chrono::time_point_cast<std::chrono::days>(time_point)};
}

Date operator+(const TimeDelta& time_delta, const Date& date)
{
    return date + time_delta;
}

Date operator-(const Date& date, const TimeDelta& time_delta)
{
    const auto time_point = std::chrono::sys_days(date.year_month_day()) - time_delta.duration();
    return Date{std::chrono::time_point_cast<std::chrono::days>(time_point)};
}

TimeDelta operator-(const Date& d1, const Date& d2)
{
    return TimeDelta{std::chrono::sys_days(d1.year_month_day()) - std::chrono::sys_days(d2.year_month_day())};
}

}

std::ostream& operator<<(std::ostream& os, const std::chrono::year& y)
{
    os.fill('0');
    os.flags(std::ios::dec | std::ios::internal);
    os.width(4 + (y < std::chrono::year{0}));
    os.imbue(std::locale::classic());
    os << static_cast<int>(y);

    if (!y.ok())
        os << " is not a valid year";

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::chrono::month& m)
{
    if (!m.ok())
        os << static_cast<unsigned>(m) << " is not a valid month";
    else
        os << date::format("%b", date::month(static_cast<unsigned>(m)));
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::chrono::day& d)
{
    os.fill('0');
    os.flags(std::ios::dec | std::ios::internal);
    os.width(2 + (d < std::chrono::day{0}));
    os.imbue(std::locale::classic());
    os << static_cast<unsigned>(d);

    if (!d.ok())
        os << " is not a valid day";

    return os;
}
