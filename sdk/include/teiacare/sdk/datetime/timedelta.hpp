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

#include <chrono>
#include <ostream>

namespace tc::sdk
{
class timedelta
{
public:
    constexpr explicit timedelta() noexcept;
    constexpr explicit timedelta(const std::chrono::steady_clock::duration& duration) noexcept;
    constexpr explicit timedelta(std::chrono::steady_clock::duration&& duration) noexcept;

    template <class... Durations>
    explicit timedelta(const Durations&... durations);

    template <class... Durations>
    explicit timedelta(Durations&&... durations);

    constexpr inline bool is_null() const noexcept;

    constexpr inline std::chrono::hours hours() const;
    constexpr inline std::chrono::minutes minutes() const;
    constexpr inline std::chrono::seconds seconds() const;
    constexpr inline std::chrono::milliseconds milliseconds() const;
    constexpr inline std::chrono::microseconds microseconds() const;
    constexpr inline std::chrono::nanoseconds nanoseconds() const;

    constexpr inline std::chrono::years total_years() const;
    constexpr inline std::chrono::months total_months() const;
    constexpr inline std::chrono::days total_days() const;
    constexpr inline std::chrono::hours total_hours() const;
    constexpr inline std::chrono::minutes total_minutes() const;
    constexpr inline std::chrono::seconds total_seconds() const;
    constexpr inline std::chrono::milliseconds total_milliseconds() const;
    constexpr inline std::chrono::microseconds total_microseconds() const;
    constexpr inline std::chrono::nanoseconds total_nanoseconds() const;

    /*!
     * \brief Equality operator.
     * \param other the timedelta to compare against.
     * \return true if the two timedelta objects are the same.
     */
    constexpr inline bool operator==(const timedelta& other) const noexcept;

    /*!
     * \brief Inequality operator.
     * \param other the timedelta to compare against.
     * \return true if the two timedelta objects are the different.
     */
    constexpr inline bool operator!=(const timedelta& other) const noexcept;

    constexpr inline timedelta operator+(const timedelta& other) const noexcept;
    constexpr inline timedelta operator-(const timedelta& other) const noexcept;
    constexpr inline timedelta operator*(int64_t multiplier) const noexcept;
    constexpr inline timedelta operator/(int64_t divider) const noexcept(false);

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param dt the timedelta object to stream.
     * \return reference to the output stream operator, with the timedelta string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const timedelta& td);

    /*!
     * \brief Get the timedelta string representation.
     * \tparam DurationT specify the datetime precision, by default is std::chrono::milliseconds.
     * \return String representation of the current timedelta.
     */
    template <class DurationT = std::chrono::milliseconds>
    std::string to_string() const noexcept(false);

private:
    const std::chrono::nanoseconds _duration;
};

constexpr timedelta::timedelta() noexcept
    : _duration{decltype(_duration)::zero()}
{
}

constexpr timedelta::timedelta(const std::chrono::steady_clock::duration& duration) noexcept
    : _duration{duration}
{
}

constexpr timedelta::timedelta(std::chrono::steady_clock::duration&& duration) noexcept
    : _duration{std::forward<std::chrono::steady_clock::duration&&>(duration)}
{
}

template <class... Durations>
timedelta::timedelta(const Durations&... durations)
    : _duration{(durations + ...)}
{
}

template <class... Durations>
timedelta::timedelta(Durations&&... durations)
    : _duration{(std::forward<Durations&&>(durations) + ...)}
{
}

constexpr inline bool timedelta::is_null() const noexcept
{
    return _duration == decltype(_duration)::zero();
}

constexpr inline std::chrono::hours timedelta::hours() const
{
    return std::chrono::floor<std::chrono::hours>(_duration);
}

constexpr inline std::chrono::minutes timedelta::minutes() const
{
    return std::chrono::floor<std::chrono::minutes>(_duration) -
           std::chrono::floor<std::chrono::hours>(_duration);
}

constexpr inline std::chrono::seconds timedelta::seconds() const
{
    return std::chrono::floor<std::chrono::seconds>(_duration) -
           std::chrono::floor<std::chrono::minutes>(_duration);
}

constexpr inline std::chrono::milliseconds timedelta::milliseconds() const
{
    return std::chrono::floor<std::chrono::milliseconds>(_duration) -
           std::chrono::floor<std::chrono::seconds>(_duration);
}

constexpr inline std::chrono::microseconds timedelta::microseconds() const
{
    return std::chrono::floor<std::chrono::microseconds>(_duration) -
           std::chrono::floor<std::chrono::milliseconds>(_duration);
}

constexpr inline std::chrono::nanoseconds timedelta::nanoseconds() const
{
    return std::chrono::floor<std::chrono::nanoseconds>(_duration) -
           std::chrono::floor<std::chrono::microseconds>(_duration);
}

constexpr inline std::chrono::years timedelta::total_years() const
{
    return std::chrono::duration_cast<std::chrono::years>(_duration);
}

constexpr inline std::chrono::months timedelta::total_months() const
{
    return std::chrono::duration_cast<std::chrono::months>(_duration);
}

constexpr inline std::chrono::days timedelta::total_days() const
{
    return std::chrono::duration_cast<std::chrono::days>(_duration);
}

constexpr inline std::chrono::hours timedelta::total_hours() const
{
    return std::chrono::duration_cast<std::chrono::hours>(_duration);
}

constexpr inline std::chrono::minutes timedelta::total_minutes() const
{
    return std::chrono::duration_cast<std::chrono::minutes>(_duration);
}

constexpr inline std::chrono::seconds timedelta::total_seconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(_duration);
}

constexpr inline std::chrono::milliseconds timedelta::total_milliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(_duration);
}

constexpr inline std::chrono::microseconds timedelta::total_microseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
}

constexpr inline std::chrono::nanoseconds timedelta::total_nanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration);
}

constexpr inline bool timedelta::operator==(const timedelta& other) const noexcept
{
    return _duration == other._duration;
}

constexpr inline bool timedelta::operator!=(const timedelta& other) const noexcept
{
    return !operator==(other);
}

constexpr inline timedelta timedelta::operator+(const timedelta& other) const noexcept
{
    return timedelta{_duration + other._duration};
}

constexpr inline timedelta timedelta::operator-(const timedelta& other) const noexcept
{
    return timedelta{_duration - other._duration};
}

constexpr inline timedelta timedelta::operator*(int64_t multiplier) const noexcept
{
    return timedelta{_duration * multiplier};
}

constexpr inline timedelta timedelta::operator/(int64_t divider) const noexcept(false)
{
    return timedelta{_duration / divider};
}

}
