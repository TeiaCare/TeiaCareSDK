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

#include <teiacare/sdk/argparse/flag_argument.hpp>
#include <teiacare/sdk/argparse/optional_argument.hpp>
#include <teiacare/sdk/argparse/positional_argument.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace tc::sdk
{
#define TC_PARSE_CLI(parser, argc, argv) \
    switch (parser.parse(argc, argv))    \
    {                                    \
    case tc::sdk::parse_result::success: \
        break;                           \
    case tc::sdk::parse_result::quit:    \
        return EXIT_SUCCESS;             \
    case tc::sdk::parse_result::error:   \
        return EXIT_FAILURE;             \
    }

enum class parse_result
{
    success,
    error,
    quit
};

class argument_parser
{
public:
    explicit argument_parser(const std::string& program_name, const std::string& program_version, const std::string& program_description = "") noexcept;

    template <typename T>
    void add_positional(const std::string& name, T& var, const std::string& description = "")
    {
        add_positional(std::make_unique<tc::sdk::positional_argument<T>>(name, var, description));
    }

    template <typename T>
    void add_positional(std::unique_ptr<tc::sdk::positional_argument<T>>&& positional_arg)
    {
        _positionals.emplace_back(std::move(positional_arg));
    }

    template <typename T>
    void add_option(const std::string& name_long, const std::string& name_short, T& var, const T& default_value = T(), const std::string& description = "", bool required = false, const std::string& env_var = "")
    {
        add_option(std::make_unique<tc::sdk::optional_argument<T>>(name_long, name_short, var, default_value, description, required, env_var));
    }

    template <typename T>
    void add_option(std::unique_ptr<tc::sdk::optional_argument<T>>&& optional_arg)
    {
        _optionals.emplace_back(std::move(optional_arg));
    }

    void add_flag(const std::string& name_long, const std::string& name_short, bool& var, const std::string& description = "", const std::string& env_var = "")
    {
        add_flag(std::make_unique<tc::sdk::flag_argument>(name_long, name_short, var, description, env_var));
    }

    void add_flag(std::unique_ptr<tc::sdk::flag_argument>&& flag_arg)
    {
        _flags.emplace_back(std::move(flag_arg));
    }

    parse_result parse(int argc, char* argv[]) const;

protected:
    parse_result parse(std::vector<std::string>&& args) const;
    void print_help() const;
    void print_version() const;
    void print_description() const;
    void print_usage() const;
    void print_arguments() const;

private:
    tc::sdk::argument_base* find_argument(std::string_view name, const std::vector<std::unique_ptr<tc::sdk::argument_base>>& args) const;
    std::pair<std::string, std::string> split_key_value(const std::string& arg) const;
    std::string vector_to_string(const std::vector<std::string>& v) const;
    void check_parsed_arguments(const std::vector<std::unique_ptr<tc::sdk::argument_base>>& args) const;

    constexpr bool is_option(std::string_view arg) const
    {
        return arg.starts_with("-");
    }

    constexpr bool is_long_option(std::string_view arg) const
    {
        return arg.size() > 2 && arg.starts_with("--") && arg.at(2) != '-';
    }

    constexpr bool is_short_option(std::string_view arg) const
    {
        return arg.size() > 1 && arg.starts_with("-") && arg.at(1) != '-';
    }

    const std::string _program_name;
    const std::string _program_version;
    const std::string _program_description;

    std::vector<std::unique_ptr<tc::sdk::argument_base>> _positionals;
    std::vector<std::unique_ptr<tc::sdk::argument_base>> _optionals;
    std::vector<std::unique_ptr<tc::sdk::argument_base>> _flags;

    bool _help_flag = false;
    bool _version_flag = false;
};

}
