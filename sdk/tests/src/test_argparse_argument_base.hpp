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

#include <teiacare/sdk/argparse/argument_base.hpp>

#include <cstdlib>
#include <gtest/gtest.h>

namespace tc::sdk::tests
{
class test_argparse_argument_base : public ::testing::Test
{
protected:
    explicit test_argparse_argument_base()
    {
    }

    ~test_argparse_argument_base() override
    {
    }

    template <class T>
    class argument : public argument_base
    {
    public:
        using argument_base::argument_base;
        void parse(const std::string&) override
        {
            _parsed = true;
        }

        T convert_wrapper(const std::string& value)
        {
            return convert<T>(value);
        }
    };
};

void set_env(const std::string& name, const std::string& value);

void clear_env(const std::string& name);

}
