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

/**
 * @example example_geometry_range.cpp
 * @brief Simple example of tc::sdk::range
 */

#include <teiacare/sdk/geometry/range.hpp>

#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::set_level(spdlog::level::trace);

    auto r1 = tc::sdk::range(1, 4);
    auto r2 = tc::sdk::range(2, 3);
    auto r3 = tc::sdk::range(2, 5);
    auto r4 = tc::sdk::range(5, 8);

    spdlog::info("{} contains {} : {}", r1.to_string(), r2.to_string(), r1.contains(r2));
    spdlog::info("{} contains {} : {}", r1.to_string(), r3.to_string(), r1.contains(r3));
    spdlog::info("{} intersects {} : {}", r1.to_string(), r2.to_string(), r1.intersects(r2));
    spdlog::info("{} intersects {} : {}", r1.to_string(), r4.to_string(), r1.intersects(r4));
    spdlog::info("{} clamped into {} : {}", 9, r1.to_string(), r1.clamp(9));

    return 0;
}
