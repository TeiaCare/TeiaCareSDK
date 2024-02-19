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

#include <atomic>
#include <mutex>
#include <type_traits>
#include <concepts>
#include <functional>

namespace tc::sdk
{
/*!
 * \class observable
 * \brief Utility class to observe a value with a user provided callback
 * 
 * \tparam T Observed item type
 * \tparam CallbackT User defined callback type
 * 
 * The callback is invoked every time the observed value is updated.  
 * The observable callback receives as a parameter the updated observed value.  
 * Thread safe.  
 * 
 * \note T must be Equality Comparable, i.e. it must implement the equality comparison operator:
 * \code
 * bool operator==(T const&) const { }
 * \endcode
 */
template<std::equality_comparable T>
class observable // : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor
     * \tparam T initial value for the observed value
     * \tparam CallbackT callable object that is invoked every time the value is changed
     * 
     * Creates a tc::sdk::observable instance. 
     */
    explicit observable(const T& t, const std::function<void(T)>& callback)
        : _value{ t }
        , _callback{ callback }
        , _is_callback_enabled{ true }
    {
        // static_assert(std::is_invocable_v<CallbackT, T>, "CallbackT must be an invocable object that accepts T");
        // static_assert(std::is_same_v<void, std::invoke_result_t<CallbackT, T>>, "CallbackT must return void");
    }

    /*!
     * \brief Constructor
     * \tparam T initial value for the observed value
     * \tparam CallbackT callable object that is invoked every time the value is changed
     * 
     * Creates a tc::sdk::observable instance. 
     */
    explicit observable(T&& t, const std::function<void(T)>& callback)
        : _value{ std::forward<T>(t) }
        , _callback{ callback }
        , _is_callback_enabled{ true }
    {
        // static_assert(std::is_invocable_v<CallbackT, T>, "CallbackT must be an invocable object that accepts T");
        // static_assert(std::is_same_v<void, std::invoke_result_t<CallbackT, T>>, "CallbackT must return void");
    }

    /*!
     * \brief Destructor
     * 
     * Destructs this.
     */
    ~observable() = default;

    /*!
     * \brief Check if the callback is currently enabled
     * \return true if the callback is currently enabled
     */
    bool callback_enabled() const
    {
        std::lock_guard lock(_mutex);
        return _is_callback_enabled.load();
    }

    /*!
     * \brief Enable or disable the callback temporarily
     * \param is_enabled enable or disable callback
     */
    void callback_enabled(bool is_enabled) 
    {
        std::lock_guard lock(_mutex);
        _is_callback_enabled.store(is_enabled);
    }

    /*!
     * \brief Return the observed value
     * \return const observed value
     */
    operator T const() const
    {
        return value();
    }

    /*!
     * \brief Return the observed value
     * \return observed value
     */
    operator T()
    {
        return value();
    }

    /*!
     * \brief Return the observed value
     * \return the observed value
     */
    T value() const
    {
        std::lock_guard lock(_mutex);
        return _value;
    }

    /*!
     * \brief Update the observed value
     * \tparam t the updated value
     * \return a reference to self
     * 
     * If the parameter value is different from the observed one the callback is invoked.
     */
    observable& operator=(T&& t)
    {
        set(std::forward<T>(t));
        return *this;
    }

    /*!
     * \brief Update the observed value
     * \tparam t the updated value
     * \return a reference to self
     * 
     * If the parameter value is different from the observed one the callback is invoked.
     */
    observable& operator=(const T& t)
    {
        set(t);
        return *this;
    }

protected:
    void set(const T& value)
    {
        std::lock_guard lock(_mutex);

        if (value == _value)
            return;
        
        _value = value;
        
        if(_is_callback_enabled)
            _callback(_value);
    }
    
    void set(T&& value)
    {
        std::lock_guard lock(_mutex);

        if (value == _value)
            return;
        
        _value = value;

        if(_is_callback_enabled)
            _callback(_value);
    }

private:
    mutable std::mutex _mutex;
    T _value;
    const std::function<void(T)> _callback;
    std::atomic_bool _is_callback_enabled;
};

}
