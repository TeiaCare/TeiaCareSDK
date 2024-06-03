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

#include <teiacare/sdk/datetime/timedelta.hpp>

namespace tc::sdk
{
std::ostream& operator<<(std::ostream& stream, const timedelta& td)
{
    return stream << td.to_string();
}

template <class DurationT>
std::string timedelta::to_string() const noexcept(false)
{
    constexpr const char zero = '0';
    const auto hms = std::chrono::hh_mm_ss(std::chrono::duration_cast<DurationT>(_duration));
    std::stringstream os{};

    if (hms.is_negative())
        os << '-';

    os.width(2);
    os.fill(zero);
    os << hms.hours().count() << ':';

    os.width(2);
    os.fill(zero);
    os << hms.minutes().count() << ':';

    os.width(2);
    os.fill(zero);
    os << hms.seconds().count();

    if (std::is_same_v<DurationT, std::chrono::seconds>)
        return os.str();

    os << '.';

    if constexpr (std::is_same_v<DurationT, std::chrono::milliseconds>)
        os.width(3);

    if constexpr (std::is_same_v<DurationT, std::chrono::microseconds>)
        os.width(6);

    if constexpr (std::is_same_v<DurationT, std::chrono::nanoseconds>)
        os.width(9);

    DurationT sub = hms.subseconds();
    os.fill(zero);
    os << sub.count();
    return os.str();
}

template std::string timedelta::to_string<std::chrono::seconds>() const noexcept(false);
template std::string timedelta::to_string<std::chrono::milliseconds>() const noexcept(false);
template std::string timedelta::to_string<std::chrono::microseconds>() const noexcept(false);
template std::string timedelta::to_string<std::chrono::nanoseconds>() const noexcept(false);

}
