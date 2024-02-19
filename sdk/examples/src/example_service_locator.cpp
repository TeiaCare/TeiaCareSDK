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

#include <sdk/service_locator.hpp>
#include <string>
#include <iostream>

struct IService
{
    virtual ~IService() = default;
    virtual void call() const = 0;
};

struct ServiceA : IService
{
    void call() const override { std::cout << "A" << std::endl; }
};

struct ServiceB : IService
{
    void call() const override { std::cout << "B" << std::endl; }
};

class ServiceX : public IService
{
public:
    explicit ServiceX(std::string x, double y) : _x {x}, _y{y}{}
    void call() const override { std::cout << _x << std::endl; }
    void non_virtual_call() const { std::cout << _y << std::endl; }

private:
    std::string _x = "";
    double _y = 0.0;
};

auto main(int argc, char** argv) -> int
{
    auto& s = tc::sdk::service_locator::instance();
    std::shared_ptr<IService> service = nullptr;
    bool is_registered = false;

    is_registered = s.register_service<IService, ServiceA>(); // true
    service = s.get<IService>(); // ServiceA implementation
    service->call();

    is_registered = s.register_service<IService, ServiceB>(); // false, there is an already registered implementation for IService interface
    service = s.get<IService>(); // ServiceA implementation, again, since it was already registered first
    service->call();

    bool is_unregistered = s.unregister_service<IService>(); // true

    is_registered = s.register_service<IService, ServiceB>(); // true
    service = s.get<IService>(); // now get return ServiceB_1 since there was not any registered implementation for IService interface
    service->call();

    is_registered = s.register_service<IService, ServiceX>("__message__", 1.2345); // true
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