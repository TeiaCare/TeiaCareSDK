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

#include <sdk/non_copyable.hpp>

#include <memory>
#include <algorithm>
#include <type_traits>

namespace tc::sdk
{
/*! 
 * \class task
 * \brief Move-only callable object.
 * 
 * This class represents a callable object. Can be initialized with any invocable type that supports operator().  
 * Internally the class implements a type-erasure idiom to accept any callable signature without exposing it to the outside.
 */
class task : private non_copyable
{
private:
    struct task_base
    {
        virtual ~task_base() = default;
        // task_base(const task_base&) = delete;
        // task_base(task_base&&) noexcept = delete;
        // task_base& operator=(const task_base&) = delete;
        // task_base& operator=(task_base&&) = delete;

        virtual void invoke() = 0;
    };

    template<typename FunctionType>
    struct task_impl : task_base
    {
        explicit task_impl(FunctionType&& f) 
            : _func{ std::move(f) } 
        { 
            static_assert(std::is_invocable_v<decltype(f)>); 
        }

        ~task_impl() noexcept override = default;
        // task_impl(const task_impl&) = delete;
        // task_impl(task_impl&&) noexcept = delete;
        // task_impl& operator=(const task_impl&) = delete;
        // task_impl& operator=(task_impl&&) = delete;
        
        void invoke() override { _func(); }
        FunctionType _func;
    };

public:
    /*!
     * \brief Default constructor.
     * \param f Callable parameterless object wrapped within this task instance.
     * 
     * Creates a task instance with the given callable object.
     * The callable object can be e.g. a lambda function, a functor, a free function or a class method bound to an object.
     */
    template<typename FunctionType>
    explicit task(FunctionType&& f) 
        : _impl{ std::make_unique<task_impl<FunctionType>>(std::move(f)) } 
    {
    }
    
    /*!
     * \brief Move constructor.
     * \param other task object.
     * 
     * Move constructs a task instance to this.
     */
    task(task&& other) noexcept;

    /*!
     * \brief Destructor
     * 
     * Destructs this.
     */
    ~task() noexcept = default;

    /*!
     * \brief Move assignment operator
     * 
     * Move assignment operator marked as deleted. 
     */
    task& operator=(task&&) = delete;

    /*!
     * \brief operator()
     * 
     * Invokes the task.
     */
    void operator()();
    
    /*!
     * \brief invoke
     * 
     * Invokes the task.
     * Explicit overload of operator().
     */
    void invoke();

private:
    std::unique_ptr<task_base> _impl;
};

}

