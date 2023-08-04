#pragma once

#include <gtest/gtest.h>
#include <sdk/thread_pool.hpp>

namespace tc::sdk::tests
{
class test_thread_pool : public ::testing::Test
{
protected:
    explicit test_thread_pool()
        : tp { std::make_unique<tc::sdk::thread_pool>() }
    {
    }

    ~test_thread_pool() override
    {
        tp->stop();
    }

    const unsigned int num_threads = 4;
    const unsigned int max_threads_count = std::thread::hardware_concurrency();
    std::unique_ptr<tc::sdk::thread_pool> tp;
};

}
