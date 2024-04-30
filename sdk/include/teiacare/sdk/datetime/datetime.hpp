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
class datetime
{
public:
    explicit constexpr datetime() noexcept
        : _tp{decltype(_tp)::min()}
    {
    }

    explicit constexpr datetime(const std::chrono::system_clock::time_point& tp) noexcept
        : _tp{tp}
    {
    }
    explicit constexpr datetime(std::chrono::system_clock::time_point&& tp) noexcept
        : _tp{std::forward<decltype(tp)&&>(tp)}
    {
    }

    explicit constexpr datetime(const tc::sdk::date& date) noexcept
        : _tp{date.to_duration()}
    {
    }

    explicit constexpr datetime(const tc::sdk::time& time) noexcept
        : _tp{time.to_duration()}
    {
    }

    explicit constexpr datetime(const tc::sdk::date& date, const tc::sdk::time& time) noexcept
        : _tp{date.to_duration() + time.to_duration()}
    {
    }

    explicit constexpr datetime(
        const std::chrono::year& y,
        const std::chrono::month& m,
        const std::chrono::day& d,
        const std::chrono::hours& hh,
        const std::chrono::minutes& mm,
        const std::chrono::seconds& ss,
        const std::chrono::milliseconds& ms = 0ms,
        const std::chrono::microseconds& us = 0us,
        const std::chrono::nanoseconds& ns = 0ns) noexcept
        : _tp{std::chrono::sys_days(std::chrono::year_month_day(y, m, d)) + hh + mm + ss + ms + us + ns}
    {
    }

    constexpr inline bool is_valid() const
    {
        return _tp > decltype(_tp)::min() && _tp < decltype(_tp)::max();
    }

    constexpr inline tc::sdk::date date() const
    {
        std::chrono::sys_days total_days = std::chrono::floor<std::chrono::days>(_tp);
        std::chrono::year_month_day ymd{total_days};
        return tc::sdk::date(ymd);
    }

    constexpr inline tc::sdk::time time() const
    {
        std::chrono::sys_days total_days = std::chrono::floor<std::chrono::days>(_tp);
        std::chrono::system_clock::duration time_duration = _tp - total_days;
        return tc::sdk::time(time_duration);
    }

    constexpr inline std::chrono::system_clock::time_point to_time_point() const noexcept
    {
        return _tp;
    }

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::datetime utc_now() noexcept
    {
        const auto now = std::chrono::system_clock::now();
        return tc::sdk::datetime{std::chrono::time_point_cast<DurationT>(now)};
    }

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::datetime from_string(const std::string& str, const std::string& format = "%FT%T") noexcept(false);

    /*!
     * \brief Equality operator.
     * \param other the datetime to compare against.
     * \return true if the two datetime objects are the same.
     */
    constexpr inline bool operator==(const datetime& other) const noexcept
    {
        return _tp == other._tp;
    }

    /*!
     * \brief Inequality operator.
     * \param other the datetime to compare against.
     * \return true if the two datetime objects are the different.
     */
    constexpr inline bool operator!=(const datetime& other) const noexcept
    {
        return !operator==(other);
    }

    constexpr tc::sdk::datetime operator+(const timedelta& delta)
    {
        return tc::sdk::datetime{_tp + delta.total_nanoseconds()};
    }

    constexpr tc::sdk::datetime operator-(const timedelta& delta)
    {
        return tc::sdk::datetime{_tp - delta.total_nanoseconds()};
    }

    constexpr inline bool operator<(const datetime& other) const noexcept
    {
        return _tp < other._tp;
    }

    constexpr inline bool operator>(const datetime& other) const noexcept
    {
        return _tp > other._tp;
    }

    constexpr inline timedelta operator-(const datetime& other) const noexcept
    {
        return tc::sdk::timedelta{_tp - other._tp};
    }

    /*!
     * \brief Get the datetime string representation
     * \return String representation of the current datetime.
     */
    template <class DurationT = std::chrono::milliseconds>
    std::string to_string(const std::string& format = "%FT%T") const;

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param dt the datetime object to stream.
     * \return reference to the output stream operator, with the datetime string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const datetime& dt)
    {
        return stream << dt.to_string();
    }

private:
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> _tp;
};

template <class DurationT>
std::string datetime::to_string(const std::string& format) const
{
    return date::format(format, std::chrono::time_point_cast<DurationT>(_tp));
}

template <class DurationT>
tc::sdk::datetime tc::sdk::datetime::from_string(const std::string& str, const std::string& format)
{
    std::chrono::sys_time<DurationT> parsed_time;
    std::stringstream ss{str};
    ss >> date::parse(format, parsed_time);
    if (ss.fail())
        throw std::runtime_error("Failed to parse " + str);

    return tc::sdk::datetime(parsed_time);
}

}
