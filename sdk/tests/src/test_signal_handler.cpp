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

#include "test_signal_handler.hpp"

#include <barrier>
#include <csignal>

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_signal_handler_suite,
    test_signal_handler,
    testing::Values(
        signal_handler_params({.signal = SIGINT,
                               .name = "SIGINT",
                               .callback = [](const char* msg, int sig) {EXPECT_EQ(SIGINT, sig); EXPECT_STREQ("SIGINT", msg); }}),
        signal_handler_params({.signal = SIGABRT,
                               .name = "SIGABRT",
                               .callback = [](const char* msg, int sig) {EXPECT_EQ(SIGABRT, sig); EXPECT_STREQ("SIGABRT", msg); }}),
        signal_handler_params({.signal = SIGTERM,
                               .name = "SIGTERM",
                               .callback = [](const char* msg, int sig) {EXPECT_EQ(SIGTERM, sig); EXPECT_STREQ("SIGTERM", msg); }})),
    [](auto info) { return info.param.name; });

// NOLINTNEXTLINE
TEST_P(test_signal_handler, raise)
{
    const signal_handler_params params = GetParam();
    tc::sdk::install_signal_handlers(params.callback);

    signal_thread = std::jthread([&params] {
        std::raise(params.signal);
    });

    tc::sdk::wait_for_quit();
}

// NOLINTNEXTLINE
TEST_P(test_signal_handler, quit)
{
    const signal_handler_params params = GetParam();
    tc::sdk::install_signal_handlers(params.callback);

    signal_thread = std::jthread([&params] {
        tc::sdk::quit(params.name, params.signal);
    });

    tc::sdk::wait_for_quit();
}

// NOLINTNEXTLINE
TEST_F(test_signal_handler_shutdown, null_callback)
{
    tc::sdk::install_signal_handlers(nullptr);

    std::barrier sync(2, []() noexcept {});
    signal_thread = std::jthread([&sync] {
        sync.arrive_and_wait();
        tc::sdk::quit("NULL_CALLBACK");
    });

    auto arrival_token = sync.arrive();
    tc::sdk::wait_for_quit();
    (void)arrival_token;
}

// NOLINTNEXTLINE
TEST_F(test_signal_handler_shutdown, quit_arguments)
{
    const auto callback = [](const char* msg, int signal) {
        EXPECT_EQ("CUSTOM_MSG", msg);
        EXPECT_EQ(1234, signal);
    };
    tc::sdk::install_signal_handlers(callback);

    std::barrier sync(2, []() noexcept {});
    signal_thread = std::jthread([&sync] {
        sync.arrive_and_wait();
        tc::sdk::quit("CUSTOM_MSG", 1234);
    });

    auto arrival_token = sync.arrive();
    tc::sdk::wait_for_quit();
    (void)arrival_token;
}

// NOLINTNEXTLINE
TEST_F(test_signal_handler_shutdown, multiple_quit)
{
    constexpr int total_count = 1'000;
    std::barrier sync(2, []() noexcept {});

    int callback_count = 0;
    const auto callback = [&callback_count](const char*, int) { ++callback_count; };
    tc::sdk::install_signal_handlers(callback);

    signal_thread = std::jthread([&sync] {
        for (auto i = 0; i < total_count; ++i)
        {
            sync.arrive_and_wait();
            tc::sdk::quit("QUIT");
        }
    });

    for (auto i = 0; i < total_count; ++i)
    {
        auto arrival_token = sync.arrive();
        tc::sdk::wait_for_quit();
        (void)arrival_token;
    }

    signal_thread.join();
    EXPECT_EQ(total_count, callback_count);
}

}
