#pragma once

#include <functional>

namespace tc::sdk
{
/*!
 * \brief Install signal handlers for SIGINT, SIGILL, SIGABRT, SIGTERM OS signals.
 * \param callback user defined callback that is invoked with the signal id as argument (OS dependent) when the signal is rised.
 */
void install_signal_handlers(std::function<void(const char*, int)> callback = {});

/*!
 * \brief Wait until a shutdown signal handler has been invoked.
 * 
 * This function blocks until either an OS signal is rised or tc::sdk::quit() is called from another thread.
 */
void wait_for_quit();

/*!
 * \brief Unlock the thread that is waiting on tc::sdk::wait_for_quit()
 */
void quit(const char* message, int signal=0);
}
