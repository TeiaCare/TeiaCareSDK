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

#include "test_geometry_rectangle.hpp"

#include <sstream>
#include <type_traits>

namespace tc::sdk::tests
{
using types = testing::Types<int, float, double, long, long long>;
TYPED_TEST_SUITE(test_geometry_rectangle_t, types);

TYPED_TEST(test_geometry_rectangle_t, create)
{
    using RectT = TypeParam;
    tc::sdk::rectangle<RectT> p0;
    tc::sdk::rectangle<RectT> p1{};
    tc::sdk::rectangle<RectT> p2 = p1;
    auto p3 = tc::sdk::rectangle<RectT>{};
    const auto p4 = tc::sdk::rectangle<RectT>();
    const auto p5(p4);
    tc::sdk::rectangle<RectT> p6(tc::sdk::rectangle<RectT>{});

    auto temp = tc::sdk::rectangle<RectT>{};
    tc::sdk::rectangle<RectT> p7(std::move(temp));

    tc::sdk::rectangle<RectT> p8(tc::sdk::point<RectT>(), 0, 0);
    tc::sdk::rectangle<RectT> p9(0, 0, 0, 0);

    auto validate = [](auto p) {
        EXPECT_EQ(p.width(), 0);
        EXPECT_EQ(p.height(), 0);
        EXPECT_EQ(p.size(), tc::sdk::size<RectT>());
        EXPECT_EQ(p.area(), 0);
        EXPECT_EQ(p.center(), tc::sdk::point<double>());
        EXPECT_EQ(p.top_left(), tc::sdk::point<RectT>());
        EXPECT_EQ(p.top_right(), tc::sdk::point<RectT>());
        EXPECT_EQ(p.bottom_left(), tc::sdk::point<RectT>());
        EXPECT_EQ(p.bottom_right(), tc::sdk::point<RectT>());
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
    validate(p8);
    validate(p9);
}

TYPED_TEST(test_geometry_rectangle_t, compare)
{
    using RectT = TypeParam;
    tc::sdk::rectangle<RectT> p0(tc::sdk::point<RectT>(1, 1), 1, 2);
    tc::sdk::rectangle<RectT> p1(tc::sdk::point<RectT>(1, 1), 2, 1);
    tc::sdk::rectangle<RectT> p2(tc::sdk::point<RectT>(3, 3), 1, 2);
    tc::sdk::rectangle<RectT> p3(tc::sdk::point<RectT>(1, 1), 1, 2);

    EXPECT_TRUE(p0 != p1);
    EXPECT_TRUE(p1 != p0);

    EXPECT_TRUE(p0 != p2);
    EXPECT_TRUE(p2 != p0);

    EXPECT_TRUE(p0 == p3);
    EXPECT_TRUE(p3 == p0);
}

TYPED_TEST(test_geometry_rectangle_t, center)
{
    using RectT = TypeParam;
    tc::sdk::rectangle<RectT> p0(tc::sdk::point<RectT>(2, 2), 0, 0);
    tc::sdk::rectangle<RectT> p1(tc::sdk::point<RectT>(2, 2), 8, 8);
    tc::sdk::rectangle<RectT> p2(tc::sdk::point<RectT>(0, 0), 1, 1);

    EXPECT_EQ(p0.center(), tc::sdk::point<double>(2.0, 2.0));
    EXPECT_EQ(p1.center(), tc::sdk::point<double>(6.0, 6.0));
    EXPECT_EQ(p2.center(), tc::sdk::point<double>(0.5, 0.5));
}

// TYPED_TEST(test_geometry_rectangle_t, getter_setter)
// {
//     using RectT = TypeParam;
//     tc::sdk::rectangle<RectT> p0(tc::sdk::point<RectT>(2, 2), 8, 8);

//     EXPECT_EQ(p0.start(), tc::sdk::point<LineT>(1, 1));
//     EXPECT_EQ(p0.end(), tc::sdk::point<LineT>(9, 9));

//     const tc::sdk::point<LineT> new_start(2, 2);
//     p0.set_start(new_start);
//     EXPECT_EQ(p0.start(), new_start);

//     const tc::sdk::point<LineT> new_end(8, 8);
//     p0.set_end(new_end);
//     EXPECT_EQ(p0.end(), new_end);
// }

TYPED_TEST(test_geometry_rectangle_t, reshape_input_validation)
{
    using RectT = TypeParam;
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(10, 10), 20, 20);

    // Test with zero or negative output dimensions
    auto result1 = rect.reshape(100, 100, 0, 100);
    EXPECT_TRUE(result1.is_null());

    auto result2 = rect.reshape(100, 100, 100, 0);
    EXPECT_TRUE(result2.is_null());

    if constexpr (std::is_signed_v<RectT>)
    {
        auto result3 = rect.reshape(100, 100, -10, 100);
        EXPECT_TRUE(result3.is_null());

        auto result4 = rect.reshape(100, 100, 100, -10);
        EXPECT_TRUE(result4.is_null());
    }

    // Test with zero or negative input dimensions
    auto result5 = rect.reshape(0, 100, 100, 100);
    EXPECT_TRUE(result5.is_null());

    auto result6 = rect.reshape(100, 0, 100, 100);
    EXPECT_TRUE(result6.is_null());

    if constexpr (std::is_signed_v<RectT>)
    {
        auto result7 = rect.reshape(-10, 100, 100, 100);
        EXPECT_TRUE(result7.is_null());

        auto result8 = rect.reshape(100, -10, 100, 100);
        EXPECT_TRUE(result8.is_null());
    }

    // Test with null rectangle
    tc::sdk::rectangle<RectT> null_rect;
    auto result9 = null_rect.reshape(100, 100, 50, 50);
    EXPECT_TRUE(result9.is_null());
}

TYPED_TEST(test_geometry_rectangle_t, reshape_no_scaling_needed)
{
    using RectT = TypeParam;

    // Rectangle in input coordinates: position (10, 10), size 20x20
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(10, 10), 20, 20);

    // No scaling needed when input and output dimensions are the same
    auto result = rect.reshape(100, 100, 100, 100);

    EXPECT_EQ(result.top_left().x(), 10);
    EXPECT_EQ(result.top_left().y(), 10);
    EXPECT_EQ(result.width(), 20);
    EXPECT_EQ(result.height(), 20);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_scale_down)
{
    using RectT = TypeParam;

    // Rectangle covering entire input space
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(0, 0), 100, 100);

    // Scale down from 100x100 to 50x50 (scale factor = 0.5)
    auto result = rect.reshape(100, 100, 50, 50);

    EXPECT_EQ(result.top_left().x(), 0);
    EXPECT_EQ(result.top_left().y(), 0);
    EXPECT_EQ(result.width(), 50);
    EXPECT_EQ(result.height(), 50);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_scale_up)
{
    using RectT = TypeParam;

    // Small rectangle in input space
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(25, 25), 50, 50);

    // Scale up from 100x100 to 200x200 (scale factor = 2.0)
    auto result = rect.reshape(100, 100, 200, 200);

    EXPECT_EQ(result.top_left().x(), 50);
    EXPECT_EQ(result.top_left().y(), 50);
    EXPECT_EQ(result.width(), 100);
    EXPECT_EQ(result.height(), 100);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_letterboxing_horizontal)
{
    using RectT = TypeParam;

    // Rectangle in letterboxed input (wider than tall)
    // Input: 200x100, Output: 100x100 (aspect ratio different)
    // Scale = min(200/100, 100/100) = min(2.0, 1.0) = 1.0
    // Scaled output in input space: 100x100, centered with padding (50, 0)
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(75, 50), 50, 25);

    auto result = rect.reshape(200, 100, 100, 100);

    // Remove padding offset (50, 0) and scale by 1.0
    // Expected: ((75-50)*1, (50-0)*1, 50*1, 25*1) = (25, 50, 50, 25)
    EXPECT_EQ(result.top_left().x(), 25);
    EXPECT_EQ(result.top_left().y(), 50);
    EXPECT_EQ(result.width(), 50);
    EXPECT_EQ(result.height(), 25);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_letterboxing_vertical)
{
    using RectT = TypeParam;

    // Rectangle in letterboxed input (taller than wide)
    // Input: 100x200, Output: 100x100 (aspect ratio different)
    // Scale = min(100/100, 200/100) = min(1.0, 2.0) = 1.0
    // Scaled output in input space: 100x100, centered with padding (0, 50)
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(50, 75), 25, 50);

    auto result = rect.reshape(100, 200, 100, 100);

    // Remove padding offset (0, 50) and scale by 1.0
    // Expected: ((50-0)*1, (75-50)*1, 25*1, 50*1) = (50, 25, 25, 50)
    EXPECT_EQ(result.top_left().x(), 50);
    EXPECT_EQ(result.top_left().y(), 25);
    EXPECT_EQ(result.width(), 25);
    EXPECT_EQ(result.height(), 50);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_clamping_boundaries)
{
    using RectT = TypeParam;

    // Rectangle that extends beyond input boundaries
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(0, 0), 100, 100);

    // Scale up significantly to test clamping
    auto result = rect.reshape(100, 100, 10, 10);

    // Should be clamped to output boundaries [0, 10] x [0, 10]
    EXPECT_EQ(result.top_left().x(), 0);
    EXPECT_EQ(result.top_left().y(), 0);
    EXPECT_EQ(result.width(), 10);
    EXPECT_EQ(result.height(), 10);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_partial_clipping)
{
    using RectT = TypeParam;

    // Rectangle that will be partially outside output boundaries after transformation
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(80, 80), 40, 40);

    // Scale up from 100x100 to 200x200
    auto result = rect.reshape(100, 100, 200, 200);

    // Expected transformation: (80*2, 80*2, 40*2, 40*2) = (160, 160, 80, 80)
    // Should be clamped: right edge at 240 -> 200, bottom edge at 240 -> 200
    EXPECT_EQ(result.top_left().x(), 160);
    EXPECT_EQ(result.top_left().y(), 160);
    EXPECT_EQ(result.width(), 40);  // 200 - 160 = 40
    EXPECT_EQ(result.height(), 40); // 200 - 160 = 40
}

TYPED_TEST(test_geometry_rectangle_t, reshape_completely_outside)
{
    using RectT = TypeParam;

    // Rectangle positioned outside the scaled region (in padding area)
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(0, 0), 10, 10);

    // Input 200x100, Output 50x50 with letterboxing
    // Scale = min(200/50, 100/50) = min(4, 2) = 2
    // Scaled output: 50*2 = 100x100, padding: (50, 0)
    // Rectangle at (0,0) is in the padding area on the left
    auto result = rect.reshape(200, 100, 50, 50);

    // After removing padding and scaling, rectangle would be at negative coordinates
    // Should be clamped to result in zero width/height or minimal valid rectangle
    EXPECT_TRUE(result.width() >= 0);
    EXPECT_TRUE(result.height() >= 0);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_floating_point_precision)
{
    using RectT = TypeParam;

    if constexpr (std::is_floating_point_v<RectT>)
    {
        // Test with dimensions that don't divide evenly
        tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(10.5, 15.7), 20.3, 25.8);

        // Non-integer scaling factor
        auto result = rect.reshape(100, 100, 33, 33);

        // Should handle floating point calculations correctly
        EXPECT_TRUE(result.top_left().x() >= 0);
        EXPECT_TRUE(result.top_left().y() >= 0);
        EXPECT_TRUE(result.width() >= 0);
        EXPECT_TRUE(result.height() >= 0);
        EXPECT_TRUE(result.top_left().x() + result.width() <= 33);
        EXPECT_TRUE(result.top_left().y() + result.height() <= 33);
    }
}

TYPED_TEST(test_geometry_rectangle_t, reshape_center_rectangle)
{
    using RectT = TypeParam;

    // Rectangle at center of input space
    tc::sdk::rectangle<RectT> rect(tc::sdk::point<RectT>(40, 40), 20, 20);

    // Scale down by factor of 2
    auto result = rect.reshape(100, 100, 50, 50);

    // Expected: center rectangle should remain at center, scaled down
    EXPECT_EQ(result.top_left().x(), 20);
    EXPECT_EQ(result.top_left().y(), 20);
    EXPECT_EQ(result.width(), 10);
    EXPECT_EQ(result.height(), 10);
}

TYPED_TEST(test_geometry_rectangle_t, reshape_extreme_aspect_ratios)
{
    using RectT = TypeParam;

    // Test with very wide input
    tc::sdk::rectangle<RectT> rect1(tc::sdk::point<RectT>(100, 45), 200, 10);
    auto result1 = rect1.reshape(400, 100, 100, 100);

    // Scale = min(400/100, 100/100) = min(4, 1) = 1
    // Scaled output: 100x100, padding: (150, 0)
    // Transform: ((100-150)*1, (45-0)*1, 200*1, 10*1) but clamped
    EXPECT_TRUE(result1.top_left().x() >= 0);
    EXPECT_TRUE(result1.width() >= 0);
    EXPECT_TRUE(result1.height() >= 0);

    // Test with very tall input
    tc::sdk::rectangle<RectT> rect2(tc::sdk::point<RectT>(45, 100), 10, 200);
    auto result2 = rect2.reshape(100, 400, 100, 100);

    // Scale = min(100/100, 400/100) = min(1, 4) = 1
    // Scaled output: 100x100, padding: (0, 150)
    EXPECT_TRUE(result2.top_left().y() >= 0);
    EXPECT_TRUE(result2.width() >= 0);
    EXPECT_TRUE(result2.height() >= 0);
}

// NOLINTNEXTLINE
TYPED_TEST(test_geometry_rectangle_t, ostream)
{
    using RectT = TypeParam;
    tc::sdk::rectangle<RectT> p(tc::sdk::point<RectT>(2, 2), 0, 0);

    std::stringstream stream;
    stream << p;
    EXPECT_STREQ(stream.str().c_str(), p.to_string().c_str());
}

}
