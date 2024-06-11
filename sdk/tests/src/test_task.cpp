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

#include "test_task.hpp"

#include <thread>

namespace tc::sdk::tests
{
bool Functor::functor_invoked = false;

// NOLINTNEXTLINE
TEST(test_task, explicit_ctor)
{
    bool task_invoked = false;

    auto t = tc::sdk::task([&] { task_invoked = true; });
    t();

    EXPECT_TRUE(task_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, move_ctor)
{
    bool task_invoked = false;

    auto t = tc::sdk::task([&] { task_invoked = true; });
    auto t_move(std::move(t));
    t_move();

    EXPECT_TRUE(task_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, invoke)
{
    bool task_invoked = false;

    auto t = tc::sdk::task([&] { task_invoked = true; });
    t.invoke();

    EXPECT_TRUE(task_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, operator_call)
{
    bool task_invoked = false;

    auto t = tc::sdk::task([&] { task_invoked = true; });
    t();

    EXPECT_TRUE(task_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, std_function)
{
    bool task_invoked = false;

    std::function<void()> func = [&] { task_invoked = true; };
    auto t = tc::sdk::task(std::move(func));
    t();

    EXPECT_TRUE(task_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, lambda_move)
{
    bool task_invoked = false;

    auto lambda = [&] { task_invoked = true; };
    auto t = tc::sdk::task(std::move(lambda));
    t();

    EXPECT_TRUE(task_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, bind_global_function)
{
    global_function_invoked = false;

    auto t = tc::sdk::task(std::bind(tc::sdk::tests::global_function));
    t();

    EXPECT_TRUE(global_function_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, bind_global_function_template)
{
    global_function_template_invoked = false;

    auto t = tc::sdk::task(std::bind(tc::sdk::tests::global_function_template<bool>));
    t();

    EXPECT_TRUE(global_function_template_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, bind_member_function)
{
    tc::sdk::tests::Foo foo;
    foo.member_function_invoked = false;

    auto t = tc::sdk::task(std::bind(&Foo::member_function, &foo));
    t();

    EXPECT_TRUE(foo.member_function_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, bind_member_function_template)
{
    tc::sdk::tests::Foo foo;
    foo.member_function_template_invoked = false;

    auto t = tc::sdk::task(std::bind(&Foo::member_function_template<bool>, &foo));
    t();

    EXPECT_TRUE(foo.member_function_template_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, bind_member_function_template_class)
{
    tc::sdk::tests::FooT<bool> foo;
    foo.member_function_template_class_invoked = false;

    auto t = tc::sdk::task(std::bind(&FooT<bool>::member_function_template_class, &foo));
    t();

    EXPECT_TRUE(foo.member_function_template_class_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, bind_functor)
{
    tc::sdk::tests::Functor functor;
    functor.functor_invoked = false;

    auto t = tc::sdk::task(std::bind(&Functor::operator(), &functor));
    t();

    EXPECT_TRUE(functor.functor_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, functor)
{
    tc::sdk::tests::Functor functor;
    Functor::functor_invoked = false;

    auto t = tc::sdk::task(std::move(functor));
    t();

    EXPECT_TRUE(Functor::functor_invoked);
    EXPECT_TRUE(functor.functor_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, free_function)
{
    global_function_invoked = false;

    auto t = tc::sdk::task(global_function);
    t();

    EXPECT_TRUE(global_function_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, free_function_template)
{
    global_function_template_invoked = false;

    auto t = tc::sdk::task(global_function_template<bool>);
    t();

    EXPECT_TRUE(global_function_template_invoked);
}

// NOLINTNEXTLINE
TEST(test_task, multiple_invoke)
{
    constexpr int total_invoke_count = 1'000;
    int task_invoked_count = 0;

    auto t = tc::sdk::task([&] { ++task_invoked_count; });

    for (auto i = 0; i < total_invoke_count; ++i)
    {
        t();
    }

    EXPECT_EQ(task_invoked_count, total_invoke_count);
}

// NOLINTNEXTLINE
TEST(test_task, parallel_invoke)
{
    constexpr int total_invoke_count = 1'000;
    std::atomic_int task_invoked_count = 0;

    auto t1 = tc::sdk::task([&] {
        while (task_invoked_count < total_invoke_count)
        {
            ++task_invoked_count;
        }
    });

    auto t2 = tc::sdk::task([&] {
        while (task_invoked_count < total_invoke_count)
        {
            ++task_invoked_count;
        }
    });

    std::jthread thread1([&t1] { t1(); });
    std::jthread thread2([&t2] { t2(); });

    EXPECT_EQ(task_invoked_count, total_invoke_count);
}

}
