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

#include "test_blocking_queue.hpp"

#include <gtest/gtest.h>

using namespace std::string_literals;

namespace tc::sdk::tests
{
class test_blocking_queue_push_int : public test_blocking_queue<int>
{
};
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_push,
    test_blocking_queue_push_int,
    testing::Values(std::vector<int>{1, 2, 3}));
TEST_P(test_blocking_queue_push_int, push_const_ref)
{
    run_push_const_ref(GetParam());
}
TEST_P(test_blocking_queue_push_int, push_move)
{
    run_push_move(GetParam());
}

class test_blocking_queue_try_push_int : public test_blocking_queue<int>
{
};
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_try_push,
    test_blocking_queue_try_push_int,
    testing::Values(std::vector<int>{1, 2, 3, 4, 5, 6}));
TEST_P(test_blocking_queue_try_push_int, try_push_const_ref)
{
    run_try_push_const_ref(GetParam());
}
TEST_P(test_blocking_queue_try_push_int, try_push_move)
{
    run_try_push_move(GetParam());
}

class test_blocking_queue_push_const_char_ptr : public test_blocking_queue<const char*>
{
};
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_push,
    test_blocking_queue_push_const_char_ptr,
    testing::Values(std::vector<const char*>{"1", "2", "3"}));
TEST_P(test_blocking_queue_push_const_char_ptr, push_const_ref)
{
    run_push_const_ref(GetParam());
}
TEST_P(test_blocking_queue_push_const_char_ptr, push_move)
{
    run_push_move(GetParam());
}

class test_blocking_queue_try_push_const_char_ptr : public test_blocking_queue<const char*>
{
};
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_try_push,
    test_blocking_queue_try_push_const_char_ptr,
    testing::Values(std::vector<const char*>{"1", "2", "3", "4", "5", "6"}));
TEST_P(test_blocking_queue_try_push_const_char_ptr, push_const_ref)
{
    run_try_push_const_ref(GetParam());
}
TEST_P(test_blocking_queue_try_push_const_char_ptr, push_move)
{
    run_try_push_move(GetParam());
}

class test_blocking_queue_push_string : public test_blocking_queue<std::string>
{
};
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_push,
    test_blocking_queue_push_string,
    testing::Values(std::vector<std::string>{"1"s, "2"s, "3"s}));
TEST_P(test_blocking_queue_push_string, push_const_ref)
{
    run_push_const_ref(GetParam());
}
TEST_P(test_blocking_queue_push_string, push_move)
{
    run_push_move(GetParam());
}

class test_blocking_queue_try_push_string : public test_blocking_queue<std::string>
{
};
// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_try_push,
    test_blocking_queue_try_push_string,
    testing::Values(std::vector<std::string>{"1"s, "2"s, "3"s, "4"s, "5"s, "6"s}));
TEST_P(test_blocking_queue_try_push_string, push_const_ref)
{
    run_try_push_const_ref(GetParam());
}
TEST_P(test_blocking_queue_try_push_string, push_move)
{
    run_try_push_move(GetParam());
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Test Suites
/////////////////////////////////////////////////////////////////////////////////////////////

// Type = int
class test_blocking_queue_producer_consumer_int
    : public test_blocking_queue_producer_consumer<int>
{
};

template <>
blocking_queue_params_factory<int>::Items blocking_queue_params_factory<int>::create_items()
{
    Items items;
    for (size_t idx = 0; idx < blocking_queue_params_factory::ItemsSize; idx++)
        items.at(idx) = idx;
    return items;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_producer_consumer,
    test_blocking_queue_producer_consumer_int,
    testing::Values(
        blocking_queue_params_factory<int>({.name = "1_producers_1_consumers", .producers = 1, .consumers = 1, .queue_capacity = 1000}),
        blocking_queue_params_factory<int>({.name = "1_producers_2_consumers", .producers = 1, .consumers = 2, .queue_capacity = 1000}),
        blocking_queue_params_factory<int>({.name = "2_producers_1_consumers", .producers = 2, .consumers = 1, .queue_capacity = 1000}),
        blocking_queue_params_factory<int>({.name = "2_producers_4_consumers", .producers = 2, .consumers = 4, .queue_capacity = 100}),
        blocking_queue_params_factory<int>({.name = "4_producers_2_consumers", .producers = 4, .consumers = 2, .queue_capacity = 100}),
        blocking_queue_params_factory<int>({.name = "4_producers_4_consumers", .producers = 4, .consumers = 4, .queue_capacity = 100}),
        blocking_queue_params_factory<int>({.name = "8_producers_8_consumers", .producers = 8, .consumers = 8, .queue_capacity = 10})),
    [](const auto& info) { return info.param.name; });
TEST_P(test_blocking_queue_producer_consumer_int, test)
{
    producer_consumer(GetParam());
} // NOLINT

// Type = const char*
class test_blocking_queue_producer_consumer_const_char_ptr
    : public test_blocking_queue_producer_consumer<const char*>
{
};

template <>
blocking_queue_params_factory<const char*>::Items blocking_queue_params_factory<const char*>::create_items()
{
    Items items;
    for (size_t idx = 0; idx < blocking_queue_params_factory::ItemsSize; idx++)
        items.at(idx) = std::to_string(idx).c_str();
    return items;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_producer_consumer,
    test_blocking_queue_producer_consumer_const_char_ptr,
    testing::Values(
        blocking_queue_params_factory<const char*>({.name = "1_producers_1_consumers", .producers = 1, .consumers = 1, .queue_capacity = 1000}),
        blocking_queue_params_factory<const char*>({.name = "1_producers_2_consumers", .producers = 1, .consumers = 2, .queue_capacity = 1000}),
        blocking_queue_params_factory<const char*>({.name = "2_producers_1_consumers", .producers = 2, .consumers = 1, .queue_capacity = 1000}),
        blocking_queue_params_factory<const char*>({.name = "2_producers_4_consumers", .producers = 2, .consumers = 4, .queue_capacity = 100}),
        blocking_queue_params_factory<const char*>({.name = "4_producers_2_consumers", .producers = 4, .consumers = 2, .queue_capacity = 100}),
        blocking_queue_params_factory<const char*>({.name = "4_producers_4_consumers", .producers = 4, .consumers = 4, .queue_capacity = 100}),
        blocking_queue_params_factory<const char*>({.name = "8_producers_8_consumers", .producers = 8, .consumers = 8, .queue_capacity = 10})),
    [](auto info) { return info.param.name; });
TEST_P(test_blocking_queue_producer_consumer_const_char_ptr, test)
{
    producer_consumer(GetParam());
} // NOLINT

// Type = std::string
class test_blocking_queue_producer_consumer_string
    : public test_blocking_queue_producer_consumer<std::string>
{
};

template <>
blocking_queue_params_factory<std::string>::Items blocking_queue_params_factory<std::string>::create_items()
{
    Items items;
    for (size_t idx = 0; idx < blocking_queue_params_factory::ItemsSize; idx++)
        items.at(idx) = std::to_string(idx);
    return items;
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(
    test_blocking_queue_producer_consumer,
    test_blocking_queue_producer_consumer_string,
    testing::Values(
        blocking_queue_params_factory<std::string>({.name = "1_producers_1_consumers", .producers = 1, .consumers = 1, .queue_capacity = 1000}),
        blocking_queue_params_factory<std::string>({.name = "1_producers_2_consumers", .producers = 1, .consumers = 2, .queue_capacity = 1000}),
        blocking_queue_params_factory<std::string>({.name = "2_producers_1_consumers", .producers = 2, .consumers = 1, .queue_capacity = 1000}),
        blocking_queue_params_factory<std::string>({.name = "2_producers_4_consumers", .producers = 2, .consumers = 4, .queue_capacity = 100}),
        blocking_queue_params_factory<std::string>({.name = "4_producers_2_consumers", .producers = 4, .consumers = 2, .queue_capacity = 100}),
        blocking_queue_params_factory<std::string>({.name = "4_producers_4_consumers", .producers = 4, .consumers = 4, .queue_capacity = 100}),
        blocking_queue_params_factory<std::string>({.name = "8_producers_8_consumers", .producers = 8, .consumers = 8, .queue_capacity = 10})),
    [](auto info) { return info.param.name; });
TEST_P(test_blocking_queue_producer_consumer_string, test)
{
    producer_consumer(GetParam());
} // NOLINT
}
