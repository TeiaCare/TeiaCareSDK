#pragma once

#include <chrono>

namespace tc::sdk
{
/*! \typedef clock alias for std::chrono::steady_clock.
 */
using clock = std::chrono::steady_clock;

/*! \typedef time_point alias for std::chrono::steady_clock::time_point.
 */
using time_point = std::chrono::steady_clock::time_point;

/*! \typedef time_duration alias for std::chrono::steady_clock::duration.
 */
using time_duration = std::chrono::steady_clock::duration;

// Clock traits

// template<class>
// struct is_duration : std::false_type {};

// template<class Rep, class Period>
// struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};

}