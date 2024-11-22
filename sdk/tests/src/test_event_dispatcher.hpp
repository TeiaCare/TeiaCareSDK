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

#include <teiacare/sdk/event_dispatcher.hpp>

#include <gtest/gtest.h>

namespace tc::sdk::tests
{
class test_event_dispatcher : public ::testing::Test
{
protected:
    explicit test_event_dispatcher()
        : e{std::make_unique<tc::sdk::event_dispatcher>()}
    {
    }

    ~test_event_dispatcher() override
    {
        e->stop();
    }

    std::unique_ptr<tc::sdk::event_dispatcher> e;
};

}
