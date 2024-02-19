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
#include <sdk/non_moveable.hpp>

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <algorithm>

namespace tc::sdk
{
/*! 
 * \class blocking_queue
 * \brief Thread safe, blocking queue
 * \tparam T Queue items type
 * 
 * The queue has a fixed capacity (i.e. maximum number of items that can be hold).  
 * When the queue is full and a new item is needs to be inserted via blocking_queue::push() the queue blocks until an item is popped.  
 * Viceversa, when the queue is empty and an item is required via blocking_queue::pop(), the queue blocks until the first item is pushed.
 */
template <typename T>
class blocking_queue : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor
     * \param capacity set the maximum number of items the queue can hold
     * 
     * Creates a tc::sdk::blocking_queue instance. 
     */
    explicit blocking_queue(size_t capacity)
        : _capacity(std::clamp(capacity, size_t{1}, std::numeric_limits<size_t>::max()))
    {
    }

    /*!
     * \brief Destructor
     * 
     * Destructs this.
     */
    ~blocking_queue() = default;

    /*!
     * \brief Insert an item into the queue
     * \tparam item The item to be inserted
     *
     * If this method is called when the queue is full the calling thread is blocked until an item is popped from the queue.
     */
    void push(const T& item)
    {
        std::unique_lock lock(_mutex);
        if(is_full())
            _last_item_popped.wait(lock, [this] { return !is_full(); });
    
        _queue.push(item);
        push_impl(std::move(lock));
    }

    /*!
     * \brief Insert an item into the queue
     * \tparam item The item to be inserted
     *
     * If this method is called when the queue is full the calling thread is blocked until an item is popped from the queue.
     * This is an overload of blocking_queue::push(const T&) which emplaces the item instead of using a const ref.
     */
    void push(T&& item)
    {
        std::unique_lock lock(_mutex);
        if(is_full())
            _last_item_popped.wait(lock, [this] { return !is_full(); });
        
        _queue.emplace(item);
        push_impl(std::move(lock));
    }

    /*!
     * \brief Try to insert an item into the queue
     * \tparam item The item to be inserted
     * \return true if the item was inserted
     *
     * If this method is called when the queue is full the calling thread is not blocked and false is returned.
     * Otherwise the thread is locked until the item is inserted, then true is returned.
     */
    bool try_push(const T& item)
    {
        std::unique_lock lock(_mutex);
        if (is_full())
            return false;

        _queue.push(item);
        push_impl(std::move(lock));
        
        return true;
    }

    /*!
     * \brief Try to insert an item into the queue
     * \tparam item The item to be inserted
     * \return true if the item was inserted
     *
     * If this method is called when the queue is full the calling thread is not blocked and false is returned.
     * Otherwise the thread is locked until the item is inserted, then true is returned.
     * This is an overload of blocking_queue::try_push(const T&) which emplaces the item instead of using a const ref.
     */
    bool try_push(T&& item)
    {
        std::unique_lock lock(_mutex);
        if (is_full())
            return false;

        _queue.emplace(item);
        push_impl(std::move(lock));
        
        return true;
    }

    /*!
     * \brief Retrieve an item from the queue
     * \return T value
     *
     * If this method is called when the queue is full the calling thread is blocked until an item is pushed in the queue.
     */
    T pop()
    {
        std::unique_lock lock(_mutex);
        if (is_empty())
            _first_item_pushed.wait(lock, [this] { return !is_empty(); });
    
        T item = _queue.front();
        _queue.pop();
        pop_impl(std::move(lock));

        return item;
    }

    /*!
     * \brief Try to retrieve an item from the queue
     * \return std::optional<T> value
     *
     * If this method is called when the queue is empty the calling thread is not blocked and std::nullopt is returned.
     * Otherwise the thread is locked until the item is removed, then std::optional<T> is returned.
     */
    std::optional<T> try_pop()
    {
        std::unique_lock lock(_mutex);
        if (is_empty())
            return std::nullopt;

        T item = _queue.front();
        _queue.pop();
        pop_impl(std::move(lock));
        
        return std::move(std::optional(item));
    }

    /*!
     * \brief Get the number of items currently in the queue
     * \return queue size
     */
    [[nodiscard]] size_t size() const
    {
        std::lock_guard lock(_mutex);
        return _queue.size();
    }

    /*!
     * \brief Get the maximum number of items that the queue can hold
     * \return queue capacity
     */
    [[nodiscard]] size_t capacity() const
    {
        std::lock_guard lock(_mutex);
        return _capacity;
    }

private:
    std::queue<T> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _last_item_popped;
    std::condition_variable _first_item_pushed;
    const size_t _capacity;

    inline void push_impl(std::unique_lock<std::mutex>&& lock) 
    {
        const bool is_first_item_pushed = _queue.size() == 1;
        lock.unlock();
        
        if (is_first_item_pushed)
            _first_item_pushed.notify_all();
    }

    inline void pop_impl(std::unique_lock<std::mutex>&& lock)
    {
        const bool is_last_item_popped = _queue.size() == _capacity - 1;
        lock.unlock();
        
        if(is_last_item_popped)
            _last_item_popped.notify_all();
    }

    inline bool is_empty() const { return _queue.empty(); }
    inline bool is_full() const { return _queue.size() >= _capacity; }
};

}
