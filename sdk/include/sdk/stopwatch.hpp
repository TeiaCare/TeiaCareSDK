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

#include <sdk/clock.hpp>
#include <sdk/non_copyable.hpp>
#include <sdk/non_moveable.hpp>

namespace tc::sdk
{
/*!
 * \class stopwatch
 * \brief Stopwatch object that measures the amount of elapsed time from a given time point.
 */
class stopwatch : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor.
     * 
     * Creates a tc::sdk::stopwatch instance. 
     */
    explicit stopwatch() = default;

    /*!
     * \brief Destructor.
     * 
     * Destructs this.
     */
    ~stopwatch() noexcept = default;

    /*!
     * \brief Get the start time.
     * \return start_time time_point
     */
    [[nodiscard]] tc::sdk::time_point start_time() const noexcept
    {
        return _start_time;
    }

    /*!
     * \brief Get the elapsed time.
     * \return elapsed time_duration
     */
    [[nodiscard]] tc::sdk::time_duration elapsed() const noexcept
    {
        return tc::sdk::clock::now() - _start_time;
    }

    /*!
     * \brief Get the elapsed time.
     * \return elapsed time_duration
     * 
     * Override to require the elapsed time with a gived duration type (i.e. std::chrono::milliseconds)
     */
    template<class DurationT>
    [[nodiscard]] DurationT elapsed() const noexcept
    {
        static_assert(tc::sdk::is_duration<DurationT>::value, 
            "\nDurationT must be a duration type");

        return std::chrono::duration_cast<DurationT>(elapsed());
    }

    /*!
     * \brief Reset the start time.
     */
    void reset() noexcept
    {
        _start_time = tc::sdk::clock::now();
    }

private:
    tc::sdk::time_point _start_time = tc::sdk::clock::now();
};

}
