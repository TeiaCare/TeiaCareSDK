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

#include <teiacare/sdk/argparse/argument_parser.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>

namespace tc::sdk
{
argument_parser::argument_parser(const std::string& program_name, const std::string& program_version, const std::string& program_description) noexcept
    : _program_name{program_name}
    , _program_version{program_version}
    , _program_description{program_description}
{
    add_flag("help", "h", _help_flag, "Display this help message");
    add_flag("version", "v", _version_flag, "Display program version");
}

parse_result argument_parser::parse(int argc, char* argv[]) const
{
    try
    {
        return parse(std::vector<std::string>(argv + 1, argv + argc));
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        print_help();
        return parse_result::error;
    }
}

parse_result argument_parser::parse(std::vector<std::string>&& args) const
{
    for (const auto& opt : _optionals)
    {
        opt->parse_from_env();
    }

    for (const auto& flag : _flags)
    {
        flag->parse_from_env();
    }

    auto next_positional_arg = _positionals.begin();
    for (size_t i = 0; i < args.size(); ++i)
    {
        const std::string raw_argument = args[i];

        if (is_option(raw_argument))
        {
            std::string arg = "";

            if (is_long_option(raw_argument))
            {
                // Trim the "--" prefix for long options
                arg = raw_argument.substr(2);
            }
            else if (is_short_option(raw_argument))
            {
                // Trim the "-" prefix for short options
                arg = raw_argument.substr(1);
            }

            const auto [key, value] = split_key_value(arg);

            if (key == "help" || key == "h")
            {
                print_help();
                return parse_result::quit;
            }

            if (key == "version" || key == "v")
            {
                print_version();
                return parse_result::quit;
            }

            if (auto opt = find_argument(key, _optionals); opt != nullptr)
            {
                if (!value.empty())
                {
                    opt->parse(value);
                }
                else if (i + 1 < args.size())
                {
                    const std::string next_arg = args[++i];
                    opt->parse(next_arg);
                }
                else
                {
                    throw std::runtime_error("Missing value for argument: --" + key);
                }
            }
            else if (auto flag = find_argument(key, _flags); flag != nullptr)
            {
                flag->parse("");
            }
            else
            {
                throw std::runtime_error("Unknown argument: --" + key);
            }
        }
        else
        {
            // Positional Argument
            if (next_positional_arg != _positionals.end())
            {
                next_positional_arg->get()->parse(raw_argument);
                next_positional_arg++;
            }
            else
            {
                throw std::runtime_error("Unknown argument: " + raw_argument);
            }
        }
    }

    // Check all Positional Arguments have been set, otherwise report an error.
    check_parsed_arguments(_positionals);

    // Check required Optional Arguments have all been set, otherwise report an error
    check_parsed_arguments(_optionals);

    return parse_result::success;
}

void argument_parser::print_help() const
{
    print_version();
    print_description();
    print_usage();
    print_arguments();
}

void argument_parser::print_version() const
{
    std::cout << _program_name << " " << _program_version << std::endl;
}

void argument_parser::print_description() const
{
    std::cout << _program_description << std::endl;
}

void argument_parser::print_usage() const
{
    constexpr const char* pad = "    ";
    std::cout << "\nUsage: \n";
    std::cout << pad << _program_name;

    for (const auto& pos : _positionals)
    {
        std::cout << " <" << pos->get_name_long() << ">";
    }

    for (const auto& opt : _optionals)
    {
        std::cout << " [--" << opt->get_name_long() << "=<value>]";
    }

    for (const auto& flag : _flags)
    {
        std::cout << " [--" << flag->get_name_long() << "]";
    }

    std::cout << std::endl;
}

void argument_parser::print_arguments() const
{
    constexpr const char* pad = "    ";
    constexpr int item_width = 30;

    std::cout << std::left;
    std::cout << "\nOptions: \n";

    for (const auto& pos : _positionals)
    {
        std::cout << pad;
        std::cout << std::setw(item_width) << pos->get_name_long();
        std::cout << std::setw(item_width) << pos->get_description();
        std::cout << "\n";
    }

    for (const auto& opt : _optionals)
    {
        std::cout << pad;
        std::cout << std::setw(item_width) << "-" + opt->get_name_short() + ", --" + opt->get_name_long();
        std::cout << std::setw(item_width) << opt->get_description();
        if (const auto env = opt->get_env(); !env.empty())
        {
            std::cout << std::setw(item_width) << "[env:" + env + "]";
        }
        std::cout << "\n";
    }

    for (const auto& flag : _flags)
    {
        std::cout << pad;
        std::cout << std::setw(item_width) << "-" + flag->get_name_short() + ", --" + flag->get_name_long();
        std::cout << std::setw(item_width) << flag->get_description();
        if (const auto env = flag->get_env(); !env.empty())
        {
            std::cout << std::setw(item_width) << "[env:" + env + "]";
        }
        std::cout << "\n";
    }

    std::cout << std::endl;
}

tc::sdk::argument_base* argument_parser::find_argument(std::string_view name, const std::vector<std::unique_ptr<tc::sdk::argument_base>>& args) const
{
    auto it = std::find_if(args.begin(), args.end(), [&](const std::unique_ptr<tc::sdk::argument_base>& arg) {
        return name == arg->get_name_short() || name == arg->get_name_long();
    });

    return it == args.end() ? nullptr : it->get();
}

std::pair<std::string, std::string> argument_parser::split_key_value(const std::string& arg) const
{
    std::string key = "";
    std::string value = "";

    const size_t eq_pos = arg.find('=');
    if (eq_pos != std::string::npos)
    {
        key = arg.substr(0, eq_pos);
        value = arg.substr(eq_pos + 1);
    }
    else
    {
        key = arg;
    }

    return std::make_pair(key, value);
}

std::string argument_parser::vector_to_string(const std::vector<std::string>& v) const
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        oss << v[i];
        if (i < v.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

void argument_parser::check_parsed_arguments(const std::vector<std::unique_ptr<tc::sdk::argument_base>>& args) const
{
    std::vector<std::string> missing_arguments{};
    for (const auto& arg : args)
    {
        if (arg->is_required() && !arg->is_parsed())
        {
            missing_arguments.emplace_back(arg->get_name_long());
        }
    }

    if (!missing_arguments.empty())
    {
        throw std::runtime_error("Missing required arguments: " + vector_to_string(missing_arguments));
    }
}

}
