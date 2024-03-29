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

#include "test_observable.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST(test_observable, callback_enabled)
{
    const int value = 12345;
    int callback_triggered_count = 0;
    const auto callback = [&](int) { ++callback_triggered_count; };

    auto o = tc::sdk::observable<int>(value, callback);
    ASSERT_TRUE(o.callback_enabled());

    o = value;
    ASSERT_EQ(callback_triggered_count, 0);

    o = value + 1;
    ASSERT_EQ(callback_triggered_count, 1);

    o.callback_enabled(true);
    ASSERT_TRUE(o.callback_enabled());

    o = value + 2;
    ASSERT_EQ(callback_triggered_count, 2);

    ASSERT_TRUE(o.callback_enabled());
}

// NOLINTNEXTLINE
TEST(test_observable, callback_disabled)
{
    const int value = 12345;
    int callback_triggered_count = 0;
    const auto callback = [&](int) { ++callback_triggered_count; };

    auto o = tc::sdk::observable<int>(value, callback);
    ASSERT_TRUE(o.callback_enabled());

    o.callback_enabled(false);
    ASSERT_FALSE(o.callback_enabled());

    o = value;
    ASSERT_EQ(callback_triggered_count, 0);

    o = value + 1;
    ASSERT_EQ(callback_triggered_count, 0);

    o.callback_enabled(true);
    ASSERT_TRUE(o.callback_enabled());

    o = value;
    ASSERT_EQ(callback_triggered_count, 1);

    ASSERT_TRUE(o.callback_enabled());
}

// NOLINTNEXTLINE
TEST(test_observable, set_same_value)
{
    const auto value = 12345;
    int callback_triggered_count = 0;
    const auto callback = [&](int) { ++callback_triggered_count; };
    auto o = tc::sdk::observable<int>(value, callback);
    EXPECT_EQ(o, 12345);

    // const T&
    const auto new_value = value;
    o = new_value;
    EXPECT_EQ(callback_triggered_count, 0);
    EXPECT_EQ(o, 12345);

    // T&&
    o = 12345; // NOLINT
    EXPECT_EQ(callback_triggered_count, 0);
    EXPECT_EQ(o, 12345);
}

// NOLINTNEXTLINE
TEST(test_observable, set_new_value)
{
    const auto value = 12345;
    int callback_triggered_count = 0;
    const auto callback = [&](int) { ++callback_triggered_count; };
    auto o = tc::sdk::observable<int>(value, callback);
    EXPECT_EQ(o, value);

    // const T&
    const auto new_value = 12346;
    o = new_value;
    EXPECT_EQ(callback_triggered_count, 1);
    EXPECT_EQ(o, 12346);

    // T&&
    o = 12347; // NOLINT
    EXPECT_EQ(callback_triggered_count, 2);
    EXPECT_EQ(o, 12347);
}

// NOLINTNEXTLINE
TEST(test_observable, set_new_value_callback_disabled)
{
    const auto value = 12345;
    int callback_triggered_count = 0;
    const auto callback = [&](int) { ++callback_triggered_count; };
    auto o = tc::sdk::observable<int>(value, callback);
    EXPECT_EQ(o, value);

    o.callback_enabled(false);

    // const T&
    const auto new_value = 12346;
    o = new_value;
    EXPECT_EQ(callback_triggered_count, 0);
    EXPECT_EQ(o, 12346);

    // T&&
    o = 12347; // NOLINT
    EXPECT_EQ(callback_triggered_count, 0);
    EXPECT_EQ(o, 12347);
}

// NOLINTNEXTLINE
TEST(test_observable, multiple_thread_same_value)
{
    const auto value = 12345;
    int callback_triggered_count = 0;
    const int v = 1;
    const auto callback = [&](int value) { ++callback_triggered_count; EXPECT_EQ(value, v); };
    auto o = tc::sdk::observable<int>(value, callback);

    {
        auto worker = [&o](int value) { o = value; };
        std::jthread t1(worker, v);
        std::jthread t2(worker, v);
        std::jthread t3(worker, v);
    }

    EXPECT_EQ(callback_triggered_count, 1);
    EXPECT_EQ(o, v);
}

// NOLINTNEXTLINE
TEST(test_observable, multiple_thread_different_value)
{
    const auto value = 12345;
    int callback_triggered_count = 0;
    const auto callback = [&](int) { ++callback_triggered_count; };
    auto o = tc::sdk::observable<int>(value, callback);

    auto worker = [&o](int v) { o = v; };

    {
        std::jthread t1(worker, 1);
        std::jthread t2(worker, 2);
        std::jthread t3(worker, 3);
    }

    EXPECT_EQ(callback_triggered_count, 3);
    EXPECT_THAT(o, testing::AnyOf(1, 2, 3));
}

// NOLINTNEXTLINE
TEST(test_observable, custom_type)
{
    int callback_triggered_count = 0;
    const auto callback = [&](foo_t) { ++callback_triggered_count; };

    foo_t foo("1");
    auto o1 = tc::sdk::observable<foo_t>(foo, callback);
    EXPECT_EQ(o1.value().get(), foo.get());
    EXPECT_EQ(callback_triggered_count, 0);

    o1 = foo_t("1_update");
    EXPECT_EQ(o1.value().get(), "1_update");
    EXPECT_EQ(callback_triggered_count, 1);

    auto o2 = tc::sdk::observable<foo_t>(foo_t("2"), callback);
    EXPECT_EQ(o2.value().get(), "2");
    EXPECT_EQ(callback_triggered_count, 1);

    o2 = foo_t("2_update");
    EXPECT_EQ(o2.value().get(), "2_update");
    EXPECT_EQ(callback_triggered_count, 2);
}

// NOLINTNEXTLINE
TEST(test_observable, callback_lambda)
{
    counter_t counter;
    auto o = tc::sdk::observable<int>(0, [&counter](int value) {
        static int local_counter = 0;
        local_counter += value;
        counter.update(value);
        EXPECT_EQ(local_counter, counter.value());
    });

    EXPECT_EQ(o.value(), 0);

    const int total_updates = 10;
    for (int update_index = 1; update_index < total_updates; ++update_index)
    {
        o = update_index;
        EXPECT_EQ(o.value(), update_index);
    }
}

// NOLINTNEXTLINE
TEST(test_observable, callback_bind)
{
    counter_t counter;
    auto o = tc::sdk::observable<int>(0, std::bind(&counter_t::update, &counter, std::placeholders::_1));
    EXPECT_EQ(o.value(), 0);

    const int total_updates = 10;
    int local_counter = 0;
    for (int update_index = 1; update_index < total_updates; ++update_index)
    {
        o = update_index;
        EXPECT_EQ(o.value(), update_index);

        local_counter += update_index;
        EXPECT_EQ(counter.value(), local_counter);
    }
}

// NOLINTNEXTLINE
TEST(test_observable, callback_free_function)
{
    auto o = tc::sdk::observable<int>(0, &free_function);
    EXPECT_EQ(o.value(), 0);

    const int total_updates = 10;
    int local_counter = 0;
    for (int update_index = 1; update_index < total_updates; ++update_index)
    {
        o = update_index;
        EXPECT_EQ(o.value(), update_index);

        local_counter += update_index;
        EXPECT_EQ(global_counter, local_counter);
    }
}

// NOLINTNEXTLINE
TEST(test_observable, callback_numeric)
{
    {
        const int value = 12345;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<int>(0, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const float value = 1.2345f;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<float>(0.f, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const double value = 1.2345;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<double>(0.0, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const size_t value = 12345;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<size_t>(std::size_t{0}, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const uint32_t value = 12345;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<uint32_t>(uint32_t{0}, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const unsigned long long value = 12345;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<unsigned long long>(uint32_t{0}, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const char value = 127;
        bool callback_triggered = false;
        auto o = tc::sdk::observable<char>(char{0}, [&](decltype(value) v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }
}

// NOLINTNEXTLINE
TEST(test_observable, callback_string)
{
    {
        const std::string value = "update";
        bool callback_triggered = false;
        auto o = tc::sdk::observable<std::string>(std::string("observable"), [&](const std::string& v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }

    {
        const char* value = "update";
        bool callback_triggered = false;
        auto o = tc::sdk::observable<std::string>(std::string("observable"), [&](const std::string& v) {
            EXPECT_EQ(v, value);
            callback_triggered = true;
        });

        o = value;
        EXPECT_EQ(o.value(), value);
        EXPECT_TRUE(callback_triggered);
    }
}

}