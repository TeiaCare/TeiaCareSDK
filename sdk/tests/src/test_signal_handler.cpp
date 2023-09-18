#include "test_signal_handler.hpp"
#include <csignal>
#include <barrier>
#include <syncstream>

namespace tc::sdk::tests
{
//NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_signal_handler_suite,
    test_signal_handler,
    testing::Values(
        signal_handler_params({ 
            .signal=SIGINT,
            .name="SIGINT",
            .callback=[](const char* msg, int sig){EXPECT_EQ(SIGINT, sig); EXPECT_EQ("SIGINT", msg);} 
        }),
        signal_handler_params({
            .signal=SIGILL,
            .name="SIGILL",
            .callback=[](const char* msg, int sig){EXPECT_EQ(SIGILL, sig); EXPECT_EQ("SIGILL", msg);} 
        }),
        signal_handler_params({
            .signal=SIGABRT,
            .name="SIGABRT",
            .callback=[](const char* msg, int sig){EXPECT_EQ(SIGABRT, sig); EXPECT_EQ("SIGABRT", msg);} 
        }),
        signal_handler_params({
            .signal=SIGFPE,
            .name="SIGFPE",
            .callback=[](const char* msg, int sig){EXPECT_EQ(SIGFPE, sig); EXPECT_EQ("SIGFPE", msg);} 
        }),
        signal_handler_params({
            .signal=SIGSEGV,
            .name="SIGSEGV",
            .callback=[](const char* msg, int sig){EXPECT_EQ(SIGSEGV, sig); EXPECT_EQ("SIGSEGV", msg);} 
        }),
        signal_handler_params({
            .signal=SIGTERM,
            .name="SIGTERM",
            .callback=[](const char* msg, int sig){EXPECT_EQ(SIGTERM, sig); EXPECT_EQ("SIGTERM", msg);} 
        })
    ), [](auto info) { return info.param.name; }
);

// NOLINTNEXTLINE
TEST_P(test_signal_handler, raise)
{
    const signal_handler_params params = GetParam();
    tc::sdk::install_signal_handlers(params.callback);
    
    signal_thread = std::jthread([&params]{
        std::raise(params.signal);
    });

    tc::sdk::wait_for_quit();
}

// NOLINTNEXTLINE
TEST_P(test_signal_handler, quit)
{
    const signal_handler_params params = GetParam();
    tc::sdk::install_signal_handlers(params.callback);
    
    signal_thread = std::jthread([&params]{
        tc::sdk::quit(params.name, params.signal);
    });

    tc::sdk::wait_for_quit();
}

// NOLINTNEXTLINE
TEST_F(test_signal_handler_shutdown, null_callback)
{
    tc::sdk::install_signal_handlers(nullptr);

    std::barrier sync(2, []() noexcept {});
    signal_thread = std::jthread([&sync]{
        sync.arrive_and_wait();
        tc::sdk::quit("NULL_CALLBACK");
    });

    auto arrival_token = sync.arrive();
    tc::sdk::wait_for_quit();
}

// NOLINTNEXTLINE
TEST_F(test_signal_handler_shutdown, quit_arguments)
{
    const auto callback = [](const char* msg, int signal){
        EXPECT_EQ("CUSTOM_MSG", msg);
        EXPECT_EQ(1234, signal);
    };
    tc::sdk::install_signal_handlers(callback);

    std::barrier sync(2, []() noexcept {});
    signal_thread = std::jthread([&sync]{
        sync.arrive_and_wait();
        tc::sdk::quit("CUSTOM_MSG", 1234);
    });

    auto arrival_token = sync.arrive();
    tc::sdk::wait_for_quit();
}

// NOLINTNEXTLINE
TEST_F(test_signal_handler_shutdown, multiple_quit)
{
    constexpr size_t total_count = 1'000;
    std::barrier sync(2, []() noexcept {});

    int callback_count = 0;
    const auto callback = [&callback_count](const char*, int){ ++callback_count; };
    tc::sdk::install_signal_handlers(callback);

    signal_thread = std::jthread([&sync, total_count]{
        for(auto i=0; i<total_count; ++i)
        {
            sync.arrive_and_wait();
            tc::sdk::quit("QUIT");
        }
    });

    for(auto i=0; i<total_count; ++i)
    {
        auto arrival_token = sync.arrive();
        tc::sdk::wait_for_quit();
    }

    signal_thread.join();
    EXPECT_EQ(total_count, callback_count);
}

}
