#pragma once

#include <chrono>

namespace tc::sdk::examples::utils
{

class elapsed_timer
{
public:
    template<typename T>
    explicit elapsed_timer(const std::chrono::duration<T> duration)
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
