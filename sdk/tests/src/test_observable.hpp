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

#include <gtest/gtest.h>
#include <sdk/observable.hpp>

namespace tc::sdk::tests
{
class foo_t
{
public:
    foo_t(const std::string& s) : _s{ s } {}
    bool operator==(foo_t const&) const = default;

    std::string get() const { return _s; }
    void set(const std::string& s) { _s = s; }
private:
    std::string _s;
};

class counter_t
{
public:
    bool operator==(counter_t const&) const = default;
    void update(int increment) { _value += increment; }
    int value() const { return _value; }
private:
    int _value = 0;
};

static double global_counter = 0.0;
void free_function(int increment)
{
    global_counter += increment;
}

}
