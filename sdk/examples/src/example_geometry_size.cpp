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
 * @example example_geometry_size.cpp
 * @brief Simple example of tc::sdk::size
 */

#include <teiacare/sdk/geometry/size.hpp>

#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::set_level(spdlog::level::trace);

    tc::sdk::size<unsigned> s0;
    auto s1 = tc::sdk::size<unsigned>(640, 480);

    spdlog::info("{} is_null : {}", s0.str(), s0.is_null());
    spdlog::info("{} is_null : {}", s1.str(), s1.is_null());
    spdlog::info("{} width : {}", s1.str(), s1.width());
    spdlog::info("{} height : {}", s1.str(), s1.height());

    return 0;
}
