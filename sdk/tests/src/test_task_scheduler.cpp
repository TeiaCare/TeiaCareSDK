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

#include "test_task_scheduler.hpp"

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_task_scheduler, start_stop)
{
    EXPECT_TRUE(ts->start());
    EXPECT_FALSE(ts->start());

    EXPECT_TRUE(ts->stop());
    EXPECT_FALSE(ts->stop());

    EXPECT_TRUE(ts->start(1));
    EXPECT_FALSE(ts->start(1));

    EXPECT_TRUE(ts->stop());
    EXPECT_TRUE(ts->start(std::numeric_limits<unsigned>::max()));

    EXPECT_TRUE(ts->stop());
    EXPECT_TRUE(ts->start(std::numeric_limits<unsigned>::min()));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, run_after_stop)
{
    EXPECT_TRUE(ts->start());
    EXPECT_TRUE(ts->stop());

    ts->every("TASK_ID", 10ms, simple_task);

    EXPECT_EQ(ts->tasks_size(), 0);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, stop_immediate_while_running)
{
    EXPECT_TRUE(ts->start());

    ts->in(1min, simple_task);
    ts->at(1min + tc::sdk::clock::now(), simple_task);
    ts->every(1min, simple_task);

    EXPECT_TRUE(ts->stop());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, tasks_size_different_id)
{
    EXPECT_EQ(ts->tasks_size(), 0);
    constexpr int task_count = 32;

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    for (auto n = 0; n < task_count; ++n)
    {
        EXPECT_EQ(ts->tasks_size(), n);
        auto task_id = std::to_string(n);
        ts->every(std::move(task_id), 10ms, simple_task);
        EXPECT_EQ(ts->tasks_size(), n + 1);
    }

    EXPECT_EQ(ts->tasks_size(), task_count);
    EXPECT_TRUE(ts->stop());
    EXPECT_EQ(ts->tasks_size(), 0);

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, tasks_size_same_id)
{
    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    constexpr int task_count = 32;

    for (auto n = 0; n < task_count; ++n)
    {
        auto task_id = "TASK_ID";
        ts->every(std::move(task_id), 10ms, simple_task);
        EXPECT_EQ(ts->tasks_size(), 1);
    }

    EXPECT_EQ(ts->tasks_size(), 1); // only one task is run since task_id are not unique
    EXPECT_TRUE(ts->stop());
    EXPECT_EQ(ts->tasks_size(), 0);

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, scheduled_tasks)
{
    constexpr int task_count = 32;

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    for (auto n = 0; n < task_count; ++n)
    {
        auto task_id = std::to_string(n);
        ts->every(std::move(task_id), 10ms, simple_task);
        EXPECT_TRUE(ts->is_scheduled(task_id));
    }

    EXPECT_TRUE(ts->stop());
    EXPECT_EQ(ts->tasks_size(), 0);

    for (auto n = 0; n < task_count; ++n)
    {
        const auto task_id = std::to_string(n);
        EXPECT_FALSE(ts->is_scheduled(task_id));
    }

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    ts->every("TASK_ID_1", 10ms, simple_task);
    EXPECT_TRUE(ts->is_scheduled("TASK_ID_1"));
    EXPECT_FALSE(ts->is_scheduled("TASK_ID_2"));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, enabled_tasks)
{
    constexpr int task_count = 32;

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    for (auto n = 0; n < task_count; ++n)
    {
        auto task_id = std::to_string(n);
        ts->every(std::move(task_id), 10ms, simple_task);
        EXPECT_TRUE(ts->is_enabled(task_id));
    }

    // Disable all tasks
    for (auto n = 0; n < task_count; ++n)
    {
        const auto task_id = std::to_string(n);
        EXPECT_TRUE(ts->set_enabled(task_id, false));
        EXPECT_FALSE(ts->is_enabled(task_id));
    }

    // Enable all tasks
    for (auto n = 0; n < task_count; ++n)
    {
        const auto task_id = std::to_string(n);
        EXPECT_TRUE(ts->set_enabled(task_id, true));
        EXPECT_TRUE(ts->is_enabled(task_id));
    }
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, disabled_tasks)
{
    constexpr int task_count = 32;

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    for (auto n = 0; n < task_count; ++n)
    {
        auto task_id = std::to_string(n);
        ts->every(std::move(task_id), 10ms, simple_task);
    }

    EXPECT_TRUE(ts->stop());

    for (auto n = 0; n < task_count; ++n)
    {
        const auto task_id = std::to_string(n);
        EXPECT_FALSE(ts->is_enabled(task_id));
    }
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, reenabled_tasks)
{
    const auto scheduled_task_id = "TASK_1";
    const auto not_scheduled_task_id = "TASK_2";

    EXPECT_TRUE(ts->start());
    ts->every(std::move(scheduled_task_id), 10ms, simple_task);

    EXPECT_TRUE(ts->is_enabled(scheduled_task_id));
    EXPECT_FALSE(ts->is_enabled(not_scheduled_task_id));

    EXPECT_TRUE(ts->set_enabled(scheduled_task_id, false));
    EXPECT_FALSE(ts->set_enabled(not_scheduled_task_id, false));

    EXPECT_TRUE(ts->set_enabled(scheduled_task_id, true));
    EXPECT_FALSE(ts->set_enabled(not_scheduled_task_id, true));

    EXPECT_TRUE(ts->is_enabled(scheduled_task_id));
    EXPECT_FALSE(ts->is_enabled(not_scheduled_task_id));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, removed_tasks)
{
    constexpr int task_count = 32;

    EXPECT_TRUE(ts->start());
    EXPECT_EQ(ts->tasks_size(), 0);

    for (auto n = 0; n < task_count; ++n)
    {
        auto task_id = std::to_string(n);
        ts->every(std::move(task_id), 10ms, simple_task);
    }

    // Remove all tasks
    for (auto n = 0; n < task_count; ++n)
    {
        const auto task_id = std::to_string(n);
        EXPECT_TRUE(ts->remove_task(task_id));
        EXPECT_FALSE(ts->is_scheduled(task_id));
    }

    EXPECT_EQ(ts->tasks_size(), 0);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, removed_missing_tasks)
{
    const auto scheduled_task_id = "TASK_1";
    const auto not_scheduled_task_id = "TASK_2";

    EXPECT_TRUE(ts->start());
    ts->every(std::move(scheduled_task_id), 10ms, simple_task);

    EXPECT_TRUE(ts->remove_task(scheduled_task_id));
    EXPECT_FALSE(ts->remove_task(not_scheduled_task_id));

    EXPECT_FALSE(ts->is_scheduled(scheduled_task_id));
    EXPECT_FALSE(ts->is_scheduled(not_scheduled_task_id));

    ts->every(std::move(scheduled_task_id), 10ms, simple_task);
    EXPECT_TRUE(ts->is_scheduled(scheduled_task_id));
    EXPECT_EQ(ts->tasks_size(), 1);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, duplicate_task_id)
{
    const char* task_id = "TASK_ID";
    EXPECT_FALSE(ts->is_scheduled(task_id));

    ts->start();

    EXPECT_TRUE(ts->in(task_id, 3min, simple_task).has_value());
    EXPECT_FALSE(ts->in(task_id, 3min, simple_task).has_value());

    EXPECT_TRUE(ts->is_scheduled(task_id));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler, remove_last_scheduled_task)
{
}

///////////////////////////////////////////////////////////
// IN
///////////////////////////////////////////////////////////
class test_task_scheduler_in : public test_task_scheduler_api
{
};

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, simple)
{
    ts->start();
    ts->in(3ms, task);
    // This must be evaluated as a XOR since we cannot be sure if
    // the task has already been executed or not.
    // In any case either the task is already executed (thus the tasks_size must be zero)
    // or the the tasks_size is equal to one and the task has not been executed yet.
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, task_argument)
{
    ts->start();

    int arg = 42;
    ts->in(3ms, task_with_argument, arg);
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, task_arguments)
{
    ts->start();

    tc::sdk::tests::test_task_scheduler_api::DummyClass dummy_obj{.ok = true};
    ts->in(3ms, task_with_arguments, "dummy", dummy_obj, std::string("dummy"));
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, task_id)
{
    ts->start();
    const auto task_id = "TASK_ID";
    ts->in("TASK_ID", 2min, task);

    EXPECT_FALSE(is_executed());
    EXPECT_TRUE(is_pending());
    EXPECT_TRUE(ts->is_scheduled(task_id));
    EXPECT_TRUE(ts->is_enabled(task_id));
    EXPECT_TRUE(ts->set_enabled(task_id, false));
    EXPECT_FALSE(ts->is_enabled(task_id));
    EXPECT_FALSE(ts->get_interval(task_id).has_value());
    EXPECT_FALSE(ts->update_interval(task_id, 3ms)); // Can be updated for "every()" api only
    EXPECT_FALSE(ts->get_interval(task_id).has_value());
    EXPECT_TRUE(ts->remove_task(task_id));
    EXPECT_FALSE(ts->remove_task(task_id));
    EXPECT_FALSE(ts->is_scheduled(task_id));
    EXPECT_FALSE(is_pending());
    EXPECT_FALSE(ts->update_interval(task_id, 3ms));
    EXPECT_FALSE(ts->get_interval(task_id).has_value());

    ts->in("TASK_ID", 3ms, task);
    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
    EXPECT_FALSE(ts->is_scheduled(task_id));
    EXPECT_FALSE(ts->is_enabled(task_id));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, unique_task_id)
{
    ts->start();
    ts->in("TASK_ID", 1min, task);
    ts->in("TASK_ID", 2min, task);
    ts->in("TASK_ID", 3min, task);

    EXPECT_EQ(ts->tasks_size(), 1);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, return_value)
{
    ts->start();
    const int arg = 42;
    auto result = ts->in(5ms, task_with_result, arg);

    EXPECT_TRUE(result.has_value());
    auto future_result = std::move(result.value());
    EXPECT_EQ(future_result.get(), arg + 1);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_in, wait_completion)
{
    ts->start();
    auto result = ts->in(5ms, task);

    EXPECT_TRUE(result.has_value());
    result->wait();
    EXPECT_TRUE(is_executed());
}

///////////////////////////////////////////////////////////
// AT
///////////////////////////////////////////////////////////
class test_task_scheduler_at : public test_task_scheduler_api
{
};

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, simple)
{
    ts->start();
    ts->at(tc::sdk::clock::now() + 3ms, task);
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, task_argument)
{
    ts->start();

    int arg = 42;
    ts->at(tc::sdk::clock::now() + 3ms, task_with_argument, arg);
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, task_arguments)
{
    ts->start();

    tc::sdk::tests::test_task_scheduler_api::DummyClass dummy_obj{.ok = true};
    ts->at(tc::sdk::clock::now() + 3ms, task_with_arguments, "dummy", dummy_obj, std::string("dummy"));
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, task_id)
{
    ts->start();
    const auto task_id = "TASK_ID";
    ts->at("TASK_ID", tc::sdk::clock::now() + 2min, task);

    EXPECT_FALSE(is_executed());
    EXPECT_TRUE(is_pending());
    EXPECT_TRUE(ts->is_scheduled(task_id));
    EXPECT_TRUE(ts->is_enabled(task_id));
    EXPECT_TRUE(ts->set_enabled(task_id, false));
    EXPECT_FALSE(ts->is_enabled(task_id));
    EXPECT_FALSE(ts->get_interval(task_id).has_value());
    EXPECT_FALSE(ts->update_interval(task_id, 3ms)); // Can be updated for "every()" api only
    EXPECT_FALSE(ts->get_interval(task_id).has_value());
    EXPECT_TRUE(ts->remove_task(task_id));
    EXPECT_FALSE(ts->remove_task(task_id));
    EXPECT_FALSE(ts->is_scheduled(task_id));
    EXPECT_FALSE(is_pending());
    EXPECT_FALSE(ts->update_interval(task_id, 3ms));
    EXPECT_FALSE(ts->get_interval(task_id).has_value());

    ts->at("TASK_ID", tc::sdk::clock::now() + 3ms, task);
    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
    EXPECT_FALSE(ts->is_scheduled(task_id));
    EXPECT_FALSE(ts->is_enabled(task_id));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, unique_task_id)
{
    ts->start();
    ts->at("TASK_ID", tc::sdk::clock::now() + 1min, task);
    ts->at("TASK_ID", tc::sdk::clock::now() + 2min, task);
    ts->at("TASK_ID", tc::sdk::clock::now() + 3min, task);

    EXPECT_EQ(ts->tasks_size(), 1);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, return_value)
{
    ts->start();
    const int arg = 42;
    auto result = ts->at(tc::sdk::clock::now() + 5ms, task_with_result, arg);

    EXPECT_TRUE(result.has_value());
    auto future_result = std::move(result.value());
    EXPECT_EQ(future_result.get(), arg + 1);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, wait_completion)
{
    ts->start();
    auto result = ts->at(tc::sdk::clock::now() + 5ms, task);

    EXPECT_TRUE(result.has_value());
    result->wait();
    EXPECT_TRUE(is_executed());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_at, add_before_start)
{
    std::optional<std::future<void>> result;

    result = ts->at(tc::sdk::clock::now() + 5ms, task);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(is_executed());

    result = ts->at(tc::sdk::clock::now() + 5ms, task_with_argument, 42);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(is_executed());

    result = ts->at("task_id", tc::sdk::clock::now() + 5ms, task_with_argument, 42);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(is_executed());

    ts->start();

    result = ts->at(tc::sdk::clock::now() + 5ms, task);
    EXPECT_TRUE(result.has_value());
    result->wait();

    result = ts->at(tc::sdk::clock::now() + 5ms, task_with_argument, 42);
    EXPECT_TRUE(result.has_value());
    result->wait();

    result = ts->at("task_id", tc::sdk::clock::now() + 5ms, task_with_argument, 42);
    EXPECT_TRUE(result.has_value());
    result->wait();

    EXPECT_TRUE(is_executed());
}

///////////////////////////////////////////////////////////
// EVERY
///////////////////////////////////////////////////////////
class test_task_scheduler_every : public test_task_scheduler_api
{
};

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, simple)
{
    ts->start();
    EXPECT_TRUE(ts->every(3ms, task));
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_TRUE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, task_argument)
{
    ts->start();

    int arg = 42;
    EXPECT_TRUE(ts->every(5ms, task_with_argument, arg));
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_TRUE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, task_arguments)
{
    ts->start();

    tc::sdk::tests::test_task_scheduler_api::DummyClass dummy_obj{.ok = true};
    EXPECT_TRUE(ts->every(5ms, task_with_arguments, "dummy", dummy_obj, std::string("dummy")));
    EXPECT_TRUE(is_executed() ^ is_pending());

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_TRUE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, task_id)
{
    ts->start();
    const auto task_id = "TASK_ID";
    EXPECT_TRUE(ts->every("TASK_ID", 2min, task));

    EXPECT_FALSE(is_executed());
    EXPECT_TRUE(is_pending());
    EXPECT_TRUE(ts->is_scheduled(task_id));
    EXPECT_TRUE(ts->is_enabled(task_id));
    EXPECT_TRUE(ts->set_enabled(task_id, false));
    EXPECT_FALSE(ts->is_enabled(task_id));
    EXPECT_EQ(ts->get_interval(task_id).value(), 2min);
    EXPECT_TRUE(ts->update_interval(task_id, 5ms));
    EXPECT_EQ(ts->get_interval(task_id).value(), 5ms);
    EXPECT_TRUE(ts->remove_task(task_id));
    EXPECT_FALSE(ts->remove_task(task_id));
    EXPECT_FALSE(ts->is_scheduled(task_id));
    EXPECT_FALSE(is_pending());
    EXPECT_FALSE(ts->update_interval(task_id, 5ms));
    EXPECT_FALSE(ts->get_interval(task_id).has_value());

    EXPECT_TRUE(ts->every("TASK_ID", 2min, task));
    EXPECT_TRUE(ts->update_interval(task_id, 5ms));
    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_TRUE(is_pending());
    EXPECT_TRUE(ts->is_scheduled(task_id));
    EXPECT_TRUE(ts->is_enabled(task_id));

    ts->stop();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
    EXPECT_FALSE(ts->is_scheduled(task_id));
    EXPECT_FALSE(ts->is_enabled(task_id));
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, unique_task_id)
{
    ts->start();
    ts->every("TASK_ID", 1min, task);
    ts->every("TASK_ID", 2min, task);
    ts->every("TASK_ID", 3min, task);

    EXPECT_EQ(ts->tasks_size(), 1);
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, delay)
{
    ts->start();
    EXPECT_TRUE(ts->every(
        "TASK_ID",
        tc::sdk::task_scheduler::interval_t{5ms},
        tc::sdk::task_scheduler::delay_t{100ms},
        task));

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_TRUE(is_pending());
}

// NOLINTNEXTLINE
TEST_F(test_task_scheduler_every, self_removing)
{
    ts->start();
    EXPECT_TRUE(ts->every(
        "TASK_ID",
        tc::sdk::task_scheduler::interval_t{5ms},
        tc::sdk::task_scheduler::delay_t{100ms},
        self_removing_task,
        std::string("TASK_ID")));

    wait_execution();

    EXPECT_TRUE(is_executed());
    EXPECT_FALSE(is_pending());
}
}
