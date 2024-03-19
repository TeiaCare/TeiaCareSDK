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

#include <teiacare/sdk/geometry/point.hpp>
#include <teiacare/sdk/geometry/size.hpp>

#include <string>

namespace tc::sdk
{
/*!
 * \class rectangle
 * \brief 2D rectangle specified by a position point (top-left corner) and a pair of width/height dimensions.
 * \tparam T Underlying coordinates and dimensions type
 *
 */
template <typename T>
class rectangle
{
public:
    /*!
     * \brief Default Constructor. Creates a default tc::sdk::rectangle instance.
     */
    constexpr rectangle() = default;

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::rectangle instance into another one.
     */
    constexpr rectangle(const rectangle&) = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::rectangle instance into another one.
     */
    constexpr rectangle(rectangle&&) = default;

    /*!
     * \brief Constructor. Creates tc::sdk::rectangle instance with given width and height dimensions.
     * \param position the position of the rectangle (top-left corner).
     * \param w the width of the rectangle.
     * \param h the height of the rectangle.
     */
    constexpr explicit rectangle(tc::sdk::point<T> position, T w, T h) noexcept
        : _position{position}
        , _width{w}
        , _height{h}
    {
    }

    /*!
     * \brief Constructor. Creates tc::sdk::rectangle instance with given width and height dimensions.
     * \param x the x coordinate of the rectangle position (top-left corner).
     * \param y the y coordinate of the rectangle position (top-left corner).
     * \param w the width of the rectangle.
     * \param h the height of the rectangle.
     */
    constexpr explicit rectangle(T x, T y, T w, T h) noexcept
        : _position{x, y}
        , _width{w}
        , _height{h}
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::rectangle instance to another one.
     */
    rectangle& operator=(const rectangle&) = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::rectangle instance to another one.
     */
    rectangle& operator=(rectangle&&) = default;

    /*!
     * \brief Equality operator.
     * \param other the rectangle to compare against.
     * \return true if the two rectangles have the same dimensions and the same position.
     */
    constexpr inline bool operator==(const rectangle& other) const noexcept
    {
        return _position == other._position &&
               _width == other._width &&
               _height == other._height;
    }

    /*!
     * \brief Inequality operator.
     * \param other the rectangle to compare against.
     * \return true if the two rectangles have the different dimensions or different position.
     */
    constexpr inline bool operator!=(const rectangle& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Check if the rectangle is the null.
     * \return true if both width and height are zero.
     */
    constexpr bool is_null() const noexcept
    {
        return _width <= 0 && _height <= 0;
    }

    /*!
     * \brief Width dimension getter.
     * \return Width dimension of the rectangle.
     */
    constexpr inline T width() const noexcept
    {
        return _width;
    }

    /*!
     * \brief Height dimension getter.
     * \return Height dimension of the rectangle.
     */
    constexpr inline T height() const noexcept
    {
        return _height;
    }

    /*!
     * \brief Width coordinate setter. Set the width dimension of the rectangle.
     */
    inline void set_width(T width) noexcept
    {
        _width = width;
    }

    /*!
     * \brief Height coordinate setter. Set the height dimension of the rectangle.
     */
    inline void set_height(T height) noexcept
    {
        _height = height;
    }

    /*!
     * \brief Size setter.
     * \return Set the size of the rectangle.
     */
    constexpr inline void set_size(T width, T height) noexcept
    {
        _width = width;
        _height = height;
    }

    /*!
     * \brief Set the rectangle position.
     * \param position the new top left coordinate of the rectangle.
     */
    constexpr inline void set_position(tc::sdk::point<T> position) noexcept
    {
        _position = position;
    }

    /*!
     * \brief Top left coordinate getter.
     * \return Top left coordinate of the rectangle.
     */
    constexpr inline tc::sdk::point<T> top_left() const noexcept
    {
        return tc::sdk::point<T>(_position);
    }

    /*!
     * \brief Top right coordinate getter.
     * \return Top right coordinate of the rectangle.
     */
    constexpr inline tc::sdk::point<T> top_right() const noexcept
    {
        return tc::sdk::point<T>(_position.x() + _width, _position.y());
    }

    /*!
     * \brief Bottom left coordinate getter.
     * \return Bottom left coordinate of the rectangle.
     */
    constexpr inline tc::sdk::point<T> bottom_left() const noexcept
    {
        return tc::sdk::point<T>(_position.x(), _position.y() + _height);
    }

    /*!
     * \brief Bottom right coordinate getter.
     * \return Bottom right coordinate of the rectangle.
     */
    constexpr inline tc::sdk::point<T> bottom_right() const noexcept
    {
        return tc::sdk::point<T>(_position.x() + _width, _position.y() + _height);
    }

    /*!
     * \brief Size getter.
     * \return The size of the rectangle.
     */
    constexpr inline tc::sdk::size<T> size() const noexcept
    {
        return tc::sdk::size<T>(_width, _height);
    }

    /*!
     * \brief Area getter.
     * \return The area of the rectangle.
     */
    constexpr inline T area() const noexcept
    {
        return _width * _height;
    }

    /*!
     * \brief Center getter.
     * \return The center coordinate of the rectangle.
     */
    constexpr inline tc::sdk::point<T> center() const noexcept
    {
        return {_position.x() + _width / 2, _position.y() + _height / 2};
    }

    /*!
     * \brief Check if the given point is strictly contained within the current rectangle.
     * \param point The point that is checked.
     * \return true if the point is strictly contained in the current rectangle.
     */
    constexpr inline bool contains(const tc::sdk::point<T>& point) const noexcept
    {
        return point.x() > top_left().x() &&
               point.x() < bottom_right().x() &&
               point.y() > top_left().y() &&
               point.y() < bottom_right().y();
    }

    /*!
     * \brief Check if the given point is strictly contained within the current rectangle.
     * \param point The point that is checked.
     * \return true if the point is strictly contained in the current rectangle.
     */
    inline void translate(T delta_x, T delta_y) noexcept
    {
        _position.add_delta(delta_x, delta_y);
    }

    /*!
     * \brief Get the rectangle string representation
     * \return String representation of the current rectangle.
     */
    constexpr std::string str() const
    {
        return std::string(_position.str() + " : (" + std::to_string(_width) + "x" + std::to_string(_height) + ")");
    }

    friend std::ostream& operator<<(std::ostream& stream, const rectangle& r)
    {
        return stream << r.str();
    }

private:
    tc::sdk::point<T> _position{};
    T _width{};
    T _height{};
};

}