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

#include <teiacare/sdk/datetime/datetime.hpp>

#include "date.h"

namespace tc::sdk
{
std::ostream& operator<<(std::ostream& stream, const datetime& dt)
{
    return stream << dt.to_string();
}

template <class DurationT>
std::string datetime::to_string(const std::string& format) const noexcept(false)
{
    return ::date::format(format, std::chrono::time_point_cast<DurationT>(_tp));
}

template std::string datetime::to_string<std::chrono::seconds>(const std::string& format) const noexcept(false);
template std::string datetime::to_string<std::chrono::milliseconds>(const std::string& format) const noexcept(false);
template std::string datetime::to_string<std::chrono::microseconds>(const std::string& format) const noexcept(false);
template std::string datetime::to_string<std::chrono::nanoseconds>(const std::string& format) const noexcept(false);

template <class DurationT>
tc::sdk::datetime tc::sdk::datetime::from_string(const std::string& str, const std::string& format) noexcept(false)
{
    std::chrono::sys_time<DurationT> parsed_time;
    std::stringstream ss{str};
    ss >> ::date::parse(format, parsed_time);
    if (ss.fail())
        throw std::runtime_error("Failed to parse " + str);

    return tc::sdk::datetime(parsed_time);
}

template tc::sdk::datetime datetime::from_string<std::chrono::seconds>(const std::string& str, const std::string& format) noexcept(false);
template tc::sdk::datetime datetime::from_string<std::chrono::milliseconds>(const std::string& str, const std::string& format) noexcept(false);
template tc::sdk::datetime datetime::from_string<std::chrono::microseconds>(const std::string& str, const std::string& format) noexcept(false);
template tc::sdk::datetime datetime::from_string<std::chrono::nanoseconds>(const std::string& str, const std::string& format) noexcept(false);

template <class DurationT>
tc::sdk::datetime datetime::utc_now() noexcept
{
    const auto now = std::chrono::system_clock::now();
    return tc::sdk::datetime{std::chrono::time_point_cast<DurationT>(now)};
}

template tc::sdk::datetime datetime::utc_now<std::chrono::seconds>() noexcept;
template tc::sdk::datetime datetime::utc_now<std::chrono::milliseconds>() noexcept;
template tc::sdk::datetime datetime::utc_now<std::chrono::microseconds>() noexcept;
template tc::sdk::datetime datetime::utc_now<std::chrono::nanoseconds>() noexcept;

}