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
template <typename T>
class optional_argument : public argument_base
{
public:
    explicit optional_argument(const std::string& name_long, const char name_short[2], T& var, const T& default_value = T(), const std::string& description = "", bool required = false, const std::string& env_var = "") noexcept
        : argument_base(name_long, name_short, description, required, env_var)
        , _var{var}
    {
        _var = default_value;
    }

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

    optional_argument* set_default_value(const T& default_value)
    {
        _var = default_value;
        return this;
    }

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
