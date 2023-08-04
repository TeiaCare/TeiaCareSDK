#include "test_event_dispatcher.hpp"
#include <chrono>
#include <future>
#include <semaphore>
#include <latch>
#include <barrier>

namespace tc::sdk::tests
{
// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, start_twice)
{
    EXPECT_TRUE(e->start());
    EXPECT_FALSE(e->start());
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, stop_twice)
{
    EXPECT_FALSE(e->stop());
    EXPECT_TRUE(e->start());
    
    EXPECT_TRUE(e->stop());
    EXPECT_FALSE(e->stop());
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_void_handler)
{
    const auto event_name = "EVENT_NAME";
    
    EXPECT_TRUE(e->add_handler(event_name, [](){}));
    EXPECT_TRUE(e->add_handler<>(event_name, [](){}));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_missing_event_handler)
{
    EXPECT_FALSE(e->remove_handler(0));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_missing_event)
{
    EXPECT_FALSE(e->remove_event(""));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_handler_after_event_removal)
{
    const auto event_name = "EVENT_NAME";

    const auto handler_id = e->add_handler(event_name, [](){});
    EXPECT_TRUE(e->remove_event(event_name));
    EXPECT_FALSE(e->remove_handler(handler_id));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_event_after_handler_removal)
{
    const auto event_name = "EVENT_NAME";

    const auto handler_id = e->add_handler(event_name, [](){});
    EXPECT_TRUE(e->remove_handler(handler_id));
    EXPECT_FALSE(e->remove_event(event_name));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_event_handler_twice_same_name)
{
    const auto event_name = "EVENT_NAME";

    auto id1 = e->add_handler(event_name, [](){});
    EXPECT_EQ(id1, 1);
    
    auto id2 = e->add_handler(event_name, [](){});
    EXPECT_EQ(id2, 2);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_event_handler_twice_same_name_different_type)
{
    const auto event_name = "EVENT_NAME";

    auto id1 = e->add_handler(event_name, [](){});
    EXPECT_EQ(id1, 1);
    
    auto id2 = e->add_handler<std::string>(event_name, [](const std::string& s){});
    EXPECT_EQ(id2, 2);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_event_twice_same_name)
{
    // GTEST_SKIP_("Thread Sanitizer Warning");

    const auto event_name = "EVENT_NAME";
    EXPECT_FALSE(e->remove_event(event_name)); // still unregistered

    e->add_handler(event_name, [](){});
    EXPECT_TRUE(e->remove_event(event_name));

    EXPECT_FALSE(e->remove_event(event_name)); // already removed
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_handler_twice_same_id)
{
    constexpr unsigned long id = 1;
    EXPECT_FALSE(e->remove_handler(id)); // still unregistered

    EXPECT_EQ(id, e->add_handler("", [](){}));
    EXPECT_TRUE(e->remove_handler(id));

    EXPECT_FALSE(e->remove_handler(id)); // already removed
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_event_handler_twice_different_name)
{
    EXPECT_TRUE(e->add_handler("1", [](){}));
    EXPECT_TRUE(e->add_handler("2", [](){}));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_event_handler_twice_different_name_same_type)
{
    EXPECT_TRUE(e->add_handler<std::string>("1", [](const std::string& s){}));
    EXPECT_TRUE(e->add_handler<std::string>("2", [](const std::string& s){}));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_remove_event_handlers_id_progressive)
{
    auto id1 = e->add_handler("1", [](){});
    EXPECT_EQ(id1, 1);
    EXPECT_TRUE(e->remove_handler(id1));

    auto id2 = e->add_handler("2", [](){});
    EXPECT_EQ(id2, 2);
    EXPECT_TRUE(e->remove_handler(id2));

    auto id3 = e->add_handler("3", [](){});
    EXPECT_EQ(id3, 3);
    EXPECT_TRUE(e->remove_handler(id3));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_event_handler_after_start)
{
    e->start();
    EXPECT_TRUE(e->add_handler("", [](){}));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_event_handler_after_start)
{
    e->start();
    auto id = e->add_handler("", [](){});
    EXPECT_EQ(id, 1);
    EXPECT_TRUE(e->remove_handler(id));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_event_handler_after_stop)
{
    e->start();
    e->stop();
    EXPECT_TRUE(e->add_handler("", [](){}));
}


// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, remove_event_handler_after_stop)
{
    e->start();
    e->stop();
    auto id = e->add_handler("", [](){});
    EXPECT_EQ(id, 1);
    EXPECT_TRUE(e->remove_handler(id));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_event_n_times_void_payload)
{
    const auto event_name = "EVENT_NAME";
    e->start();
    
    constexpr int total_count = 10;
    std::atomic<int> call_count = 0;

    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    
    e->add_handler<>(event_name, [&call_count, &promise]
    {
        call_count++;
        if(call_count >= total_count)
            promise.set_value();
    });

    for (int expected_count = 0; expected_count < total_count; ++expected_count)
    {
        EXPECT_TRUE(e->emit(event_name));
    }

    future.wait();
    EXPECT_EQ(call_count, total_count);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_event_n_times_same_payload)
{
    const auto event_name = "EVENT_NAME";
    e->start();
    
    constexpr int value = 10;
    constexpr int total_count = 10;
    std::atomic<int> call_counter = 0;

    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    e->add_handler<int>(event_name, [&call_counter, &promise](int value)
    {
        call_counter += value;
        if(call_counter >= total_count * value)
            promise.set_value(call_counter);
    });

    for (int expected_count = 0; expected_count < total_count; ++expected_count)
    {
        EXPECT_TRUE(e->emit(event_name, value));
    }

    future.wait();
    EXPECT_EQ(call_counter, total_count * value);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_same_event_name_same_handler_types)
{
    const auto event_name = "EVENT_NAME";
    EXPECT_TRUE(e->add_handler<int>(event_name, [](int i){}));
    EXPECT_TRUE(e->add_handler<int>(event_name, [](int i){}));

    EXPECT_TRUE(e->add_handler<std::string>(event_name, [](const std::string& s){}));
    EXPECT_TRUE(e->add_handler<std::string>(event_name, [](const std::string& s){}));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, add_same_event_name_different_handler_types)
{
    const auto event_name = "EVENT_NAME";
    EXPECT_TRUE(e->add_handler<int>(event_name, [](int i){}));
    EXPECT_TRUE(e->add_handler<unsigned int>(event_name, [](unsigned int i){}));
    EXPECT_TRUE(e->add_handler<unsigned long>(event_name, [](unsigned long i){}));
    EXPECT_TRUE(e->add_handler<long long>(event_name, [](long long i){}));

    EXPECT_TRUE(e->add_handler<std::string>(event_name, [](std::string s){}));
    EXPECT_TRUE(e->add_handler<std::string&>(event_name, [](std::string& s){}));
    EXPECT_TRUE(e->add_handler<std::string&&>(event_name, [](std::string&& s){}));
    EXPECT_TRUE(e->add_handler<const std::string&>(event_name, [](const std::string& s){}));
    EXPECT_TRUE(e->add_handler<std::string>(event_name, [](const std::string s){}));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_same_event_name_same_handler_types)
{
    const auto event_name = "EVENT_NAME";
    e->start();

    std::promise<int> promise_1;
    std::future<int> future_1 = promise_1.get_future();
    EXPECT_TRUE(e->add_handler<int>(event_name, [&promise_1](int i){ promise_1.set_value(i); }));

    std::promise<int> promise_2;
    std::future<int> future_2 = promise_2.get_future();
    EXPECT_TRUE(e->add_handler<int>(event_name, [&promise_2](int i){ promise_2.set_value(i); }));

    const int expected_value = 123456789;
    EXPECT_TRUE(e->emit(event_name, expected_value));

    const int value_1 = future_1.get();
    const int value_2 = future_2.get();

    EXPECT_EQ(expected_value, value_2);
    EXPECT_EQ(expected_value, value_1);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_same_event_name_different_handler_types)
{
    // GTEST_SKIP_("Thread Sanitizer Warning");

    const auto event_name = "EVENT_NAME";
    e->start();

    // int
    auto int_payload = 0;
    std::promise<int> int_promise;
    std::future<int> int_future = int_promise.get_future();
    EXPECT_TRUE(e->add_handler<int>(event_name, [&int_promise, &int_payload](int i)
    {
        int_payload = i;
        int_promise.set_value(i);
    }));

    // string
    std::string str_payload = "";
    std::promise<std::string> str_promise;
    std::future<std::string> str_future = str_promise.get_future();
    EXPECT_TRUE(e->add_handler<std::string>(event_name, [&str_promise, &str_payload](std::string s)
    {
        str_payload = s;
        str_promise.set_value(s);
    }));

    // int
    const int expected_value_int = 123456789;
    EXPECT_TRUE(e->emit(event_name, expected_value_int));
    
    // string
    const std::string expected_value_str = "some_random_string";
    EXPECT_TRUE(e->emit(event_name, expected_value_str));

    // int
    auto int_future_value = int_future.get();
    EXPECT_EQ(expected_value_int, int_future_value);
    EXPECT_EQ(expected_value_int, int_payload);
    
    // string
    auto str_future_value = str_future.get();
    EXPECT_EQ(expected_value_str, str_future_value);
    EXPECT_EQ(expected_value_str, str_payload);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_event_consecutive_start_stop)
{
    const auto event_name = "EVENT_NAME";

    constexpr int total_count = 10;
    for (int expected_count = 0; expected_count < total_count; ++expected_count)
    {
        e->start();
        std::promise<int> promise;
        std::future<int> future = promise.get_future();
        EXPECT_TRUE(e->add_handler<int>(event_name, [&promise](int i){ promise.set_value(i); }));

        const int expected_value = 123456789;
        EXPECT_TRUE(e->emit(event_name, expected_value));

        const int value = future.get();
        EXPECT_EQ(expected_value, value);
        e->stop();
    }
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, sync_multiple_events_on_multiple_threads)
{
    const auto event_name = "EVENT_NAME";
    constexpr unsigned int dispatcher_threads = 4;
    e->start(dispatcher_threads);

    std::atomic<int> call_count = 0;
    std::barrier sync(dispatcher_threads + 1, []() noexcept {});

    for (int thread_idx = 0; thread_idx < dispatcher_threads; ++thread_idx)
    {
        e->add_handler<float, std::string, int>(event_name, [&sync, &call_count](float f, const std::string& s, int i)
        {
            ++call_count;
            sync.arrive_and_wait();
        });
    }

    constexpr int total_count = 10;
    for (int expected_count = 1; expected_count < total_count; ++expected_count)
    {
        EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
        sync.arrive_and_wait();

        EXPECT_EQ(call_count, dispatcher_threads * expected_count);
    }
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_missing_event)
{
    e->start();
    const auto event_name = "EVENT_NAME";
    EXPECT_FALSE(e->emit(""));
    EXPECT_FALSE(e->emit("", 1, 2, 3));
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_event_with_different_overloads)
{
    e->start();
    const auto event_name = "EVENT_NAME";

    std::atomic<int> call_count = 0;
    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    e->add_handler<float, std::string, int>(event_name, [&promise, &call_count](float f, const std::string& s, int i)
    {
        ++call_count;
        promise.set_value();
    });

    EXPECT_FALSE((e->emit(event_name, 1.f, "payload", 1)));
    EXPECT_FALSE((e->emit(event_name, 1.f, std::string("payload"), 3.0)));
    EXPECT_FALSE((e->emit(event_name, 1, std::string("payload"), 3)));
    EXPECT_FALSE((e->emit(event_name, 1.f, 2, 3)));
    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1))); // ok

    future.wait();
    EXPECT_EQ(call_count, 1);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_after_remove_handler)
{
    e->start();
    const auto event_name = "EVENT_NAME";

    std::atomic<int> call_count = 0;
    std::counting_semaphore<2> sync(0);
    const auto handler_id = e->add_handler<float, std::string, int>(event_name, [&sync, &call_count](float f, const std::string& s, int i)
    {
        ++call_count;
        sync.release();
    });

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.acquire();
    EXPECT_EQ(call_count, 1);

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.acquire();
    EXPECT_EQ(call_count, 2);

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.acquire();
    EXPECT_EQ(call_count, 3);
    
    EXPECT_TRUE(e->remove_handler(handler_id));
    EXPECT_FALSE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    EXPECT_EQ(call_count, 3);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_after_remove_event)
{
    e->start();
    const auto event_name = "EVENT_NAME";

    std::atomic<int> call_count = 0;
    std::counting_semaphore<2> sync(0);
    e->add_handler<float, std::string, int>(event_name, [&sync, &call_count](float f, const std::string& s, int i)
    {
        ++call_count;
        sync.release();
    });

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.acquire();
    EXPECT_EQ(call_count, 1);

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.acquire();
    EXPECT_EQ(call_count, 2);

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.acquire();
    EXPECT_EQ(call_count, 3);
    
    EXPECT_TRUE(e->remove_event(event_name));
    EXPECT_FALSE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    EXPECT_EQ(call_count, 3);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_after_remove_specific_handler)
{
    e->start(2);
    const auto event_name = "EVENT_NAME";

    std::atomic<int> call_count = 0;
    std::barrier sync(3, []() noexcept {});
    
    const auto event_id_1 = e->add_handler(event_name, [&sync, &call_count]()
    {
        ++call_count;
        sync.arrive_and_wait();
    });

    const auto event_id_2 = e->add_handler<float, std::string, int>(event_name, [&sync, &call_count](float f, const std::string& s, int i)
    {
        ++call_count;
        sync.arrive_and_wait();
    });

    EXPECT_TRUE((e->emit(event_name)));
    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 2);

    EXPECT_TRUE((e->emit(event_name)));
    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 4);
    
    EXPECT_TRUE(e->remove_handler(event_id_1));
    
    {
        EXPECT_FALSE((e->emit(event_name)));
        auto t = sync.arrive();
    }

    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 5);

    EXPECT_TRUE(e->remove_handler(event_id_2));

    {
        EXPECT_FALSE((e->emit(event_name)));
        auto t = sync.arrive();
    }

    {
        EXPECT_FALSE((e->emit(event_name, 1.f, std::string("payload"), 1)));
        auto t = sync.arrive();
    }

    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 5);
}

// NOLINTNEXTLINE
TEST_F(test_event_dispatcher, push_after_remove_event_with_multiple_handlers)
{
    e->start(2);
    const auto event_name = "EVENT_NAME";

    std::atomic<int> call_count = 0;
    std::barrier sync(3, []() noexcept {});

    const auto event_id_1 = e->add_handler(event_name, [&sync, &call_count]()
    {
        ++call_count;
        sync.arrive_and_wait();
    });

    const auto event_id_2 = e->add_handler<float, std::string, int>(event_name, [&sync, &call_count](float f, const std::string& s, int i)
    {
        ++call_count;
        sync.arrive_and_wait();
    });

    EXPECT_TRUE((e->emit(event_name)));
    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 2);

    EXPECT_TRUE((e->emit(event_name)));
    EXPECT_TRUE((e->emit(event_name, 1.f, std::string("payload"), 1)));
    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 4);
    
    EXPECT_TRUE(e->remove_event(event_name));

    {
        EXPECT_FALSE((e->emit(event_name)));
        auto t = sync.arrive();
    }
    
    {
        EXPECT_FALSE((e->emit(event_name, 1.f, std::string("payload"), 1)));
        auto t = sync.arrive();
    }

    sync.arrive_and_wait();
    EXPECT_EQ(call_count, 4);
}

}