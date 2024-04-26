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
class TimeDelta
{
public:
    constexpr explicit TimeDelta() noexcept;
    constexpr explicit TimeDelta(const std::chrono::steady_clock::duration& duration) noexcept;
    constexpr explicit TimeDelta(std::chrono::steady_clock::duration&& duration) noexcept;

    template <class... Durations>
    explicit TimeDelta(const Durations&... durations);

    template <class... Durations>
    explicit TimeDelta(Durations&&... durations);

    constexpr inline bool is_null() const noexcept;

    constexpr inline std::chrono::hours hours() const
    {
        return std::chrono::floor<std::chrono::hours>(_duration);
    }

    constexpr inline std::chrono::minutes minutes() const
    {
        return std::chrono::floor<std::chrono::minutes>(_duration) -
               std::chrono::floor<std::chrono::hours>(_duration);
    }

    constexpr inline std::chrono::seconds seconds() const
    {
        return std::chrono::floor<std::chrono::seconds>(_duration) -
               std::chrono::floor<std::chrono::minutes>(_duration);
    }

    constexpr inline std::chrono::milliseconds milliseconds() const
    {
        return std::chrono::floor<std::chrono::milliseconds>(_duration) -
               std::chrono::floor<std::chrono::seconds>(_duration);
    }

    constexpr inline std::chrono::microseconds microseconds() const
    {
        return std::chrono::floor<std::chrono::microseconds>(_duration) -
               std::chrono::floor<std::chrono::milliseconds>(_duration);
    }

    constexpr inline std::chrono::nanoseconds nanoseconds() const
    {
        return std::chrono::floor<std::chrono::nanoseconds>(_duration) -
               std::chrono::floor<std::chrono::microseconds>(_duration);
    }

    constexpr inline std::chrono::years total_years() const
    {
        return std::chrono::duration_cast<std::chrono::years>(_duration);
    }

    constexpr inline std::chrono::months total_months() const
    {
        return std::chrono::duration_cast<std::chrono::months>(_duration);
    }

    constexpr inline std::chrono::days total_days() const
    {
        return std::chrono::duration_cast<std::chrono::days>(_duration);
    }

    constexpr inline std::chrono::hours total_hours() const
    {
        return std::chrono::duration_cast<std::chrono::hours>(_duration);
    }

    constexpr inline std::chrono::minutes total_minutes() const
    {
        return std::chrono::duration_cast<std::chrono::minutes>(_duration);
    }

    constexpr inline std::chrono::seconds total_seconds() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(_duration);
    }

    constexpr inline std::chrono::milliseconds total_milliseconds() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_duration);
    }

    constexpr inline std::chrono::microseconds total_microseconds() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
    }

    constexpr inline std::chrono::nanoseconds total_nanoseconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration);
    }

    /*!
     * \brief Equality operator.
     * \param other the TimeDelta to compare against.
     * \return true if the two TimeDelta objects are the same.
     */
    constexpr inline bool operator==(const TimeDelta& other) const noexcept
    {
        return _duration == other._duration;
    }

    /*!
     * \brief Inequality operator.
     * \param other the TimeDelta to compare against.
     * \return true if the two TimeDelta objects are the different.
     */
    constexpr inline bool operator!=(const TimeDelta& other) const noexcept
    {
        return !operator==(other);
    }

    constexpr inline TimeDelta operator+(const TimeDelta& other) const noexcept
    {
        return TimeDelta{_duration + other._duration};
    }

    constexpr inline TimeDelta operator-(const TimeDelta& other) const noexcept
    {
        return TimeDelta{_duration - other._duration};
    }

    constexpr inline TimeDelta operator*(int64_t multiplier) const noexcept
    {
        return TimeDelta{_duration * multiplier};
    }

    constexpr inline TimeDelta operator/(int64_t divider) const noexcept(false)
    {
        return TimeDelta{_duration / divider};
    }

    /*!
     * \brief Get the TimeDelta string representation.
     * \tparam DurationType specify the DateTime precision, by default is std::chrono::milliseconds.
     * \return String representation of the current TimeDelta.
     */
    template <class DurationType = std::chrono::milliseconds>
    std::string to_string() const
    {
        constexpr const char zero = '0';
        const auto hms = std::chrono::hh_mm_ss(std::chrono::duration_cast<DurationType>(_duration));
        std::stringstream os{};

        if (hms.is_negative())
            os << '-';

        os.width(2);
        os.fill(zero);
        os << hms.hours().count() << ':';
       
        os.width(2);
        os.fill(zero);
        os << hms.minutes().count() << ':';
       
        os.width(2);
        os.fill(zero);
        os << hms.seconds().count();

        if constexpr (std::is_same_v<DurationType, std::chrono::seconds>)
            return os.str();

        os << '.';

        if constexpr (std::is_same_v<DurationType, std::chrono::milliseconds>)
            os.width(3);

        if constexpr (std::is_same_v<DurationType, std::chrono::microseconds>)
            os.width(6);

        if constexpr (std::is_same_v<DurationType, std::chrono::nanoseconds>)
            os.width(9);

        DurationType sub = hms.subseconds();
        os.fill(zero);
        os << sub.count();
        return os.str();
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param dt the TimeDelta object to stream.
     * \return reference to the output stream operator, with the TimeDelta string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const TimeDelta& td)
    {
        return stream << td.to_string();
    }

private:
    const std::chrono::nanoseconds _duration;
};

constexpr TimeDelta::TimeDelta() noexcept
    : _duration{decltype(_duration)::zero()}
{
}

constexpr TimeDelta::TimeDelta(const std::chrono::steady_clock::duration& duration) noexcept
    : _duration{duration}
{
}

constexpr TimeDelta::TimeDelta(std::chrono::steady_clock::duration&& duration) noexcept
    : _duration{std::forward<std::chrono::steady_clock::duration&&>(duration)}
{
}

template <class... Durations>
TimeDelta::TimeDelta(const Durations&... durations)
    : _duration{(durations + ...)}
{
}

template <class... Durations>
TimeDelta::TimeDelta(Durations&&... durations)
    : _duration{(std::forward<Durations&&>(durations) + ...)}
{
}

constexpr inline bool TimeDelta::is_null() const noexcept
{
    return _duration == decltype(_duration)::zero();
}

}
