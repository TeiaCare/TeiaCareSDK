#pragma once

#include "date/date.h"
#include <ostream>

namespace tc::sdk  
{
class TimeDelta
{
    using DurationT = std::chrono::system_clock::duration;
    const DurationT _duration;

public:
    explicit TimeDelta();

    template<class ... Durations>
    explicit TimeDelta(const Durations&... durations);

    template<class ... Durations>
    explicit TimeDelta(Durations&&... durations);

    constexpr bool is_valid() const;

    std::chrono::hours hours() const 
    {
        return std::chrono::hh_mm_ss<DurationT>(_duration).hours();
    }

    std::chrono::minutes minutes() const 
    {
        return std::chrono::hh_mm_ss<DurationT>(_duration).minutes();
    }

    std::chrono::seconds seconds() const 
    {
        return std::chrono::hh_mm_ss<DurationT>(_duration).seconds();
    }

    std::chrono::milliseconds milliseconds() const 
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::hh_mm_ss<DurationT>(_duration - std::chrono::floor<std::chrono::seconds>(_duration)).subseconds());
    }

    std::chrono::microseconds microseconds() const    
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::hh_mm_ss<DurationT>(_duration - std::chrono::floor<std::chrono::milliseconds>(_duration)).subseconds());
    }

    std::chrono::nanoseconds nanoseconds() const 
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::hh_mm_ss<DurationT>(_duration - std::chrono::floor<std::chrono::microseconds>(_duration)).subseconds());
    }

    template<class Duration = typename TimeDelta::DurationT>
    Duration total_duration() const
    {
        return std::chrono::duration_cast<Duration>(_duration);
    }

    std::chrono::years total_years() const { return total_duration<std::chrono::years>(); }
    std::chrono::months total_months() const { return total_duration<std::chrono::months>(); }
    std::chrono::days total_days() const { return total_duration<std::chrono::days>(); }
    std::chrono::hours total_hours() const { return total_duration<std::chrono::hours>(); }
    std::chrono::minutes total_minutes() const { return total_duration<std::chrono::minutes>(); }
    std::chrono::seconds total_seconds() const { return total_duration<std::chrono::seconds>(); }
    std::chrono::milliseconds total_milliseconds() const { return total_duration<std::chrono::milliseconds>(); }
    std::chrono::microseconds total_microseconds() const { return total_duration<std::chrono::microseconds>(); }
    std::chrono::nanoseconds total_nanoseconds() const { return total_duration<std::chrono::nanoseconds>(); }

    // TODO: move to protected
    const DurationT duration() const;
protected:

public:
    friend bool operator==(const TimeDelta& td1, const TimeDelta& td2);
    friend std::ostream& operator<<(std::ostream& os, const TimeDelta& td);
    friend TimeDelta operator+(const TimeDelta& t1, const TimeDelta& t2);
    friend TimeDelta operator-(const TimeDelta& t1, const TimeDelta& t2);
    friend TimeDelta operator*(const TimeDelta& t, int64_t multiplier);
    friend TimeDelta operator*(int64_t multiplier, const TimeDelta& t);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeDelta impl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TimeDelta::TimeDelta()
    : _duration{ DurationT::min() }
{
}

template<class ... Durations>
TimeDelta::TimeDelta(const Durations& ... durations)
    : _duration{ (durations + ...) }
{
}

template<class ... Durations>
TimeDelta::TimeDelta(Durations&& ... durations)
    : _duration{ (std::forward<Durations&&>(durations) + ...) }
{
}

constexpr bool TimeDelta::is_valid() const
{
    return _duration != DurationT::min();
}

const TimeDelta::DurationT TimeDelta::duration() const
{
    return _duration;
}

std::ostream& operator<<(std::ostream& os, const std::chrono::hh_mm_ss<std::chrono::system_clock::duration>& tod)
{
    if (tod.is_negative())
        os << '-';

    if (tod.hours() < std::chrono::hours{10})
        os << '0';

    os << tod.hours().count() << ':';

    if (tod.minutes() < std::chrono::minutes{10})
        os << '0';

    os << tod.minutes().count() << ':' << tod.seconds() << '.' << tod.subseconds();
    return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// friend operators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool operator==(const TimeDelta& td1, const TimeDelta& td2)
{
    return td1.duration() == td2.duration();
}

std::ostream& operator<<(std::ostream& os, const TimeDelta& t)
{
    // if (!t.is_valid())
    //     os << "invalid TimeDelta";
    return os << std::chrono::hh_mm_ss<TimeDelta::DurationT>(t.duration());
}

TimeDelta operator+(const TimeDelta& t1, const TimeDelta& t2)
{
    return TimeDelta{ t1.duration() + t2.duration() };
}

TimeDelta operator-(const TimeDelta& t1, const TimeDelta& t2)
{
    return TimeDelta{ t1.duration() - t2.duration() };
}

TimeDelta operator*(const TimeDelta& t, int64_t multiplier)
{
    return TimeDelta{ t.duration() * multiplier };
}

TimeDelta operator*(int64_t multiplier, const TimeDelta& t)
{
    return t * multiplier;
}

}
