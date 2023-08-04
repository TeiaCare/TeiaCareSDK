#include <sdk/shutdown_signal_handler.hpp>
#include <csignal>
#include <future>
#include <string>

namespace tc::sdk
{
static std::function<void(const char*)> shutdown_callback;
static std::promise<void> shutdown_notifier;
static std::future<void> shutdown_waiter;
static std::once_flag shutdown_flag;

void quit(const char* message)
{
    std::call_once(shutdown_flag, [&](const char* message){
        if(shutdown_callback)
        {
            shutdown_callback(message);
        }

        shutdown_notifier.set_value(); 
    }, message);
}

void wait_for_shutdown()
{
    shutdown_waiter.wait();
}

void shutdown_signal_handler(int signal)
{
    const std::string message = std::string("Shutdown signal received: ") + std::to_string(signal);
    tc::sdk::quit(message.c_str());
}

void install_shutdown_signal_handlers(std::function<void(const char*)> callback)
{
    shutdown_callback = callback;
    shutdown_notifier = std::promise<void>();
    shutdown_waiter = shutdown_notifier.get_future();

    std::signal(SIGINT, shutdown_signal_handler);
    std::signal(SIGILL, shutdown_signal_handler);
    std::signal(SIGABRT, shutdown_signal_handler);
    std::signal(SIGTERM, shutdown_signal_handler);
}

}
