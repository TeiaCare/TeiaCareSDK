#pragma once

#include <chrono>
#include <type_traits>
#include <sdk/clock.hpp>

namespace tc::sdk
{
class rate_limiter {
public:
    explicit rate_limiter(size_t rate);
    ~rate_limiter() = default;
    rate_limiter(const rate_limiter&) = delete;
    rate_limiter(rate_limiter&&) noexcept = delete;
    rate_limiter& operator=(const rate_limiter&) = delete;
    rate_limiter& operator=(rate_limiter&&) = delete;

    void sync();

private:
    using duration_rep = double;
    const std::chrono::duration<duration_rep> time_between_frames; // 1/rate
    std::chrono::time_point<tc::sdk::clock, std::remove_const_t<decltype(time_between_frames)>> tp;
};
}
