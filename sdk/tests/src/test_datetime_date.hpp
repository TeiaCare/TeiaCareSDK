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

#include <teiacare/sdk/datetime/date.hpp>

#include <gtest/gtest.h>

using namespace std::chrono_literals;

namespace tc::sdk::tests
{
class test_datetime_date : public ::testing::Test
{
protected:
    explicit test_datetime_date()
    {
    }

    ~test_datetime_date() override
    {
    }
};

}
