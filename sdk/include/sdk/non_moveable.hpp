#pragma once

namespace tc::sdk
{
/*!
 * \class non_moveable
 * \brief Utility class expected to be used as a base class to define non-moveable classes.
 * \note It is not necessary to publicly derive from this class.
 * It is sufficient to derive privately to prevent move operations on the derived class.
 * \code
   class my_class : private non_moveable
   { ... };
   \endcode
 */
class non_moveable
{
protected:
    /*!
     * \brief Constructor
     * 
     * Default constructor for tc::sdk::non_moveable 
     */
    inline non_moveable() = default;

    /*!
     * \brief Destructor
     * 
     * Default constructor for tc::sdk::non_moveable 
     */
    inline ~non_moveable() = default;

    /*!
     * \brief Move constructor
     * 
     * Move constructor marked as deleted. 
     */
    non_moveable(non_moveable&& ) = delete;
    
    /*!
     * \brief Move assignment operator
     * 
     * Move assignment operator marked as deleted. 
     */
    non_moveable& operator=(non_moveable&&) = delete;
};

}
