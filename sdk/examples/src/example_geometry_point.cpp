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
 * @example example_geometry_point.cpp
 * @brief Simple example of tc::sdk::point
 */

#include <teiacare/sdk/geometry/point.hpp>

#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::set_level(spdlog::level::trace);

    tc::sdk::point<int> p0;
    auto p1 = tc::sdk::point<int>(2, 3);
    spdlog::info("{} is_origin : {}", p0.str(), p0.is_origin());
    spdlog::info("{} is_origin : {}", p1.str(), p1.is_origin());

    auto prev_p0 = p0;
    p0.add_delta(2, 4);
    spdlog::info("{} increment(2, 4) = {}", prev_p0.str(), p0.str());
    
    tc::sdk::point<float> p2;
    auto p3 = tc::sdk::point<float>(.2f, .3f);
    spdlog::info("{} is_origin : {}", p2.str(), p2.is_origin());
    spdlog::info("{} is_origin : {}", p3.str(), p3.is_origin());
    
    auto prev_p3 = p3;
    p3.add_delta(.2f, .4f);
    spdlog::info("{} increment(.2f, .4f) = {}", prev_p3.str(), p3.str());

    return 0;
}
