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

#include <chrono>
#include <ostream>

namespace tc::sdk
{
/*!
 * \class time
 * \brief Represents a time object that includes hours, minutes, seconds, and subseconds with various utility functions for manipulation, comparison, and string conversion.
 *
 * The tc::sdk::time class allows users to create and interact with time instances, perform arithmetic operations,
 * and convert to/from string representations.
 */
class time
{
public:
    /*!
     * \brief Default constructor initializing an invalid time.
     */
    explicit constexpr time() noexcept
        : _hh_mm_ss{std::chrono::nanoseconds::max()}
    {
    }

    /*!
     * \brief Constructor initializing the time from one or more durations.
     * \param durations The durations to initialize the time, normalized within 24 hours.
     */
    template <class... Durations>
    explicit constexpr time(const Durations&... durations) noexcept
        : _hh_mm_ss{(durations + ...) % std::chrono::hours(24)}
    {
    }

    /*!
     * \brief Constructor initializing the time by moving one or more durations.
     * \param durations The durations to move and initialize the time, normalized within 24 hours.
     */
    template <class... Durations>
    explicit constexpr time(Durations&&... durations) noexcept
        : _hh_mm_ss{(std::forward<Durations&&>(durations) + ...) % std::chrono::hours(24)}
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::time instance to another one.
     */
    constexpr time& operator=(const time&) noexcept = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::time instance to another one.
     */
    constexpr time& operator=(time&&) noexcept = default;

    /*!
     * \brief Checks if the time is valid (i.e., within 24 hours).
     * \return true if the time is valid, false otherwise.
     */
    constexpr bool is_valid() const noexcept
    {
        return _hh_mm_ss.to_duration() <= std::chrono::hours(24);
    }

    /*!
     * \brief Retrieves the hours component of the time.
     * \return The hours as std::chrono::hours.
     */
    constexpr inline std::chrono::hours hours() const noexcept
    {
        return _hh_mm_ss.hours();
    }

    /*!
     * \brief Retrieves the minutes component of the time.
     * \return The minutes as std::chrono::minutes.
     */
    constexpr inline std::chrono::minutes minutes() const noexcept
    {
        return _hh_mm_ss.minutes();
    }

    /*!
     * \brief Retrieves the seconds component of the time.
     * \return The seconds as std::chrono::seconds.
     */
    constexpr inline std::chrono::seconds seconds() const noexcept
    {
        return _hh_mm_ss.seconds();
    }

    /*!
     * \brief Retrieves the milliseconds component of the time.
     * \return The milliseconds as std::chrono::milliseconds.
     */
    constexpr inline std::chrono::milliseconds milliseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_hh_mm_ss.subseconds());
    }

    /*!
     * \brief Retrieves the microseconds component of the time.
     * \return The microseconds as std::chrono::microseconds.
     */
    constexpr inline std::chrono::microseconds microseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_hh_mm_ss.subseconds());
    }

    /*!
     * \brief Retrieves the nanoseconds component of the time.
     * \return The nanoseconds as std::chrono::nanoseconds.
     */
    constexpr inline std::chrono::nanoseconds nanoseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_hh_mm_ss.subseconds());
    }

    /*!
     * \brief Converts the time to its total duration from the start of the day.
     * \return The total duration as std::chrono::nanoseconds.
     */
    constexpr inline std::chrono::nanoseconds to_duration() const noexcept
    {
        return _hh_mm_ss.to_duration();
    }

    /*!
     * \brief Equality operator.
     * \param other the time to compare against.
     * \return true if the two time objects are the same.
     */
    constexpr inline bool operator==(const time& other) const noexcept
    {
        return _hh_mm_ss.to_duration() == other._hh_mm_ss.to_duration();
    }

    /*!
     * \brief Inequality operator.
     * \param other the datetime to compare against.
     * \return true if the two datetime objects are the different.
     */
    constexpr inline bool operator!=(const time& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Adds a timedelta to the current time.
     * \param delta The tc::sdk::timedelta to add.
     * \return A new time object representing the result.
     */
    constexpr tc::sdk::time operator+(const timedelta& delta) const noexcept
    {
        return tc::sdk::time{_hh_mm_ss.to_duration() + delta.total_nanoseconds()};
    }

    /*!
     * \brief Subtracts a timedelta from the current time.
     * \param delta The tc::sdk::timedelta to subtract.
     * \return A new time object representing the result.
     */
    constexpr tc::sdk::time operator-(const timedelta& delta) const noexcept
    {
        return tc::sdk::time{_hh_mm_ss.to_duration() - delta.total_nanoseconds()};
    }

    /*!
     * \brief Checks if the current time is less than another time.
     * \param other The time to compare against.
     * \return true if the current time is less, false otherwise.
     */
    constexpr inline bool operator<(const time& other) const noexcept
    {
        return _hh_mm_ss.to_duration() < other._hh_mm_ss.to_duration();
    }

    /*!
     * \brief Checks if the current time is greater than another time.
     * \param other The time to compare against.
     * \return true if the current time is greater, false otherwise.
     */
    constexpr inline bool operator>(const time& other) const noexcept
    {
        return _hh_mm_ss.to_duration() > other._hh_mm_ss.to_duration();
    }

    /*!
     * \brief Calculates the time difference between two time objects.
     * \param other The time to subtract from the current time.
     * \return A tc::sdk::timedelta representing the difference.
     */
    constexpr inline timedelta operator-(const time& other) const noexcept
    {
        return tc::sdk::timedelta{_hh_mm_ss.to_duration() - other._hh_mm_ss.to_duration()};
    }

    /*!
     * \brief Adds the time of two time objects.
     * \param other The time to add.
     * \return A tc::sdk::timedelta representing the combined time.
     */
    constexpr inline timedelta operator+(const time& other) const noexcept
    {
        return tc::sdk::timedelta{_hh_mm_ss.to_duration() + other._hh_mm_ss.to_duration()};
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param t the time object to stream.
     * \return reference to the output stream operator, with the time string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const time& t);

    /*!
     * \brief Get the time string representation
     * \return String representation of the current time.
     */
    template <class DurationT = std::chrono::milliseconds>
    std::string to_string(const std::string& format = "%T") const noexcept(false); // equivalent to "%H:%M:%S"

    /*!
     * \brief Creates a time object from a string representation.
     * \param str The string representing the time.
     * \param format The format to interpret the string (default: "%T").
     * \return A time object representing the parsed time.
     */
    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::time from_string(const std::string& str, const std::string& format = "%T") noexcept(false);

    /*!
     * \brief Gets the current UTC time.
     * \return A tc::sdk::time object representing the current UTC time.
     */
    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::time utc_now() noexcept;

private:
    std::chrono::hh_mm_ss<std::chrono::nanoseconds> _hh_mm_ss;
};

}
