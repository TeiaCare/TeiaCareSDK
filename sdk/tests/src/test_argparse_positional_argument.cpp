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

#include "test_argparse_positional_argument.hpp"

#include "test_argparse_argument_base.hpp"

namespace tc::sdk::tests
{
TEST_F(test_argparse_positional_argument, default_properties)
{
    {
        std::string value = "default_value";
        tc::sdk::positional_argument<std::string> arg("long_name", value);

        EXPECT_FALSE(arg.is_parsed());
        EXPECT_TRUE(arg.is_required());
        EXPECT_EQ(arg.get_name_long(), "long_name");
        EXPECT_EQ(arg.get_name_short(), "");
        EXPECT_EQ(arg.get_description(), "");
        EXPECT_EQ(arg.get_env(), "");
        EXPECT_EQ(value, "");
    }

    {
        std::string value = "default_value";
        tc::sdk::positional_argument<std::string> arg("long_name", value, "Argument Description");

        EXPECT_FALSE(arg.is_parsed());
        EXPECT_TRUE(arg.is_required());
        EXPECT_EQ(arg.get_name_long(), "long_name");
        EXPECT_EQ(arg.get_name_short(), "");
        EXPECT_EQ(arg.get_description(), "Argument Description");
        EXPECT_EQ(arg.get_env(), "");
        EXPECT_EQ(value, "");
    }
}

TEST_F(test_argparse_positional_argument, parse_error)
{
    using Type = int;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_THROW(arg.parse("a"), std::runtime_error);
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_EQ(value, 0);
}

TEST_F(test_argparse_positional_argument, parse_int)
{
    using Type = int;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("12345"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_EQ(value, 12345);
}

TEST_F(test_argparse_positional_argument, parse_float)
{
    using Type = float;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("1.2345"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_FLOAT_EQ(value, 1.2345f);
}

TEST_F(test_argparse_positional_argument, parse_double)
{
    using Type = double;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("1.2345"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_DOUBLE_EQ(value, 1.2345);
}

TEST_F(test_argparse_positional_argument, parse_string)
{
    using Type = std::string;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("dummy_string"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_EQ(value, std::string("dummy_string"));
}

TEST_F(test_argparse_positional_argument, parse_const_char_ptr)
{
    using Type = const char*;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("dummy_string"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_EQ(std::string(value), std::string("dummy_string"));
}

TEST_F(test_argparse_positional_argument, parse_vector_int)
{
    using Type = std::vector<int>;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("1,2,3,4,5"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_EQ(value, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST_F(test_argparse_positional_argument, parse_vector_string)
{
    using Type = std::vector<std::string>;
    Type value;
    tc::sdk::positional_argument<Type> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse("1,2,3,4,5"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_EQ(value, (std::vector<std::string>{"1", "2", "3", "4", "5"}));
}

TEST_F(test_argparse_positional_argument, parse_empty_string)
{
    int value = 1;
    tc::sdk::positional_argument<int> arg("long_name", value);

    EXPECT_THROW(arg.parse(""), std::runtime_error);
    EXPECT_EQ(value, 0);
    EXPECT_FALSE(arg.is_parsed());
}

TEST_F(test_argparse_positional_argument, parse_twice)
{
    int value = 1;
    tc::sdk::positional_argument<int> arg("long_name", value);

    EXPECT_EQ(value, 0);

    EXPECT_NO_THROW(arg.parse("3"));
    EXPECT_EQ(value, 3);

    EXPECT_NO_THROW(arg.parse("4"));
    EXPECT_EQ(value, 4);
}

TEST_F(test_argparse_positional_argument, parse_from_env_variable)
{
    int value = 1;
    tc::sdk::positional_argument<int> arg("long_name", value);

    EXPECT_NO_THROW(arg.parse_from_env());
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_EQ(value, 0);
}

}
