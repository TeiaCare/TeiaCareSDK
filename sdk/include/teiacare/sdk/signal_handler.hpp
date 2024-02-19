// Copyright 2024 TeiaCare
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
