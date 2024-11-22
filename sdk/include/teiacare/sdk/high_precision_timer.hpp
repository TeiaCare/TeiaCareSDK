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

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

namespace tc::sdk
{
/*!
 * \class high_precision_timer
 * \brief High Precision Timer that triggers a given callback with milliseconds precision.
 *
 * The timer callback can be set to any callable object (lambda, free function, member function)
 * It is mandatory to set the callback interval calling high_precision_timer::start with a duration argument.
 */
class high_precision_timer : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor
     *
     * Creates a tc::sdk::high_precision_timer instance.
     */
    explicit high_precision_timer();

    /*!
     * \brief Destructor
     *
     * Destructs this. If the high_precision_timer is running its task is stopped before deletion.
     */
    ~high_precision_timer();

    /*!
     * \brief Start running the worker task
     * \param interval Rate at which the user callback is triggered
     * \return true if worker has been started, false otherwise
     *
     * If a callback has already been set this function starts the worker thread and returns true.
     * Otherwise returns false and does nothing (the worker thread is not started).
     * Note that this function blocks the caller therad until the worker thread has been started.
     */
    bool start(clock::duration&& interval);

    /*!
     * \brief Stop the worker task
     *
     * The worker thread is joined so that it is possible to restart the timer calling high_precision_timer::start again.
     */
    void stop();

    /*!
     * \brief Set user callback to be invoked periodically
     * \param func Callback function
     * \param args Callback arguments
     *
     * The interval at which the callback is triggered must be specified when calling high_precision_timer::start.
     * This function does not start the worker thread but only sets the user callback.
     */
    template <typename TaskFunction, typename... Args>
    void set_callback(TaskFunction&& func, Args&&... args)
    {
        auto task_func = [t = std::forward<TaskFunction>(func), params = std::make_tuple(std::forward<Args&&>(args)...)] {
            return std::apply(t, params);
        };

        _task = std::make_unique<tc::sdk::task>(std::move(task_func));
    }

    /*!
     * \brief Get the number of invoked callbacks
     * \return number of invoked callbacks
     *
     * A callback is invoked every time the previous callback execution lasts less than the execution interval.
     */
    uint64_t invoked_callback_count() const;

    /*!
     * \brief Get the number of missed callbacks
     * \return number of missed callbacks
     *
     * A callback is missed every time the previous callback execution lasts more than the execution interval.
     */
    uint64_t missed_callback_count() const;

protected:
    void worker();
    void update_next_start_time();

private:
    bool _is_running;
    clock::duration _interval;
    std::unique_ptr<tc::sdk::task> _task;
    clock::time_point _next_task_timepoint;
    std::thread _worker_thread;
    std::condition_variable _worker_cv;
    std::mutex _worker_mutex;
    std::atomic_uint64_t _invoked_callback_count = 0;
    std::atomic_uint64_t _missed_callback_count = 0;
};

}
