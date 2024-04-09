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

#include <teiacare/sdk/thread_pool.hpp>

namespace tc::sdk
{
thread_pool::thread_pool()
    : _is_running{false}
{
}

thread_pool::~thread_pool()
{
    if (_is_running)
        stop();
}

bool thread_pool::start(const unsigned int num_threads)
{
    std::scoped_lock lock(_is_running_mutex);
    if (_is_running)
        return false;

    _is_running = true;

    const auto thread_count = std::clamp(num_threads, 1u, std::thread::hardware_concurrency());
    _threads.reserve(thread_count);
    is_ready = std::make_shared<std::latch>(thread_count + 1);

    for (unsigned int i = 0; i < thread_count; ++i)
    {
        _threads.emplace_back([this] { worker(); });
    }

    is_ready->arrive_and_wait();
    return true;
}

bool thread_pool::stop()
{
    std::scoped_lock lock(_is_running_mutex);
    if (!_is_running)
        return false;

    _is_running = false;

    {
        std::scoped_lock lock(_task_mutex);
        _task_queue = {};
    }

    _task_cv.notify_all();

    for (auto&& t : _threads)
    {
        if (t.joinable())
            t.join();
    }

    _threads.clear();

    return true;
}

size_t thread_pool::threads_count() const
{
    return _threads.size();
}

bool thread_pool::is_running() const
{
    return _is_running;
}

void thread_pool::worker()
{
    is_ready->arrive_and_wait();

    while (_is_running)
    {
        std::unique_lock lock(_task_mutex);

        _task_cv.wait(lock, [this] { return !_task_queue.empty() || !_is_running; });
        if (!_is_running)
            return;

        auto task = std::move(_task_queue.front());
        _task_queue.pop();
        lock.unlock();

        task();
    }
}

void thread_pool::enqueue_task(tc::sdk::task&& task)
{
    {
        std::scoped_lock lock(_task_mutex);
        _task_queue.emplace(std::move(task));
    }

    _task_cv.notify_one();
}

}
