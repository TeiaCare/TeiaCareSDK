#pragma once

#include <sdk/non_copyable.hpp>
#include <sdk/non_moveable.hpp>

#include <type_traits>
#include <unordered_map>
#include <typeinfo>
#include <memory>

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
     * \brief Add a new service
     * 
     * TODO
     */
    template<typename ServiceInterfaceT, typename ServiceT, typename ... Args>
    constexpr auto register_service(Args... args) -> bool
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
     * \brief Remove a service
     * 
     * TODO
     */
    template<typename ServiceInterfaceT>
    constexpr auto unregister_service() -> bool
    {
        static_assert(std::is_abstract_v<ServiceInterfaceT>,
            "\nServiceInterfaceT is not an abstract class");

        if(!_services.contains(get_id<ServiceInterfaceT>()))
            return false;
        
        _services.erase(get_id<ServiceInterfaceT>());
        return true;
    }

    /*!
     * \brief Check if a service is currently registered
     * 
     * TODO
     */
    template<typename ServiceInterfaceT>
    constexpr auto is_service_registered() -> bool
    {
        static_assert(std::is_abstract_v<ServiceInterfaceT>,
            "\nServiceInterfaceT is not an abstract class");

        return _services.contains(get_id<ServiceInterfaceT>());
    }

    /*!
     * \brief Get a registered service instance
     * 
     * TODO
     */
    template<typename ServiceInterfaceT>
    constexpr auto get() const -> std::shared_ptr<ServiceInterfaceT>
    {
        static_assert(std::is_abstract_v<ServiceInterfaceT>);

        if (auto service = _services.find(get_id<ServiceInterfaceT>()); service != _services.end())
            return std::static_pointer_cast<ServiceInterfaceT>(service->second);

        return nullptr;
    }

private:
    explicit service_locator() = default;

    template<typename T>
    auto get_id() const noexcept -> size_t
    {
        return std::move(typeid(T).hash_code());
    }

    std::unordered_map<size_t, std::shared_ptr<void>> _services;
};

}