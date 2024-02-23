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

#include <teiacare/sdk/task_scheduler.hpp>

namespace tc::sdk
{
task_scheduler::task_scheduler()
{
}

task_scheduler::~task_scheduler()
{
    stop();
}

bool task_scheduler::start(const unsigned int num_threads)
{
    if (!_tp.start(num_threads))
        return false;

    std::promise<void> thread_started_notifier;
    std::future<void> thread_started_watcher = thread_started_notifier.get_future();

    _scheduler_thread = std::thread([this, &thread_started_notifier] {
        thread_started_notifier.set_value();

        while (_tp.is_running())
        {
            std::unique_lock lock(_update_tasks_mtx);

            if (_tasks.empty())
            {
                _update_tasks_cv.wait(lock, [this] { return !_tp.is_running() || !_tasks.empty(); });
            }
            else
            {
                // If the last enqueued task is the first to be scheduled _next_task_timepoint must be updated to its timepoint.
                // Check if the condition variable _update_tasks_cv is triggered because of a timeout (i.e. _next_task_timepoint has just been reached),
                // or because of a new notification (i.e. a new task has been enqueued and the condition variable is notified):
                // in case of a timeout proceed with update_tasks(), otherwise continue.
                _next_task_timepoint = _tasks.begin()->first;
                if (_update_tasks_cv.wait_until(lock, _next_task_timepoint.load()) == std::cv_status::no_timeout)
                    continue;
            }

            if (!_tp.is_running())
                return;

            update_tasks();
        }
    });

    thread_started_watcher.wait();

    return true;
}

bool task_scheduler::stop()
{
    if (!_tp.stop())
        return false;

    _update_tasks_cv.notify_all();

    {
        std::scoped_lock lock(_update_tasks_mtx);
        _tasks.clear();
    }

    if (_scheduler_thread.joinable())
        _scheduler_thread.join();

    return true;
}

size_t task_scheduler::tasks_size()
{
    std::scoped_lock lock(_update_tasks_mtx);
    return _tasks.size();
}

bool task_scheduler::is_scheduled(const std::string& task_id)
{
    std::scoped_lock lock(_update_tasks_mtx);
    return get_task_iterator(task_id) != _tasks.end();
}

bool task_scheduler::is_enabled(const std::string& task_id)
{
    std::scoped_lock lock(_update_tasks_mtx);
    if (auto task = get_task_iterator(task_id); task != _tasks.end())
        return task->second.is_enabled();

    return false;
}

bool task_scheduler::set_enabled(const std::string& task_id, bool is_enabled)
{
    std::scoped_lock lock(_update_tasks_mtx);
    if (auto task = get_task_iterator(task_id); task != _tasks.end())
    {
        task->second.set_enabled(is_enabled);
        return true;
    }

    return false;
}

bool task_scheduler::remove_task(const std::string& task_id)
{
    std::scoped_lock lock(_update_tasks_mtx);
    if (auto task = get_task_iterator(task_id); task != _tasks.end())
    {
        _tasks.erase(task);
        return true;
    }

    return false;
}

std::optional<sdk::clock::duration> task_scheduler::get_interval(const std::string& task_id)
{
    std::scoped_lock lock(_update_tasks_mtx);
    if (auto task_iterator = get_task_iterator(task_id); task_iterator != _tasks.end() && task_iterator->second.interval().has_value())
    {
        return task_iterator->second.interval();
    }

    return std::nullopt;
}

bool task_scheduler::update_interval(const std::string& task_id, interval_t interval)
{
    std::unique_lock lock(_update_tasks_mtx);
    if (auto task_iterator = get_task_iterator(task_id); task_iterator != _tasks.end() && task_iterator->second.interval().has_value())
    {
        const auto task_interval = task_iterator->second.interval().value();
        auto task_next_start_time = task_iterator->first - task_interval;

        const auto now = tc::sdk::clock::now();
        while (now > task_next_start_time)
            task_next_start_time += interval;

        task_iterator->second.set_interval(interval);
        _tasks.emplace(std::move(task_next_start_time), std::move(task_iterator->second));
        _tasks.erase(task_iterator);
        lock.unlock();

        _update_tasks_cv.notify_one();
        return true;
    }

    return false;
}

bool task_scheduler::add_task(tc::sdk::clock::time_point&& timepoint, schedulable_task&& st)
{
    if (!_tp.is_running())
        return false;

    {
        std::scoped_lock lock(_update_tasks_mtx);

        if (already_exists(st.hash()))
            return false;

        _tasks.emplace(std::move(timepoint), std::move(st));
    }

    _update_tasks_cv.notify_one();
    return true;
}

void task_scheduler::update_tasks()
{
    decltype(_tasks) recursive_tasks;

    // All the tasks whose start time is before tc::sdk::clock::now(),
    // (which are the ones from _tasks.begin(), up to last_task_to_process)
    // can be enqueued in the TaskPool.
    auto last_task_to_process = _tasks.upper_bound(tc::sdk::clock::now());
    for (auto it = _tasks.begin(); it != last_task_to_process; it++)
    {
        if (it->second.is_enabled())
        {
            _tp.run([t = it->second.clone()] { t->invoke(); });
            // _tp.run([t = it->second.clone()] { t->invoke(); }, it->second.hash());
        }

        // Keep track of recursive tasks if task has a valid interval value.
        if (it->second.interval().has_value())
        {
            // Make sure that next_start_time is greater than tc::sdk::clock::now(),
            // otherwise the task is scheduled in the past.
            // Increment next_start_time starting from current start_time with a step equal to t->interval().value()
            // in order to keep the scheduling with a fixed sample rate.
            const auto task_interval = it->second.interval().value();
            auto task_next_start_time = it->first + task_interval;
            const auto now = tc::sdk::clock::now();
            while (now > task_next_start_time)
                task_next_start_time += task_interval;

            recursive_tasks.emplace(std::move(task_next_start_time), std::move(it->second));
        }
    }

    // Erase tasks already processed.
    _tasks.erase(_tasks.begin(), last_task_to_process);

    // Re-schedule recursive tasks.
    _tasks.merge(recursive_tasks);
}

auto task_scheduler::get_task_iterator(const std::string& task_id) -> decltype(_tasks)::iterator
{
    return std::find_if(_tasks.begin(), _tasks.end(), [hash = _hasher(task_id)](auto&& it) {
        if (it.second.hash().has_value())
        {
            return hash == it.second.hash().value();
        }
        return false;
    });
}

bool task_scheduler::already_exists(const std::optional<size_t>& opt_hash)
{
    if (!opt_hash.has_value())
        return false;

    return std::any_of(_tasks.begin(), _tasks.end(), [hash = opt_hash.value()](auto&& it) {
        if (it.second.hash().has_value())
        {
            return hash == it.second.hash().value();
        }
        return false;
    });
}

}
