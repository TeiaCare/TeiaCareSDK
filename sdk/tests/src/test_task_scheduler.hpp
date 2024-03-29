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

#include <teiacare/sdk/task_scheduler.hpp>

#include <gtest/gtest.h>
#include <latch>

using namespace std::chrono_literals;

namespace tc::sdk::tests
{
class test_task_scheduler : public ::testing::Test
{
protected:
    test_task_scheduler()
        : ts{std::make_unique<tc::sdk::task_scheduler>()}
    {
    }

    ~test_task_scheduler() override
    {
        ts->stop();
    }

    std::unique_ptr<tc::sdk::task_scheduler> ts;
    std::function<void()> simple_task = [] { std::this_thread::sleep_for(5ms); };
};

class test_task_scheduler_api : public test_task_scheduler
{
protected:
    void wait_execution()
    {
        _sync.wait();
    }

    bool is_executed() const
    {
        return _is_executed;
    }

    bool is_pending() const
    {
        return ts->tasks_size() > 0;
    }

    std::function<void()> task = [this] {
        std::this_thread::sleep_for(sleep_time);
        _is_executed = true;

        if (!_sync.try_wait())
            _sync.count_down();
    };

    std::function<void(int)> task_with_argument = [this](int arg) {
        std::this_thread::sleep_for(sleep_time);
        _is_executed = true;
        EXPECT_EQ(arg, 42);

        if (!_sync.try_wait())
            _sync.count_down();
    };

    struct DummyClass
    {
        bool ok = false;
    };

    std::function<void(const char*, DummyClass, const std::string&)> task_with_arguments = [this](const char*, DummyClass dd, const std::string&) {
        std::this_thread::sleep_for(sleep_time);
        _is_executed = true;
        EXPECT_TRUE(dd.ok);

        if (!_sync.try_wait())
            _sync.count_down();
    };

    std::function<int(int)> task_with_result = [this](int arg) {
        std::this_thread::sleep_for(sleep_time);
        return arg + 1;
    };

    std::function<void(const std::string&)> self_removing_task = [this](const std::string& task_id) {
        std::this_thread::sleep_for(sleep_time);
        _is_executed = true;
        ts->remove_task(task_id);

        if (!_sync.try_wait())
            _sync.count_down();
    };

private:
    std::latch _sync{1};
    bool _is_executed{false};
    std::chrono::milliseconds sleep_time{2};
};

}
