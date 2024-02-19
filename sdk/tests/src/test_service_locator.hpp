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
#include <sdk/service_locator.hpp>
#include <string>

namespace tc::sdk::tests
{
struct IService
{
    virtual ~IService() = default;
    virtual std::string call() const = 0;
};

struct ServiceA : IService
{
    std::string call() const override { return "A"; }
    int a = 0;
};

struct ServiceB : IService
{
    std::string call() const override { return "B"; }
    int b = 0;
};

struct ServiceX : IService
{
    explicit ServiceX(std::string x, double y) : _x {x}, _y{y}{}
    std::string call() const override { return _x; }
    double non_virtual_call() const { return _y; }
    std::string _x = "";
    double _y = 0.0;
};

struct IAnotherService
{
    virtual std::string call() const = 0;
};

struct AnotherService : public IAnotherService
{
    virtual std::string call() const { return "AnotherService"; }
};

class test_service_locator : public ::testing::Test
{
protected:
    explicit test_service_locator() : s{tc::sdk::service_locator::instance()}
    {
    }

    void SetUp() override 
    {
    }

    void TearDown() override 
    {
        s.unregister_service<IService>();
        s.unregister_service<IAnotherService>();
    }

    tc::sdk::service_locator& s;
};

}

namespace tc::sdk
{
    class service_locator;
    bool operator==(const tc::sdk::service_locator& s1, const tc::sdk::service_locator& s2) { return &s1 == &s2; }
}