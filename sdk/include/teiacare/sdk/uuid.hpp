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

#include <array>
#include <ostream>
#include <stdint.h>
#include <string>

namespace tc::sdk
{
class uuid_generator;

/*!
 * \class uuid
 * \brief UUID V4.
 *
 * UUID object can be created empty using the default tc::sdk::uuid constructor or
 * must be created using tc::sdk::uuid_generator::create method.
 */
class uuid
{
    friend class tc::sdk::uuid_generator;

public:
    /*!
     * \brief Constructor. Create an empty instance of a tc::sdk::uuid object.
     */
    explicit constexpr uuid() noexcept
        : _bytes{std::array<uint8_t, 16>()}
    {
    }

    /*!
     * \brief Copy constructor. Copy a tc::sdk::uuid instance to another one.
     */
    uuid(const uuid& other) = default;

    /*!
     * \brief Move constructor. Move a tc::sdk::uuid instance to another one.
     */
    uuid(uuid&& other) = default;

    /*!
     * \brief Move assignment operator. Move a tc::sdk::uuid instance to another one.
     */
    uuid& operator=(uuid&& other) = default;

    /*!
     * \brief Copy assignment operator. Copy a tc::sdk::uuid instance to another one.
     */
    uuid& operator=(const uuid& other) = default;

    /*!
     * \brief Get the uuid string representation
     */
    [[nodiscard]] std::string to_string() const;

    /*!
     * \brief Equality operator.
     * \param other the uuid to compare against.
     * \return true if the two uuid have the same bytes.
     */
    constexpr inline bool operator==(const uuid& other) const noexcept
    {
        return _bytes == other._bytes;
    }

    /*!
     * \brief Inequality operator.
     * \param other the uuid to compare against.
     * \return true if the two lines have different bytes.
     */
    constexpr inline bool operator!=(const uuid& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Comparison operator less.
     * \param other the uuid to compare against.
     * \return true if this instance has a byte payload less than the other instance.
     */
    constexpr inline bool operator<(const uuid& other) const noexcept
    {
        return _bytes < other._bytes;
    }

    /*!
     * \brief Comparison operator greater.
     * \param other the uuid to compare against.
     * \return true if this instance has a byte payload greater than the other instance.
     */
    constexpr inline bool operator>(const uuid& other) const noexcept
    {
        return _bytes > other._bytes;
    }

    /*!
     * \brief Comparison operator less or equal.
     * \param other the uuid to compare against.
     * \return true if this instance has a byte payload less or equal than the other instance.
     */
    constexpr inline bool operator<=(const uuid& other) const noexcept
    {
        return !operator>(other);
    }

    /*!
     * \brief Comparison operator greater or equal.
     * \param other the uuid to compare against.
     * \return true if this instance has a byte payload greater or equal than the other instance.
     */
    constexpr inline bool operator>=(const uuid& other) const noexcept
    {
        return !operator<(other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const uuid& u) noexcept
    {
        return stream << u.to_string();
    }

private:
    explicit uuid(const std::array<uint8_t, 16>& bytes) noexcept;
    alignas(uint32_t) std::array<uint8_t, 16> _bytes;
};

}
