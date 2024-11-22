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
// limitations under the License.#pragma once

#include <teiacare/sdk/argparse/argument_base.hpp>

namespace tc::sdk
{
/*!
 * \class positional_argument
 * \brief Represents a positional argument for command-line parsing.
 * \tparam T Type of the argument variable.
 *
 * The tc::sdk::positional_argument class is a specialization of tc::sdk::argument_base that handles
 * command-line arguments specified in a positional manner.
 */
template <typename T>
class positional_argument : public argument_base
{
public:
    /*!
     * \brief Constructs a positional argument.
     * \param name Name of the argument.
     * \param var Reference to the variable that will hold the parsed value.
     * \param description Description of the argument (optional).
     */
    explicit positional_argument(const std::string& name, T& var, const std::string& description = "") noexcept
        : argument_base(name, "", description, true)
        , _var(var)
    {
        _var = T();
    }

    /*!
     * \brief Parses and converts the provided value.
     * \param value String value to parse and convert.
     */
    void parse(const std::string& value) override
    {
        _var = convert<T>(value);
        _parsed = true;
    }

private:
    T& _var;
};

}
