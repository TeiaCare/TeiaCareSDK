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
 * @example example_geometry_rectangle.cpp
 * @brief Simple example of tc::sdk::rectangle
 */

#include <teiacare/sdk/geometry/rectangle.hpp>

#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::set_level(spdlog::level::trace);

    int width = 4;
    int height = 4;
    auto position = tc::sdk::point<int>(1, 1);
    auto r1 = tc::sdk::rectangle<int>(position, width, height);
    spdlog::info("{}", r1.to_string());
    spdlog::info("top_left: {}", r1.top_left().to_string());
    spdlog::info("top_right: {}", r1.top_right().to_string());
    spdlog::info("bottom_left: {}", r1.bottom_left().to_string());
    spdlog::info("bottom_right: {}", r1.bottom_right().to_string());
    spdlog::info("size: {}", r1.size().to_string());

    auto new_position = tc::sdk::point<int>(2, 2);
    r1.set_position(new_position);
    spdlog::info("new_position: {} = {}", new_position.to_string(), r1.to_string());

    r1.translate(1, 2);
    spdlog::info("translate: {},{} = {}", 1, 2, r1.to_string());

    return 0;
}
