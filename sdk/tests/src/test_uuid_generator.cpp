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
    std::array<tc::sdk::uuid, 1'000> uuids;
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
TEST_F(test_uuid_generator, empty_string)
{
    const char* uuid_str = "";
    check_is_not_valid(uuid_str);
    check_from_string_throws(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, invalid_string)
{
    const char* uuid_str = "12345678901234567890";
    check_is_not_valid(uuid_str);
    check_from_string_throws(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, missing_last_brackets)
{
    const char* uuid_str = "{db8dd9fc-8f93-4fd1-b444-2476b3bc4072";
    check_is_not_valid(uuid_str);
    check_from_string_throws(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, missing_first_brackets)
{
    const char* uuid_str = "db8dd9fc-8f93-4fd1-b444-2476b3bc4072}";
    check_is_not_valid(uuid_str);
    check_from_string_throws(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, invalid_character)
{
    const char* uuid_str = "db8dd9fc-8f93-4fd1-b444-qwertyzxcvbn";
    check_is_not_valid(uuid_str);
    check_from_string_throws(uuid_str);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, null_string)
{
    const char* uuid_str = null_uuid_str.c_str();
    check_is_valid(uuid_str);
    check_from_string_does_not_throw(uuid_str);
    check_reverse_conversion(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, valid_string_with_brakets)
{
    const char* uuid_str = "{db8dd9fc-8f93-4fd1-b444-2476b3bc4072}";
    check_is_valid(uuid_str);
    check_from_string_does_not_throw(uuid_str);

    // In this case the conversion from string to uuid and viceversa does not work:
    // The input string is properly converted to a uuid object, but by design the string representation of a tc::sdk::uuid does not have brackets,
    // so because of this reason the check cannot be performed since the two strings differs by the first and last parenthesis.

    // check_reverse_conversion(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, valid_string_without_brakets)
{
    const char* uuid_str = "db8dd9fc-8f93-4fd1-b444-2476b3bc4072";
    check_is_valid(uuid_str);
    check_from_string_does_not_throw(uuid_str);
    check_reverse_conversion(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, valid_string_all_uppercase)
{
    const char* uuid_str = "DB8DD9FC-8F93-4FD1-B444-2476B3BC4072";
    check_is_valid(uuid_str);
    check_from_string_does_not_throw(uuid_str);
    check_reverse_conversion(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, valid_string_all_lowercase)
{
    const char* uuid_str = "db8dd9fc-8f93-4fd1-b444-2476b3bc4072";
    check_is_valid(uuid_str);
    check_from_string_does_not_throw(uuid_str);
    check_reverse_conversion(uuid_str);
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, valid_string_mixed_case)
{
    const char* uuid_str = "Db8Dd9fC-8f93-4Fd1-B444-2476b3bC4072";
    check_is_valid(uuid_str);
    check_from_string_does_not_throw(uuid_str);
    check_reverse_conversion(uuid_str);
}

}
