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
