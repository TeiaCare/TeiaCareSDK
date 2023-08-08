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
template <typename T>
struct is_duration : std::false_type {};

template <typename T>
struct is_duration<std::chrono::duration<tc::sdk::clock::rep, T>> : std::true_type {};

template <typename T>
struct is_time_point : std::false_type {};

template <typename T>
struct is_time_point<std::chrono::time_point<tc::sdk::clock, T>> : std::true_type {};

template <typename T>
struct is_std_chrono { static constexpr bool value = is_duration<T>::value || is_time_point<T>::value; };

}