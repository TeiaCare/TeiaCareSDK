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

#include <teiacare/sdk/uuid.hpp>

namespace tc::sdk
{
uuid::uuid(const std::array<uint8_t, 16>& bytes) noexcept
    : _bytes{bytes}
{
}
 
std::string uuid::to_string() const
{
    constexpr char uuid_characters[17] = "0123456789abcdef";
    constexpr char null[37] = "00000000-0000-0000-0000-000000000000";
    std::string uuid_str{null};

    for (size_t i = 0, index = 0; i < 36; ++i)
    {
        if (i == 8 || i == 13 || i == 18 || i == 23)
        {
            continue;
        }

        uuid_str[i] = uuid_characters[_bytes[index] >> 4 & 0x0f];
        uuid_str[++i] = uuid_characters[_bytes[index] & 0x0f];
        index++;
    }

    return uuid_str;
}

}
