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

#include <teiacare/sdk/geometry/line.hpp>
#include <teiacare/sdk/geometry/point.hpp>
#include <teiacare/sdk/geometry/size.hpp>
#include <teiacare/sdk/math.hpp>

#include <algorithm>
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
     * \return true if both width and height are zero or negative.
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
    constexpr inline tc::sdk::point<double> center() const noexcept
    {
        return tc::sdk::point<double>(_position.x() + _width * 0.5, _position.y() + _height * 0.5);
    }

    /*!
     * \brief Check if the given point is strictly contained within the current rectangle.
     * \param point The point that is checked.
     * \return true if the point is strictly contained in the current rectangle.
     */
    bool contains(tc::sdk::point<T> point) const noexcept
    {
        return point.x() > _position.x() &&
               point.y() > _position.y() &&
               point.x() < _position.x() + _width &&
               point.y() < _position.y() + _height;
    }

    /*!
     * \brief Check if the given rectangle is strictly contained within the current rectangle.
     * \param other The rectangle that is checked.
     * \return true if the other rectangle is strictly contained in the current rectangle.
     */
    bool contains(tc::sdk::rectangle<T> other) const noexcept
    {
        return _position.x() < other._position.x() &&
               _position.y() < other._position.y() &&
               _position.x() + _width >= other._position.x() + other._width &&
               _position.y() + _height >= other._position.y() + other._height;
    }

    /*!
     * \brief Check if the given rectangle overlaps the current rectangle.
     * \param other The rectangle to check the intersection with.
     * \return true if this and the other rectangles are overlapped.
     */
    bool intersects(tc::sdk::rectangle<T> other) const noexcept
    {
        return _position.x() + _width > other._position.x() &&
               _position.y() + _height > other._position.y() &&
               _position.x() < other._position.x() + other._width &&
               _position.y() < other._position.y() + other._height &&
               !other.is_null() &&
               !is_null();
    }

    /*!
     * \brief Check if the given line pass through the current rectangle.
     * \param line The line to check the intersection with.
     * \return true if the given line intersects the current rectangle.
     */
    bool intersects(tc::sdk::line<T> line) const noexcept
    {
        return contains(line.start()) ||
               contains(line.end()) ||
               line.intersects(tc::sdk::line<T>(top_left(), top_right())) ||
               line.intersects(tc::sdk::line<T>(top_right(), bottom_right())) ||
               line.intersects(tc::sdk::line<T>(bottom_right(), bottom_left())) ||
               line.intersects(tc::sdk::line<T>(bottom_left(), top_left()));
    }

    /** Returns the region that is the overlap between this and another rectangle.
        If the two rectangles don't overlap, the rectangle returned will be empty.
    */
    tc::sdk::rectangle<T> get_intersection(tc::sdk::rectangle<T> other) const noexcept
    {
        if (is_null() || other.is_null())
            return tc::sdk::rectangle<T>{};

        auto intersection_x = tc::sdk::max(_position.x(), other._position.x());
        auto intersection_y = tc::sdk::max(_position.y(), other._position.y());

        auto intersection_width = tc::sdk::min(_position.x() + _width, other._position.x() + other._width) - intersection_x;
        if (intersection_width < T())
            return tc::sdk::rectangle<T>{};

        auto intersection_height = tc::sdk::min(_position.y() + _height, other._position.y() + other._height) - intersection_y;
        if (intersection_height < T())
            return tc::sdk::rectangle<T>{};

        tc::sdk::point<T> intersection_position(intersection_x, intersection_y);
        return tc::sdk::rectangle<T>(intersection_position, intersection_width, intersection_height);
    }

    /** Returns the smallest rectangle that contains both this one and the one passed-in.

        If either this or the other rectangle are empty, they will not be counted as
        part of the resulting region.
    */
    tc::sdk::rectangle<T> get_union(tc::sdk::rectangle<T> other) const noexcept
    {
        if (other.is_null())
            return *this;

        if (is_null())
            return other;

        auto union_x = tc::sdk::min(_position.x(), other._position.x());
        auto union_y = tc::sdk::min(_position.y(), other._position.y());

        auto union_width = tc::sdk::max(_position.x() + _width, other._position.x() + other._width) - union_x;
        auto union_height = tc::sdk::max(_position.y() + _height, other._position.y() + other._height) - union_y;

        tc::sdk::point<T> union_position(union_x, union_y);
        return tc::sdk::rectangle<T>(union_position, union_width, union_height);
    }

    /*!
     * \brief Translate the rectangle by a given delta_x and delta_y offsets.
     * \param delta_x the offset to be applied on the x direction
     * \param delta_y the offset to be applied on the y direction
     */
    void translate(T delta_x, T delta_y) noexcept
    {
        _position.add_delta(delta_x, delta_y);
    }

    /*!
     * \brief Create a copy of the rectangle translated by a given delta_x and delta_y offsets.
     * \param delta_x the offset to be applied on the x direction
     * \param delta_y the offset to be applied on the y direction
     * \return a translated copy of the current rectangle
     */
    tc::sdk::rectangle<T> translated(T delta_x, T delta_y) const noexcept
    {
        const auto translated_position = tc::sdk::point<T>(_position);
        translated_position.add_delta(delta_x, delta_y);
        return tc::sdk::rectangle<T>(translated_position, _width, _height);
    }

    /*!
     * \brief Calculate Intersection over Union (IoU) with another rectangle.
     * \param other The rectangle to calculate IoU with.
     * \return The IoU value as a ratio of the intersection area to the union area.
     */
    double calculate_iou(const tc::sdk::rectangle<T>& other) const noexcept
    {
        // Early exit for null rectangles
        if (is_null() || other.is_null())
        {
            return 0.0;
        }

        const auto intersection_rect = get_intersection(other);
        if (intersection_rect.is_null())
        {
            return 0.0;
        }

        const auto intersection_area = intersection_rect.area();
        const auto union_area = area() + other.area() - intersection_area;

        // Avoid division by zero
        if (union_area == T(0.0))
        {
            return 0.0;
        }

        return static_cast<double>(intersection_area) / static_cast<double>(union_area);
    }

    /*!
     * \brief Transform this rectangle from input coordinate space back to output coordinate space.
     * This method handles the inverse transformation of letterboxing/padding applied during scaling.
     * \param input_width The width of the input coordinate space.
     * \param input_height The height of the input coordinate space.
     * \param output_width The width of the output coordinate space.
     * \param output_height The height of the output coordinate space.
     * \return A rectangle transformed back to output coordinates.
     */
    tc::sdk::rectangle<T> reshape(
        T input_width,
        T input_height,
        T output_width,
        T output_height) const noexcept
    {
        // Input validation
        if (output_width <= T{} || output_height <= T{} ||
            input_width <= T{} || input_height <= T{} ||
            is_null())
        {
            return tc::sdk::rectangle<T>{};
        }

        // Calculate scale ratios for width and height
        const auto scale_width = static_cast<double>(input_width) / output_width;
        const auto scale_height = static_cast<double>(input_height) / output_height;

        // Use the smaller ratio to maintain aspect ratio (letterboxing)
        const auto scale = tc::sdk::min(scale_width, scale_height);

        // Calculate actual scaled dimensions of the output in input space
        const auto scaled_output_width = output_width * scale;
        const auto scaled_output_height = output_height * scale;

        // Calculate padding offsets (where letterbox bars are added)
        const auto x_padding_offset = (input_width - scaled_output_width) * 0.5;
        const auto y_padding_offset = (input_height - scaled_output_height) * 0.5;

        // Get input box boundaries (using this rectangle)
        const auto input_left = static_cast<double>(top_left().x());
        const auto input_top = static_cast<double>(top_left().y());
        const auto input_right = input_left + width();
        const auto input_bottom = input_top + height();

        // Remove padding and scale back to output coordinates
        const auto inverse_scale = 1.0 / scale;
        const auto output_left = (input_left - x_padding_offset) * inverse_scale;
        const auto output_right = (input_right - x_padding_offset) * inverse_scale;
        const auto output_top = (input_top - y_padding_offset) * inverse_scale;
        const auto output_bottom = (input_bottom - y_padding_offset) * inverse_scale;

        // Clamp to output boundaries
        const auto clamped_left = std::clamp(static_cast<T>(output_left), T{}, output_width);
        const auto clamped_right = std::clamp(static_cast<T>(output_right), T{}, output_width);
        const auto clamped_top = std::clamp(static_cast<T>(output_top), T{}, output_height);
        const auto clamped_bottom = std::clamp(static_cast<T>(output_bottom), T{}, output_height);

        // Calculate final dimensions
        const auto final_width = tc::sdk::max(T{}, clamped_right - clamped_left);
        const auto final_height = tc::sdk::max(T{}, clamped_bottom - clamped_top);

        // Create and return the transformed rectangle
        return tc::sdk::rectangle<T>(clamped_left, clamped_top, final_width, final_height);
    }

    /*!
     * \brief Get the rectangle string representation
     * \return String representation of the current rectangle.
     */
    constexpr std::string to_string() const
    {
        return std::string(_position.to_string() + " : (" + std::to_string(_width) + "x" + std::to_string(_height) + ")");
    }

    /*!
     * \brief Output stream operator.
     * \param stream the output stream to write into.
     * \param r the rectangle object to stream.
     * \return reference to the output stream operator, with the rectangle string representation written into it.
     */
    friend std::ostream& operator<<(std::ostream& stream, const rectangle& r)
    {
        return stream << r.to_string();
    }

private:
    tc::sdk::point<T> _position{};
    T _width{};
    T _height{};
};

}
