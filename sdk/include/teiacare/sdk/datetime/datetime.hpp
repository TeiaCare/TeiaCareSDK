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

namespace tc::sdk
{
class Date;
class TimeDelta;

class DateTime
{
    const std::chrono::system_clock::time_point _tp;

public:
    explicit DateTime();

    explicit DateTime(const std::chrono::system_clock::time_point& tp)
        : _tp(tp)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const DateTime& dt)
    {
        return os << date::format("%FT%T", dt._tp);
    }

    template <class... Durations>
    explicit DateTime(const std::chrono::year_month_day& ymd, Durations&&... durations);

    template <class... Durations>
    explicit DateTime(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d, Durations&&... durations);

    // static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
    // today();

    // static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
    // now(const std::string& timezone_name = "");

    // static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
    // utcnow();

    // template <class Rep>
    // static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
    // fromtimestamp(Rep timestamp, const std::string& timezone_name = "");

    // template <class Rep>
    // static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
    // utcfromtimestamp(Rep timestamp);

    // static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
    // strptime(const std::string& date_string, const std::string& format);

    // Date        date()  const;
    // const date::year  year()  const;
    // const date::month month() const;
    // const date::day   day()   const;

    // std::string timestamp() const;

    // std::string ctime() const;
    // std::string isoformat(const std::string& sep="T") const;
    // std::string strftime(const std::string& format) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DateTime
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
template <class Duration>
DateTime<Duration> operator+(const DateTime<Duration>& x, const TimeDelta& y);

template <class Duration>
DateTime<Duration> operator+(const DateTime<Duration>& x, const TimeDelta& y);

template <class Duration>
DateTime<Duration> operator-(const TimeDelta& y, const DateTime<Duration>& x);

// TODO: allow two different Duration template params ?
template <class Duration>
TimeDelta operator-(const DateTime<Duration>& x, const DateTime<Duration>& y);

template <class Duration>
bool operator<(const DateTime<Duration>& x, const DateTime<Duration>& y);

template <class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DateTime<Duration>& date);
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DateTime impl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DateTime::DateTime()
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

template <class... Durations>
DateTime::DateTime(const std::chrono::year_month_day& ymd, Durations&&... durations)
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

template <class... Durations>
DateTime::DateTime(const std::chrono::year& y, const std::chrono::month& m, const std::chrono::day& d, Durations&&... durations)
    : _tp{std::chrono::system_clock::time_point::min()}
{
}

/*
template <class Duration>
inline DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::today()
{
    return date::make_zoned(date::current_zone(), date::floor<Duration>(std::chrono::system_clock::now()));
}

template <class Duration>
inline DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::now(const std::string& timezone_name)
{
    if (timezone_name == "")
    {
        return today();
    }
    return date::make_zoned(timezone_name, date::floor<Duration>(std::chrono::system_clock::now()));
}

template <class Duration>
inline DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::utcnow()
{
    return {date::floor<Duration>(std::chrono::system_clock::now())};
}

template <class Duration>
template <class Rep>
inline DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::fromtimestamp(Rep timestamp, const std::string& timezone_name)
{
    auto nanos = static_cast<unsigned>(1e9 * std::fmod(timestamp, 1)); // might loose precision here
    auto tp = std::chrono::system_clock::from_time_t(timestamp) + std::chrono::nanoseconds(nanos);
    if (timezone_name == "")
    {
        return {date::make_zoned(date::current_zone(), tp)};
    }
    else
    {
        return {date::make_zoned(timezone_name, tp)};
    }
}

template <class Duration>
template <class Rep>
inline DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::utcfromtimestamp(Rep timestamp)
{
    auto nanos = static_cast<unsigned>(1e9 * std::fmod(timestamp, 1)); // might loose precision here
    auto tp = std::chrono::system_clock::from_time_t(timestamp) + std::chrono::nanoseconds(nanos);

    return {tp};
}

template <class Duration>
inline DateTime<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::strptime(const std::string& date_string, const std::string& format)
{
    using namespace std::chrono;

    date::local_seconds tp;
    std::istringstream ss(date_string);
    ss >> date::parse(format, tp);
    auto zt = date::make_zoned(date::current_zone(), tp);
    return {zt};
}

template <class Duration>
inline date::fields<typename std::common_type<Duration, std::chrono::seconds>::type>
DateTime<Duration>::fields_ymd_time() const
{
    using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
    auto tp = zoned_time().get_local_time();
    auto ld = date::floor<date::days>(tp);
    date::fields<CT> fds{date::year_month_day{ld}, date::time_of_day<CT>{tp - ld}};
    return fds;
}

template <class Duration>
inline Date DateTime<Duration>::date() const
{
    return fields_ymd_time().ymd;
}

template <class Duration>
inline const date::year DateTime<Duration>::year() const
{
    return date().year();
}

template <class Duration>
inline const date::month DateTime<Duration>::month() const
{
    return date().month();
}

template <class Duration>
inline const date::day DateTime<Duration>::day() const
{
    return date().day();
}

template <class Duration>
inline const date::time_zone* DateTime<Duration>::time_zone() const
{
    return zt_.get_time_zone();
}

template <class Duration>
inline const std::string& DateTime<Duration>::tzinfo() const
{
    return zt_.get_time_zone()->name();
}

template <class Duration>
inline TimeDelta DateTime<Duration>::utcoffset() const
{
    auto offset = zoned_time().get_info().offset;
    return {std::chrono::seconds{offset}};
}

template <class Duration>
inline std::string DateTime<Duration>::timestamp() const
{
    std::stringstream ss;
    ss << std::fixed << zoned_time().get_sys_time().time_since_epoch().count() / 1000000000.0;
    return ss.str();
}

template <class Duration>
inline std::string DateTime<Duration>::ctime() const
{
    return strftime("%c");
}

template <class Duration>
inline std::string DateTime<Duration>::isoformat(const std::string& sep) const
{
    return strftime("%Y%m%d" + sep + "%H:%M:%S");
}

template <class Duration>
inline std::string DateTime<Duration>::strftime(const std::string& format) const
{
    return date::format(format.c_str(), zt_);
}

template <class Duration>
inline DateTime<Duration> operator+(const DateTime<Duration>& x, const TimeDelta& y)
{
    // TODO make this work for non default Duration
    using namespace std::chrono;
    auto add = x.zoned_time().get_sys_time() + seconds(y.total_seconds()) + microseconds(y.microseconds());
    return {date::make_zoned(x.zoned_time().get_time_zone(), add)};
}

template <class Duration>
inline DateTime<Duration> operator+(const TimeDelta& y, const DateTime<Duration>& x)
{
    return x + y;
}

template <class Duration>
inline DateTime<Duration> operator-(const DateTime<Duration>& x, const TimeDelta& y)
{
    using namespace std::chrono;
    auto diff = x.zoned_time().get_sys_time() - seconds(y.total_seconds()) - microseconds(y.microseconds());
    return {date::make_zoned(x.zoned_time().get_time_zone(), diff)};
}

template <class Duration>
inline TimeDelta operator-(const DateTime<Duration>& x, const DateTime<Duration>& y)
{
    return {x.zoned_time().get_sys_time() - y.zoned_time().get_sys_time()};
}

template <class Duration>
inline bool operator<(const DateTime<Duration>& x, const DateTime<Duration>& y)
{
    return x.zoned_time().get_sys_time() < y.zoned_time().get_sys_time();
}

template <class CharT, class Traits, class Duration>
inline std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DateTime<Duration>& date)
{
    return os << date.zoned_time();
}
*/
}
