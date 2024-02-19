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

#include "date/date.h"
#include <ostream>
#include <teiacare/sdk/datetime/timedelta.hpp>

namespace tc::sdk 
{
class Time 
{
    using DurationT = std::chrono::system_clock::duration;
    const std::chrono::hh_mm_ss<DurationT> _hh_mm_ss;

public:
    explicit Time();

    template<class ... Durations>
    explicit Time(const Durations&... durations);

    template<class ... Durations>
    explicit Time(Durations&&... durations);

    constexpr bool is_valid() const;

    std::chrono::hours hours() const;
    std::chrono::minutes minutes() const;
    std::chrono::seconds seconds() const;
    std::chrono::milliseconds milliseconds() const;
    std::chrono::microseconds microseconds() const;
    std::chrono::nanoseconds nanoseconds() const;

    template<class Duration = typename Time::DurationT>
    std::string to_string(const char* fmt = "%T") const; // equivalent to "%H:%M:%S"
    std::string iso_string() const;
    
    template<class Duration = typename Time::DurationT>
    static Time now();

    // TODO!
    static Time from_string(const char* str);
    static Time from_string(const std::string& str);

protected:
    const std::chrono::hh_mm_ss<DurationT>& hh_mm_ss() const;

public:
    friend bool operator==(const Time& t1, const Time& t2);
    friend std::ostream& operator<<(std::ostream& os, const Time& time);
    friend Time operator+(const Time& time, const TimeDelta& time_delta);
    friend Time operator+(const TimeDelta& time_delta, const Time& time);
    friend Time operator-(const Time& time, const TimeDelta& time_delta);    
    friend TimeDelta operator-(const Time& t1, const Time& t2);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time impl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Time::Time()
    : _hh_mm_ss{ DurationT::min() }
{
}

template<class ... Durations>
Time::Time(const Durations& ... durations)
    : _hh_mm_ss{ (durations + ...) % std::chrono::hours(24) }
{
}

template<class ... Durations>
Time::Time(Durations&& ... durations)
    : _hh_mm_ss{ (std::forward<Durations&&>(durations) + ...) % std::chrono::hours(24) }
{
}

constexpr bool Time::is_valid() const
{
    return _hh_mm_ss.to_duration() != DurationT::min();
}

const std::chrono::hh_mm_ss<Time::DurationT>& Time::hh_mm_ss() const
{
    return _hh_mm_ss;
}

std::chrono::hours Time::hours() const
{
    return _hh_mm_ss.hours();
}

std::chrono::minutes Time::minutes() const
{
    return _hh_mm_ss.minutes();
}

std::chrono::seconds Time::seconds() const
{
    return _hh_mm_ss.seconds();
}

std::chrono::milliseconds Time::milliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(_hh_mm_ss.subseconds());
}

std::chrono::microseconds Time::microseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(_hh_mm_ss.subseconds());
}

std::chrono::nanoseconds Time::nanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(_hh_mm_ss.subseconds());
}

std::string Time::iso_string() const
{
    return to_string<std::chrono::seconds>("%H:%M:%S");
}

template<class Duration>
std::string Time::to_string(const char* fmt) const
{
    return date::format(fmt, std::chrono::floor<Duration>(_hh_mm_ss.to_duration()));
}

template<class Duration>
Time Time::now() 
{
    auto now = std::chrono::system_clock::now();
    return Time{ std::chrono::floor<Duration>(now) - std::chrono::floor<std::chrono::days>(now) };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// friend operators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool operator==(const Time& t1, const Time& t2)
{
    return t1.hh_mm_ss().to_duration() == t2.hh_mm_ss().to_duration();
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
    return os << time.iso_string();
}

Time operator+(const Time& time, const TimeDelta& time_delta)
{
    return Time{ time.hh_mm_ss().to_duration() + time_delta.duration() };
}

Time operator+(const TimeDelta& time_delta, const Time& time)
{
    return time + time_delta;
}

Time operator-(const Time& time, const TimeDelta& time_delta)
{
    return Time{ time.hh_mm_ss().to_duration() - time_delta.duration() };
}

TimeDelta operator-(const Time& t1, const Time& t2)
{
    return TimeDelta{ t1.hh_mm_ss().to_duration() - t2.hh_mm_ss().to_duration() };
}

}
