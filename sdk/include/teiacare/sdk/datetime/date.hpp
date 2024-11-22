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
#include <teiacare/sdk/datetime/timedelta.hpp>

#include <chrono>
#include <ostream>

namespace tc::sdk
{
/*!
 * \class date
 * \brief Represents a date object with various utility functions for date manipulation and comparison.
 *
 * The tc::sdk::date class provides operations such as date comparisons, conversions, and string representations.
 */
class date
{
public:
    /*!
     * \brief Default constructor. Initializes an invalid date.
     */
    explicit constexpr date() noexcept
        : _ymd{}
    {
    }

    /*!
     * \brief Constructor initializing the date from a std::chrono::year_month_day.
     * \param ymd The year_month_day value to initialize the date.
     */
    explicit constexpr date(const std::chrono::year_month_day& ymd) noexcept
        : _ymd{ymd}
    {
    }

    /*!
     * \brief Constructor initializing the date by moving a std::chrono::year_month_day.
     * \param ymd The year_month_day value to move and initialize the date.
     */
    explicit constexpr date(std::chrono::year_month_day&& ymd) noexcept
        : _ymd{std::move(ymd)}
    {
    }

    /*!
     * \brief Constructor initializing the date from separate year, month, and day components.
     * \param y The year component.
     * \param m The month component.
     * \param d The day component.
     */
    explicit constexpr date(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d) noexcept
        : _ymd{y, m, d}
    {
    }

    /*!
     * \brief Constructor initializing the date from separate year, month, and day components by moving them.
     * \param y The year component.
     * \param m The month component.
     * \param d The day component.
     */
    explicit constexpr date(std::chrono::year&& y, std::chrono::month&& m, std::chrono::day&& d) noexcept
        : _ymd{std::move(y), std::move(m), std::move(d)}
    {
    }

    /*!
     * \brief Constructor initializing the date from a time point.
     * \param timepoint The system time point representing the date.
     */
    explicit constexpr date(const tc::sdk::sys_time_point& timepoint) noexcept
        : _ymd{std::chrono::floor<std::chrono::days>(timepoint)}
    {
    }

    /*!
     * \brief Constructor initializing the date from a duration in nanoseconds.
     * \param duration The duration since the epoch in nanoseconds.
     */
    explicit constexpr date(const std::chrono::nanoseconds& duration) noexcept
        : date(tc::sdk::sys_time_point(duration))
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::date instance to another one.
     */
    constexpr date& operator=(const date&) noexcept = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::date instance to another one.
     */
    constexpr date& operator=(date&&) noexcept = default;

    /*!
     * \brief Checks if the date is valid.
     * \return true if the date is valid, false otherwise.
     */
    constexpr bool is_valid() const noexcept
    {
        return _ymd.ok();
    }

    /*!
     * \brief Retrieves the year component of the date.
     * \return The year.
     */
    constexpr inline std::chrono::year year() const noexcept
    {
        return _ymd.year();
    }

    /*!
     * \brief Retrieves the month component of the date.
     * \return The month.
     */
    constexpr inline std::chrono::month month() const noexcept
    {
        return _ymd.month();
    }

    /*!
     * \brief Retrieves the day component of the date.
     * \return The day.
     */
    constexpr inline std::chrono::day day() const noexcept
    {
        return _ymd.day();
    }

    /*!
     * \brief Retrieves the weekday of the date.
     * \return The weekday.
     */
    constexpr inline std::chrono::weekday weekday() const noexcept
    {
        return std::chrono::weekday(_ymd);
    }

    /*!
     * \brief Retrieves the ISO weekday representation of the date.
     * \return The ISO weekday as an unsigned integer.
     */
    constexpr unsigned iso_weekday() const
    {
        return weekday().iso_encoding();
    }

    /*!
     * \brief Converts the date to a system time point.
     * \return The system time point representation of the date.
     */
    constexpr inline std::chrono::sys_days to_timepoint() const noexcept
    {
        return std::chrono::sys_days(_ymd);
    }

    /*!
     * \brief Equality operator.
     * \param other the date to compare against.
     * \return true if the two date objects are the same.
     */
    constexpr inline bool operator==(const date& other) const noexcept
    {
        return _ymd == other._ymd;
    }

    /*!
     * \brief Inequality operator.
     * \param other the date to compare against.
     * \return true if the two date objects are the different.
     */
    constexpr inline bool operator!=(const date& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Adds a time delta to the current date.
     * \param delta The tc::sdk::timedelta to add.
     * \return A new date that represents the result.
     */
    constexpr tc::sdk::date operator+(const timedelta& delta) const noexcept
    {
        return tc::sdk::date{std::chrono::sys_days(_ymd) + delta.total_nanoseconds()};
    }

    /*!
     * \brief Subtracts a time delta from the current date.
     * \param delta The tc::sdk::timedelta to subtract.
     * \return A new date that represents the result.
     */
    constexpr tc::sdk::date operator-(const timedelta& delta) const noexcept
    {
        return tc::sdk::date{std::chrono::sys_days(_ymd) - delta.total_nanoseconds()};
    }

    /*!
     * \brief Checks if the current date is less than another date.
     * \param other The date to compare against.
     * \return true if the current date is less, false otherwise.
     */
    constexpr inline bool operator<(const date& other) const noexcept
    {
        return _ymd < other._ymd;
    }

    /*!
     * \brief Checks if the current date is greater than another date.
     * \param other The date to compare against.
     * \return true if the current date is greater, false otherwise.
     */
    constexpr inline bool operator>(const date& other) const noexcept
    {
        return _ymd > other._ymd;
    }

    /*!
     * \brief Calculates the time difference between two dates.
     * \param other The date to subtract from the current date.
     * \return A tc::sdk::timedelta representing the difference.
     */
    inline timedelta operator-(const date& other) const noexcept
    {
        return tc::sdk::timedelta{std::chrono::sys_days(_ymd) - std::chrono::sys_days(other._ymd)};
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param t the date object to stream.
     * \return reference to the output stream operator, with the date string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const date& d);

    /*!
     * \brief Get the date string representation
     * \return String representation of the current date.
     */
    std::string to_string(const std::string& format = "%F") const noexcept(false); // equivalent to "%Y-%m-%d"

    /*!
     * \brief Creates a date object from a string representation.
     * \param str The string representing the date.
     * \param format The format to interpret the string (default: "%F").
     * \return A date object representing the parsed date.
     */
    static tc::sdk::date from_string(const std::string& str, const std::string& format = "%F") noexcept(false);

    /*!
     * \brief Gets the current date.
     * \return A tc::sdk::date object representing today's date.
     */
    static tc::sdk::date today() noexcept;

private:
    std::chrono::year_month_day _ymd;
};

}
