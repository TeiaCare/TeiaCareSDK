#include <sdk/signal_handler.hpp>
#include <csignal>
#include <semaphore>

namespace tc::sdk
{
static std::function<void(const char*, int)> signal_callback;
static std::binary_semaphore sync(0);

void quit(const char* message, int signal)
{
    if(signal_callback)
    {
        signal_callback(message, signal);
    }

    sync.release();
    
    // static std::once_flag signal_flag;
    // std::call_once(signal_flag, quit, message, signal);
}

void wait_for_quit()
{
    sync.acquire();
}

void signal_handler(int signal)
{
    const char* message;
    switch(signal)
    {
        case SIGINT:   message = "SIGINT";   break;
        case SIGABRT:  message = "SIGABRT";  break;
        case SIGTERM:  message = "SIGTERM";  break;
        default: message = "UNKNOWN_SIGNAL";
    }

    tc::sdk::quit(message, signal);
}

void install_signal_handlers(std::function<void(const char*, int)> callback)
{
    signal_callback = callback;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGABRT, signal_handler);
    std::signal(SIGTERM, signal_handler);
}

}
