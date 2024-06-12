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

#include <teiacare/sdk/non_copyable.hpp>

#include <memory>
#include <type_traits>
#include <utility>

namespace tc::sdk
{
/*!
 * \class task
 * \brief Generic type-erased callable object.
 *
 * This class represents a callable object. Can be initialized with any invocable type that supports operator().
 * Internally the class implements a type-erasure idiom to accept any callable signature without exposing it to the outside.
 */
class task : private tc::sdk::non_copyable
{
public:
    /*!
     * \brief Default constructor.
     * \param callable Callable parameterless object wrapped within this task instance.
     *
     * Creates a moveable-only task instance with the given callable object.
     * The callable object can be e.g. a lambda function, a functor, a free function or a class method bound to an object.
     */
    template <typename CallableType>
    explicit task(CallableType&& callable)
        : _callable(std::make_unique<task_impl<CallableType>>(std::move(callable)))
    {
    }

    /*!
     * \brief Move Constructor. Move a tc::sdk::task instance into another one.
     */
    task(task&& other) noexcept = default;

    /*!
     * \brief Move assignment operator marked as deleted.
     */
    task& operator=(task&& other) noexcept = delete;

    /*!
     * \brief Destructor
     *
     * Destructs this.
     */
    ~task() noexcept = default;

    /*!
     * \brief invoke
     *
     * Invokes the task.
     */
    void invoke() const
    {
        _callable->invoke();
    }

    /*!
     * \brief operator()
     *
     * Invokes the task.
     */
    void operator()() const
    {
        invoke();
    }

private:
    struct task_base
    {
        virtual ~task_base() = default;
        virtual void invoke() = 0;
    };

    template <typename CallableType>
    struct task_impl : public task_base
    {
        explicit task_impl(CallableType callable)
            : callable(std::move(callable))
        {
        }

        ~task_impl() noexcept override = default;

        void invoke() override
        {
            callable();
        }

    private:
        CallableType callable;
    };

    std::unique_ptr<task_base> _callable;
};

}
