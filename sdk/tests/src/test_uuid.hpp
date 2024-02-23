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

#include <teiacare/sdk/uuid.hpp>
#include <teiacare/sdk/uuid_generator.hpp>

#include <gtest/gtest.h>

namespace tc::sdk::tests
{
class test_uuid : public ::testing::Test
{
protected:
    explicit test_uuid()
    {
    }

    const tc::sdk::uuid create() const
    {
        auto& generator = tc::sdk::uuid_generator::instance();
        return generator.create();
    }

    const tc::sdk::uuid null_uuid = tc::sdk::uuid();
    const std::string null_uuid_str = "00000000-0000-0000-0000-000000000000";
};

}
