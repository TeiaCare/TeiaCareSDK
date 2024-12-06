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
#include <filesystem>

namespace tc::sdk
{

/**
 * @cond SKIP_DOXYGEN
 */
template <typename T>
struct is_vector : std::false_type
{
};

template <typename T>
struct is_vector<std::vector<T>> : std::true_type
{
};
/** @endcond */

/*!
 * \class argument_base
 * \brief Base class for command-line arguments.
 *
 * The tc::sdk::argument_base class serves as an abstract base for various types of command-line arguments,
 * providing common functionality for parsing values and handling environment variables.
 */
class argument_base
{
public:
    /*!
     * \brief Constructs an argument with the given properties.
     * \param name_long Long name of the argument (e.g., "--option").
     * \param name_short Short name of the argument (e.g., "-o") (optional).
     * \param description Description of the argument (optional).
     * \param required Indicates if the argument is required (default=false).
     * \param environment_var Name of the environment variable to parse (optional).
     */
    explicit argument_base(const std::string& name_long, const std::string& name_short = "", const std::string& description = "", bool required = false, const std::string& environment_var = "") noexcept
        : _required{required}
        , _name_long{name_long}
        , _name_short{name_short}
        , _description{description}
        , _env{environment_var}
    {
    }

    /*!
     * \brief Destructor
     *
     * Destructs this.
     */
    virtual ~argument_base() = default;

    /*!
     * \brief Pure virtual function to parse the provided value.
     * \param value String value to parse and convert.
     */
    virtual void parse(const std::string& value) = 0;

    /*!
     * \brief Parses the argument's value from an environment variable, if set.
     *
     * This function checks if the specified environment variable exists and,
     * if so, parses its value using the parse method.
     */
    void parse_from_env() noexcept
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

    /*!
     * \brief Checks if the argument has been parsed.
     * \return True if the argument has been parsed, false otherwise.
     */
    constexpr bool is_parsed() const noexcept
    {
        return _parsed;
    }

    /*!
     * \brief Checks if the argument is required.
     * \return True if the argument is required, false otherwise.
     */
    constexpr bool is_required() const noexcept
    {
        return _required;
    }

    /*!
     * \brief Gets the short name of the argument.
     * \return A constant reference to the short name of the argument.
     */
    constexpr const std::string& get_name_short() const noexcept
    {
        return _name_short;
    }

    /*!
     * \brief Gets the long name of the argument.
     * \return A constant reference to the long name of the argument.
     */
    constexpr const std::string& get_name_long() const noexcept
    {
        return _name_long;
    }

    /*!
     * \brief Gets the description of the argument.
     * \return A constant reference to the description of the argument.
     */
    constexpr const std::string& get_description() const noexcept
    {
        return _description;
    }

    /*!
     * \brief Gets the name of the associated environment variable.
     * \return A constant reference to the name of the environment variable.
     */
    constexpr const std::string& get_env() const noexcept
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

    // Specialization for const char*
    template <typename T>
    typename std::enable_if_t<std::is_same_v<T, const char*>, T>
    convert(const std::string& str) const
    {
        return str.c_str();
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

    // Specialization for std::filesystem::path
    template <typename T>
    typename std::enable_if_t<std::is_same_v<T, std::filesystem::path>, T>
    convert(const std::string& str) const
    {
        try
        {
            return std::filesystem::path(str);
        }
        catch (...)
        {
            throw std::runtime_error("Conversion error: " + _name_long + "=" + str);
        }
    }

    // Generic implementation (for every type different from int, float, double, std::string, std::vector and std::filesystem::path)
    template <typename T>
    typename std::enable_if_t<
        !std::is_same_v<T, int> &&
            !std::is_same_v<T, float> &&
            !std::is_same_v<T, double> &&
            !std::is_same_v<T, std::string> &&
            !std::is_same_v<T, const char*> &&
            !std::is_same_v<T, std::filesystem::path> &&
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
