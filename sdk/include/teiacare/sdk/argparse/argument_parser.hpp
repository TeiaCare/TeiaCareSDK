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
/*!
 * \enum class parse_result
 * \brief Represents the result of parsing command-line arguments.
 *
 * The tc::sdk::parse_result enum class indicates the outcome of parsing:
 * - success: Parsing was successful, and the program can proceed.
 * - error: An error occurred during parsing, and the program should terminate.
 * - quit: No error occurred during parsing, but the program should terminate. (i.e. program invoked with a help or version flag).
 */
enum class parse_result
{
    success,
    error,
    quit
};

/*!
 * \class argument_parser
 * \brief Command-line argument parser for handling flags, options, and positional arguments.
 *
 * The tc::sdk::argument_parser class facilitates the definition and parsing of command-line arguments,
 * including positional arguments, optional parameters, and flags. It also provides mechanisms
 * for displaying help and version information.
 *
 * Example usage:
 * \code{.cpp}
 * tc::sdk::argument_parser parser("ProgramName", "v1.0.0", "A sample program description");
 *
 * int value;
 * parser.add_positional("input", value, "Input file path");
 *
 * bool verbose;
 * parser.add_flag("verbose", "v", verbose, "Enable verbose output");
 *
 * if (parser.parse(argc, argv) == tc::sdk::parse_result::success) {
 *     // Proceed with program logic
 * }
 * \endcode
 * \see TC_PARSE_CLI
 */
class argument_parser
{
public:
    /*!
     * \brief Constructs an argument parser with program information.
     * \param program_name Name of the program.
     * \param program_version Version of the program.
     * \param program_description Description of the program (optional).
     */
    explicit argument_parser(const std::string& program_name, const std::string& program_version, const std::string& program_description = "") noexcept;

    /*!
     * \brief Adds a positional argument to the parser.
     * \tparam T Type of the argument.
     * \param name Name of the positional argument.
     * \param var Variable to store the parsed value.
     * \param description Description of the argument (optional).
     */
    template <typename T>
    void add_positional(const std::string& name, T& var, const std::string& description = "")
    {
        add_positional(std::make_unique<tc::sdk::positional_argument<T>>(name, var, description));
    }

    /*!
     * \brief Adds a positional argument using a unique pointer.
     * \tparam T Type of the argument.
     * \param positional_arg Unique pointer to the positional argument object.
     */
    template <typename T>
    void add_positional(std::unique_ptr<tc::sdk::positional_argument<T>>&& positional_arg)
    {
        _positionals.emplace_back(std::move(positional_arg));
    }

    /*!
     * \brief Adds an optional argument to the parser.
     * \tparam T Type of the argument.
     * \param name_long Long name of the argument (e.g., "--option").
     * \param name_short Short name of the argument (e.g., "-o").
     * \param var Variable to store the parsed value.
     * \param default_value Default value for the argument.
     * \param description Description of the argument (optional).
     * \param required Whether the argument is required (default=false).
     * \param env_var Environment variable to use for the argument (optional).
     */
    template <typename T>
    void add_option(const std::string& name_long, const std::string& name_short, T& var, const T& default_value = T(), const std::string& description = "", bool required = false, const std::string& env_var = "")
    {
        add_option(std::make_unique<tc::sdk::optional_argument<T>>(name_long, name_short, var, default_value, description, required, env_var));
    }

    /*!
     * \brief Adds an optional argument using a unique pointer.
     * \tparam T Type of the argument.
     * \param optional_arg Unique pointer to the optional argument object.
     */
    template <typename T>
    void add_option(std::unique_ptr<tc::sdk::optional_argument<T>>&& optional_arg)
    {
        _optionals.emplace_back(std::move(optional_arg));
    }

    /*!
     * \brief Adds a flag argument to the parser.
     * \param name_long Long name of the flag (e.g., "--flag").
     * \param name_short Short name of the flag (e.g., "-f").
     * \param var Boolean variable to store the flag state.
     * \param description Description of the flag (optional).
     * \param env_var Environment variable to use for the flag (optional).
     */
    void add_flag(const std::string& name_long, const std::string& name_short, bool& var, const std::string& description = "", const std::string& env_var = "")
    {
        add_flag(std::make_unique<tc::sdk::flag_argument>(name_long, name_short, var, description, env_var));
    }

    /*!
     * \brief Adds a flag argument using a unique pointer.
     * \param flag_arg Unique pointer to the flag argument object.
     */
    void add_flag(std::unique_ptr<tc::sdk::flag_argument>&& flag_arg)
    {
        _flags.emplace_back(std::move(flag_arg));
    }

    /*!
     * \brief Parses the command-line arguments.
     * \param argc Number of command-line arguments.
     * \param argv Array of command-line arguments.
     * \return A tc::sdk::parse_result indicating the outcome of the parsing (success, error, or quit).
     */
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

/*!
 * \def TC_PARSE_CLI
 * \brief Macro for parsing command-line arguments and handling parse results.
 * \param parser An instance of the tc::sdk::argument_parser class.
 * \param argc The number of command-line arguments.
 * \param argv The array of command-line arguments.
 *
 * The TC_PARSE_CLI macro simplifies the parsing logic by switching on the result
 * of the tc::sdk::argument_parser::parse() method. Depending on the tc::sdk::parse_result, it either:
 * - Continues execution if parsing was successful (tc::sdk::parse_result::success).
 * - Exits with EXIT_SUCCESS if the user requested to quit (tc::sdk::parse_result::quit).
 * - Exits with EXIT_FAILURE if there was an error during parsing (tc::sdk::parse_result::error).
 */
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

}
