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
 * @example example_uuid.cpp
 * @brief Simple example of tc::sdk::uuid and tc::sdk::uuid_generator
 */

#include <teiacare/sdk/uuid.hpp>
#include <teiacare/sdk/uuid_generator.hpp>

#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

int main()
{
    auto uuid_generator = tc::sdk::uuid_generator::instance();

    {
        spdlog::info("\n\ntc::sdk::uuid::uuid() [null/empty UUID]");
        auto null = tc::sdk::uuid();
        spdlog::info(null.to_string());
    }
    spdlog::info("\n\ntc::sdk::uuid_generator::from_string()");
    {
        auto uuid = uuid_generator.from_string("a142011e-58fa-4415-a0ea-e3ace6597fc8");
        spdlog::info(uuid.str());
    }

    {
        auto uuid = uuid_generator.from_string("ef25b786-d431-4612-8fb9-42afd24d22c6");
        spdlog::info(uuid.to_string());
    }

    spdlog::info("\n\ntc::sdk::uuid_generator::create()");
    for (int i = 0; i < 10; ++i)
    {
        auto uuid = uuid_generator.create();
        spdlog::info(uuid.to_string());
    }

    return 0;
}
