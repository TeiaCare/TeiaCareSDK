#include <sdk/rate_limiter.hpp>
#include <sdk/clock.hpp>
#include <spdlog/spdlog.h>
#include "utils/elapsed_timer.hpp"

using namespace std::chrono_literals;

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    {
        const size_t fps = 4;
        tc::sdk::rate_limiter limiter(4);
        int tick_counter = 1;
        spdlog::info("Limiter @ 4fps --- Start");

        const tc::sdk::examples::utils::elapsed_timer timer(5s);
        while(!timer.is_elapsed())
        {
            spdlog::info("[TICK] - {}", tick_counter);
            
            // simulate some "random" delay
            const auto sleep_time = 10;
            std::this_thread::sleep_for(tick_counter * std::chrono::milliseconds(sleep_time));
            ++tick_counter;
            limiter.sync();
        }
        spdlog::info("Limiter @ 4fps --- Stop");
    }

    spdlog::info("");

    {
        const size_t fps = 1;
        tc::sdk::rate_limiter limiter(1);
        int tick_counter = 1;
        spdlog::info("Limiter @ 1fps --- Start");
        
        const tc::sdk::examples::utils::elapsed_timer timer(5s);
        while(!timer.is_elapsed())
        {
            spdlog::info("[TICK] - {}", tick_counter);
            
            // simulate some "random" delay
            const auto sleep_time = 10;
            std::this_thread::sleep_for(tick_counter * std::chrono::milliseconds(sleep_time));
            ++tick_counter;
            limiter.sync();
        }
        spdlog::info("Limiter @ 1fps --- Stop");
    }
    
    spdlog::info("finished");
    return 0;
}
