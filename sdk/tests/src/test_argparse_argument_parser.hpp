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

#include <teiacare/sdk/argparse/argument_parser.hpp>

#include <gtest/gtest.h>
#include <utility>

#define argc_argv(command_line)                                 \
    std::istringstream iss(command_line);                       \
    std::vector<std::string> command_line_args{"program_name"}; \
    std::string arg;                                            \
    while (iss >> arg)                                          \
    {                                                           \
        command_line_args.push_back(arg);                       \
    }                                                           \
    std::vector<char*> args;                                    \
    for (const auto& arg : command_line_args)                   \
    {                                                           \
        args.push_back(const_cast<char*>(arg.c_str()));         \
    }                                                           \
    int argc = args.size();                                     \
    char** argv = args.data();

namespace tc::sdk::tests
{
class test_argparse_argument_parser : public ::testing::Test
{
protected:
    explicit test_argparse_argument_parser()
        : parser{tc::sdk::argument_parser("program_name", "v1.0.0", "program_description")}
    {
    }

    ~test_argparse_argument_parser() override
    {
    }

    void validate_optionals(const std::string& command_line)
    {
        argc_argv(command_line);

        int opt_1;
        parser.add_option("first", "f", opt_1);

        std::string opt_2;
        parser.add_option("second", "s", opt_2);

        std::vector<int> opt_3;
        parser.add_option("third", "t", opt_3);

        const tc::sdk::parse_result r = parser.parse(argc, argv);
        EXPECT_EQ(r, tc::sdk::parse_result::success);
        EXPECT_EQ(opt_1, 1);
        EXPECT_EQ(opt_2, "two");
        EXPECT_EQ(opt_3, (std::vector<int>{3, 4, 5}));
    }

    void validate_flags(const std::string& command_line)
    {
        argc_argv(command_line);

        bool flag_a;
        parser.add_flag("flag_a", "a", flag_a);

        bool flag_b;
        parser.add_flag("flag_b", "b", flag_b);

        bool flag_c;
        parser.add_flag("flag_c", "c", flag_c);

        const tc::sdk::parse_result r = parser.parse(argc, argv);
        EXPECT_EQ(r, tc::sdk::parse_result::success);
        EXPECT_TRUE(flag_a);
        EXPECT_TRUE(flag_b);
        EXPECT_TRUE(flag_c);
    }

    tc::sdk::argument_parser parser;
};

}
