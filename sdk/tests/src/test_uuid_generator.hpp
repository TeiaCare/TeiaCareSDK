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

#include <teiacare/sdk/uuid_generator.hpp>

#include <gtest/gtest.h>

namespace tc::sdk::tests
{
class test_uuid_generator : public ::testing::Test
{
protected:
    explicit test_uuid_generator()
        : g{tc::sdk::uuid_generator::instance()}
    {
    }

    ~test_uuid_generator() override
    {
    }

    void check_format_and_size(const std::string& uuid_str)
    {
        EXPECT_EQ(uuid_str.size(), 36);

        // dashes chars
        {
            EXPECT_EQ('-', uuid_str[8]);
            EXPECT_EQ('-', uuid_str[13]);
            EXPECT_EQ('-', uuid_str[18]);
            EXPECT_EQ('-', uuid_str[23]);
        }

        // alphanumeric chars
        {
            // TODO: perform explicit check to verify that each i-th items of the string
            // satisfies the alphanumeric regex [0-9][a-z][A-Z]

            // 0-7
            for (int index = 0; index <= 7; ++index)
                EXPECT_NE('-', uuid_str[index]);

            // 9-12
            for (int index = 9; index <= 12; ++index)
                EXPECT_NE('-', uuid_str[index]);

            // 14-17
            for (int index = 14; index <= 17; ++index)
                EXPECT_NE('-', uuid_str[index]);

            // 19-22
            for (int index = 19; index <= 22; ++index)
                EXPECT_NE('-', uuid_str[index]);

            // 23-35
            for (int index = 24; index <= 35; ++index)
                EXPECT_NE('-', uuid_str[index]);
        }
    }

    tc::sdk::uuid_generator& g;
    const std::string null_uuid_str = "00000000-0000-0000-0000-000000000000";
};

}

namespace tc::sdk
{
bool operator==(const tc::sdk::uuid_generator& g1, const tc::sdk::uuid_generator& g2)
{
    return &g1 == &g2;
}
}
