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

#include <cmath>
#include <string>

namespace tc::sdk
{
/*!
 * \class point
 * \brief 2D point specified by a pair of (x,y) coordinates.
 * \tparam T Coordinates type
 *
 */
template <typename T>
class point
{
public:
    /*!
     * \brief Default Constructor. Creates a default tc::sdk::point instance.
     */
    constexpr point() = default;

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::point instance into another one.
     */
    constexpr point(const point&) = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::point instance into another one.
     */
    constexpr point(point&&) = default;

    /*!
     * \brief Constructor. Creates tc::sdk::point instance with given X and Y coordinates.
     * \param x the X coordinate of the point
     * \param y the Y coordinate of the point
     */
    constexpr explicit point(T x, T y) noexcept
        : _x{x}
        , _y{y}
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::point instance to another one.
     */
    point& operator=(const point&) = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::point instance to another one.
     */
    point& operator=(point&&) = default;

    /*!
     * \brief Equality operator.
     * \param other the point to compare against.
     * \return true if the two points have the same coordinates.
     */
    constexpr inline bool operator==(const point& other) const noexcept
    {
        return _x == other._x && _y == other._y;
    }

    /*!
     * \brief Inequality operator.
     * \param other the point to compare against.
     * \return true if the two points have the different coordinates.
     */
    constexpr inline bool operator!=(const point& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Check if the point is the origin.
     * \return true if the point has coordinates (0,0)
     */
    constexpr bool is_origin() const noexcept
    {
        return *this == point();
    }

    /*!
     * \brief X coordinate getter.
     * \return X coordinate of the point.
     */
    constexpr inline T x() const noexcept
    {
        return _x;
    }

    /*!
     * \brief Y coordinate getter.
     * \return Y coordinate of the point.
     */
    constexpr inline T y() const noexcept
    {
        return _y;
    }

    /*!
     * \brief X coordinate setter. Set the X coordinate of the point.
     */
    inline void set_x(T x) noexcept
    {
        _x = x;
    }

    /*!
     * \brief Y coordinate setter. Set the Y coordinate of the point.
     */
    inline void set_y(T y) noexcept
    {
        _y = y;
    }

    /*!
     * \brief Increments point's coordinate of delta_x and delta_y values.
     * \param delta_x Value to be added to the current X coordinate.
     * \param delta_y Value to be added to the current Y coordinate.
     */
    void add_delta(T delta_x, T delta_y) noexcept
    {
        _x += delta_x;
        _y += delta_y;
    }

    /*!
     * \brief Decrements point's coordinate of delta_x and delta_y values.
     * \param delta_x Value to be added to the current X coordinate.
     * \param delta_y Value to be added to the current Y coordinate.
     */
    void sub_delta(T delta_x, T delta_y) noexcept
    {
        _x -= delta_x;
        _y -= delta_y;
    }

    /*!
     * \brief Distance between the current point and the origin.
     * \return The resulting distance.
     */
    constexpr T distance_from_origin() const noexcept
    {
        return static_cast<T>(std::sqrt(_x * _x + _y * _y));
    }

    /*!
     * \brief Distance between the current point and another one.
     * \param other The point to compute the distance from.
     * \return The resulting distance.
     */
    constexpr T distance(point other) const noexcept
    {
        T dx = other._x - _x;
        T dy = other._y - _y;
        return static_cast<T>(std::sqrt(dx * dx + dy * dy));
    }

    /*!
     * \brief Add two points.
     * \param other The point to be added to the current one.
     * \return The resulting point.
     */
    constexpr point operator+(point other) const noexcept
    {
        return point{_x + other._x, _y + other._y};
    }

    /*!
     * \brief Add another point's coordinate to the current one.
     * \param other The point to be added to the current one.
     * \return The current point instance after the addition of the other.
     */
    point& operator+=(point other) noexcept
    {
        _x += other._x;
        _y += other._y;
        return *this;
    }

    /*!
     * \brief Subtract two points.
     * \param other The point to be subtracted to the current one.
     * \return The resulting point.
     */
    constexpr point operator-(point other) const noexcept
    {
        return point{_x - other._x, _y - other._y};
    }

    /*!
     * \brief Subtract another point's coordinate to the current one.
     * \param other The point to be subtracted to the current one.
     * \return The current point instance after the subtraction of the other.
     */
    point& operator-=(point other) noexcept
    {
        _x -= other._x;
        _y -= other._y;
        return *this;
    }

    /*!
     * \brief Multiply current point's coordinate by a scalar value.
     * \param scalar The scalar value to multiply the current point with.
     * \return The current point instance after the scalar multiplication with the scalar value.
     */
    template <typename ScalarT>
    point& operator*=(ScalarT scalar) noexcept
    {
        _x = static_cast<T>(_x * scalar);
        _y = static_cast<T>(_y * scalar);
        return *this;
    }

    /*!
     * \brief Multiply a point by a given scalar value.
     * \param scalar The scalar value to multiply the resulting point with.
     * \return The resulting point.
     */
    template <typename ScalarT>
    constexpr point operator*(ScalarT scalar) const noexcept
    {
        return point{static_cast<T>(_x * scalar), static_cast<T>(_y * scalar)};
    }

    /*!
     * \brief Divide current point's coordinate by a scalar value.
     * \param scalar The scalar value to multiply the current point with.
     * \return The current point instance after the scalar multiplication with the scalar value.
     */
    template <typename ScalarT>
    point& operator/=(ScalarT scalar)
    {
        _x = static_cast<T>(_x / scalar);
        _y = static_cast<T>(_y / scalar);
        return *this;
    }

    /*!
     * \brief Divide a point by a given scalar value.
     * \param scalar The scalar value to divide the resulting point with.
     * \return The resulting point.
     */
    template <typename ScalarT>
    constexpr point operator/(ScalarT scalar) const
    {
        return point{static_cast<T>(_x / scalar), static_cast<T>(_y / scalar)};
    }

    /*!
     * \brief Get the point string representation
     * \return String representation of the current point.
     */
    constexpr std::string to_string() const
    {
        return std::string("(" + std::to_string(_x) + ", " + std::to_string(_y) + ")");
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param p the point object to stream.
     * \return reference to the output stream operator, with the point string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const point& p)
    {
        return stream << p.to_string();
    }

private:
    T _x{};
    T _y{};
};

}
