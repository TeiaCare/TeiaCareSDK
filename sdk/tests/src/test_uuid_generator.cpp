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

#include "test_uuid_generator.hpp"

#include <algorithm>
#include <array>

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_uuid_generator, instance)
{
    EXPECT_EQ(g, tc::sdk::uuid_generator::instance());
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, create_unique)
{
    std::array<tc::sdk::uuid, 1'000> uuids; // NOLINT
    std::generate(uuids.begin(), uuids.end(), [this] { return g.create(); });

    auto is_unique = [](auto& uuids) -> bool {
        std::sort(uuids.begin(), uuids.end());
        const auto last_unique = std::unique(uuids.begin(), uuids.end());
        return last_unique == uuids.end();
    };

    EXPECT_TRUE(is_unique(uuids));
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, format_and_size_null)
{
    const auto uuid = tc::sdk::uuid();
    check_format_and_size(uuid.to_string());
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, format_and_size_create)
{
    const auto uuid = g.create();
    check_format_and_size(uuid.to_string());
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, format_and_size_from_null_string)
{
    const auto uuid = g.from_string(null_uuid_str);
    check_format_and_size(uuid.to_string());
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, format_and_size_from_valid_string)
{
    const auto uuid = g.from_string("6951298d-38b2-4758-840d-6c362a27c5a8");
    check_format_and_size(uuid.to_string());
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_null)
{
    {
        const auto uuid = g.from_string(null_uuid_str);
        EXPECT_EQ(uuid, tc::sdk::uuid());
    }

    {
        const auto uuid = g.from_string(null_uuid_str.c_str());
        EXPECT_EQ(uuid, tc::sdk::uuid());
    }
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_empty)
{
    EXPECT_THROW(g.from_string(std::string("")), std::runtime_error); // NOLINT
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_invalid)
{
    EXPECT_THROW(g.from_string(std::string("12345678901234567890")), std::runtime_error); // NOLINT
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_missing_last_brackets)
{
    EXPECT_THROW(g.from_string(std::string("{db8dd9fc-8f93-4fd1-b444-2476b3bc4072")), std::runtime_error); // NOLINT
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_missing_first_brackets)
{
    EXPECT_THROW(g.from_string(std::string("db8dd9fc-8f93-4fd1-b444-2476b3bc4072}")), std::runtime_error); // NOLINT
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_invalid_character)
{
    const auto uuid_str = "db8dd9fc-8f93-4fd1-b444-qwertyzxcvbn";
    EXPECT_THROW(g.from_string(std::string(uuid_str)), std::runtime_error); // NOLINT
    EXPECT_THROW(g.from_string(uuid_str), std::runtime_error); // NOLINT
    EXPECT_FALSE(g.is_valid(std::string(uuid_str))); // NOLINT
    EXPECT_FALSE(g.is_valid(uuid_str)); // NOLINT
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_valid_brackets)
{
    const auto valid_uuid = g.from_string("{db8dd9fc-8f93-4fd1-b444-2476b3bc4072}");
    EXPECT_EQ(valid_uuid, g.from_string("db8dd9fc-8f93-4fd1-b444-2476b3bc4072"));
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_valid_uuid)
{
    const char* uuid_str = "44b6f253-1414-4444-b10e-d3d92c15f6f2";

    {
        const auto uuid = g.from_string(std::string(uuid_str));
        std::cout << uuid << "\n";
        std::cout << uuid.to_string() << "\n";
        std::cout << uuid_str << "\n";
        EXPECT_EQ(uuid.to_string(), std::string(uuid_str));
    }

    {
        const auto uuid = g.from_string(uuid_str);
        EXPECT_EQ(uuid.to_string(), uuid_str);
    }
}

}
