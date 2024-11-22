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

#include "test_geometry_point.hpp"

namespace tc::sdk::tests
{
using types = testing::Types<int, unsigned int, float, double, long, long long>;
TYPED_TEST_SUITE(test_geometry_point_t, types);

TYPED_TEST(test_geometry_point_t, create)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0;
    tc::sdk::point<PointT> p1{};
    tc::sdk::point<PointT> p2 = p1;
    auto p3 = tc::sdk::point<PointT>{};
    const auto p4 = tc::sdk::point<PointT>();
    const auto p5(p4);
    tc::sdk::point<PointT> p6(tc::sdk::point<PointT>{});

    auto temp = tc::sdk::point<PointT>{};
    tc::sdk::point<PointT> p7(std::move(temp));

    auto validate = [](auto p) {
        EXPECT_EQ(p.x(), 0);
        EXPECT_EQ(p.y(), 0);
        EXPECT_TRUE(p.is_origin());
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

TYPED_TEST(test_geometry_point_t, compare)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);
    tc::sdk::point<PointT> p2(3, 4);

    EXPECT_TRUE(p0 != p1);
    EXPECT_TRUE(p0 != p2);
    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p2 == p1);
}

TYPED_TEST(test_geometry_point_t, getter_setter)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);

    EXPECT_EQ(p0.x(), 1);
    EXPECT_EQ(p0.y(), 2);

    const PointT new_x(8);
    p0.set_x(new_x);
    EXPECT_EQ(p0.x(), new_x);

    const PointT new_y(9);
    p0.set_y(new_y);
    EXPECT_EQ(p0.y(), new_y);
}

TYPED_TEST(test_geometry_point_t, add_delta)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);

    const auto delta = PointT(2);
    p0.add_delta(delta, delta);
    EXPECT_EQ(p0, p1);
}

TYPED_TEST(test_geometry_point_t, sub_delta)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);

    const auto delta = PointT(2);
    p1.sub_delta(delta, delta);
    EXPECT_EQ(p1, p0);
}

TYPED_TEST(test_geometry_point_t, distance_origin)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);
    const auto abs_error = PointT(0.00001);

    const auto d0 = p0.distance_from_origin();
    const auto expected_distance0 = PointT(2.236068); // sqrt(1^2 + 2^2) = 2.236068
    EXPECT_NEAR(static_cast<double>(d0), static_cast<double>(expected_distance0), static_cast<double>(abs_error));

    const auto d1 = p1.distance_from_origin();
    const auto expected_distance1 = PointT(5); // sqrt(3^2 + 4^2) = 5
    EXPECT_NEAR(static_cast<double>(d1), static_cast<double>(expected_distance1), static_cast<double>(abs_error));
}

TYPED_TEST(test_geometry_point_t, distance_point)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);
    const auto abs_error = PointT(0.00001);

    const auto expected_distance = PointT(2.828427); // sqrt((1+3)^2 + (2+4)^2) = 2.828427

    const auto d0 = p0.distance(p1);
    EXPECT_NEAR(static_cast<double>(d0), static_cast<double>(expected_distance), static_cast<double>(abs_error));

    const auto d1 = p1.distance(p0);
    EXPECT_NEAR(static_cast<double>(d1), static_cast<double>(expected_distance), static_cast<double>(abs_error));
}

TYPED_TEST(test_geometry_point_t, operator_add)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);

    auto op = p0 + p1;
    auto temp = p0;
    temp += p1;

    EXPECT_EQ(op.x(), 4);
    EXPECT_EQ(op.y(), 6);
    EXPECT_EQ(temp.x(), 4);
    EXPECT_EQ(temp.y(), 6);
}

TYPED_TEST(test_geometry_point_t, operator_sub)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(3, 4);

    auto op = p0 - p1;
    auto temp = p0;
    temp -= p1;

    EXPECT_EQ(op.x(), PointT(-2));
    EXPECT_EQ(op.y(), PointT(-2));
    EXPECT_EQ(temp.x(), PointT(-2));
    EXPECT_EQ(temp.y(), PointT(-2));
}

TYPED_TEST(test_geometry_point_t, operator_mul)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(0, 2);
    const auto scalar = PointT(3);

    auto op = p0 * scalar;
    auto temp = p0;
    temp *= scalar;

    EXPECT_EQ(op.x(), PointT(0));
    EXPECT_EQ(op.y(), PointT(6));
    EXPECT_EQ(temp.x(), PointT(0));
    EXPECT_EQ(temp.y(), PointT(6));
}

TYPED_TEST(test_geometry_point_t, operator_div)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 9);
    const auto scalar = PointT(3);
    const double abs_error = 0.000001;

    auto op = p0 / scalar;
    auto temp = p0;
    temp /= scalar;

    EXPECT_NEAR(static_cast<double>(op.x()), static_cast<double>(p0.x() / scalar), abs_error);
    EXPECT_NEAR(static_cast<double>(op.y()), static_cast<double>(p0.y() / scalar), abs_error);
    EXPECT_NEAR(static_cast<double>(temp.x()), static_cast<double>(p0.x() / scalar), abs_error);
    EXPECT_NEAR(static_cast<double>(temp.y()), static_cast<double>(p0.y() / scalar), abs_error);
}

TYPED_TEST(test_geometry_point_t, to_string)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p0(1, 2);
    tc::sdk::point<PointT> p1(PointT(-3), PointT(-4));

    if constexpr (std::is_same_v<PointT, unsigned int>)
    {
        // Overflow!
        // The PointT is "unsigned int" so when using negative values it overflows.
        // So the expected values are:
        // std::numeric_limits<unsigned int>::max() + 1 + p1.x()
        // = 4294967296 + 1 + 3
        // = 4294967293
        // and
        // std::numeric_limits<unsigned int>::max() + 1 + p1.y()
        // = 4294967296 + 1 + 4
        // = 4294967292
        auto x = std::numeric_limits<unsigned int>::max() + 1 + p1.x();
        auto y = std::numeric_limits<unsigned int>::max() + 1 + p1.y();
        EXPECT_EQ(p1.to_string(), "(" + std::to_string(x) + ", " + std::to_string(y) + ")");

        // No overflow on positive values.
        EXPECT_EQ(p0.to_string(), "(1, 2)");
    }

    if constexpr (std::is_same_v<PointT, int> || std::is_same_v<PointT, long>)
    {
        EXPECT_EQ(p0.to_string(), "(1, 2)");
        EXPECT_EQ(p1.to_string(), "(-3, -4)");
    }

    if constexpr (std::is_same_v<PointT, float> || std::is_same_v<PointT, double>)
    {
        EXPECT_EQ(p0.to_string(), "(1.000000, 2.000000)");
        EXPECT_EQ(p1.to_string(), "(-3.000000, -4.000000)");
    }
}

// NOLINTNEXTLINE
TYPED_TEST(test_geometry_point_t, ostream)
{
    using PointT = TypeParam;
    tc::sdk::point<PointT> p(1, 2);

    std::stringstream stream;
    stream << p;
    EXPECT_STREQ(stream.str().c_str(), p.to_string().c_str());
}

}
