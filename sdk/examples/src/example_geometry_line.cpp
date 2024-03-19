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
 * @example example_geometry_line.cpp
 * @brief Simple example of tc::sdk::line
 */

#include <teiacare/sdk/geometry/line.hpp>

#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::set_level(spdlog::level::trace);

    tc::sdk::line<int> l0;
    auto l1 = tc::sdk::line(tc::sdk::point<int>(1, 1), tc::sdk::point<int>(4, 4));
    auto l2 = tc::sdk::line(tc::sdk::point<int>(1, 1), tc::sdk::point<int>(4, 1));

    spdlog::info("{} is_null : {}", l0.str(), l0.is_null());
    spdlog::info("{} is_null : {}", l1.str(), l1.is_null());
    spdlog::info("{} is_vertical : {}", l2.str(), l2.is_vertical());
    spdlog::info("{} is_horizontal : {}", l2.str(), l2.is_horizontal());

    return 0;
}
