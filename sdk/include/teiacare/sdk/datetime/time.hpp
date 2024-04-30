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
#include <chrono>
#include <ostream>

namespace tc::sdk
{
class time
{
public:
    explicit constexpr time() noexcept
        : _hh_mm_ss{std::chrono::nanoseconds::min()}
    {
    }

    template <class... Durations>
    explicit constexpr time(const Durations&... durations) noexcept
        : _hh_mm_ss{(durations + ...) % std::chrono::hours(24)}
    {
    }

    template <class... Durations>
    explicit constexpr time(Durations&&... durations) noexcept
        : _hh_mm_ss{(std::forward<Durations&&>(durations) + ...) % std::chrono::hours(24)}
    {
    }

    constexpr bool is_valid() const noexcept
    {
        return _hh_mm_ss.to_duration() != std::chrono::nanoseconds::min();
    }

    constexpr inline std::chrono::hours hours() const noexcept
    {
        return _hh_mm_ss.hours();
    }

    constexpr inline std::chrono::minutes minutes() const noexcept
    {
        return _hh_mm_ss.minutes();
    }

    constexpr inline std::chrono::seconds seconds() const noexcept
    {
        return _hh_mm_ss.seconds();
    }

    constexpr inline std::chrono::milliseconds milliseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_hh_mm_ss.subseconds());
    }

    constexpr inline std::chrono::microseconds microseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_hh_mm_ss.subseconds());
    }

    constexpr inline std::chrono::nanoseconds nanoseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_hh_mm_ss.subseconds());
    }

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::time utc_now() noexcept
    {
        const auto now = std::chrono::system_clock::now();
        return tc::sdk::time{std::chrono::floor<DurationT>(now) - std::chrono::floor<std::chrono::days>(now)};
    }

    template <class DurationT = std::chrono::milliseconds>
    static tc::sdk::time from_string(const std::string& str, const std::string& format = "%T") noexcept(false);

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

    constexpr tc::sdk::time operator+(const timedelta& delta) const noexcept
    {
        return tc::sdk::time{_hh_mm_ss.to_duration() + delta.total_nanoseconds()};
    }

    constexpr tc::sdk::time operator-(const timedelta& delta) const noexcept
    {
        return tc::sdk::time{_hh_mm_ss.to_duration() - delta.total_nanoseconds()};
    }

    constexpr inline bool operator<(const time& other) const noexcept
    {
        return _hh_mm_ss.to_duration() < other._hh_mm_ss.to_duration();
    }

    constexpr inline bool operator>(const time& other) const noexcept
    {
        return _hh_mm_ss.to_duration() > other._hh_mm_ss.to_duration();
    }

    constexpr inline timedelta operator-(const time& other) const noexcept
    {
        return tc::sdk::timedelta{_hh_mm_ss.to_duration() - other._hh_mm_ss.to_duration()};
    }

    template <class DurationT = std::chrono::milliseconds>
    std::string to_string(const char* format = "%T") const; // equivalent to "%H:%M:%S"

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param t the time object to stream.
     * \return reference to the output stream operator, with the time string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const time& t)
    {
        return stream << t.to_string();
    }

private:
    const std::chrono::hh_mm_ss<std::chrono::nanoseconds> _hh_mm_ss;
};

template <class DurationT>
std::string time::to_string(const char* format) const
{
    return ::date::format(format, std::chrono::floor<DurationT>(_hh_mm_ss.to_duration()));
}

template <class DurationT>
tc::sdk::time tc::sdk::time::from_string(const std::string& str, const std::string& format)
{
    std::chrono::sys_time<DurationT> parsed_time;
    std::stringstream ss{str};
    ss >> ::date::parse(format, parsed_time);
    if (ss.fail())
        throw std::runtime_error("Failed to parse " + str);

    return tc::sdk::time(parsed_time);
}

}
