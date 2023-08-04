#include <ssts/task_scheduler.hpp>
#include <spdlog/spdlog.h>
#include "utils/utils.hpp"

int main() 
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");
    spdlog::info(ssts::version());

    ssts::task_scheduler s(8);
    s.start();
    
    ssts::utils::timer t;
    
    s.every("Foo"s, 2s, []{ spdlog::info("Foo"); });

    s.every("Bar"s, 2s, []{ spdlog::info("Bar"); });

    s.in(1ms, []{ spdlog::info("one millis!"); });

    spdlog::info("before update");
    std::this_thread::sleep_for(3s);

    s.update_interval("foo"s, 100ms); // Wrong task ID
    s.update_interval("Bar"s, 100ms);

    spdlog::info("after update");
    std::this_thread::sleep_for(2s);

    spdlog::info("Task Scheduler shutdown");
    s.stop();

    spdlog::info("Task Scheduler finished");
    return 0;
}
