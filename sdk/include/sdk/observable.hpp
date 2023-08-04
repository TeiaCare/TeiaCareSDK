#pragma once

#include <atomic>
#include <mutex>
#include <type_traits>
#include <concepts>

namespace tc::sdk
{
/*
* T must be Equality Comparable, i.e. it must implement the equality comparison operator:
* bool operator==(T const&) const { }
*/

template<std::equality_comparable T, typename CallbackT>
class observable
{
public:
    explicit observable(const T& t, const CallbackT& callback)
        : _value{ t }
        , _callback{ callback }
        , _is_callback_enabled{ true }
    {
        static_assert(std::is_invocable_v<CallbackT, T>, "CallbackT must be an invocable object that accepts T");
        static_assert(std::is_same_v<void, std::invoke_result_t<CallbackT, T>>, "CallbackT must return void");
    }

    explicit observable(T&& t, const CallbackT& callback)
        : _value{ std::forward<T>(t) }
        , _callback{ callback }
        , _is_callback_enabled{ true }
    {
        static_assert(std::is_invocable_v<CallbackT, T>, "CallbackT must be an invocable object that accepts T");
        static_assert(std::is_same_v<void, std::invoke_result_t<CallbackT, T>>, "CallbackT must return void");
    }

    observable(observable&) = delete;
    observable(const observable&) = delete;
    observable(observable&&) noexcept = delete;
    observable& operator=(const observable&) = delete;
    observable& operator=(observable&&) = delete;
    ~observable() = default;

    bool callback_enabled() const
    {
        std::lock_guard lock(_mutex);
        return _is_callback_enabled.load();
    }

    void callback_enabled(bool is_enabled) 
    {
        std::lock_guard lock(_mutex);
        _is_callback_enabled.store(is_enabled);
    }

    operator T const() const
    {
        return value();
    }

    operator T()
    {
        return value();
    }

    T value() const
    {
        std::lock_guard lock(_mutex);
        return _value;
    }

    observable& operator=(T&& t)
    {
        set(std::forward<T>(t));
        return *this;
    }

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
    const CallbackT _callback;
    std::atomic_bool _is_callback_enabled;
};

}
