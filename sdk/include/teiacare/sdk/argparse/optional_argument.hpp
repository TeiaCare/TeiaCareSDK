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

#include <functional>

namespace tc::sdk
{
/*!
 * \class optional_argument
 * \brief Represents an optional argument for command-line parsing.
 * \tparam T Type of the argument variable.
 *
 * The tc::sdk::optional_argument class is a specialization of tc::sdk::argument_base used for
 * handling optional command-line arguments with default values and optional validation.
 */
template <typename T>
class optional_argument : public argument_base
{
public:
    /*!
     * \brief Constructs an optional argument.
     * \param name_long Long name of the argument (e.g., "--option").
     * \param name_short Short name of the argument (e.g., "-o").
     * \param var Reference to the variable that will hold the parsed value.
     * \param default_value Default value for the argument.
     * \param description Description of the argument (optional).
     * \param required Indicates if the argument is required (default=false).
     * \param env_var Name of the environment variable to parse (optional).
     */
    explicit optional_argument(const std::string& name_long, const std::string& name_short, T& var, const T& default_value = T(), const std::string& description = "", bool required = false, const std::string& env_var = "") noexcept
        : argument_base(name_long, name_short, description, required, env_var)
        , _var{var}
    {
        _var = default_value;
    }

    /*!
     * \brief Parses and converts the provided value.
     * \param value String value to parse and convert.
     * \throws std::runtime_error If the value does not pass validation.
     */
    void parse(const std::string& value) override
    {
        const T converted_value = convert<T>(value);
        if (_validator && !_validator(converted_value))
        {
            throw std::runtime_error("Validation Error: " + get_name_long() + "=" + value);
        }

        _var = converted_value;
        _parsed = true;
    }

    /*!
     * \brief Sets a validation function for the argument.
     * \param validator_callback A callback function to validate the argument value.
     * \return Pointer to the current tc::sdk::optional_argument instance for method chaining.
     *
     * The callback shall return false or throw an exception in order to report a validation error.
     * If the validation succeeds the callback must return true.
     */
    optional_argument* set_validator(std::function<bool(const T&)> validator_callback)
    {
        _validator = validator_callback;
        return this;
    }

private:
    T& _var;
    std::function<bool(const T&)> _validator;
};

}
