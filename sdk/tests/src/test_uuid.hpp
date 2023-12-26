#pragma once

#include <gtest/gtest.h>
#include <sdk/uuid.hpp>
#include <sdk/uuid_generator.hpp>

namespace tc::sdk::tests
{
class test_uuid : public ::testing::Test
{
protected:
    explicit test_uuid()
    {
    }

    const tc::sdk::uuid create() const
    {
        auto& generator = tc::sdk::uuid_generator::instance();
        return generator.create();
    }

    const tc::sdk::uuid null_uuid = tc::sdk::uuid();
    const std::string null_uuid_str = "00000000-0000-0000-0000-000000000000";
};

}
