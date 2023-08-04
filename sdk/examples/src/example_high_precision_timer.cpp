#include <sdk/high_precision_timer.hpp>
#include <spdlog/spdlog.h>
#include "utils/callback_timer.hpp"

using namespace std::chrono_literals;

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    tc::sdk::examples::utils::callback_timer debug_timer(1s, [](unsigned i){spdlog::info("{}", i);});

    {
        tc::sdk::high_precision_timer timer;
        spdlog::info("Timer @ 10ms --- Start");

        timer.set_callback([]{ spdlog::info("@"); });
        timer.start(10ms);
        std::this_thread::sleep_for(2s);

        spdlog::info("Timer @ 10ms --- Stop");
        timer.stop();
    }

    {
        tc::sdk::high_precision_timer timer;
        spdlog::info("Timer @ 250ms --- Start");

        timer.set_callback([]{ spdlog::info("@"); });
        timer.start(250ms);
        std::this_thread::sleep_for(2s);

        spdlog::info("Timer @ 250ms --- Stop");
        timer.stop();
    }

    return 0;
}
