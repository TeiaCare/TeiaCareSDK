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

namespace tc::sdk
{
/*!
 * \class size
 * \brief 2D size specified by a pair of width/height dimensions.
 * \tparam T Dimensions type
 *
 */
template <typename T>
class size
{
public:
    /*!
     * \brief Default Constructor. Creates a default tc::sdk::size instance.
     */
    constexpr size() = default;

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::size instance into another one.
     */
    constexpr size(const size&) = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::size instance into another one.
     */
    constexpr size(size&&) = default;

    /*!
     * \brief Constructor. Creates tc::sdk::size instance with given width and height dimensions.
     * \param w the width of the size.
     * \param h the height of the size.
     */
    constexpr explicit size(T w, T h) noexcept
        : _width{w}
        , _height{h}
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::size instance to another one.
     */
    size& operator=(const size&) = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::size instance to another one.
     */
    size& operator=(size&&) = default;

    /*!
     * \brief Equality operator.
     * \param other the size to compare against.
     * \return true if the two sizes have the same dimensions.
     */
    constexpr inline bool operator==(const size& other) const noexcept
    {
        return _width == other._width && _height == other._height;
    }

    /*!
     * \brief Inequality operator.
     * \param other the size to compare against.
     * \return true if the two sizes have the different dimensions.
     */
    constexpr inline bool operator!=(const size& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Check if the size is the null.
     * \return true if both width and height are zero.
     */
    constexpr bool is_null() const noexcept
    {
        return _width == 0 && _height == 0;
    }

    /*!
     * \brief Width dimension getter.
     * \return Width dimension of the size.
     */
    constexpr inline T width() const noexcept
    {
        return _width;
    }

    /*!
     * \brief Height dimension getter.
     * \return Height dimension of the size.
     */
    constexpr inline T height() const noexcept
    {
        return _height;
    }

    /*!
     * \brief Width coordinate setter. Set the width dimension of the size.
     */
    inline void set_width(T width) noexcept
    {
        _width = width;
    }

    /*!
     * \brief Height coordinate setter. Set the height dimension of the size.
     */
    inline void set_height(T height) noexcept
    {
        _height = height;
    }

    /*!
     * \brief Get the size string representation
     * \return String representation of the current size.
     */
    constexpr std::string to_string() const
    {
        return std::string("(" + std::to_string(_width) + "x" + std::to_string(_height) + ")");
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param z the size object to stream.
     * \return reference to the output stream operator, with the size string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const size& s)
    {
        return stream << s.to_string();
    }

private:
    T _width{};
    T _height{};
};

}
