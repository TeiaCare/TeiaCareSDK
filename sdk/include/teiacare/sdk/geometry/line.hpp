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

#include <optional>
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
        return (_start == other._start && _end == other._end) ||
               (_start == other._end && _end == other._start);
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
     * \brief Check if the given line segments intersects the current instance.
     * \param other the line to check the intersection against.
     * \return true if the two lines intersects.
     */
    bool intersects(tc::sdk::line<T> other) const noexcept
    {
        return get_intersection(other) != std::nullopt;
    }

    /*!
     * \brief Computes the intersection between the given line and the current one.
     * \param other the compute to check the intersection against.
     * \return std::nullopt if the two lines do not intersect, otherwise a std::optional<tc::sdk::point<double>> with the intersection point.
     */
    std::optional<tc::sdk::point<double>> get_intersection(tc::sdk::line<T> other) const noexcept
    {
        // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment

        auto is_unit_range = [](double value) {
            return 0.0 <= value && value <= 1.0;
        };

        const double x_A = static_cast<double>(_start.x());
        const double y_A = static_cast<double>(_start.y());

        const double x_B = static_cast<double>(_end.x());
        const double y_B = static_cast<double>(_end.y());

        const double x_C = static_cast<double>(other._start.x());
        const double y_C = static_cast<double>(other._start.y());

        const double x_D = static_cast<double>(other._end.x());
        const double y_D = static_cast<double>(other._end.y());

        const double denom = ((x_A - x_B) * (y_C - y_D) - (y_A - y_B) * (x_C - x_D));

        const double t = ((x_A - x_C) * (y_C - y_D) - (y_A - y_C) * (x_C - x_D)) / denom;
        if (!is_unit_range(t))
            return std::nullopt;

        const double u = ((x_A - x_B) * (y_A - y_C) - (y_A - y_B) * (x_A - x_C)) / denom;
        if (!is_unit_range(-u))
            return std::nullopt;

        const double intersection_x = x_A + t * (x_B - x_A);
        const double intersection_y = y_A + t * (y_B - y_A);

        return tc::sdk::point<double>(intersection_x, intersection_y);
    }

    /*!
     * \brief Get the line string representation
     * \return String representation of the current line.
     */
    constexpr std::string to_string() const
    {
        return std::string("(" + _start.to_string() + " : " + _end.to_string() + ")");
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param l the line object to stream.
     * \return reference to the output stream operator, with the line string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const line& l)
    {
        return stream << l.to_string();
    }

private:
    tc::sdk::point<T> _start{};
    tc::sdk::point<T> _end{};
};

}
