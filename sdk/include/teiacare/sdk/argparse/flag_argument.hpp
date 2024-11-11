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
 * \class flag_argument
 * \brief Represents a flag argument for command-line parsing.
 *
 * The tc::sdk::flag_argument class is a specialization of tc::sdk::argument_base used for
 * handling flag-type command-line arguments that, when present, set a boolean variable to true.
 */
class flag_argument : public argument_base
{
public:
    /*!
     * \brief Constructs a flag argument.
     * \param name_long Long name of the argument (e.g., "--flag").
     * \param name_short Short name of the argument (e.g., "-f").
     * \param var Reference to the boolean variable that will be set.
     * \param description Description of the argument (optional).
     * \param env_var Name of the environment variable to parse (optional).
     */
    explicit flag_argument(const std::string& name_long, const std::string& name_short, bool& var, const std::string& description = "", const std::string& env_var = "") noexcept
        : argument_base(name_long, name_short, description, false, env_var)
        , _var{var}
    {
    }

    /*!
     * \brief Sets the variable to true when the flag is present.
     * \param value Unused parameter (flags do not require a value).
     */
    void parse(const std::string&) override
    {
        _var = true;
        _parsed = true;
    }

private:
    bool& _var;
};

}
