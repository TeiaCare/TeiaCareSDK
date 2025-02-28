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
/*!
 * \class timedelta
 * \brief Represents a time interval with operations and conversions for different time units.
 */
class timedelta
{
public:
    /*!
     * \brief Default constructor initializing with zero duration.
     */
    constexpr explicit timedelta() noexcept
        : _duration{decltype(_duration)::zero()}
    {
    }

    /*!
     * \brief Constructs a timedelta from a steady clock duration.
     * \param duration A std::chrono::steady_clock::duration to initialize with.
     */
    constexpr explicit timedelta(const std::chrono::steady_clock::duration& duration) noexcept
        : _duration{duration}
    {
    }

    /*!
     * \brief Constructs a timedelta from an rvalue duration.
     * \param duration A rvalue reference to std::chrono::steady_clock::duration to initialize with.
     */
    constexpr explicit timedelta(std::chrono::steady_clock::duration&& duration) noexcept
        : _duration{std::forward<std::chrono::steady_clock::duration&&>(duration)}
    {
    }

    /*!
     * \brief Constructs a timedelta from multiple duration components.
     * \param durations Variadic template for initializing with multiple durations.
     */
    template <class... Durations>
    explicit timedelta(const Durations&... durations)
        : _duration{(durations + ...)}
    {
    }

    /*!
     * \brief Constructs a timedelta from rvalue duration components.
     * \param durations Variadic template for initializing with rvalue duration components.
     */
    template <class... Durations>
    explicit timedelta(Durations&&... durations)
        : _duration{(std::forward<Durations&&>(durations) + ...)}
    {
    }

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::timedelta instance to another one.
     */
    constexpr timedelta(const timedelta&) noexcept = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::timedelta instance to another one.
     */
    constexpr timedelta(timedelta&&) noexcept = default;

    /*!
     * \brief Assignment operator. Assign a tc::sdk::timedelta instance to another one.
     */
    constexpr timedelta& operator=(const timedelta&) noexcept = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::timedelta instance to another one.
     */
    constexpr timedelta& operator=(timedelta&&) noexcept = default;

    /*!
     * \brief Checks if the timedelta represents a null duration.
     * \return True if the duration is zero, otherwise false.
     */
    constexpr inline bool is_null() const noexcept
    {
        return _duration == decltype(_duration)::zero();
    }

    /*!
     * \brief Gets the hour component of the timedelta.
     * \return A std::chrono::hours representation.
     */
    constexpr inline std::chrono::hours hours() const noexcept
    {
        return std::chrono::floor<std::chrono::hours>(_duration);
    }

    /*!
     * \brief Gets the minute component of the timedelta.
     * \return A std::chrono::minutes representation.
     */
    constexpr inline std::chrono::minutes minutes() const noexcept
    {
        return std::chrono::floor<std::chrono::minutes>(_duration) -
               std::chrono::floor<std::chrono::hours>(_duration);
    }

    /*!
     * \brief Gets the second component of the timedelta.
     * \return A std::chrono::seconds representation.
     */
    constexpr inline std::chrono::seconds seconds() const noexcept
    {
        return std::chrono::floor<std::chrono::seconds>(_duration) -
               std::chrono::floor<std::chrono::minutes>(_duration);
    }

    /*!
     * \brief Gets the millisecond component of the timedelta.
     * \return A std::chrono::milliseconds representation.
     */
    constexpr inline std::chrono::milliseconds milliseconds() const noexcept
    {
        return std::chrono::floor<std::chrono::milliseconds>(_duration) -
               std::chrono::floor<std::chrono::seconds>(_duration);
    }

    /*!
     * \brief Gets the microsecond component of the timedelta.
     * \return A std::chrono::microseconds representation.
     */
    constexpr inline std::chrono::microseconds microseconds() const noexcept
    {
        return std::chrono::floor<std::chrono::microseconds>(_duration) -
               std::chrono::floor<std::chrono::milliseconds>(_duration);
    }

    /*!
     * \brief Gets the nanosecond component of the timedelta.
     * \return A std::chrono::nanoseconds representation.
     */
    constexpr inline std::chrono::nanoseconds nanoseconds() const noexcept
    {
        return std::chrono::floor<std::chrono::nanoseconds>(_duration) -
               std::chrono::floor<std::chrono::microseconds>(_duration);
    }

    /*!
     * \brief Gets the total duration in years.
     * \return A std::chrono::years representation.
     */
    constexpr inline std::chrono::years total_years() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::years>(_duration);
    }

    /*!
     * \brief Gets the total duration in months.
     * \return A std::chrono::months representation.
     */
    constexpr inline std::chrono::months total_months() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::months>(_duration);
    }

    /*!
     * \brief Gets the total duration in days.
     * \return A std::chrono::days representation.
     */
    constexpr inline std::chrono::days total_days() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::days>(_duration);
    }

    /*!
     * \brief Gets the total duration in hours.
     * \return A std::chrono::hours representation.
     */
    constexpr inline std::chrono::hours total_hours() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::hours>(_duration);
    }

    /*!
     * \brief Gets the total duration in minutes.
     * \return A std::chrono::minutes representation.
     */
    constexpr inline std::chrono::minutes total_minutes() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::minutes>(_duration);
    }

    /*!
     * \brief Gets the total duration in seconds.
     * \return A std::chrono::seconds representation.
     */
    constexpr inline std::chrono::seconds total_seconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::seconds>(_duration);
    }

    /*!
     * \brief Gets the total duration in milliseconds.
     * \return A std::chrono::milliseconds representation.
     */
    constexpr inline std::chrono::milliseconds total_milliseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(_duration);
    }

    /*!
     * \brief Gets the total duration in microseconds.
     * \return A std::chrono::microseconds representation.
     */
    constexpr inline std::chrono::microseconds total_microseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
    }

    /*!
     * \brief Gets the total duration in nanoseconds.
     * \return A std::chrono::nanoseconds representation.
     */
    constexpr inline std::chrono::nanoseconds total_nanoseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration);
    }

    /*!
     * \brief Spaceship operator.
     * \param other the timedelta to compare against.
     * \return std::strong_ordering comparing two timedelta objects.
     */
    constexpr auto operator<=>(const timedelta& other) const noexcept = default;

    /*!
     * \brief Addition operator.
     * \param other The timedelta to add.
     * \return A new timedelta representing the sum.
     */
    constexpr inline tc::sdk::timedelta operator+(const timedelta& other) const noexcept
    {
        return tc::sdk::timedelta{_duration + other._duration};
    }

    /*!
     * \brief Subtraction operator.
     * \param other The timedelta to subtract.
     * \return A new timedelta representing the difference.
     */
    constexpr inline tc::sdk::timedelta operator-(const timedelta& other) const noexcept
    {
        return tc::sdk::timedelta{_duration - other._duration};
    }

    /*!
     * \brief Multiplication operator.
     * \param multiplier The integer multiplier.
     * \return A new timedelta scaled by the multiplier.
     */
    constexpr inline tc::sdk::timedelta operator*(int64_t multiplier) const noexcept
    {
        return tc::sdk::timedelta{_duration * multiplier};
    }

    /*!
     * \brief Division operator.
     * \param divider The integer divider.
     * \return A new timedelta divided by the divider.
     * \throws std::domain_error if division by zero is attempted.
     */
    constexpr inline tc::sdk::timedelta operator/(int64_t divider) const noexcept(false)
    {
        return tc::sdk::timedelta{_duration / divider};
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param td the timedelta object to stream.
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
    std::chrono::nanoseconds _duration;
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

#if defined(__apple_build_version__)
// Workaround for Apple Clang on MacOS bug: invalid operands to binary expression ('basic_ostream<char, char_traits<char>>' and 'duration'
template <typename Rep, typename Period>
inline std::ostream& operator<<(std::ostream& os, const std::chrono::duration<Rep, Period>& d)
{
    os << d.count();
    return os;
}
#endif
