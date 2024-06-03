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

/*! \typedef sys_time_point alias for std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>.
 */
using sys_time_point = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

// Clock traits
template <typename T>
struct is_duration : std::false_type
{
};

template <typename T>
struct is_duration<std::chrono::duration<tc::sdk::clock::rep, T>> : std::true_type
{
};

template <typename T>
struct is_time_point : std::false_type
{
};

template <typename T>
struct is_time_point<std::chrono::time_point<tc::sdk::clock, T>> : std::true_type
{
};

template <typename T>
struct is_std_chrono
{
    static constexpr bool value = is_duration<T>::value || is_time_point<T>::value;
};

}
