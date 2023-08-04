#include <sdk/rate_limiter.hpp>
#include <thread>

namespace tc::sdk
{
rate_limiter::rate_limiter(size_t rate)
    : time_between_frames{ std::chrono::seconds(1) / static_cast<duration_rep>(rate) }
    , tp{ tc::sdk::clock::now() }
{
}

void rate_limiter::sync()
{
    tp += time_between_frames;
    std::this_thread::sleep_until(tp);
}

}