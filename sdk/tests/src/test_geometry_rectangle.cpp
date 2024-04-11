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

    auto validate = [](auto p)
    {
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
