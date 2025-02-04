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

#include <teiacare/sdk/clock.hpp>
#include <teiacare/sdk/datetime/date.hpp>
#include <teiacare/sdk/datetime/time.hpp>
#include <teiacare/sdk/datetime/timedelta.hpp>

#include <chrono>
#include <ostream>

using namespace std::chrono_literals;

namespace tc::sdk
{
/*!
 * \class datetime
 * \brief Represents a combined date and time object with various utility functions for datetime manipulation, comparison, and string conversion.
 *
 * The tc::sdk::datetime provides operations such as date and time extraction, arithmetic operations, and string representations.
 */
class datetime
{
public:
    /*!
     * \brief Default constructor initializing an invalid datetime.
     */
    explicit constexpr datetime() noexcept
        : _tp{decltype(_tp)::min()}
    {
    }

    /*!
     * \brief Constructor initializing the datetime from a system time point.
     * \param tp The system time point representing the datetime.
     */
    explicit constexpr datetime(const tc::sdk::sys_time_point& tp) noexcept
        : _tp{tp}
    {
    }

    /*!
     * \brief Constructor initializing the datetime by moving a system time point.
     * \param tp The system time point to move and initialize the datetime.
     */
    explicit constexpr datetime(tc::sdk::sys_time_point&& tp) noexcept
        : _tp{std::forward<decltype(tp)&&>(tp)}
    {
    }

    /*!
     * \brief Constructor initializing the datetime from a date.
     * \param date The tc::sdk::date object.
     */
    explicit constexpr datetime(const tc::sdk::date& date) noexcept
        : _tp{date.to_timepoint()}
    {
    }

    /*!
     * \brief Constructor initializing the datetime from a time.
     * \param time The tc::sdk::time object.
     */
    explicit constexpr datetime(const tc::sdk::time& time) noexcept
        : _tp{time.to_duration()}
    {
    }

    /*!
     * \brief Constructor initializing the datetime from a date and a time.
     * \param date The tc::sdk::date object.
     * \param time The tc::sdk::time object.
     */
    explicit constexpr datetime(const tc::sdk::date& date, const tc::sdk::time& time) noexcept
        : _tp{date.to_timepoint() + time.to_duration()}
    {
    }

    /*!
     * \brief Constructor initializing the datetime from year, month, day, and time components.
     * \param y The year component.
     * \param m The month component.
     * \param d The day component.
     * \param hh The hour component.
     * \param mm The minute component.
     * \param ss The second component.
     * \param ms The millisecond component (default: 0).
     * \param us The microsecond component (default: 0).
     * \param ns The nanosecond component (default: 0).
     */
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

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::datetime instance to another one.
     */
    constexpr datetime(const datetime&) noexcept = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::datetime instance to another one.
     */
    constexpr datetime(datetime&&) noexcept = default;

    /*!
     * \brief Assignment operator. Assign a tc::sdk::datetime instance to another one.
     */
    constexpr datetime& operator=(const datetime&) noexcept = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::datetime instance to another one.
     */
    constexpr datetime& operator=(datetime&&) noexcept = default;

    /*!
     * \brief Checks if the datetime is valid.
     * \return true if the datetime is valid, false otherwise.
     */
    constexpr inline bool is_valid() const
    {
        return _tp > decltype(_tp)::min() && _tp < decltype(_tp)::max();
    }

    /*!
     * \brief Retrieves the date component of the datetime.
     * \return The tc::sdk::date representing the date part.
     */
    constexpr inline tc::sdk::date date() const
    {
        std::chrono::sys_days total_days = std::chrono::floor<std::chrono::days>(_tp);
        std::chrono::year_month_day ymd{total_days};
        return tc::sdk::date(ymd);
    }

    /*!
     * \brief Retrieves the time component of the datetime.
     * \return The tc::sdk::time representing the time part.
     */
    constexpr inline tc::sdk::time time() const
    {
        std::chrono::sys_days total_days = std::chrono::floor<std::chrono::days>(_tp);
        std::chrono::nanoseconds time_duration = _tp - total_days;
        return tc::sdk::time(time_duration);
    }

    /*!
     * \brief Converts the datetime to a system time point.
     * \return The system time point representation of the datetime.
     */
    constexpr inline tc::sdk::sys_time_point to_time_point() const noexcept
    {
        return _tp;
    }

    /*!
     * \brief Spaceship operator.
     * \param other the datetime to compare against.
     * \return std::strong_ordering comparing two datetime objects.
     */
    constexpr auto operator<=>(const datetime& other) const noexcept = default;

    /*!
     * \brief Adds a timedelta to the current datetime.
     * \param delta The tc::sdk::timedelta to add.
     * \return A new datetime representing the result.
     */
    constexpr tc::sdk::datetime operator+(const timedelta& delta) const noexcept
    {
        return tc::sdk::datetime{_tp + delta.total_nanoseconds()};
    }

    /*!
     * \brief Subtracts a timedelta from the current datetime.
     * \param delta The tc::sdk::timedelta to subtract.
     * \return A new datetime representing the result.
     */
    constexpr tc::sdk::datetime operator-(const timedelta& delta) const noexcept
    {
        return tc::sdk::datetime{_tp - delta.total_nanoseconds()};
    }

    /*!
     * \brief Calculates the time difference between two datetime objects.
     * \param other The datetime to subtract from the current datetime.
     * \return A tc::sdk::timedelta representing the difference.
     */
    constexpr inline timedelta operator-(const datetime& other) const noexcept
    {
        return tc::sdk::timedelta{_tp - other._tp};
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param dt the datetime object to stream.
     * \return reference to the output stream operator, with the datetime string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const datetime& dt);

    /*!
     * \brief Get the datetime string representation.
     * \param format The format string to use for conversion (default: "%FT%T").
     * \return String representation of the current datetime.
     */
    template <class DurationT = std::chrono::milliseconds>
    std::string to_string(const std::string& format = "%FT%T") const noexcept(false);

    /*!
     * \brief Creates a datetime object from a string representation.
     * \param str The string representing the datetime.
     * \param format The format to interpret the string (default: "%FT%T").
     * \return A datetime object representing the parsed datetime.
     */
    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::datetime from_string(const std::string& str, const std::string& format = "%FT%T") noexcept(false);

    /*!
     * \brief Gets the current UTC datetime.
     * \return A tc::sdk::datetime object representing the current UTC time.
     */
    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::datetime utc_now() noexcept;

private:
    tc::sdk::sys_time_point _tp;
};

}
