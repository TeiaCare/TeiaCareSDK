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

#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace tc::sdk
{
template <typename T>
struct is_vector : std::false_type
{
};

template <typename T>
struct is_vector<std::vector<T>> : std::true_type
{
};

class argument_base
{
public:
    explicit argument_base(const std::string& name_long, const std::string& name_short = "", const std::string& description = "", bool required = false, const std::string& environment_var = "") noexcept
        : _required{required}
        , _name_long{name_long}
        , _name_short{name_short}
        , _description{description}
        , _env{environment_var}
    {
    }

    virtual ~argument_base() = default;

    virtual void parse(const std::string& value) = 0;

    void parse_from_env()
    {
        if (_env.empty())
            return;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
        const char* env_value = std::getenv(_env.c_str());
#ifdef _MSC_VER
#pragma warning(pop)
#endif

        if (env_value)
        {
            parse(std::string(env_value));
        }
    }

    constexpr bool is_parsed() const
    {
        return _parsed;
    }

    constexpr bool is_required() const
    {
        return _required;
    }

    constexpr const std::string& get_name_short() const
    {
        return _name_short;
    }

    constexpr const std::string& get_name_long() const
    {
        return _name_long;
    }

    constexpr const std::string& get_description() const
    {
        return _description;
    }

    constexpr const std::string& get_env() const
    {
        return _env;
    }

protected:
    bool _parsed = false;

    // Specialization for std::string
    template <typename T>
    typename std::enable_if_t<std::is_same_v<T, std::string>, T>
    convert(const std::string& str) const
    {
        return str;
    }

    // Specialization for int
    template <typename T>
    typename std::enable_if_t<std::is_same_v<T, int>, T>
    convert(const std::string& str) const
    {
        try
        {
            return std::stoi(str);
        }
        catch (...)
        {
            throw std::runtime_error("Conversion error: " + _name_long + "=" + str);
        }
    }

    // Specialization for float
    template <typename T>
    typename std::enable_if_t<std::is_same_v<T, float>, T>
    convert(const std::string& str) const
    {
        try
        {
            return std::stof(str);
        }
        catch (...)
        {
            throw std::runtime_error("Conversion error: " + _name_long + "=" + str);
        }
    }

    // Specialization for double
    template <typename T>
    typename std::enable_if_t<std::is_same_v<T, double>, T>
    convert(const std::string& str) const
    {
        try
        {
            return std::stod(str);
        }
        catch (...)
        {
            throw std::runtime_error("Conversion error: " + _name_long + "=" + str);
        }
    }

    // Specialization for std::vector<T>
    template <typename T>
    typename std::enable_if_t<tc::sdk::is_vector<T>::value, T>
    convert(const std::string& str) const
    {
        using VectorT = typename T::value_type;
        std::vector<VectorT> values;
        std::istringstream iss(str);
        std::string list_item;

        while (std::getline(iss, list_item, ','))
        {
            const VectorT value = convert<VectorT>(list_item);
            values.push_back(value);
        }

        return values;
    }

    // Generic implementation (for every type different from int, float, double, std::string and std::vector)
    template <typename T>
    typename std::enable_if_t<
        !std::is_same_v<T, int> &&
            !std::is_same_v<T, float> &&
            !std::is_same_v<T, double> &&
            !std::is_same_v<T, std::string> &&
            !tc::sdk::is_vector<T>::value,
        T>
    convert(const std::string& str) const
    {
        T value;
        std::istringstream iss(str);

        if (!(iss >> value) || !(iss.eof()))
        {
            throw std::runtime_error("Conversion error: " + _name_long + "=" + str);
        }
        return value;
    }

private:
    bool _required = false;
    const std::string _name_long;
    const std::string _name_short;
    const std::string _description;
    const std::string _env;
};

}
