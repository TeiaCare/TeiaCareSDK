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

using namespace std::chrono_literals;

namespace tc::sdk
{
class Date;
class TimeDelta;

class DateTime
{
    using TimePointT = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
    const TimePointT _tp;

public:
    explicit constexpr DateTime() noexcept;
    explicit constexpr DateTime(const std::chrono::system_clock::time_point& tp) noexcept;
    explicit constexpr DateTime(std::chrono::system_clock::time_point&& tp) noexcept;
    explicit constexpr DateTime(const tc::sdk::Date& date) noexcept;
    explicit constexpr DateTime(const tc::sdk::Time& time) noexcept;
    explicit constexpr DateTime(const tc::sdk::Date& date, const tc::sdk::Time& time) noexcept;
    explicit constexpr DateTime(
        const std::chrono::year& y,
        const std::chrono::month& m,
        const std::chrono::day& d,
        const std::chrono::hours& hh,
        const std::chrono::minutes& mm,
        const std::chrono::seconds& ss,
        const std::chrono::milliseconds& ms = 0ms,
        const std::chrono::microseconds& us = 0us,
        const std::chrono::nanoseconds& ns = 0ns) noexcept;

    // template <class... Durations>
    // explicit DateTime(Durations&&... durations) noexcept;

    // template <class... Durations>
    // explicit DateTime(const std::chrono::year_month_day& ymd, Durations&&... durations) noexcept;

    // template <class... Durations>
    // explicit DateTime(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d, Durations&&... durations) noexcept;

    constexpr bool is_valid() const;
    tc::sdk::Date date() const;
    tc::sdk::Time time() const;

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::DateTime from_string(const std::string& str, const std::string& format = "%FT%T");

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::DateTime utc_now(const std::string& str, const std::string& format = "%FT%T");

    /*!
     * \brief Get the DateTime string representation
     * \return String representation of the current DateTime.
     */
    template <class DurationT = std::chrono::milliseconds>
    std::string to_string(const std::string& format = "%FT%T") const
    {
        return date::format(format, std::chrono::time_point_cast<DurationT>(_tp));
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param dt the DateTime object to stream.
     * \return reference to the output stream operator, with the DateTime string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const DateTime& dt)
    {
        return stream << dt.to_string();
    }

private:
    template <class DurationT>
    static tc::sdk::DateTime try_parse_as(const std::string& str, const std::string& format = "%FT%T")
    {
        std::chrono::sys_time<DurationT> parsed_time;
        std::stringstream ss{str};
        ss >> date::parse("%FT%T", parsed_time);
        if (ss.fail())
            throw std::runtime_error("Failed to parse " + str);

        return tc::sdk::DateTime(parsed_time);
    }
};

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

constexpr DateTime::DateTime() noexcept
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

constexpr DateTime::DateTime(const std::chrono::system_clock::time_point& tp) noexcept
    : _tp{tp}
{
}

constexpr DateTime::DateTime(std::chrono::system_clock::time_point&& tp) noexcept
    : _tp{std::forward<decltype(tp)&&>(tp)}
{
}

constexpr DateTime::DateTime(const tc::sdk::Date& date) noexcept
    : _tp{date.to_duration()}
{
}

constexpr DateTime::DateTime(const tc::sdk::Time& time) noexcept
    : _tp{time.to_duration()}
{
}
constexpr DateTime::DateTime(const tc::sdk::Date& date, const tc::sdk::Time& time) noexcept
    : _tp{date.to_duration() + time.to_duration()}
{
}

constexpr DateTime::DateTime(
    const std::chrono::year& y,
    const std::chrono::month& m,
    const std::chrono::day& d,
    const std::chrono::hours& hh,
    const std::chrono::minutes& mm,
    const std::chrono::seconds& ss,
    const std::chrono::milliseconds& ms,
    const std::chrono::microseconds& us,
    const std::chrono::nanoseconds& ns) noexcept
    : _tp {std::chrono::sys_days(std::chrono::year_month_day(y, m, d)) + hh + mm + ss + ms + us + ns}
{
}

// template <class... Durations>
// DateTime::DateTime(Durations&&... durations) noexcept
//     : _tp{(std::forward<Durations&&>(durations) + ...)}
// {
// }

// template <class... Durations>
// DateTime::DateTime(const std::chrono::year_month_day& ymd, Durations&&... durations) noexcept
//     : _tp{ymd + (std::forward<Durations&&>(durations) + ...)}
// {
// }

// template <class... Durations>
// DateTime::DateTime(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d, Durations&&... durations) noexcept
//     : _tp{std::chrono::year_month_day(y, m, d) + (std::forward<Durations&&>(durations) + ...)}
// {
// }

constexpr bool DateTime::is_valid() const
{
    return _tp > TimePointT::min() && _tp < TimePointT::max();
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
tc::sdk::DateTime DateTime::from_string<std::chrono::seconds>(const std::string& str, const std::string& format)
{
    return tc::sdk::DateTime::try_parse_as<std::chrono::seconds>(str, format);
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
