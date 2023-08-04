#pragma once

#include <gtest/gtest.h>
#include <sdk/rate_limiter.hpp>
#include <functional>
#include <chrono>

namespace tc::sdk::tests
{
class elapsed_timer
{
public:
    template<typename Rep, typename Dur>
    explicit elapsed_timer(const std::chrono::duration<Rep, Dur> duration)
        : _deadline { std::chrono::steady_clock::now() + duration }
    {
    }

    bool is_elapsed() const
    {
        return std::chrono::steady_clock::now() >= _deadline; 
    }
private:
    const std::chrono::steady_clock::time_point _deadline;
};

}
