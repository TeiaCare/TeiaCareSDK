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

#include <string>

namespace tc::sdk
{
/*!
 * \class line
 * \brief 2D line specified by a pair of start/end points.
 * \tparam T Underlying points coordinate type.
 *
 */
template <typename T>
class line
{
public:
    /*!
     * \brief Default Constructor. Creates a default tc::sdk::line instance.
     */
    constexpr line() = default;

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::line instance into another one.
     */
    constexpr line(const line&) = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::line instance into another one.
     */
    constexpr line(line&&) = default;

    /*!
     * \brief Constructor. Creates tc::sdk::line instance with given start and end points.
     * \param start the start point of the line.
     * \param end the end point of the line.
     */
    constexpr explicit line(tc::sdk::point<T> start, tc::sdk::point<T> end) noexcept
        : _start{start}
        , _end{end}
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::line instance to another one.
     */
    line& operator=(const line&) = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::line instance to another one.
     */
    line& operator=(line&&) = default;

    /*!
     * \brief Equality operator.
     * \param other the line to compare against.
     * \return true if the two lines have the same start and end points.
     */
    constexpr inline bool operator==(const line& other) const noexcept
    {
        return _start == other._start && _end == other._end;
    }

    /*!
     * \brief Inequality operator.
     * \param other the line to compare against.
     * \return true if the two lines have different start or end points.
     */
    constexpr inline bool operator!=(const line& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Check if the line is a point.
     * \return true if the start and end points are the same.
     */
    constexpr bool is_null() const noexcept
    {
        return _start == _end;
    }

    /*!
     * \brief Check if the line is vertical.
     * \return true if the start and end points have the same x coordinate.
     */
    bool is_vertical() const noexcept
    {
        return _start.x() == _end.x();
    }

    /*!
     * \brief Check if the line is horizontal.
     * \return true if the start and end points have the same y coordinate.
     */
    bool is_horizontal() const noexcept
    {
        return _start.y() == _end.y();
    }

    /*!
     * \brief Equality operator.
     * \param other the line to compare against.
     * \return true if the two lines have the same start and end points.
     */
    bool operator==(line other) const noexcept
    {
        return _start == other._start && _end == other._end;
    }

    /*!
     * \brief Inequality operator.
     * \param other the line to compare against.
     * \return true if the two lines have the different start or end points.
     */
    bool operator!=(line other) const noexcept
    {
        return _start != other._start || _end != other._end;
    }

    /*!
     * \brief Start point getter.
     * \return Start point of the line.
     */
    constexpr inline tc::sdk::point<T> start() const noexcept
    {
        return _start;
    }

    /*!
     * \brief End point getter.
     * \return End point of the line.
     */
    constexpr inline tc::sdk::point<T> end() const noexcept
    {
        return _end;
    }

    /*!
     * \brief Start point setter. Set the start point of the line.
     */
    inline void set_start(tc::sdk::point<T> start) noexcept
    {
        _start = start;
    }

    /*!
     * \brief End point setter. Set the end point of the line.
     */
    inline void set_end(tc::sdk::point<T> end) noexcept
    {
        _end = end;
    }

    /*!
     * \brief Get the line string representation
     * \return String representation of the current line.
     */
    constexpr std::string str() const
    {
        return std::string("(" + _start.str() + " : " + _end.str() + ")");
    }

    friend std::ostream& operator<<(std::ostream& stream, const line& l)
    {
        return stream << l.str();
    }

private:
    tc::sdk::point<T> _start{};
    tc::sdk::point<T> _end{};
};

}