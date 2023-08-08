#include "test_uuid.hpp"

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_uuid, deafult)
{
    {
        const auto uuid = tc::sdk::uuid();
        EXPECT_EQ(uuid, null_uuid);
        EXPECT_EQ(uuid.str(), null_uuid_str);
    }
    {
        tc::sdk::uuid uuid;
        EXPECT_EQ(uuid, null_uuid);
        EXPECT_EQ(uuid.str(), null_uuid_str);
    }
    {
        tc::sdk::uuid uuid {};
        EXPECT_EQ(uuid, null_uuid);
        EXPECT_EQ(uuid.str(), null_uuid_str);
    }
    {
        const tc::sdk::uuid uuid {};
        EXPECT_EQ(uuid, null_uuid);
        EXPECT_EQ(uuid.str(), null_uuid_str);
    }
}

// NOLINTNEXTLINE
TEST_F(test_uuid, assign)
{
    const auto uuid_1 = create();
    EXPECT_NE(uuid_1, null_uuid);

    tc::sdk::uuid uuid_2 {};
    uuid_2 = uuid_1;
    EXPECT_EQ(uuid_1, uuid_2);
}

// NOLINTNEXTLINE
TEST_F(test_uuid, equality)
{
    const auto uuid_1 = create();
    const auto uuid_2 = create();

    EXPECT_FALSE(uuid_1 == uuid_2);
    EXPECT_TRUE(uuid_1 != uuid_2);

    EXPECT_TRUE(uuid_1 != null_uuid);
    EXPECT_TRUE(uuid_2 != null_uuid);
}

// NOLINTNEXTLINE
TEST_F(test_uuid, comparison)
{
    const auto uuid = create();

    EXPECT_TRUE(uuid > null_uuid);
    EXPECT_TRUE(uuid >= null_uuid);
    EXPECT_FALSE(uuid < null_uuid);
    EXPECT_FALSE(uuid <= null_uuid);
}

// NOLINTNEXTLINE
TEST_F(test_uuid, str)
{
    const auto uuid_1 = create();
    const auto uuid_2 = create();

    EXPECT_NE(uuid_1.str(), uuid_2.str());
    EXPECT_NE(uuid_1.str(), null_uuid.str());
    EXPECT_NE(uuid_2.str(), null_uuid.str());
}

// NOLINTNEXTLINE
TEST_F(test_uuid, bytes)
{
    const auto uuid_1 = create();
    const auto uuid_2 = create();

    EXPECT_NE(uuid_1.bytes(), uuid_2.bytes());
    EXPECT_NE(uuid_1.bytes(), null_uuid.bytes());
    EXPECT_NE(uuid_2.bytes(), null_uuid.bytes());
}

// NOLINTNEXTLINE
TEST_F(test_uuid, hash)
{
    const auto uuid_1 = create();
    const auto uuid_2 = create();

    EXPECT_NE(uuid_1.hash(), uuid_2.hash());
    EXPECT_NE(uuid_1.hash(), null_uuid.hash());
    EXPECT_NE(uuid_2.hash(), null_uuid.hash());
}

}