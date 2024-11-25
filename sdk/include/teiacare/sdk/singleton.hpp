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

#include <teiacare/sdk/non_copyable.hpp>
#include <teiacare/sdk/non_moveable.hpp>

namespace tc::sdk
{
template <typename T>
class singleton : private tc::sdk::non_copyable, private tc::sdk::non_moveable
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

    virtual ~singleton() = default;

protected:
    singleton() = default;
};

}
