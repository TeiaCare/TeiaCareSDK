#pragma once

#include <gtest/gtest.h>
#include <sdk/stopwatch.hpp>

namespace tc::sdk::tests
{
class test_stopwatch : public ::testing::Test
{
protected:
    explicit test_stopwatch()
    {
    }

    ~test_stopwatch() override
    {
    }

    tc::sdk::stopwatch s;
};

}