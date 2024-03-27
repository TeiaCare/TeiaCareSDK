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

#pragma once

#include <teiacare/sdk/blocking_queue.hpp>

#include <array>
#include <vector>
#include <gtest/gtest.h>

namespace tc::sdk::tests
{
template <class T>
class test_blocking_queue : public testing::TestWithParam<std::vector<T>>
{
protected:
    explicit test_blocking_queue()
        : q{tc::sdk::blocking_queue<T>(queue_capacity)}
    {
    }

    ~test_blocking_queue()
    {
    }

    const size_t queue_capacity = 4;
    tc::sdk::blocking_queue<T> q;

    void drain_queue(std::vector<T> params)
    {
        for (auto p : params)
        {
            T item = q.pop();
            EXPECT_EQ(item, p);
        }

        EXPECT_EQ(q.size(), 0);
    }

    void try_drain_queue(std::vector<T> params)
    {
        for (size_t i = 0; i < params.size(); ++i)
        {
            std::optional<T> item = q.try_pop();
            if (i < queue_capacity)
            {
                EXPECT_EQ(item.value(), params[i]);
                EXPECT_EQ(q.size(), queue_capacity - i - 1);
            }
            else
            {
                EXPECT_EQ(item, std::nullopt);
                EXPECT_EQ(q.size(), 0);
            }
        }

        EXPECT_EQ(q.size(), 0);
    }

    void run_push_const_ref(std::vector<T> params)
    {
        EXPECT_EQ(q.size(), 0);

        for (auto p : params)
            q.push(p);

        EXPECT_EQ(q.size(), params.size());
        drain_queue(params);
    }

    void run_push_move(std::vector<T> params)
    {
        EXPECT_EQ(q.size(), 0);

        for (auto p : params)
            q.push(std::move(p));

        EXPECT_EQ(q.size(), params.size());
        drain_queue(params);
    }

    void run_try_push_const_ref(std::vector<T> params)
    {
        for (size_t i = 0; i < params.size(); ++i)
        {
            if (i < queue_capacity)
            {
                EXPECT_TRUE(q.try_push(params[i]));
                EXPECT_EQ(q.size(), i + 1);
            }
            else
            {
                EXPECT_FALSE(q.try_push(params[i]));
                EXPECT_EQ(q.size(), queue_capacity);
            }
        }

        EXPECT_EQ(q.size(), queue_capacity);
        try_drain_queue(params);
    }

    void run_try_push_move(std::vector<T> params)
    {
        for (size_t i = 0; i < params.size(); ++i)
        {
            if (i < queue_capacity)
            {
                EXPECT_TRUE(q.try_push(std::move(params[i])));
                EXPECT_EQ(q.size(), i + 1);
            }
            else
            {
                EXPECT_FALSE(q.try_push(std::move(params[i])));
                EXPECT_EQ(q.size(), queue_capacity);
            }
        }

        EXPECT_EQ(q.size(), queue_capacity);
        try_drain_queue(params);
    }
};

struct blocking_queue_params
{
    std::string name;
    size_t producers;
    size_t consumers;
    size_t queue_capacity;
};

template <class ItemsT>
struct blocking_queue_params_factory : blocking_queue_params
{
    static const constexpr size_t ItemsSize = 500;
    using Items = std::array<ItemsT, ItemsSize>;
    Items items;
    Items create_items();

    blocking_queue_params_factory(blocking_queue_params p)
        : blocking_queue_params(p)
        , items{create_items()}
    {
    }
};

template <class ItemsT>
class test_blocking_queue_producer_consumer : public testing::TestWithParam<blocking_queue_params_factory<ItemsT>>
{
protected:
    void producer_consumer(const blocking_queue_params_factory<ItemsT>& params)
    {
        const auto producer_threads_count = params.producers;
        const auto consumer_threads_count = params.consumers;
        const auto queue_capacity = params.queue_capacity;
        const auto items = params.items;

        const size_t total_items_count = items.size();
        auto q = tc::sdk::blocking_queue<ItemsT>(queue_capacity);

        EXPECT_EQ(q.capacity(), queue_capacity);
        EXPECT_EQ(q.size(), 0);

        /*
        The number of items that each thread produces (producer_items_count_per_thread)
        must be an exact multiple of number of total items (total_items_count) in order to
        produce all the required items.
        */
        const auto producer_items_count_per_thread = total_items_count / producer_threads_count;

        std::vector<std::thread> producer_threads;
        for (size_t producer_id = 0; producer_id < producer_threads_count; ++producer_id)
        {
            producer_threads.emplace_back(std::thread([&] {
                for (size_t i = 0; i < producer_items_count_per_thread; ++i)
                    q.push(items[i]);
            }));
        }

        /*
        The number of items that each thread consumes (consumer_items_count_per_thread)
        must be an exact multiple of number of total items (total_items_count) in order to
        consume all the required items.
        */
        const auto consumer_items_count_per_thread = total_items_count / consumer_threads_count;

        std::vector<std::thread> consumer_threads;
        for (size_t consumer_id = 0; consumer_id < consumer_threads_count; ++consumer_id)
        {
            consumer_threads.emplace_back(std::thread([&] {
                for (size_t i = 0; i < consumer_items_count_per_thread; ++i)
                {
                    auto item = q.pop();
                    (void)item;
                }
            }));
        }

        for (auto&& producer_thread : producer_threads)
            producer_thread.join();

        for (auto&& consumer_thread : consumer_threads)
            consumer_thread.join();

        EXPECT_EQ(q.capacity(), queue_capacity);
        EXPECT_EQ(q.size(), 0);
    }
};

}
