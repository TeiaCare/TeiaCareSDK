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

#include <teiacare/sdk/rate_limiter.hpp>

#include <thread>

namespace tc::sdk
{
rate_limiter::rate_limiter(size_t rate)
    : time_between_frames{std::chrono::seconds(1) / static_cast<duration_rep>(rate)}
    , tp{tc::sdk::clock::now()}
{
}

void rate_limiter::sync()
{
    tp += time_between_frames;
    std::this_thread::sleep_until(tp);
}

}
