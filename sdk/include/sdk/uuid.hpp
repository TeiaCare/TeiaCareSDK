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

#include <string>
#include <stdint.h>

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
     * \brief Constructor
     *
     * Create an empty instance of a tc::sdk::uuid object.
     */
    explicit uuid() = default;

    /*!
     * \brief Copy constructor
     *
     * Copy a tc::sdk::uuid instance to another one.
     */
    uuid(const uuid& other);

    /*!
     * \brief Assignment operator
     *
     * Assign a tc::sdk::uuid instance to another one.
     */
    uuid& operator=(const uuid& other);

    /*!
     * \brief Get the uuid bytes representation.
     */
    std::string bytes() const;

    /*!
     * \brief Get the uuid string representation
     */
    std::string str() const;

    /*!
     * \brief Get the uuid hash representation
     */
    size_t hash() const;

    friend bool operator==(const uuid& lhs, const uuid& rhs);
    friend bool operator<(const uuid& lhs, const uuid& rhs);
    friend bool operator!=(const uuid& lhs, const uuid& rhs);
    friend bool operator>(const uuid& lhs, const uuid& rhs);
    friend bool operator<=(const uuid& lhs, const uuid& rhs);
    friend bool operator>=(const uuid& lhs, const uuid& rhs);

    friend std::ostream& operator<<(std::ostream& stream, const uuid& uuid);

private:
    uuid(uint64_t x, uint64_t y);
    uuid(const uint8_t* bytes);
    uuid(const std::string& bytes);

    void bytes(char* bytes) const;
    void str(char* res) const;

    alignas(128) uint8_t data[16]; // NOLINT
};

}

// namespace std
// {
// template <>
// struct hash<tc::sdk::uuid>
// {
//     size_t operator()(const tc::sdk::uuid& uuid) const
//     {
//         return uuid.hash();
//     }
// };
// }
