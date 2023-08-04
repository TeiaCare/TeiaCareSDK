#pragma once

#include <gtest/gtest.h>
#include <sdk/high_precision_timer.hpp>

namespace tc::sdk::tests
{
class test_high_precision_timer : public ::testing::Test
{
protected:
    explicit test_high_precision_timer()
    {
    }

    ~test_high_precision_timer() override
    {
        t.stop();
    }

    tc::sdk::high_precision_timer t;
};

}
