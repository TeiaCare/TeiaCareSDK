#pragma once

#include <sdk/clock.hpp>
#include <sdk/non_copyable.hpp>
#include <sdk/non_moveable.hpp>

#include <chrono>
#include <type_traits>

namespace tc::sdk
{
/*! 
 * \class rate_limiter
 * \brief Rate Limiter class to sync a loop with a fixed frame rate.
 */
class rate_limiter : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor
     * \param rate set the required rate value
     * 
     * Creates a tc::sdk::rate_limiter instance with the given rate. 
     */
    explicit rate_limiter(size_t rate);

    /*!
     * \brief Destructor
     * 
     * Destructs this.
     */
    ~rate_limiter() = default;

    /*!
     * \brief Sync the current thread to match the required rate.
     *
     * This method must be called continuously in a loop in order to sync it.
     */
    void sync();

private:
    using duration_rep = double;
    const std::chrono::duration<duration_rep> time_between_frames; // 1/rate
    std::chrono::time_point<tc::sdk::clock, std::remove_const_t<decltype(time_between_frames)>> tp;
};
}
