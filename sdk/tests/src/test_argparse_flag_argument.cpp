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

#include "test_argparse_flag_argument.hpp"

#include "test_argparse_argument_base.hpp"

namespace tc::sdk::tests
{
TEST_F(test_argparse_flag_argument, default_properties)
{
    {
        bool value = false;
        tc::sdk::flag_argument arg("flag_a", value);
        EXPECT_FALSE(arg.is_parsed());
        EXPECT_FALSE(arg.is_required());
        EXPECT_EQ(arg.get_name_long(), "flag_a");
        EXPECT_EQ(arg.get_name_short(), "");
        EXPECT_EQ(arg.get_description(), "");
        EXPECT_EQ(arg.get_env(), "");
        EXPECT_FALSE(value);
    }

    {
        bool value = false;
        tc::sdk::flag_argument arg("flag_a", 'a', value);
        EXPECT_FALSE(arg.is_parsed());
        EXPECT_FALSE(arg.is_required());
        EXPECT_EQ(arg.get_name_long(), "flag_a");
        EXPECT_EQ(arg.get_name_short(), "a");
        EXPECT_EQ(arg.get_description(), "");
        EXPECT_EQ(arg.get_env(), "");
        EXPECT_FALSE(value);
    }

    {
        bool value = false;
        tc::sdk::flag_argument arg("flag_a", 'a', value, "Argument Description");
        EXPECT_FALSE(arg.is_parsed());
        EXPECT_FALSE(arg.is_required());
        EXPECT_EQ(arg.get_name_long(), "flag_a");
        EXPECT_EQ(arg.get_name_short(), "a");
        EXPECT_EQ(arg.get_description(), "Argument Description");
        EXPECT_EQ(arg.get_env(), "");
        EXPECT_FALSE(value);
    }

    {
        bool value = false;
        tc::sdk::flag_argument arg("flag_a", 'a', value, "Argument Description", ENV_VAR);
        EXPECT_FALSE(arg.is_parsed());
        EXPECT_FALSE(arg.is_required());
        EXPECT_EQ(arg.get_name_long(), "flag_a");
        EXPECT_EQ(arg.get_name_short(), "a");
        EXPECT_EQ(arg.get_description(), "Argument Description");
        EXPECT_EQ(arg.get_env(), ENV_VAR);
        EXPECT_FALSE(value);
    }
}

TEST_F(test_argparse_flag_argument, parse)
{
    bool value = false;
    tc::sdk::flag_argument arg("flag_a", 'a', value);

    EXPECT_NO_THROW(arg.parse(""));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_TRUE(value);
}

TEST_F(test_argparse_flag_argument, parse_long_only)
{
    bool value = false;
    tc::sdk::flag_argument arg("flag_a", value);

    EXPECT_NO_THROW(arg.parse(""));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_TRUE(value);
}

TEST_F(test_argparse_flag_argument, parse_twice)
{
    bool value = false;
    tc::sdk::flag_argument arg("flag_a", 'a', value);

    EXPECT_NO_THROW(arg.parse(""));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_TRUE(value);

    EXPECT_NO_THROW(arg.parse("some_random_string"));
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_TRUE(value);
}

TEST_F(test_argparse_flag_argument, parse_from_env_variable_empty)
{
    bool value = false;
    tc::sdk::flag_argument arg("flag_a", 'a', value, "Argument Description", "");

    EXPECT_NO_THROW(arg.parse_from_env());
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_FALSE(value);
}

TEST_F(test_argparse_flag_argument, parse_from_env_variable_not_found)
{
    bool value = false;
    tc::sdk::flag_argument arg("flag_a", 'a', value, "Argument Description", ENV_VAR);

    tc::sdk::tests::clear_env(ENV_VAR);

    EXPECT_NO_THROW(arg.parse_from_env());
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_FALSE(value);
}

TEST_F(test_argparse_flag_argument, parse_from_env_variable_found)
{
    bool value = false;
    tc::sdk::flag_argument arg("flag_a", 'a', value, "Argument Description", ENV_VAR);

    tc::sdk::tests::clear_env(ENV_VAR);
    tc::sdk::tests::set_env(ENV_VAR, "1");

    EXPECT_NO_THROW(arg.parse_from_env());
    EXPECT_TRUE(arg.is_parsed());
    EXPECT_TRUE(value);

    tc::sdk::tests::clear_env(ENV_VAR);
}

}
