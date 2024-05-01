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
class date
{
public:
    explicit constexpr date() noexcept
        : _ymd{}
    {
    }

    explicit constexpr date(const std::chrono::year_month_day& ymd) noexcept
        : _ymd{ymd}
    {
    }

    explicit constexpr date(std::chrono::year_month_day&& ymd) noexcept
        : _ymd{std::move(ymd)}
    {
    }
    explicit constexpr date(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d) noexcept
        : _ymd{y, m, d}
    {
    }

    explicit constexpr date(std::chrono::year&& y, std::chrono::month&& m, std::chrono::day&& d) noexcept
        : _ymd{std::move(y), std::move(m), std::move(d)}
    {
    }

    explicit constexpr date(const tc::sdk::sys_time_point& timepoint) noexcept
        : _ymd{std::chrono::floor<std::chrono::days>(timepoint)}
    {
    }
    // explicit constexpr date(const std::chrono::system_clock::time_point& timepoint) noexcept
    //     : _ymd{std::chrono::floor<std::chrono::days>(timepoint)}
    // {
    // }

    explicit constexpr date(const std::chrono::nanoseconds& duration) noexcept
        : date(tc::sdk::sys_time_point(duration))
    {
    }
    // explicit constexpr date(const std::chrono::system_clock::duration& duration) noexcept
    //     : date(std::chrono::time_point<std::chrono::system_clock>(duration))
    // {
    // }

    constexpr bool is_valid() const noexcept
    {
        return _ymd.ok();
    }

    constexpr inline std::chrono::year year() const noexcept
    {
        return _ymd.year();
    }

    constexpr inline std::chrono::month month() const noexcept
    {
        return _ymd.month();
    }

    constexpr inline std::chrono::day day() const noexcept
    {
        return _ymd.day();
    }

    constexpr inline std::chrono::weekday weekday() const noexcept
    {
        return std::chrono::weekday(_ymd);
    }

    constexpr unsigned iso_weekday() const
    {
        return weekday().iso_encoding();
    }

    constexpr inline std::chrono::sys_days to_duration() const noexcept
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

    constexpr tc::sdk::date operator+(const timedelta& delta) const noexcept
    {
        return tc::sdk::date{std::chrono::sys_days(_ymd) + delta.total_nanoseconds()};
    }

    constexpr tc::sdk::date operator-(const timedelta& delta) const noexcept
    {
        return tc::sdk::date{std::chrono::sys_days(_ymd) - delta.total_nanoseconds()};
    }

    constexpr inline bool operator<(const date& other) const noexcept
    {
        return _ymd < other._ymd;
    }

    constexpr inline bool operator>(const date& other) const noexcept
    {
        return _ymd > other._ymd;
    }

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

    static tc::sdk::date from_string(const std::string& str, const std::string& format = "%T") noexcept(false);

    static tc::sdk::date today() noexcept;

private:
    const std::chrono::year_month_day _ymd;
};

// std::string date::to_string(const char* format) const
// {
//     return ::date::format(format, std::chrono::sys_days(_ymd));
// }

// tc::sdk::date tc::sdk::date::from_string(const std::string& str, const std::string& format)
// {
//     std::chrono::sys_days parsed_date;
//     std::stringstream ss{str};
//     ss >> ::date::parse(format, parsed_date);
//     if (ss.fail())
//         throw std::runtime_error("Failed to parse " + str);

//     std::chrono::system_clock::time_point tp = parsed_date;
//     return tc::sdk::date(tp);
// }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& ymd)
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

std::ostream& operator<<(std::ostream& os, const std::chrono::year& y)
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

// std::ostream& operator<<(std::ostream& os, const std::chrono::month& m)
// {
//     if (!m.ok())
//         os << static_cast<unsigned>(m) << " is not a valid month";
//     else
//         os << date::format("%b", date::month(static_cast<unsigned>(m)));
//     return os;
// }

std::ostream& operator<<(std::ostream& os, const std::chrono::day& d)
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
*/
