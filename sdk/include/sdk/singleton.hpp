#pragma once

#include <sdk/non_copyable.hpp>
#include <sdk/non_moveable.hpp>

namespace tc::sdk
{
template<typename T>
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
