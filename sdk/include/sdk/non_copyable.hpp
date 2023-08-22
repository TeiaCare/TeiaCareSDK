#pragma once

namespace tc::sdk
{
/*!
 * \class non_copyable
 * \brief Utility class expected to be used as a base class to define non-copyable classes.
 * \note It is not necessary to publicly derive from this class.
 * It is sufficient to derive privately to prevent copy operations on the derived class.
 * \code
   class my_class : private non_copyable
   { ... };
   \endcode
 */
class non_copyable
{
protected:
    /*!
     * \brief Constructor
     * 
     * Default constructor for tc::sdk::non_copyable 
     */
    inline non_copyable() = default;

    /*!
     * \brief Destructor
     * 
     * Default constructor for tc::sdk::non_copyable 
     */
    inline ~non_copyable() = default;

    /*!
     * \brief Copy constructor
     * 
     * Copy constructor marked as deleted. 
     */
    non_copyable(const non_copyable&) = delete;
    
    /*!
     * \brief Copy assignment operator
     * 
     * Copy assignment operator marked as deleted. 
     */
    non_copyable& operator=(const non_copyable&) = delete;
};

}
