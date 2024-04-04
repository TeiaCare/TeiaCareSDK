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

#include "test_geometry_line.hpp"

namespace tc::sdk::tests
{
using types = testing::Types<int, float, double, long, long long>;
TYPED_TEST_SUITE(test_geometry_line_t, types);

TYPED_TEST(test_geometry_line_t, create)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p0;
    tc::sdk::line<LineT> p1{};
    tc::sdk::line<LineT> p2 = p1;
    auto p3 = tc::sdk::line<LineT>{};
    const auto p4 = tc::sdk::line<LineT>();
    const auto p5(p4);
    tc::sdk::line<LineT> p6(tc::sdk::line<LineT>{});
    
    auto temp = tc::sdk::line<LineT>{};
    tc::sdk::line<LineT> p7(std::move(temp));

    auto validate = [](auto p)
    {
        EXPECT_EQ(p.start(), tc::sdk::point<LineT>());
        EXPECT_EQ(p.end(), tc::sdk::point<LineT>());
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

TYPED_TEST(test_geometry_line_t, compare)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 9));
    tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(9, 9), tc::sdk::point<LineT>(1, 1));
    tc::sdk::line<LineT> p2(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(2, 3));

    EXPECT_TRUE(p0 != p2);
    EXPECT_TRUE(p1 != p2);
    EXPECT_TRUE(p0 == p1); // Line direction does not matter, so p0 equals p1
    EXPECT_TRUE(p1 == p0);
}

TYPED_TEST(test_geometry_line_t, horizontal)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 9));
    tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(1, 9));
    tc::sdk::line<LineT> p2(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 1));

    EXPECT_FALSE(p0.is_horizontal()); // False, Diagonal
    EXPECT_FALSE(p1.is_horizontal()); // False, Vertical
    EXPECT_TRUE(p2.is_horizontal()); // True, horizontal
}

TYPED_TEST(test_geometry_line_t, vertical)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 9));
    tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(1, 9));
    tc::sdk::line<LineT> p2(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 1));

    EXPECT_FALSE(p0.is_vertical()); // False, Diagonal
    EXPECT_TRUE(p1.is_vertical()); // True, Vertical
    EXPECT_FALSE(p2.is_vertical()); // False, horizontal
}

TYPED_TEST(test_geometry_line_t, getter_setter)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 9));

    EXPECT_EQ(p0.start(), tc::sdk::point<LineT>(1, 1));
    EXPECT_EQ(p0.end(), tc::sdk::point<LineT>(9, 9));
    
    const tc::sdk::point<LineT> new_start(2, 2);
    p0.set_start(new_start);    
    EXPECT_EQ(p0.start(), new_start);

    const tc::sdk::point<LineT> new_end(8, 8);
    p0.set_end(new_end);    
    EXPECT_EQ(p0.end(), new_end);
}

TYPED_TEST(test_geometry_line_t, intersection)
{
    using LineT = TypeParam;
    auto check_intersection = [](tc::sdk::line<LineT> line1, tc::sdk::line<LineT> line2, tc::sdk::point<double> intersection_point)
    {
        EXPECT_TRUE(line1.intersects(line2));
        EXPECT_TRUE(line2.intersects(line1));

        EXPECT_TRUE(line1.get_intersection(line2).has_value());
        EXPECT_TRUE(line2.get_intersection(line1).has_value());

        EXPECT_EQ(line1.get_intersection(line2).value(), intersection_point);
        EXPECT_EQ(line2.get_intersection(line1).value(), intersection_point);
    };

    {
        tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(0, 0), tc::sdk::point<LineT>(2, 2));
        tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(0, 2), tc::sdk::point<LineT>(2, 0));
        auto i0 = tc::sdk::point<double>(1.0, 1.0);
        check_intersection(p0, p1, i0);
    }

    {
        tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(0, 2), tc::sdk::point<LineT>(2, 2));
        tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(1, 0), tc::sdk::point<LineT>(1, 4));
        check_intersection(p0, p1, tc::sdk::point<double>(1.0, 2.0));
    }

    {
        tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(0, 3), tc::sdk::point<LineT>(2, 1));
        tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(1, 0), tc::sdk::point<LineT>(1, 4));
        check_intersection(p0, p1, tc::sdk::point<double>(1.0, 2.0));
    }

    {
        tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(0, 2), tc::sdk::point<LineT>(2, 2));
        tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(2, 0), tc::sdk::point<LineT>(0, 4));
        check_intersection(p0, p1, tc::sdk::point<double>(1.0, 2.0));
    }

    if constexpr(std::is_same_v<LineT, float> || std::is_same_v<LineT, double>)
    {
        tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(0.0, 0.5), tc::sdk::point<LineT>(2.0, 2.5));
        tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(1.5, 0.0), tc::sdk::point<LineT>(0.0, 4.5));
        check_intersection(p0, p1, tc::sdk::point<double>(1.0, 1.5));
    }

    if constexpr(std::is_same_v<LineT, float> || std::is_same_v<LineT, double>)
    {
        tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(0.0, -0.5), tc::sdk::point<LineT>(-2.0, -2.5));
        tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(-1.5, 0.0), tc::sdk::point<LineT>(0.0, -4.5));
        check_intersection(p0, p1, tc::sdk::point<double>(-1.0, -1.5));
    }
}

TYPED_TEST(test_geometry_line_t, to_string)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p0(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 9));
    tc::sdk::line<LineT> p1(tc::sdk::point<LineT>(-1, -2), tc::sdk::point<LineT>(-3, -4));

    if constexpr(std::is_same_v<LineT, int> || std::is_same_v<LineT, long> || std::is_same_v<LineT, long long>)
    {
        EXPECT_EQ(p0.to_string(), "((1, 1) : (9, 9))");
        EXPECT_EQ(p1.to_string(), "((-1, -2) : (-3, -4))");
    }

    if constexpr(std::is_same_v<LineT, float> || std::is_same_v<LineT, double>)
    {
        EXPECT_EQ(p0.to_string(), "((1.000000, 1.000000) : (9.000000, 9.000000))");
        EXPECT_EQ(p1.to_string(), "((-1.000000, -2.000000) : (-3.000000, -4.000000))");
    }
    
}

// NOLINTNEXTLINE
TYPED_TEST(test_geometry_line_t, ostream)
{
    using LineT = TypeParam;
    tc::sdk::line<LineT> p(tc::sdk::point<LineT>(1, 1), tc::sdk::point<LineT>(9, 9));

    std::stringstream stream;
    stream << p.to_string();
    EXPECT_STREQ(stream.str().c_str(), p.to_string().c_str());
}

}
