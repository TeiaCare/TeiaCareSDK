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

#include <teiacare/sdk/datetime/date.hpp>
#include <teiacare/sdk/datetime/time.hpp>
#include <teiacare/sdk/datetime/timedelta.hpp>

#include "date/date.h"
#include <chrono>
#include <ostream>

namespace tc::sdk
{
class Date;
class TimeDelta;

class DateTime
{
    const std::chrono::system_clock::time_point _tp;

public:
    explicit DateTime();

    explicit DateTime(const std::chrono::system_clock::time_point& tp)
        : _tp(tp)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const DateTime& dt)
    {
        return os << date::format("%FT%T", dt._tp);
    }

    template <class... Durations>
    explicit DateTime(const std::chrono::year_month_day& ymd, Durations&&... durations);

    template <class... Durations>
    explicit DateTime(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d, Durations&&... durations);

    tc::sdk::Date date() const;

    tc::sdk::Time time() const;

    template <class DurationT=std::chrono::milliseconds>
    static tc::sdk::DateTime from_string(const std::string& str, const std::string& format = "%FT%T");

    // to_string(const std::string& format = "%FT%T");

    // Maybe a Ctor?
    // template <class DurationT=std::chrono::milliseconds>
    // static tc::sdk::DateTime from_time_point(const std::chrono::system_clock::time_point& tp);

    static tc::sdk::DateTime utc_now(const std::string& str, const std::string& format = "%FT%T");

private:
    template<typename DurationT>
    static tc::sdk::DateTime try_parse_as(const std::string& str, const std::string& format = "%FT%T")
    {
        std::chrono::sys_time<DurationT> t;
        std::stringstream ss{str};
        ss >> date::parse("%FT%T", t);
        if (ss.fail())
            throw std::runtime_error("Failed to parse " + str);

        return tc::sdk::DateTime(t);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DateTime
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
template <class Duration>
DateTime<Duration> operator+(const DateTime<Duration>& x, const TimeDelta& y);

template <class Duration>
DateTime<Duration> operator+(const DateTime<Duration>& x, const TimeDelta& y);

template <class Duration>
DateTime<Duration> operator-(const TimeDelta& y, const DateTime<Duration>& x);

// TODO: allow two different Duration template params ?
template <class Duration>
TimeDelta operator-(const DateTime<Duration>& x, const DateTime<Duration>& y);

template <class Duration>
bool operator<(const DateTime<Duration>& x, const DateTime<Duration>& y);

template <class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DateTime<Duration>& date);
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DateTime impl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DateTime::DateTime()
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

template <class... Durations>
DateTime::DateTime(const std::chrono::year_month_day& ymd, Durations&&... durations)
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

template <class... Durations>
DateTime::DateTime(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d, Durations&&... durations)
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

tc::sdk::Date DateTime::date() const
{
    std::chrono::sys_days total_days = std::chrono::floor<std::chrono::days>(_tp);
    std::chrono::year_month_day ymd{total_days};
    return Date(ymd);
}

tc::sdk::Time DateTime::time() const
{
    std::chrono::sys_days total_days = std::chrono::floor<std::chrono::days>(_tp);
    std::chrono::system_clock::duration time_duration = _tp - total_days;
    return tc::sdk::Time(time_duration);
}

template <>
tc::sdk::DateTime DateTime::from_string<std::chrono::milliseconds>(const std::string& str, const std::string& format)
{
    return tc::sdk::DateTime::try_parse_as<std::chrono::milliseconds>(str, format);
}

template <>
tc::sdk::DateTime DateTime::from_string<std::chrono::microseconds>(const std::string& str, const std::string& format)
{
    return tc::sdk::DateTime::try_parse_as<std::chrono::microseconds>(str, format);
}

template <>
tc::sdk::DateTime DateTime::from_string<std::chrono::nanoseconds>(const std::string& str, const std::string& format)
{
    return tc::sdk::DateTime::try_parse_as<std::chrono::nanoseconds>(str, format);
}

}
