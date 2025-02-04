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

#include "test_argparse_argument_parser.hpp"

namespace tc::sdk::tests
{
TEST_F(test_argparse_argument_parser, empty_parse)
{
    std::string command_line = "";
    argc_argv(command_line);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::success);
}

TEST_F(test_argparse_argument_parser, help_short)
{
    std::string command_line = "-h";
    argc_argv(command_line);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::quit);
}

TEST_F(test_argparse_argument_parser, help_long)
{
    std::string command_line = "--help";
    argc_argv(command_line);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::quit);
}

TEST_F(test_argparse_argument_parser, version_short)
{
    std::string command_line = "-v";
    argc_argv(command_line);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::quit);
}

TEST_F(test_argparse_argument_parser, version_long)
{
    std::string command_line = "--version";
    argc_argv(command_line);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::quit);
}

TEST_F(test_argparse_argument_parser, parse_positionals)
{
    std::string command_line = "one two three";
    argc_argv(command_line);

    std::string pos_1;
    parser.add_positional("positional_1", pos_1);

    std::string pos_2;
    parser.add_positional("positional_2", pos_2);

    std::string pos_3;
    parser.add_positional("positional_3", pos_3);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::success);
    EXPECT_EQ(pos_1, "one");
    EXPECT_EQ(pos_2, "two");
    EXPECT_EQ(pos_3, "three");
}

TEST_F(test_argparse_argument_parser, parse_positionals_required_error)
{
    std::string command_line = "one two";
    argc_argv(command_line);

    std::string pos_1;
    parser.add_positional("positional_1", pos_1);

    std::string pos_2;
    parser.add_positional("positional_2", pos_2);

    std::string pos_3;
    parser.add_positional("positional_3", pos_3);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_positionals_extras)
{
    std::string command_line = "one two three four";
    argc_argv(command_line);

    std::string pos_1;
    parser.add_positional("positional_1", pos_1);

    std::string pos_2;
    parser.add_positional("positional_2", pos_2);

    std::string pos_3;
    parser.add_positional("positional_3", pos_3);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_optionals_long_equal)
{
    std::string command_line = "--first=1 --second=two --third=3,4,5";
    validate_optionals(command_line);
}

TEST_F(test_argparse_argument_parser, parse_optionals_long_space)
{
    std::string command_line = "--first 1 --second two --third 3,4,5";
    validate_optionals(command_line);
}

TEST_F(test_argparse_argument_parser, parse_optionals_short_equal)
{
    std::string command_line = "-f=1 -s=two -t=3,4,5";
    validate_optionals(command_line);
}

TEST_F(test_argparse_argument_parser, parse_optionals_short_space)
{
    std::string command_line = "-f 1 -s two -t 3,4,5";
    validate_optionals(command_line);
}

TEST_F(test_argparse_argument_parser, parse_optionals_long_and_short)
{
    std::string command_line = "-f=1 --second two -t 3,4,5";
    validate_optionals(command_line);
}

TEST_F(test_argparse_argument_parser, parse_optionals_extras)
{
    std::string command_line = "--first 1 --second two --third 3,4,5 --fourth=missing";
    argc_argv(command_line);

    int opt_1;
    parser.add_option("first", 'f', opt_1, 0, "description", true);

    std::string opt_2;
    parser.add_option("second", 's', opt_2, std::string(), "description", true);

    std::vector<int> opt_3;
    parser.add_option("third", 't', opt_3, {1, 2, 3}, "description", true, "ENV_VAR");

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_optionals_simple)
{
    std::string command_line = "--first 1 --second two --third 3,4,5 --fourth=missing";
    argc_argv(command_line);

    int opt_1;
    parser.add_option("first", opt_1, 0, "description", true);

    std::string opt_2;
    parser.add_option("second", opt_2, std::string(), "description", true);

    std::vector<int> opt_3;
    parser.add_option("third", opt_3, {1, 2, 3}, "description", true, "ENV_VAR");

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_optionals_not_required)
{
    std::string command_line = "-s two";
    argc_argv(command_line);

    int opt_1;
    parser.add_option("first", 'f', opt_1);

    std::string opt_2;
    parser.add_option("second", 's', opt_2);

    std::vector<int> opt_3;
    parser.add_option("third", 't', opt_3);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::success);
    EXPECT_EQ(opt_1, 0);
    EXPECT_EQ(opt_2, "two");
    EXPECT_EQ(opt_3, (std::vector<int>{}));
}

TEST_F(test_argparse_argument_parser, parse_optionals_required_error)
{
    std::string command_line = "-s two";
    argc_argv(command_line);

    int opt_1;
    parser.add_option("first", 'f', opt_1, 0, "description", true);

    std::string opt_2;
    parser.add_option("second", 's', opt_2, std::string(), "description", true);

    std::vector<int> opt_3;
    parser.add_option("third", 't', opt_3, {1, 2, 3}, "description", true);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_optionals_without_value)
{
    std::string command_line = "--first 1 --second two --third";
    argc_argv(command_line);

    int opt_1;
    parser.add_option("first", 'f', opt_1, 0, "description", true);

    std::string opt_2;
    parser.add_option("second", 's', opt_2, std::string(), "description", true);

    std::vector<int> opt_3;
    parser.add_option("third", 't', opt_3, {1, 2, 3}, "description", true);

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_flags_long)
{
    std::string command_line = "--flag_a --flag_b --flag_c";
    validate_flags(command_line);
}

TEST_F(test_argparse_argument_parser, parse_flags_short)
{
    std::string command_line = "-a -b -c";
    validate_flags(command_line);
}

TEST_F(test_argparse_argument_parser, parse_flags_long_and_short)
{
    std::string command_line = "--flag_a -b --flag_c";
    validate_flags(command_line);
}

TEST_F(test_argparse_argument_parser, parse_flags_extras)
{
    std::string command_line = "--flag_a --flag_b --flag_c --flag_d";
    argc_argv(command_line);

    bool flag_a;
    parser.add_flag("flag_a", 'a', flag_a);

    bool flag_b;
    parser.add_flag("flag_b", 'b', flag_b);

    bool flag_c;
    parser.add_flag("flag_c", 'c', flag_c, "", "ENV_VAR");

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

TEST_F(test_argparse_argument_parser, parse_flags_simple)
{
    std::string command_line = "--flag_a --flag_b --flag_c --flag_d";
    argc_argv(command_line);

    bool flag_a;
    parser.add_flag("flag_a", flag_a);

    bool flag_b;
    parser.add_flag("flag_b", flag_b);

    bool flag_c;
    parser.add_flag("flag_c", flag_c, "", "ENV_VAR");

    const tc::sdk::parse_result r = parser.parse(argc, argv);
    EXPECT_EQ(r, tc::sdk::parse_result::error);
}

}
