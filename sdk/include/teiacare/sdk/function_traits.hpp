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

#include <functional>

namespace tc::sdk
{
template <typename RetType, typename... Args>
struct function_wrapper
{
    using return_t = RetType;
    using function_t = RetType(std::decay_t<Args>...);
    using wrapper_t = std::function<function_t>;
};

template <typename T>
struct function_traits;

template <typename RetType, typename... Args>
struct function_traits<RetType(Args...)> : function_wrapper<RetType, Args...> {};

template <typename RetType, typename... Args>
struct function_traits<RetType (*)(Args...)> : function_wrapper<RetType, Args...> {};

template <typename RetType, typename... Args>
struct function_traits<RetType (&)(Args...)> : function_wrapper<RetType, Args...> {};

template <typename CType, typename RetType, typename... Args>
struct function_traits<RetType (CType::*)(Args...)> : function_wrapper<RetType, Args...> {};

template <typename CType, typename RetType, typename... Args>
struct function_traits<RetType (CType::*)(Args...) const> : function_wrapper<RetType, Args...> {};

template <typename T>
struct function_traits : function_traits<decltype(&T::operator())> {};
}
