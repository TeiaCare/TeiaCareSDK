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

#include <teiacare/sdk/task.hpp>

#include <gtest/gtest.h>

namespace tc::sdk::tests
{
bool global_function_invoked = false;
void global_function()
{
    global_function_invoked = true;
};

bool global_function_template_invoked = false;
template <class T>
void global_function_template()
{
    global_function_template_invoked = true;
};

struct Foo
{
    bool member_function_invoked = false;
    void member_function()
    {
        member_function_invoked = true;
    };

    bool member_function_template_invoked = false;
    template <class T>
    void member_function_template()
    {
        member_function_template_invoked = true;
    };
};

template <class T>
struct FooT
{
public:
    bool member_function_template_class_invoked = false;
    void member_function_template_class()
    {
        member_function_template_class_invoked = true;
    };
};

struct Functor
{
    static bool functor_invoked;
    void operator()()
    {
        functor_invoked = true;
    }
};

}
