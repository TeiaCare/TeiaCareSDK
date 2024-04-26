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
class DateTime
{
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

    constexpr inline bool is_valid() const;
    tc::sdk::Date date() const;
    tc::sdk::Time time() const;

    constexpr inline std::chrono::system_clock::time_point to_time_point() const noexcept;

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::DateTime from_string(const std::string& str, const std::string& format = "%FT%T");

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::DateTime utc_now() noexcept;

    /*!
     * \brief Equality operator.
     * \param other the DateTime to compare against.
     * \return true if the two DateTime objects are the same.
     */
    constexpr inline bool operator==(const DateTime& other) const noexcept
    {
        return _tp == other._tp;
    }

    /*!
     * \brief Inequality operator.
     * \param other the DateTime to compare against.
     * \return true if the two DateTime objects are the different.
     */
    constexpr inline bool operator!=(const DateTime& other) const noexcept
    {
        return !operator==(other);
    }

    tc::sdk::DateTime operator+(const TimeDelta& delta)
    {
        return tc::sdk::DateTime{_tp + delta.total_nanoseconds()};
    }

    tc::sdk::DateTime operator-(const TimeDelta& delta)
    {
        return tc::sdk::DateTime{_tp - delta.total_nanoseconds()};
    }

    constexpr inline bool operator<(const DateTime& other) const noexcept
    {
        return _tp < other._tp;
    }

    constexpr inline bool operator>(const DateTime& other) const noexcept
    {
        return _tp > other._tp;
    }

    constexpr inline TimeDelta operator-(const DateTime& other) const noexcept
    {
        return tc::sdk::TimeDelta{_tp - other._tp};
    }

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

    const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> _tp;
};

constexpr DateTime::DateTime() noexcept
    : _tp{decltype(_tp)::min()}
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
    : _tp{std::chrono::sys_days(std::chrono::year_month_day(y, m, d)) + hh + mm + ss + ms + us + ns}
{
}

constexpr inline bool DateTime::is_valid() const
{
    return _tp > decltype(_tp)::min() && _tp < decltype(_tp)::max();
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

constexpr inline std::chrono::system_clock::time_point DateTime::to_time_point() const noexcept
{
    return _tp;
}

template <class DurationT>
tc::sdk::DateTime DateTime::utc_now() noexcept
{
    const auto now = std::chrono::system_clock::now();
    return tc::sdk::DateTime{std::chrono::time_point_cast<DurationT>(now)};
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
