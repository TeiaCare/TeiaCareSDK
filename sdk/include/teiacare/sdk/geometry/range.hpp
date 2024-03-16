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
 * \class range
 * \brief Linear range specified by a pair of min/max values.
 * \tparam T Value type.
 *
 */
template<typename T>
class range
{
public:
    /*!
     * \brief Default Constructor. Creates a default tc::sdk::range instance.
     */
    constexpr range() = default;

    /*!
     * \brief Copy Constructor. Copy a tc::sdk::range instance into another one.
     */
    constexpr range(const range&) = default;

    /*!
     * \brief Move Constructor. Copy a tc::sdk::range instance into another one.
     */
    constexpr range(range&&) = default;

    /*!
     * \brief Constructor. Creates tc::sdk::range instance with given min and max values.
     * \param min the min value of the range
     * \param max the max value of the range
     */
    constexpr explicit range(T min, T max) noexcept
        : _min{min}
        , _max{max}
    {
    }

    /*!
     * \brief Assignment operator. Assign a tc::sdk::range instance to another one.
     */
    range& operator=(const range&) = default;

    /*!
     * \brief Move assignment operator. Assign a tc::sdk::range instance to another one.
     */
    range& operator=(range&&) = default;

    /*!
     * \brief Equality operator.
     * \param other the range to compare against.
     * \return true if the two ranges have the same values.
     */
    constexpr inline bool operator==(const range& other) const noexcept
    {
        return _min == other._min && _max == other._max;
    }

    /*!
     * \brief Inequality operator.
     * \param other the range to compare against.
     * \return true if the two ranges have the different values.
     */
    constexpr inline bool operator!=(const range& other) const noexcept
    {
        return !operator==(other);
    }

    /*!
     * \brief Check if the current range is null.
     * \return true if min and max values are the same.
     */
    constexpr inline bool is_null() const noexcept
    {
        return _min == _max;
    }

    /*!
     * \brief Min getter.
     * \return Min value of the range.
     */
    constexpr inline T min() const noexcept
    {
        return _min;
    }

    /*!
     * \brief Max getter.
     * \return Max value of the range.
     */
    constexpr inline T max() const noexcept
    {
        return _max;
    }

    /*!
     * \brief Min value setter. Set the Min value of the range. 
     * If the new min value is higher than the max value the two value will be set both to the new min (i.e. creating a null range).
     */
    inline void set_min(T min) noexcept
    {
        _min = min;
        if (_max < _min)
            _max = _min;
    }

    /*!
     * \brief Max value setter. Set the Max value of the range. 
    * If the new max value is lower than the min value the two values will be set both to the new max (i.e. creating a null range).
     */
    inline void set_max(T max) noexcept
    {
        _max = max;
        if (_max < _min)
            _min = _max;
    }

    /*!
     * \brief Get the lenght of the range.
     * \return The difference between the range's max and min values.
     */
    constexpr inline T lenght() const noexcept
    {
        return _max - _min;
    }

    /*!
     * \brief Check if the given value is contained within the range.
     * \param value the value to check.
     * \return true if the value is contained in the range.
     */
    constexpr inline bool contains(T value) const noexcept
    {
        return _min <= value && value < _max;
    }

    /*!
     * \brief Check if the given range is contained within this range.
     * \param other the range to check.
     * \return true if the given range is contained in the range.
     */
    constexpr inline bool contains(range other) const noexcept
    {
        // TODO
        return true;
    }

    /*!
     * \brief Check if the given range intersects with this range.
     * \param other the range to check.
     * \return true if the given range intersects with this range.
     */
    constexpr bool intersects(range other) const noexcept
    {
        return other._min < _max && _min < other._max;
    }

    /*!
     * \brief Clamp the value within the range.
     * \param value the value to clamp.
     * \return the value clamped within the current range.
     */
    constexpr inline T clamp(T value) const noexcept
    {
        return value < _min ? _min : (_max < value ? _max : value);
    }

    /*!
     * \brief Remap a normalised value (in the 0-1 range) to the range.
     * \param value the value to map.
     * \return the value remapped to the current range.
     */
    constexpr inline T map(T value) const
    {
        // static_assert(value >= T(0) && value <= T(1));
        return _min + value * (_max - _min);
    }

    // TODO
    constexpr range get_intersection(range) const noexcept
    {
        return {};
    }

    // TODO
    constexpr range get_union(range) const noexcept
    {
        return {};
    }

    // TODO
    constexpr range lower_range(T value) const noexcept
    {
        // from min to value
        return {};
    }

    constexpr range upper_range(T value) const noexcept
    {
        // from value to max
        return {};
    }

    /*!
     * \brief Get the range string representation
     * \return String representation of the current range.
     */
    constexpr std::string str() const
    {
        return std::string("(" + std::to_string(_min) + ":" + std::to_string(_max) + ")");
    }


    friend std::ostream& operator<<(std::ostream& stream, const range& p)
    {
        return stream << r.str();
    }

private:
    T _min{};
    T _max{};
};

}
