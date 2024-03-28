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

#include <teiacare/sdk/uuid.hpp>

#include <random>

namespace tc::sdk
{
/*!
 * \class uuid_generator
 * \brief UUID Generator used to create and validate tc::sdk::uuid objects
 *
 * This class is a singleton that must be used to create instances of tc::sdk::uuid objects.
 */
class uuid_generator
{
public:
    /*!
     * \brief Static method to retrieve the single instance to the underlying generator. Note that the constructor is private, thus it is not possible to create multiple uuid_generator instances.
     * \return a reference to the uuid_generator unique instance
     */
    static uuid_generator& instance();

    /*!
     * \brief Create a random uuid V4.
     */
    [[nodiscard]] tc::sdk::uuid create();

    /*!
     * \brief Create a tc::sdk::uuid object from a std::string or a const char*.
     * \param str the input string.
     * \return the uuid object parsed from the input string.
     * \throw std::runtime_error if the input string is not convertible to a valid uuid.
     */
    [[nodiscard]] tc::sdk::uuid from_string(std::string_view str) const;

    /*!
     * \brief Check if a string is a valid uuid.
     * \param str the input string.
     * \return true if the string can be translated into a uuid object.
     */
    [[nodiscard]] bool is_valid(std::string_view str) const noexcept;

private:
    explicit uuid_generator() noexcept;
    [[nodiscard]] inline bool is_hex(const char c) const noexcept;
    [[nodiscard]] inline uint8_t hex_to_uint8(const char c) const noexcept;

    std::mt19937 _generator;
    std::uniform_int_distribution<uint32_t> _distribution;
};

}
