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
 * @example example_event_dispatcher.cpp
 * @brief Simple example of tc::sdk::event_dispatcher
 */

#include <teiacare/sdk/event_dispatcher.hpp>

#include <functional>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

/**
 * @cond SKIP_DOXYGEN
 * This section won't be documented.
 */
void free_function(float f, int i)
{
    spdlog::info("free_function - {} {}", f, i);
}

template <typename T>
void free_function_t(T t)
{
    spdlog::info("free_function_t - {}", t);
}

class Foo
{
public:
    void method(float f, int i)
    {
        spdlog::info("class_method - {}, {}", f, i);
    }

    template <typename T>
    void method_t(T t)
    {
        spdlog::info("class_method_t - {}", t);
    }
};
/** @endcond */

int main()
{
    tc::sdk::event_dispatcher e;
    e.start(2);

    /*
    Does not compile:
    e.add_handler<std::string>("event!", [](float arg){std::cout << arg << std::endl; });

    static_assert triggered:
        event_handler arguments mismatch in add_handler()!
        please match your handler arguments (input parameters) with your declaration (template specification)
    */

    /*
    Does not compile:
    e.add_handler<std::string>("event!", [](std::string arg){std::cout << arg << std::endl; return 42; });

    static_assert triggered:
        event_handler must return void!
    */

    // Verify if an event has at least one handler attached when it is emitted
    const bool no_handler = e.emit("NullEvent", "NullEvent_Payload"); // false: no handler has been attached to the event "NullEvent"
    spdlog::info("no_handler: {}", no_handler);

    // Add multiple handlers for the same event
    const unsigned long id1 = e.add_handler<std::string>("Event_A", [](const std::string& arg) { spdlog::info("{}1", arg); }); // id1==1
    spdlog::info("id1: {}", id1);

    const unsigned long id2 = e.add_handler<std::string>("Event_A", [](const std::string& arg) { spdlog::info("{}2", arg); }); // id2==2
    spdlog::info("id2: {}", id2);

    const unsigned long id3 = e.add_handler<std::string>("Event_A", [](const std::string& arg) { spdlog::info("{}3", arg); }); // id3==3
    spdlog::info("id3: {}", id3);

    const auto arg = std::string("A");
    e.emit("Event_A", arg);

    // Remove single handler using event_id
    const bool r1 = e.remove_handler(id1); // true
    spdlog::info("r1: {}", r1);

    const bool r2 = e.remove_handler(id3); // true
    spdlog::info("r2: {}", r2);

    const bool r3 = e.remove_handler(0); // false (id does not exists)
    spdlog::info("r3: {}", r3);

    const bool r4 = e.remove_handler(id3); // false (already removed)
    spdlog::info("r4: {}", r4);

    e.emit("Event_A", std::string("A")); // Now only event with ID id2 should be triggered (it prints "A2")
    std::this_thread::sleep_for(2s);

    // Empty arguments
    e.add_handler("Event_B", [] { spdlog::info("Without args"); });
    e.emit("Event_B");

    // Lambda capture
    const char* capture_arg = "capture_me!";
    e.add_handler("Event_C", [&capture_arg] { spdlog::info("With capture arg: {}", capture_arg); });
    e.emit("Event_C");

    // Multiple arguments
    e.add_handler<int, std::string, float>("Event_D", [](int i, std::string s, float f) { spdlog::info("Multiple arguments: {}, {} ,{}", i, s, f); });
    const bool r5 = e.emit("Event_D"); // false: no arguments
    spdlog::info("r5: {}", r5);

    const bool r6 = e.emit("Event_D", std::string("some_string"), 999, "wrong_argument_type"); // false: wrong arguments type and order
    spdlog::info("r6: {}", r6);

    const bool r7 = e.emit("Event_D", 999, std::string("some_string"), 1.2345f); // true: correct arguments type and order
    spdlog::info("r7: {}", r7);

    // add_handler: free function
    std::function<void(float, int)> free_function_bind = std::bind(free_function, std::placeholders::_1, std::placeholders::_2);
    e.add_handler<float, int>("Event_E", std::move(free_function_bind));
    e.emit("Event_E", 9.87654321f, 55555);

    // add_handler: free template function
    std::function<void(std::string)> free_function_t_bind = std::bind(free_function_t<std::string>, std::placeholders::_1);
    e.add_handler<std::string>("Event_F", std::move(free_function_t_bind));
    e.emit("Event_F", std::string("payload"));

    // add_handler: class method
    Foo foo_1;
    std::function<void(float, int)> class_method_bind = std::bind(&Foo::method, &foo_1, std::placeholders::_1, std::placeholders::_2);
    e.add_handler<float, int>("Event_G", std::move(class_method_bind));
    e.emit("Event_G", 9.87654321f, 55555);

    // // add_handler: class template method
    Foo foo_2;
    std::function<void(std::string)> class_method_t_bind = std::bind(&Foo::method_t<std::string>, &foo_1, std::placeholders::_1);
    e.add_handler<std::string>("Event_H", std::move(class_method_t_bind));
    e.emit("Event_H", std::string("payload"));

    std::this_thread::sleep_for(2s);
    spdlog::info("FINISH!");

    return 0;
}
