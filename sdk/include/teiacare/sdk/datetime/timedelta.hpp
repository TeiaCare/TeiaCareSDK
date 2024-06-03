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
    constexpr explicit timedelta() noexcept
        : _duration{decltype(_duration)::zero()}
    {
    }

    constexpr explicit timedelta(const std::chrono::steady_clock::duration& duration) noexcept
        : _duration{duration}
    {
    }

    constexpr explicit timedelta(std::chrono::steady_clock::duration&& duration) noexcept
        : _duration{std::forward<std::chrono::steady_clock::duration&&>(duration)}
    {
    }

    template <class... Durations>
    explicit timedelta(const Durations&... durations)
        : _duration{(durations + ...)}
    {
    }

    template <class... Durations>
    explicit timedelta(Durations&&... durations)
        : _duration{(std::forward<Durations&&>(durations) + ...)}
    {
    }

    constexpr inline bool is_null() const noexcept
    {
        return _duration == decltype(_duration)::zero();
    }

    constexpr inline std::chrono::hours hours() const noexcept
    {
        return std::chrono::floor<std::chrono::hours>(_duration);
    }

    constexpr inline std::chrono::minutes minutes() const noexcept
    {
        return std::chrono::floor<std::chrono::minutes>(_duration) -
               std::chrono::floor<std::chrono::hours>(_duration);
    }

    constexpr inline std::chrono::seconds seconds() const noexcept
    {
        return std::chrono::floor<std::chrono::seconds>(_duration) -
               std::chrono::floor<std::chrono::minutes>(_duration);
    }

    constexpr inline std::chrono::milliseconds milliseconds() const noexcept
    {
        return std::chrono::floor<std::chrono::milliseconds>(_duration) -
               std::chrono::floor<std::chrono::seconds>(_duration);
    }

    constexpr inline std::chrono::microseconds microseconds() const noexcept
    {
        return std::chrono::floor<std::chrono::microseconds>(_duration) -
               std::chrono::floor<std::chrono::milliseconds>(_duration);
    }

    constexpr inline std::chrono::nanoseconds nanoseconds() const noexcept
    {
        return std::chrono::floor<std::chrono::nanoseconds>(_duration) -
               std::chrono::floor<std::chrono::microseconds>(_duration);
    }

    constexpr inline std::chrono::years total_years() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::years>(_duration);
    }

    constexpr inline std::chrono::months total_months() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::months>(_duration);
    }

    constexpr inline std::chrono::days total_days() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::days>(_duration);
    }

    constexpr inline std::chrono::hours total_hours() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::hours>(_duration);
    }

    constexpr inline std::chrono::minutes total_minutes() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::minutes>(_duration);
    }

    constexpr inline std::chrono::seconds total_seconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::seconds>(_duration);
    }

    constexpr inline std::chrono::milliseconds total_milliseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_duration);
    }

    constexpr inline std::chrono::microseconds total_microseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
    }

    constexpr inline std::chrono::nanoseconds total_nanoseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration);
    }

    /*!
     * \brief Equality operator.
     * \param other the timedelta to compare against.
     * \return true if the two timedelta objects are the same.
     */
    constexpr inline bool operator==(const timedelta& other) const noexcept
    {
        return _duration == other._duration;
    }

    /*!
     * \brief Inequality operator.
     * \param other the timedelta to compare against.
     * \return true if the two timedelta objects are the different.
     */
    constexpr inline bool operator!=(const timedelta& other) const noexcept
    {
        return !operator==(other);
    }

    constexpr inline tc::sdk::timedelta operator+(const timedelta& other) const noexcept
    {
        return tc::sdk::timedelta{_duration + other._duration};
    }

    constexpr inline tc::sdk::timedelta operator-(const timedelta& other) const noexcept
    {
        return tc::sdk::timedelta{_duration - other._duration};
    }

    constexpr inline tc::sdk::timedelta operator*(int64_t multiplier) const noexcept
    {
        return tc::sdk::timedelta{_duration * multiplier};
    }

    constexpr inline tc::sdk::timedelta operator/(int64_t divider) const noexcept(false)
    {
        return tc::sdk::timedelta{_duration / divider};
    }
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

}

inline std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& ymd)
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

inline std::ostream& operator<<(std::ostream& os, const std::chrono::year& y)
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

inline std::ostream& operator<<(std::ostream& os, const std::chrono::month& m)
{
    if (!m.ok())
        os << static_cast<unsigned>(m) << " is not a valid month";
    else
        os << static_cast<unsigned>(m);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::chrono::day& d)
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
