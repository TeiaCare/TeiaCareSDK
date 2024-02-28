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

#include <future>

#define RANGE(lo, hi) testing::AllOf(testing::Ge(lo), testing::Le(hi))

#define TEST_TIMEOUT_BEGIN                                             \
    std::promise<void> promise_finished;                               \
    std::future<void> future_finished = promise_finished.get_future(); \
    std::thread([](std::promise<void>& finished) {
#define TEST_TIMEOUT_END(TIMEOUT_MS)         \
    finished.set_value();                    \
    }, std::ref(promise_finished)).detach(); \
    EXPECT_FALSE(future_finished.wait_for(std::chrono::milliseconds(TIMEOUT_MS)) == std::future_status::timeout);
