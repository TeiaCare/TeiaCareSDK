#pragma once

#include <functional>

namespace tc::sdk
{
void install_shutdown_signal_handlers(std::function<void(const char*)> callback = {});
void wait_for_shutdown();
void quit(const char* message);
}
