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

#include "test_geometry_range.hpp"

namespace tc::sdk::tests
{
using types = testing::Types<int, unsigned int, float, double, long>;
TYPED_TEST_SUITE(test_geometry_range_t, types);


TYPED_TEST(test_geometry_range_t, create)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0;
    tc::sdk::range<RangeT> p1{};
    tc::sdk::range<RangeT> p2 = p1;
    auto p3 = tc::sdk::range<RangeT>{};
    const auto p4 = tc::sdk::range<RangeT>();
    const auto p5(p4);
    tc::sdk::range<RangeT> p6(tc::sdk::range<RangeT>{});
    
    auto temp = tc::sdk::range<RangeT>{};
    tc::sdk::range<RangeT> p7(std::move(temp));

    auto validate = [](auto p)
    {
        EXPECT_EQ(p.min(), 0);
        EXPECT_EQ(p.max(), 0);
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

TYPED_TEST(test_geometry_range_t, compare)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 2);
    tc::sdk::range<RangeT> p1(3, 4);
    tc::sdk::range<RangeT> p2(3, 4);

    EXPECT_TRUE(p0 != p1);
    EXPECT_TRUE(p0 != p2);
    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p2 == p1);
}

TYPED_TEST(test_geometry_range_t, getter_setter)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 2);

    EXPECT_EQ(p0.min(), 1);
    EXPECT_EQ(p0.max(), 2);
    
    const RangeT new_min(8);
    p0.set_min(new_min);    
    EXPECT_EQ(p0.min(), new_min);

    const RangeT new_max(9);
    p0.set_max(new_max);    
    EXPECT_EQ(p0.max(), new_max);
}

TYPED_TEST(test_geometry_range_t, getter_setter_out_of_bounds)
{
    using RangeT = TypeParam;
    
    tc::sdk::range<RangeT> p0(5, 6);
    const RangeT new_min(8);
    p0.set_min(new_min);    
    EXPECT_EQ(p0.min(), new_min);
    EXPECT_EQ(p0.max(), new_min);

    tc::sdk::range<RangeT> p1(5, 6);
    const RangeT new_max(3);
    p1.set_max(new_max);    
    EXPECT_EQ(p1.min(), new_max);
    EXPECT_EQ(p1.max(), new_max);
}

TYPED_TEST(test_geometry_range_t, lenght)
{
    using RangeT = TypeParam;
    
    tc::sdk::range<RangeT> p0(5, 8);  
    EXPECT_EQ(p0.lenght(), 3);

    tc::sdk::range<RangeT> p1(0, 1'000'000);  
    EXPECT_EQ(p1.lenght(), 1'000'000);
}

TYPED_TEST(test_geometry_range_t, contains_value)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 4);

    EXPECT_TRUE(p0.contains(1)); // Lower bound is included
    EXPECT_TRUE(p0.contains(2));
    EXPECT_TRUE(p0.contains(3));

    EXPECT_FALSE(p0.contains(0));
    EXPECT_TRUE(p0.contains(4)); // Upper bound is not included
    EXPECT_FALSE(p0.contains(5));
}

TYPED_TEST(test_geometry_range_t, contains_range)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 4);
    tc::sdk::range<RangeT> p1(2, 3);
    tc::sdk::range<RangeT> p2(2, 5);
    tc::sdk::range<RangeT> p3(5, 8);

    EXPECT_TRUE(p0.contains(p1));
    EXPECT_FALSE(p1.contains(p0));
    EXPECT_FALSE(p0.contains(p2));
    EXPECT_FALSE(p0.contains(p3));
}

TYPED_TEST(test_geometry_range_t, intersects)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 4);
    tc::sdk::range<RangeT> p1(2, 3);
    tc::sdk::range<RangeT> p2(2, 5);
    tc::sdk::range<RangeT> p3(5, 8);

    EXPECT_TRUE(p0.intersects(p1));
    EXPECT_TRUE(p1.intersects(p0));
    EXPECT_TRUE(p0.intersects(p2));
   
    EXPECT_FALSE(p1.intersects(p3));
    EXPECT_FALSE(p0.intersects(p3));
    EXPECT_FALSE(p2.intersects(p3)); // Not strictly contained
}

TYPED_TEST(test_geometry_range_t, clamp)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(2, 5);

    // Range bounds
    EXPECT_EQ(p0.clamp(2), p0.min());
    EXPECT_EQ(p0.clamp(5), p0.max());

    // Outside range bounds
    EXPECT_EQ(p0.clamp(1), p0.min());
    EXPECT_EQ(p0.clamp(6), p0.max());

    // Inside range bounds
    EXPECT_EQ(p0.clamp(3), 3);
    EXPECT_EQ(p0.clamp(4), 4);
}

TYPED_TEST(test_geometry_range_t, map)
{
    using RangeT = TypeParam;

    tc::sdk::range<RangeT> p0(0, 100);
    EXPECT_EQ(p0.map(0), p0.min());
    EXPECT_EQ(p0.map(1), p0.max());
    EXPECT_EQ(p0.map(0.1), RangeT(10));
    EXPECT_EQ(p0.map(0.5), RangeT(50));

    tc::sdk::range<RangeT> p1(100, 200);
    EXPECT_EQ(p1.map(0), p1.min());
    EXPECT_EQ(p1.map(1), p1.max());
    EXPECT_EQ(p1.map(0.1), RangeT(110));
    EXPECT_EQ(p1.map(0.5), RangeT(150));
    
    if constexpr(!std::is_same_v<RangeT, unsigned int>)
    {
        tc::sdk::range<RangeT> p2(-10, 10);
        EXPECT_EQ(p2.map(0), p2.min());
        EXPECT_EQ(p2.map(1), p2.max());
        EXPECT_EQ(p2.map(0.05), RangeT(-9));
        EXPECT_EQ(p2.map(0.10), RangeT(-8));
        EXPECT_EQ(p2.map(0.50), RangeT(0));
        EXPECT_EQ(p2.map(0.90), RangeT(8));
        EXPECT_EQ(p2.map(0.95), RangeT(9));
    }
}

TYPED_TEST(test_geometry_range_t, union)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 3);
    tc::sdk::range<RangeT> p1(4, 6);
    tc::sdk::range<RangeT> p2(2, 5);

    EXPECT_EQ(p0.get_union(p1), tc::sdk::range<RangeT>(1, 6));
    EXPECT_EQ(p0.get_union(p2), tc::sdk::range<RangeT>(1, 5));
    EXPECT_EQ(p1.get_union(p2), tc::sdk::range<RangeT>(2, 6));

    // Commutative check
    EXPECT_EQ(p0.get_union(p1), p1.get_union(p0));
    EXPECT_EQ(p0.get_union(p2), p2.get_union(p0));
    EXPECT_EQ(p1.get_union(p2), p2.get_union(p1));
}

TYPED_TEST(test_geometry_range_t, intersection)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 3);
    tc::sdk::range<RangeT> p1(4, 6);
    tc::sdk::range<RangeT> p2(2, 5);

    EXPECT_EQ(p0.get_intersection(p1), tc::sdk::range<RangeT>()); // No intersection results in an empty range
    EXPECT_EQ(p0.get_intersection(p2), tc::sdk::range<RangeT>(2, 3));
    EXPECT_EQ(p1.get_intersection(p2), tc::sdk::range<RangeT>(4, 5));

    // Commutative check
    EXPECT_EQ(p0.get_intersection(p1), p1.get_intersection(p0));
    EXPECT_EQ(p0.get_intersection(p2), p2.get_intersection(p0));
    EXPECT_EQ(p1.get_intersection(p2), p2.get_intersection(p1));
}

TYPED_TEST(test_geometry_range_t, to_string)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p0(1, 2);
    tc::sdk::range<RangeT> p1(RangeT(-3), RangeT(-4));

    if constexpr(std::is_same_v<RangeT, unsigned int>)
    {
        // Overflow!
        // The RangeT is "unsigned int" so when using negative values it overflows.
        // So the expected values are:
        // std::numeric_limits<unsigned int>::max() + 1 + p1.x() 
        // = 4294967296 + 1 + 3 
        // = 4294967293
        // and
        // std::numeric_limits<unsigned int>::max() + 1 + p1.y() 
        // = 4294967296 + 1 + 4 
        // = 4294967292
        auto x =  std::numeric_limits<unsigned int>::max() + 1 + p1.min();
        auto y =  std::numeric_limits<unsigned int>::max() + 1 + p1.max();
        EXPECT_EQ(p1.to_string(), "(" + std::to_string(x) + ":" + std::to_string(y) + ")");

        // No overflow on positive values.
        EXPECT_EQ(p0.to_string(), "(1:2)");
    }

    if constexpr(std::is_same_v<RangeT, int> || std::is_same_v<RangeT, long>)
    {
        EXPECT_EQ(p0.to_string(), "(1:2)");
        EXPECT_EQ(p1.to_string(), "(-3:-4)");
    }

    if constexpr(std::is_same_v<RangeT, float> || std::is_same_v<RangeT, double>)
    {
        EXPECT_EQ(p0.to_string(), "(1.000000:2.000000)");
        EXPECT_EQ(p1.to_string(), "(-3.000000:-4.000000)");
    }
    
}

// NOLINTNEXTLINE
TYPED_TEST(test_geometry_range_t, ostream)
{
    using RangeT = TypeParam;
    tc::sdk::range<RangeT> p(1, 2);

    std::stringstream stream;
    stream << p.to_string();
    EXPECT_STREQ(stream.str().c_str(), p.to_string().c_str());
}

}

