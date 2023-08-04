#pragma once

#include <gtest/gtest.h>
#include <sdk/event_dispatcher.hpp>

namespace tc::sdk::tests
{
class test_event_dispatcher : public ::testing::Test
{
protected:
    explicit test_event_dispatcher()
        : e { std::make_unique<tc::sdk::event_dispatcher>() }
    {
    }

    ~test_event_dispatcher() override
    {
        e->stop();
    }

    std::unique_ptr<tc::sdk::event_dispatcher> e;
};

}
