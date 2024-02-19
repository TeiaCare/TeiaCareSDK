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

#include <sdk/uuid.hpp>

#include <random>

namespace tc::sdk
{
/*! 
 * \class uuid_generator
 * \brief UUID Generator used to create tc::sdk::uuid objects
 * 
 * This class is a singleton that must be used to create instances of tc::sdk::uuid objects.
 */
class uuid_generator
{
public:
    /*!
     * \brief Static method to retrieve the single instance to the underlying generator
     * \return the uuid_generator unique instance
     *
     * Note that the constructor is private, thus it is not possible to create multiple uuid_generator instances.
     */
    static uuid_generator& instance();

    /*!
     * \brief Create a random uuid V4.
     */
    uuid create();
    
    /*!
     * \brief Create a tc::sdk::uuid object from a std::string
     *
     * In case the provided string is not a valid uuid V4 a new valid uuid is returned.
     */
    uuid from_string(const std::string& s);
    
    /*!
     * \brief Create a tc::sdk::uuid object from a const char*
     *
     * In case the provided string is not a valid uuid V4 a new valid uuid is returned.
     */
    uuid from_string(const char* raw);

private:
    uuid_generator();
    std::mt19937_64 _generator;
    std::uniform_int_distribution<uint64_t> _distribution;
};

}
