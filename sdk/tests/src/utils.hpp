#pragma once

#include <future>

#define RANGE(lo, hi) testing::AllOf(testing::Ge(lo), testing::Le(hi))

#define TEST_TIMEOUT_BEGIN                                              \
    std::promise<void> promise_finished;                                \
    std::future<void> future_finished = promise_finished.get_future();  \
    std::thread([](std::promise<void>& finished) {

#define TEST_TIMEOUT_END(TIMEOUT_MS)            \
    finished.set_value();                       \
    }, std::ref(promise_finished)).detach();    \
    EXPECT_FALSE(future_finished.wait_for(std::chrono::milliseconds(TIMEOUT_MS)) == std::future_status::timeout);


