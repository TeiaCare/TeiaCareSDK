#pragma once

#include <gtest/gtest.h>
#include <sdk/uuid_generator.hpp>

namespace tc::sdk::tests
{
class test_uuid_generator : public ::testing::Test
{
protected:
    explicit test_uuid_generator() : g{ tc::sdk::uuid_generator::instance() }
    {
    }

    ~test_uuid_generator() override
    {
    }

    tc::sdk::uuid_generator& g;
    const std::string null_uuid_str = "00000000-0000-0000-0000-000000000000";
};

}

namespace tc::sdk
{
    bool operator==(const tc::sdk::uuid_generator& g1, const tc::sdk::uuid_generator& g2) { return &g1 == &g2; }
}
