#pragma once

#include <sdk/clock.hpp>
#include <sdk/non_copyable.hpp>
#include <sdk/non_moveable.hpp>

namespace tc::sdk
{
/*!
 * \class stopwatch
 * \brief Stopwatch object that measures the amount of elapsed time from a given time point.
 */
class stopwatch : private non_copyable, private non_moveable
{
public:
    /*!
     * \brief Constructor.
     * 
     * Creates a tc::sdk::stopwatch instance. 
     */
    explicit stopwatch() = default;

    /*!
     * \brief Destructor.
     * 
     * Destructs this.
     */
    ~stopwatch() noexcept = default;

    /*!
     * \brief Get the start time.
     * \return start_time time_point
     */
    [[nodiscard]] tc::sdk::time_point start_time() const noexcept
    {
        return _start_time;
    }

    /*!
     * \brief Get the elapsed time.
     * \return elapsed time_duration
     */
    [[nodiscard]] tc::sdk::time_duration elapsed() const noexcept
    {
        return tc::sdk::clock::now() - _start_time;
    }

    /*!
     * \brief Get the elapsed time.
     * \return elapsed time_duration
     * 
     * Override to require the elapsed time with a gived duration type (i.e. std::chrono::milliseconds)
     */
    template<class DurationT>
    [[nodiscard]] DurationT elapsed() const noexcept
    {
        static_assert(tc::sdk::is_duration<DurationT>::value, 
            "\nDurationT must be a duration type");

        return std::chrono::duration_cast<DurationT>(elapsed());
    }

    /*!
     * \brief Reset the start time.
     */
    void reset() noexcept
    {
        _start_time = tc::sdk::clock::now();
    }

private:
    tc::sdk::time_point _start_time = tc::sdk::clock::now();
};

}
