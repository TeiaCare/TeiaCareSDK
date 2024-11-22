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

/**
 * @example example_service_locator.cpp
 * @brief Simple example of tc::sdk::service_locator
 */

#include <teiacare/sdk/service_locator.hpp>

#include <spdlog/spdlog.h>

/**
 * @cond SKIP_DOXYGEN
 */

struct IService
{
    virtual ~IService() = default;
    virtual void call() const = 0;
};

struct ServiceA : IService
{
    void call() const override
    {
        spdlog::info("A");
    }
};

struct ServiceB : IService
{
    void call() const override
    {
        spdlog::info("B");
    }
};

class ServiceX : public IService
{
public:
    explicit ServiceX(std::string x, double y)
        : _x{x}
        , _y{y}
    {
    }
    void call() const override
    {
        spdlog::info(_x);
    }
    void non_virtual_call() const
    {
        spdlog::info(_y);
    }

private:
    std::string _x = "";
    double _y = 0.0;
};
/** @endcond */

auto main(int argc, char** argv) -> int
{
    spdlog::set_pattern("[%H:%M:%S.%e] %v");

    auto& s = tc::sdk::service_locator::instance();
    std::shared_ptr<IService> service = nullptr;

    bool is_registered = s.register_service<IService, ServiceA>();
    spdlog::info("IService, ServiceA is_registered: {}", is_registered); // true
    service = s.get<IService>();                                         // ServiceA implementation
    service->call();

    is_registered = s.register_service<IService, ServiceB>();
    spdlog::info("IService, ServiceB is_registered: {}", is_registered); // false, there is an already registered implementation for IService interface
    service = s.get<IService>();                                         // ServiceA implementation, again, since it was already registered first
    service->call();

    bool is_unregistered = s.unregister<IService>();
    spdlog::info("IService is_unregistered: {}", is_unregistered); // true

    is_registered = s.register_service<IService, ServiceB>();
    spdlog::info("IService, ServiceB is_registered: {}", is_registered); // true
    service = s.get<IService>();                                         // now get return ServiceB_1 since there was not any registered implementation for IService interface
    service->call();

    is_registered = s.register_service<IService, ServiceX>("__message__", 1.2345);
    spdlog::info("IService, ServiceX is_registered: {}", is_registered); // true
    service = s.get<IService>();
    service->call();

    /* //////////////////////////////////////////////////////////////////////////////////////////////////

    Note that the class provides the following compile time features:

    - Compile error when using non abstract classes to be used as interface:
    s.register_service<ServiceA, ServiceB>(); // ServiceA is not an abstract class

    - Compile error when using wrong argument to construct a type:
    s.register_service<IService, ServiceX>(123, 456); // Wrong arguments of ServiceX constructor

    - Compile error when registering a type that does not inherit from the given interface:
    s.register_service<IDummyInterface, ServiceA>(); // ServiceA does not inherit from IDummyInterface

    ////////////////////////////////////////////////////////////////////////////////////////////////// */

    return 0;
}
