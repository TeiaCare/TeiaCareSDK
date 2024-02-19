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

#include "test_service_locator.hpp"
#include <gmock/gmock.h>

namespace tc::sdk::tests
{
//NOLINTNEXTLINE
TEST_F(test_service_locator, singleton_instance)
{
    auto& instance1 = tc::sdk::service_locator::instance();
    auto& instance2 = tc::sdk::service_locator::instance();
    auto& instance3 = tc::sdk::service_locator::instance();
    
    EXPECT_EQ(instance1, s);
    EXPECT_EQ(instance2, s);
    EXPECT_EQ(instance3, s);
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, get_not_existing_interface)
{
    EXPECT_TRUE((s.register_service<IService, ServiceA>()));
    
    EXPECT_NE(s.get<IService>(), nullptr);
    EXPECT_TRUE(s.is_service_registered<IService>());

    EXPECT_EQ(s.get<IAnotherService>(), nullptr);
    EXPECT_FALSE(s.is_service_registered<IAnotherService>());
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, register_interface)
{
    EXPECT_EQ(s.get<IService>(), nullptr);
    
    {
        EXPECT_TRUE((s.register_service<IService, ServiceA>()));
        EXPECT_EQ(s.get<IService>()->call(), "A");
    }

    {
        EXPECT_FALSE((s.register_service<IService, ServiceB>()));
        EXPECT_EQ(s.get<IService>()->call(), "A");
    }
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, register_interface_with_arguments)
{
    const auto arg_string = "X";
    const auto arg_double = 1.23456789;

    EXPECT_TRUE((s.register_service<IService, ServiceX>(arg_string, arg_double)));
    
    const auto service = s.get<IService>();
    EXPECT_EQ(service->call(), arg_string);
    EXPECT_EQ(static_cast<ServiceX*>(service.get())->non_virtual_call(), arg_double);
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, register_same_interface_different_implementations)
{
    EXPECT_EQ(s.get<IService>(), nullptr);
    
    {
        EXPECT_TRUE((s.register_service<IService, ServiceA>()));
        EXPECT_EQ(s.get<IService>()->call(), "A");
    }

    EXPECT_TRUE((s.unregister_service<IService>()));

    {
        EXPECT_TRUE((s.register_service<IService, ServiceB>()));
        EXPECT_EQ(s.get<IService>()->call(), "B");
    }
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, unregister_multiple_interfaces)
{
    EXPECT_FALSE((s.unregister_service<IService>()));
    EXPECT_FALSE((s.unregister_service<IAnotherService>()));

    EXPECT_FALSE(s.is_service_registered<IService>());
    EXPECT_FALSE(s.is_service_registered<IAnotherService>());

    EXPECT_TRUE((s.register_service<IService, ServiceA>()));
    EXPECT_TRUE((s.register_service<IAnotherService, AnotherService>()));

    EXPECT_TRUE(s.is_service_registered<IService>());
    EXPECT_TRUE(s.is_service_registered<IAnotherService>());

    EXPECT_TRUE((s.unregister_service<IService>()));
    EXPECT_TRUE((s.unregister_service<IAnotherService>()));

    EXPECT_FALSE(s.is_service_registered<IService>());
    EXPECT_FALSE(s.is_service_registered<IAnotherService>());
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, unregister_multiple_times)
{
    EXPECT_FALSE((s.unregister_service<IService>()));

    EXPECT_TRUE((s.register_service<IService, ServiceA>()));
    EXPECT_NE(s.get<IService>(), nullptr);

    EXPECT_TRUE((s.unregister_service<IService>()));
    EXPECT_FALSE((s.unregister_service<IService>()));
}

//NOLINTNEXTLINE
TEST_F(test_service_locator, get_shared_instance)
{
    EXPECT_TRUE((s.register_service<IService, ServiceA>()));
    auto service_1 = s.get<IService>();
    auto service_2 = s.get<IService>();
    auto service_3 = s.get<IService>();

    EXPECT_EQ(service_1.get(), service_2.get());
    EXPECT_EQ(service_2.get(), service_3.get());
    EXPECT_EQ(service_3.get(), service_1.get());
}

}
