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
#include <teiacare/sdk/non_moveable.hpp>

#include <memory>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>

namespace tc::sdk
{
/*!
 * \class service_locator
 * \brief TODO
 *
 * TODO
 */
class service_locator : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Get service_locator instance
     */
    static service_locator& instance();

    /*!
     * \brief Destructor
     *
     * Destructs this. If the high_precision_timer is running its task is stopped before deletion.
     */
    ~service_locator() noexcept = default;

    /*!
     * \brief Create and add a new service in place
     *
     * TODO
     */
    template <typename ServiceInterfaceT, typename ServiceT, typename... Args>
    bool register_service(Args... args)
    {
        static_assert(std::is_abstract_v<ServiceInterfaceT>,
                      "\nServiceInterfaceT is not an abstract class");

        static_assert(std::is_constructible_v<ServiceT, Args...>,
                      "\nWrong arguments of ServiceT constructor");

        static_assert(std::is_base_of_v<ServiceInterfaceT, ServiceT>,
                      "\nServiceT must inherit from ServiceInterfaceT");

        auto [_, is_inserted] = _services.emplace(get_id<ServiceInterfaceT>(), std::make_shared<ServiceT>(std::forward<Args>(args)...));
        return is_inserted;
    }

    /*!
     * \brief Add an already created instance
     *
     * TODO
     */
    template <typename ServiceT>
    bool register_instance(std::shared_ptr<ServiceT> instance)
    {
        auto [_, is_inserted] = _services.emplace(get_id<ServiceT>(), std::move(instance));
        return is_inserted;
    }

    /*!
     * \brief Remove a service or an instance
     *
     * TODO
     */
    template <typename T>
    bool unregister()
    {
        const auto service_id = get_id<T>();
        if (_services.contains(service_id))
        {
            _services.erase(service_id);
            return true;
        }

        return false;
    }

    /*!
     * \brief Check if a service or an instance is currently registered
     *
     * TODO
     */
    template <typename T>
    bool is_registered()
    {
        return _services.contains(get_id<T>());
    }

    /*!
     * \brief Get a registered service instance
     *
     * TODO
     */
    template <typename ServiceT>
    std::shared_ptr<ServiceT> get() const
    {
        if (auto service = _services.find(get_id<ServiceT>()); service != _services.end())
            return std::static_pointer_cast<ServiceT>(service->second);

        return nullptr;
    }

private:
    explicit service_locator() = default;

    template <typename T>
    size_t get_id() const noexcept
    {
        return std::move(typeid(T).hash_code());
    }

    std::unordered_map<size_t, std::shared_ptr<void>> _services;
};

}
