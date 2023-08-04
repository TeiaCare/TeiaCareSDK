#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <chrono>

namespace tc::sdk::examples::utils
{
class callback_timer
{
public:
    explicit callback_timer(std::chrono::duration<double> tick_rate, std::function<void(unsigned)> callback)
        : _is_running{ false }
        , _callback{ callback }
    {
        start(tick_rate);
    }

    ~callback_timer()
    {
        stop();
    }

    callback_timer(const callback_timer&) = delete;
    callback_timer(callback_timer&&) noexcept = delete;
    callback_timer& operator=(const callback_timer&) = delete;
    callback_timer& operator=(callback_timer&&) = delete;

protected:
    void start(std::chrono::duration<double> tick_rate)
    {
        if (_is_running)
            return;

        _is_running.store(true);
        _callback_timer_thread = std::thread(&callback_timer::tick, this, tick_rate);
    }

    void stop()
    {
        if (!_is_running)
            return;

        _is_running.store(false);        
        if (_callback_timer_thread.joinable())
            _callback_timer_thread.join();
    }

protected:
    void tick(std::chrono::duration<double> tick_rate) const
    {
        unsigned tick_count = 0; 
        while(_is_running) 
        {
            _callback(tick_count);
            std::this_thread::sleep_for(tick_rate);
            ++tick_count;
        }
    }

private:
    std::atomic_bool _is_running;
    std::thread _callback_timer_thread;
    std::function<void(unsigned)> _callback;
};

}
