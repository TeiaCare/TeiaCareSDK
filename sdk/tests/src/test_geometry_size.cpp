// Copyright 2024 TeiaCare
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "test_geometry_size.hpp"

namespace tc::sdk::tests
{
using types = testing::Types<int, unsigned int, float, double, long, long long>;
TYPED_TEST_SUITE(test_geometry_size_t, types);

TYPED_TEST(test_geometry_size_t, create)
{
    using SizeT = TypeParam;
    tc::sdk::size<SizeT> p0;
    tc::sdk::size<SizeT> p1{};
    tc::sdk::size<SizeT> p2 = p1;
    auto p3 = tc::sdk::size<SizeT>{};
    const auto p4 = tc::sdk::size<SizeT>();
    const auto p5(p4);
    tc::sdk::size<SizeT> p6(tc::sdk::size<SizeT>{});

    auto temp = tc::sdk::size<SizeT>{};
    tc::sdk::size<SizeT> p7(std::move(temp));

    auto validate = [](auto p) {
        EXPECT_EQ(p.width(), 0);
        EXPECT_EQ(p.height(), 0);
        EXPECT_TRUE(p.is_null());
    };

    validate(p0);
    validate(p1);
    validate(p2);
    validate(p3);
    validate(p4);
    validate(p5);
    validate(p6);
    validate(p7);
}

TYPED_TEST(test_geometry_size_t, compare)
{
    using SizeT = TypeParam;
    tc::sdk::size<SizeT> p0(1, 2);
    tc::sdk::size<SizeT> p1(3, 4);
    tc::sdk::size<SizeT> p2(3, 4);

    EXPECT_TRUE(p0 != p1);
    EXPECT_TRUE(p0 != p2);
    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p2 == p1);
}

TYPED_TEST(test_geometry_size_t, getter_setter)
{
    using SizeT = TypeParam;
    tc::sdk::size<SizeT> p0(1, 2);

    EXPECT_EQ(p0.width(), 1);
    EXPECT_EQ(p0.height(), 2);

    const SizeT new_w(8);
    p0.set_width(new_w);
    EXPECT_EQ(p0.width(), new_w);

    const SizeT new_h(9);
    p0.set_height(new_h);
    EXPECT_EQ(p0.height(), new_h);
}

TYPED_TEST(test_geometry_size_t, to_string)
{
    using SizeT = TypeParam;
    tc::sdk::size<SizeT> p0(1, 2);
    tc::sdk::size<SizeT> p1(SizeT(-3), SizeT(-4));

    if constexpr (std::is_same_v<SizeT, unsigned int>)
    {
        // Overflow!
        // The SizeT is "unsigned int" so when using negative values it overflows.
        // So the expected values are:
        // std::numeric_limits<unsigned int>::max() + 1 + p1.x()
        // = 4294967296 + 1 + 3
        // = 4294967293
        // and
        // std::numeric_limits<unsigned int>::max() + 1 + p1.y()
        // = 4294967296 + 1 + 4
        // = 4294967292
        auto x = std::numeric_limits<unsigned int>::max() + 1 + p1.width();
        auto y = std::numeric_limits<unsigned int>::max() + 1 + p1.height();
        EXPECT_EQ(p1.to_string(), "(" + std::to_string(x) + "x" + std::to_string(y) + ")");

        // No overflow on positive values.
        EXPECT_EQ(p0.to_string(), "(1x2)");
    }

    if constexpr (std::is_same_v<SizeT, int> || std::is_same_v<SizeT, long>)
    {
        EXPECT_EQ(p0.to_string(), "(1x2)");
        EXPECT_EQ(p1.to_string(), "(-3x-4)");
    }

    if constexpr (std::is_same_v<SizeT, float> || std::is_same_v<SizeT, double>)
    {
        EXPECT_EQ(p0.to_string(), "(1.000000x2.000000)");
        EXPECT_EQ(p1.to_string(), "(-3.000000x-4.000000)");
    }
}

// NOLINTNEXTLINE
TYPED_TEST(test_geometry_size_t, ostream)
{
    using SizeT = TypeParam;
    tc::sdk::size<SizeT> p(1, 2);

    std::stringstream stream;
    stream << p;
    EXPECT_STREQ(stream.str().c_str(), p.to_string().c_str());
}

}
