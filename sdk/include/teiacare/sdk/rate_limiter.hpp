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
#include <teiacare/sdk/non_copyable.hpp>
#include <teiacare/sdk/non_moveable.hpp>

#include <chrono>
#include <type_traits>

namespace tc::sdk
{
/*! 
 * \class rate_limiter
 * \brief Rate Limiter class to sync a loop with a fixed frame rate.
 */
class rate_limiter : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor
     * \param rate set the required rate value
     * 
     * Creates a tc::sdk::rate_limiter instance with the given rate. 
     */
    explicit rate_limiter(size_t rate);

    /*!
     * \brief Destructor
     * 
     * Destructs this.
     */
    ~rate_limiter() = default;

    /*!
     * \brief Sync the current thread to match the required rate.
     *
     * This method must be called continuously in a loop in order to sync it.
     */
    void sync();

private:
    using duration_rep = double;
    const std::chrono::duration<duration_rep> time_between_frames; // 1/rate
    std::chrono::time_point<tc::sdk::clock, std::remove_const_t<decltype(time_between_frames)>> tp;
};
}
