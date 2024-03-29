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

#include <teiacare/sdk/uuid_generator.hpp>

#include <limits>
#include <random>
#include <stdexcept>

namespace tc::sdk
{
uuid_generator& uuid_generator::instance()
{
    static uuid_generator instance;
    return instance;
}

[[nodiscard]] tc::sdk::uuid uuid_generator::create()
{
    alignas(uint32_t) std::array<uint8_t, 16> bytes;

    for (int i = 0; i < 16; i += 4)
        *reinterpret_cast<uint32_t*>(bytes.data() + i) = _distribution(_generator);

    // variant must be 10xxxxxx
    bytes[8] &= 0xBF;
    bytes[8] |= 0x80;

    // version must be 0100xxxx
    bytes[6] &= 0x4F;
    bytes[6] |= 0x40;

    return tc::sdk::uuid{bytes};
}

[[nodiscard]] tc::sdk::uuid uuid_generator::from_string(std::string_view str) const
{
    if (str.empty())
        throw std::runtime_error("Invalid uuid format: unable to generate uuid from an empty string");

    size_t has_braces = 0;
    if (str.front() == '{')
        has_braces = 1;

    if (has_braces && str.back() != '}')
        throw std::runtime_error("Invalid uuid format: first character is '{' but there is not a matching '}' as last character");

    size_t byte_index = 0;
    bool is_first_digit = true;
    alignas(uint32_t) std::array<uint8_t, 16> bytes;

    for (size_t i = has_braces; i < str.size() - has_braces; ++i)
    {
        if (str[i] == '-')
            continue;

        if (byte_index >= 16)
        {
            throw std::runtime_error("Invalid uuid format: uuid size is greater than 16 bytes");
        }

        if (!is_hex(str[i]))
        {
            throw std::runtime_error("Invalid uuid format: invalid character found: " + std::to_string(str[i]));
        }

        if (is_first_digit)
        {
            bytes[byte_index] = hex_to_uint8(str[i]) << 4;
            is_first_digit = false;
        }
        else
        {
            bytes[byte_index] = bytes[byte_index] | hex_to_uint8(str[i]);
            byte_index++;
            is_first_digit = true;
        }
    }

    if (byte_index < 16)
    {
        throw std::runtime_error("Invalid uuid format: uuid size is less than 16 bytes");
    }

    return tc::sdk::uuid{bytes};
}

[[nodiscard]] bool uuid_generator::is_valid(std::string_view str) const noexcept
{
    if (str.empty())
        return false;

    size_t has_braces = 0;
    if (str.front() == '{')
        has_braces = 1;

    if (has_braces && str.back() != '}')
        return false;

    size_t byte_index = 0;
    bool is_first_digit = true;

    for (size_t i = has_braces; i < str.size() - has_braces; ++i)
    {
        if (str[i] == '-')
            continue;

        if (byte_index >= 16 || !is_hex(str[i]))
        {
            return false;
        }

        if (is_first_digit)
        {
            is_first_digit = false;
        }
        else
        {
            byte_index++;
            is_first_digit = true;
        }
    }

    if (byte_index < 16)
    {
        return false;
    }

    return true;
}

uuid_generator::uuid_generator() noexcept
    : _generator(std::mt19937(std::random_device()()))
    , _distribution(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())
{
}

[[nodiscard]] bool uuid_generator::is_hex(const char c) const noexcept
{
    return (c >= char('0') && c <= char('9')) ||
           (c >= char('a') && c <= char('f')) ||
           (c >= char('A') && c <= char('F'));
}

[[nodiscard]] uint8_t uuid_generator::hex_to_uint8(const char c) const noexcept
{
    if (c >= char('0') && c <= char('9'))
        return static_cast<uint8_t>(c - char('0'));

    if (c >= char('a') && c <= char('f'))
        return static_cast<uint8_t>(10 + c - char('a'));

    if (c >= char('A') && c <= char('F'))
        return static_cast<uint8_t>(10 + c - char('A'));

    return uint8_t{0};
}

}
