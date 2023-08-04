#include <sdk/observable.hpp>
#include <spdlog/spdlog.h>

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    auto observable = tc::sdk::observable(-1, [](int value){ spdlog::info("Updated: {}", value); });
    
    const int total_updates = 10;
    for (int i = 0; i < total_updates; ++i)
    {
        observable = i; // Trigger updates
    }
    
    spdlog::warn("[Callback Disabled]");
    observable.callback_enabled(false);
    observable = -1; // Does not trigger updates (callback disabled)

    spdlog::warn("[Callback Enabled]");
    observable.callback_enabled(true);
    observable = -1; // Does not trigger updates (even if callback is enabled the value is not updated)
    observable = 1; // Trigger updates (callback enabled and value updated)
    observable = 1; // Does not trigger updates (the value is not updated)
    observable = 1; // Does not trigger updates (the value is not updated)

    return 0;
}
