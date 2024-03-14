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

#include <teiacare/sdk/high_precision_timer.hpp>

#include <future>

namespace tc::sdk
{
high_precision_timer::high_precision_timer()
    : _is_running{true}
    , _interval{clock::duration::max()}
    , _task{nullptr}
{
}

high_precision_timer::~high_precision_timer()
{
    stop();
}

bool high_precision_timer::start(clock::duration&& interval)
{
    if (!_task)
        return false;

    stop();

    _invoked_callback_count = 0;
    _missed_callback_count = 0;

    _next_task_timepoint = clock::now() + interval;
    _interval = interval;

    std::promise<void> thread_started_notifier;
    std::future<void> thread_started_watcher = thread_started_notifier.get_future();

    _worker_thread = std::thread([this, &thread_started_notifier] {
        {
            std::unique_lock<std::mutex> lock(_worker_mutex);
            _is_running = true;
        }
        thread_started_notifier.set_value();
        worker();
    });

    thread_started_watcher.wait();
    return true;
}

void high_precision_timer::stop()
{
    {
        std::unique_lock<std::mutex> lock(_worker_mutex);
        _is_running = false;
    }

    _worker_cv.notify_all();

    if (_worker_thread.joinable())
        _worker_thread.join();
}

uint64_t high_precision_timer::invoked_callback_count() const
{
    return _invoked_callback_count;
}

uint64_t high_precision_timer::missed_callback_count() const
{
    return _missed_callback_count;
}

void high_precision_timer::worker()
{
    std::unique_lock lock(_worker_mutex);
    while (_is_running)
    {
        if (_worker_cv.wait_until(lock, _next_task_timepoint, [this] { return !_is_running; }))
            break;

        _task->invoke();
        ++_invoked_callback_count;
        update_next_start_time();
    }
}

void high_precision_timer::update_next_start_time()
{
    auto task_next_start_time = _next_task_timepoint;
    const auto now = clock::now();
    while (now > task_next_start_time)
    {
        task_next_start_time += _interval;
        ++_missed_callback_count;
    }
    // Adjust _missed_callback_count removing the last execution from the counter.
    --_missed_callback_count;
    _next_task_timepoint = task_next_start_time;
}

}
