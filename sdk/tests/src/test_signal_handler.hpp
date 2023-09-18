#pragma once

#include <gtest/gtest.h>
#include <sdk/signal_handler.hpp>
#include <thread>

namespace tc::sdk::tests
{
struct signal_handler_params
{
    int signal;
    const char* name;
    std::function<void(const char*, int)> callback;
};

class test_signal_handler_shutdown : public testing::Test
{
protected:
    explicit test_signal_handler_shutdown()
    {
    }

    ~test_signal_handler_shutdown() override
    {
    }

    std::jthread signal_thread;
};

class test_signal_handler 
    : public test_signal_handler_shutdown
    , public testing::WithParamInterface<signal_handler_params>
{
};

}
