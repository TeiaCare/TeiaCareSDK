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

/**
 * @example example_argparse.cpp
 * @brief Command line argument parsing example of tc::sdk::argument_parser, tc::sdk::flag_argument, tc::sdk::optional_argument and tc::sdk::positional_argument
 */

#include <teiacare/sdk/argparse/argument_parser.hpp>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

/**
 * @cond SKIP_DOXYGEN
 */
template <typename T>
std::string to_string(const std::vector<T>& v)
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
/** @endcond */

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    tc::sdk::argument_parser parser("SDK argparse example", "v1.0.0", "This is the program description");

    std::string output_file;
    parser.add_positional("output_file", output_file, "The output file");

    std::string input_file;
    parser.add_positional("input_file", input_file, "The input file");

    int iterations;
    parser.add_option("iterations", "i", iterations, 10, "Iterations count", false, "TC_ITERATIONS");

    float threshold;
    parser.add_option("threshold", "t", threshold, 0.5f, "Threshold");

    std::vector<int> the_list;
    parser.add_option("list", "l", the_list, std::vector<int>{1, 2, 3}, "List");

    bool verbose;
    parser.add_flag("verbose", "x", verbose, "Enable verbose output", "TC_VERBOSE");

    std::string str_opt;
    parser.add_option(std::make_unique<tc::sdk::optional_argument<std::string>>("str_opt", "s", str_opt, "default_value", "Threshold"));

    bool flag;
    auto flag_arg = std::make_unique<tc::sdk::flag_argument>("flag", "f", flag, "Some random flag", "TC_FLAG");
    parser.add_flag(std::move(flag_arg));

    double double_opt;
    auto double_arg = std::make_unique<tc::sdk::optional_argument<double>>("double", "d", double_opt, 5.6798, "Double argument", true);
    double_arg->set_validator([](const double& value) {
        return value > 10.0;
    });
    parser.add_option(std::move(double_arg));

    switch (parser.parse(argc, argv))
    {
    case tc::sdk::parse_result::success:
        break;
    case tc::sdk::parse_result::quit:
        return EXIT_SUCCESS;
    case tc::sdk::parse_result::error:
        return EXIT_SUCCESS; // Normally here you will return EXIT_FAILURE, but we avoid failure since CI runs without any command line arguments.
    }

    // Alternatively, the previous switch statement is available via the following macro:
    // TC_PARSE_CLI(parser, argc, argv);

    spdlog::info("Input file: {}", input_file);
    spdlog::info("Output file: {}", output_file);
    spdlog::info("Iterations: {}", iterations);
    spdlog::info("Threshold: {}", threshold);
    spdlog::info("Verbose: {}", (verbose ? "true" : "false"));
    spdlog::info("Str Opt: {}", str_opt);
    spdlog::info("Double Opt: ", double_opt);
    spdlog::info("Flag: {}", (flag ? "true" : "false"));
    spdlog::info("List: {}", to_string(the_list));

    return EXIT_SUCCESS;
}
