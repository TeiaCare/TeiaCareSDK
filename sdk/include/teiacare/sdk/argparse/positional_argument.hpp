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
template <typename T>
class positional_argument : public argument_base
{
public:
    explicit positional_argument(const std::string& name, T& var, const std::string& description = "") noexcept
        : argument_base(name, "", description, true)
        , _var(var)
    {
    }

    void parse(const std::string& value) override
    {
        _var = convert<T>(value);
        _parsed = true;
    }

private:
    T& _var;
};

}
