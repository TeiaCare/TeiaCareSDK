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

#include <teiacare/sdk/clock.hpp>
#include <teiacare/sdk/non_copyable.hpp>
#include <teiacare/sdk/non_moveable.hpp>
#include <teiacare/sdk/task.hpp>
#include <teiacare/sdk/thread_pool.hpp>

#include <chrono>
#include <condition_variable>
#include <future>
#include <map>
#include <optional>
#include <string>

namespace tc::sdk
{
/*!
 * \class task_scheduler
 * \brief Task Scheduler that can launch tasks on based several time-based policies
 *
 * This class is used to manage a queue of tasks using a fixed number of threads.
 * The actual task execution is delegated to an internal tc::sdk::task_pool object.
 */
class task_scheduler : private non_copyable, private non_moveable
{
public:
    using delay_t = tc::sdk::clock::duration;
    using interval_t = tc::sdk::clock::duration;

private:
    class schedulable_task : private non_copyable, private non_moveable
    {
    public:
        template <typename FunctionType>
        explicit schedulable_task(FunctionType&& f)
            : _task{std::make_shared<tc::sdk::task>(std::forward<FunctionType>(f))}
            , _is_enabled{true}
        {
        }

        template <typename FunctionType>
        explicit schedulable_task(FunctionType&& f, size_t hash)
            : _task{std::make_shared<tc::sdk::task>(std::forward<FunctionType>(f))}
            , _is_enabled{true}
            , _hash{hash}
        {
        }

        template <typename FunctionType>
        explicit schedulable_task(FunctionType&& f, tc::sdk::clock::duration interval)
            : _task{std::make_shared<tc::sdk::task>(std::forward<FunctionType>(f))}
            , _is_enabled{true}
            , _interval{interval}
        {
        }

        template <typename FunctionType>
        explicit schedulable_task(FunctionType&& f, size_t hash, tc::sdk::clock::duration interval)
            : _task{std::make_shared<tc::sdk::task>(std::forward<FunctionType>(f))}
            , _is_enabled{true}
            , _interval{interval}
            , _hash{hash}
        {
        }

        ~schedulable_task()
        {
        }

        schedulable_task(schedulable_task&& other) noexcept
            : _task{std::move(other._task)}
            , _is_enabled{std::move(other._is_enabled)}
            , _interval{std::move(other._interval)}
            , _hash{std::move(other._hash)}
        {
        }

        void invoke()
        {
            _task->invoke();
        }

        std::shared_ptr<schedulable_task> clone()
        {
            return std::shared_ptr<schedulable_task>(new schedulable_task(this));
        }

        void set_enabled(bool is_enabled)
        {
            _is_enabled = is_enabled;
        }
        bool is_enabled() const
        {
            return _is_enabled;
        }

        void set_interval(tc::sdk::clock::duration interval)
        {
            _interval = interval;
        }
        std::optional<tc::sdk::clock::duration> interval() const
        {
            return _interval;
        }

        std::optional<size_t> hash() const
        {
            return _hash;
        }

    private:
        std::shared_ptr<tc::sdk::task> _task;
        bool _is_enabled;
        std::optional<tc::sdk::clock::duration> _interval;
        std::optional<size_t> _hash;

        schedulable_task(schedulable_task* st)
            : _task{st->_task}
            , _is_enabled{st->_is_enabled}
            , _interval{st->_interval}
            , _hash{st->_hash}
        {
        }
    };

public:
    /*!
     * \brief Constructor
     *
     * Creates a tc::sdk::task_scheduler instance
     */
    explicit task_scheduler();

    /*!
     * \brief Destructor
     *
     * Destructs this. If the task_scheduler is running its tasks are stopped first
     */
    ~task_scheduler();

    /*!
     * \brief Start running tasks
     * \param num_threads Number of threads that will be used in the underlying tc::sdk::thread_pool
     * \return true if started successfully
     *
     * This function starts the task_scheduler worker thread.
     * The function is guaranteed to return after the scheduler thread is started.
     */
    bool start(const unsigned int num_threads = std::thread::hardware_concurrency());

    /*!
     * \brief Stop all running tasks
     * \return true if stopped successfully
     *
     * This function stops the task_scheduler execution and stops all the running tasks.
     */
    bool stop();

    /*!
     * \brief Get the number of scheduled tasks
     * \return Number of tasks to be run
     *
     * This function return the number of tasks that are currently scheduled for execution (both enabled and disabled).
     */
    size_t tasks_size();

    /*!
     * \brief Check if a task is scheduled
     * \param task_id task_id to check
     * \return bool indicating if the task is currently scheduled
     *
     * If a task has been started without a task_id it is not possible to query its status.
     * In case a task_id is not found this function return false.
     * If a task is no longer scheduled it must be added using one of the following APIs:
     * tc::sdk::task_scheduler::in, tc::sdk::task_scheduler::at, tc::sdk::task_scheduler::every.
     */
    bool is_scheduled(const std::string& task_id);

    /*!
     * \brief Check if a task is enabled
     * \param task_id task_id to check
     * \return bool indicating if the task is currently enabled
     *
     * If a task has been started without a task_id it is not possible to query its status.
     * In case a task_id is not found this function return false.
     * By default new tasks are enabled.
     * A task can be enabled or disabled by calling tc::sdk::task_scheduler::set_enabled.
     */
    bool is_enabled(const std::string& task_id);

    /*!
     * \brief Enable or disable task
     * \param task_id task_id to enable or disable
     * \param is_enabled true enables, false disables the given task_id
     * \return bool indicating if the task is currently enabled
     *
     * If a task has been started without a task_id it is not possible to update its status.
     * In case a task_id is not found this function return false.
     * It is possible to check if a task is enabled or disabled by calling tc::sdk::task_scheduler::is_enabled.
     */
    bool set_enabled(const std::string& task_id, bool is_enabled);

    /*!
     * \brief Remove a task
     * \param task_id task_id to remove
     * \return bool indicating if the task has been properly removed
     *
     * If a task has been started without a task_id it is not possible to remove it.
     * In case a task_id is not found this function return false.
     * It is possible to check if a task is scheduled by calling tc::sdk::task_scheduler::is_scheduled.
     */
    bool remove_task(const std::string& task_id);

    /*!
     * \brief Retrieve task interval.
     * \param task_id task_id to retrieve.
     * \return std::optional<sdk::clock::duration> interval associated with given task_id.
     *
     * If a task is not recursive (i.e. has not been started with every() APIs)
     * or the task has not been assigned a task_id, it is not possible to retrieve its interval.
     * In case of any failure (task_id not found or task non recursive) this function returns std::nullopt.
     */
    std::optional<sdk::clock::duration> get_interval(const std::string& task_id);

    /*!
     * \brief Update a task interval
     * \param task_id task_id to update
     * \param interval new task interval to set
     * \return bool indicating if the task has been properly updated
     *
     * If a task is not recursive (i.e. has not been started with every() APIs)
     * or the task has not been assigned a task_id, it is not possible to update it.
     * In case of any failure (task_id not found or task non recursive) this function return false.
     */
    bool update_interval(const std::string& task_id, interval_t interval);

    /*!
     * \brief Spawn a task at a given time_point
     *
     * TODO
     */
    template <typename TaskFunction>
    auto at(tc::sdk::clock::time_point&& timepoint, TaskFunction&& func)
        -> std::optional<std::future<std::invoke_result_t<TaskFunction>>>
    {
        auto task = [t = std::forward<TaskFunction>(func)] {
            return t();
        };

        using ReturnType = std::invoke_result_t<TaskFunction>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();

        if (!add_task(std::move(timepoint), schedulable_task(std::move(task_wrapper))))
            return std::nullopt;

        return future;
    }

    /*!
     * \brief Spawn a task at a given time_point
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto at(tc::sdk::clock::time_point&& timepoint, TaskFunction&& func, Args&&... args)
        -> std::optional<std::future<std::invoke_result_t<TaskFunction, Args...>>>
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] {
            return std::apply(t, params);
        };

        using ReturnType = std::invoke_result_t<TaskFunction, Args...>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();

        if (!add_task(std::move(timepoint), schedulable_task(std::move(task_wrapper))))
            return std::nullopt;

        return future;
    }

    /*!
     * \brief Spawn a task at a given time_point
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto at(std::string&& task_id, tc::sdk::clock::time_point&& timepoint, TaskFunction&& func, Args&&... args)
        -> std::optional<std::future<std::invoke_result_t<TaskFunction, Args...>>>
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] {
            return std::apply(t, params);
        };

        using ReturnType = std::invoke_result_t<TaskFunction, Args...>;
        auto task_wrapper = std::packaged_task<ReturnType()>(task);
        std::future<ReturnType> future = task_wrapper.get_future();

        if (!add_task(std::move(timepoint), schedulable_task(std::move(task_wrapper), _hasher(task_id))))
            return std::nullopt;

        return future;
    }

    /*!
     * \brief Spawn a task at a given time_duration
     *
     * TODO
     */
    template <typename TaskFunction>
    auto in(delay_t&& delay, TaskFunction&& func)
        -> std::optional<std::future<std::invoke_result_t<TaskFunction>>>
    {
        return at(
            std::forward<tc::sdk::clock::time_point>(tc::sdk::clock::now() + delay),
            std::forward<TaskFunction>(func));
    }

    /*!
     * \brief Spawn a task at a given time_duration
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto in(delay_t&& delay, TaskFunction&& func, Args&&... args)
        -> std::optional<std::future<std::invoke_result_t<TaskFunction, Args...>>>
    {
        return at(
            std::forward<tc::sdk::clock::time_point>(tc::sdk::clock::now() + delay),
            std::forward<TaskFunction>(func),
            std::forward<Args>(args)...);
    }

    /*!
     * \brief Spawn a task at a given time_duration
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto in(std::string&& task_id, delay_t&& delay, TaskFunction&& func, Args&&... args)
        -> std::optional<std::future<std::invoke_result_t<TaskFunction, Args...>>>
    {
        return at(
            std::forward<std::string>(task_id),
            std::forward<tc::sdk::clock::time_point>(tc::sdk::clock::now() + delay),
            std::forward<TaskFunction>(func),
            std::forward<Args>(args)...);
    }

    /*!
     * \brief Spawn a task periodically
     *
     * TODO
     */
    template <typename TaskFunction>
    auto every(interval_t&& interval, TaskFunction&& func) -> bool
    {
        auto task = [t = std::forward<TaskFunction>(func)] {
            return t();
        };

        return add_task(tc::sdk::clock::now(), schedulable_task(std::move(task), interval));
    }

    /*!
     * \brief Spawn a task periodically
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto every(interval_t&& interval, TaskFunction&& func, Args&&... args) -> bool
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] {
            return std::apply(t, params);
        };

        return add_task(tc::sdk::clock::now(), schedulable_task(std::move(task), interval));
    }

    /*!
     * \brief Spawn a task periodically
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto every(std::string&& task_id, interval_t&& interval, TaskFunction&& func, Args&&... args) -> bool
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] {
            return std::apply(t, params);
        };

        return add_task(tc::sdk::clock::now(), schedulable_task(std::move(task), _hasher(task_id), interval));
    }

    /*!
     * \brief Spawn a task periodically
     *
     * TODO
     */
    template <typename TaskFunction, typename... Args>
    auto every(std::string&& task_id, interval_t&& interval, delay_t&& delay, TaskFunction&& func, Args&&... args) -> bool
    {
        auto task = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args>(args)...)] {
            return std::apply(t, params);
        };

        return add_task(tc::sdk::clock::now() + delay, schedulable_task(std::move(task), _hasher(task_id), interval));
    }

private:
    tc::sdk::thread_pool _tp;
    std::thread _scheduler_thread;
    std::multimap<tc::sdk::clock::time_point, schedulable_task> _tasks;
    std::condition_variable _update_tasks_cv;
    std::mutex _update_tasks_mtx;
    std::hash<std::string> _hasher;
    std::atomic<tc::sdk::clock::time_point> _next_task_timepoint;

    bool add_task(tc::sdk::clock::time_point&& timepoint, schedulable_task&& st);
    void update_tasks();
    auto get_task_iterator(const std::string& task_id) -> decltype(_tasks)::iterator;
    bool already_exists(const std::optional<size_t>& opt_hash);
};

}
