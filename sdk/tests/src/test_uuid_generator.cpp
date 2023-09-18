#include "test_uuid_generator.hpp"
#include <algorithm>

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_uuid_generator, instance)
{
    EXPECT_EQ(g, tc::sdk::uuid_generator::instance());
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, create_unique)
{
    std::array<tc::sdk::uuid, 1'000> uuids;
    std::generate(uuids.begin(), uuids.end(), [this]{ return g.create(); });

    auto is_unique = [](auto uuids) -> bool {
        std::sort(uuids.begin(), uuids.end());
        const auto last_unique = std::unique(uuids.begin(), uuids.end());
        return last_unique == uuids.end();
    };

    EXPECT_TRUE(is_unique(uuids));
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_null)
{
    {
        const auto uuid = g.from_string(null_uuid_str);
        EXPECT_EQ(uuid, tc::sdk::uuid());
    }

    {
        const auto uuid = g.from_string(null_uuid_str.c_str());
        EXPECT_EQ(uuid, tc::sdk::uuid());
    }
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_empty)
{
    {
        const auto uuid = g.from_string(std::string("12345678901234567890"));
        EXPECT_NE(uuid, tc::sdk::uuid());
    }

    {
        const auto uuid = g.from_string("12345678901234567890");
        EXPECT_NE(uuid, tc::sdk::uuid());
    }
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_valid_uuid)
{
    const char* uuid_str = "44b6f253-1414-4444-b10e-d3d92c15f6f2";

    {
        const auto uuid = g.from_string(std::string(uuid_str));
        EXPECT_EQ(uuid.str(), uuid_str);
    }

    {
        const auto uuid = g.from_string(uuid_str);
        EXPECT_EQ(uuid.str(), uuid_str);
    }
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_not_valid_uuid)
{
    const char* uuid_str = "12345678-abcd-efgh-123456789012";

    {
        const auto uuid = g.from_string(std::string(uuid_str));
        EXPECT_NE(uuid, tc::sdk::uuid());
    }

    {
        const auto uuid = g.from_string(uuid_str);
        EXPECT_NE(uuid, tc::sdk::uuid());
    }
}

// NOLINTNEXTLINE
TEST_F(test_uuid_generator, from_string_wrong_format)
{
    const char* uuid_str = "12345678-abcd-";

    {
        const auto uuid = g.from_string(std::string(uuid_str));
        EXPECT_NE(uuid, tc::sdk::uuid());
    }

    {
        const auto uuid = g.from_string(uuid_str);
        EXPECT_NE(uuid, tc::sdk::uuid());
    }
}

}
