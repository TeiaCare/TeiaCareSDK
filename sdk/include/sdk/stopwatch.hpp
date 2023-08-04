#pragma once

#include <sdk/clock.hpp>

namespace tc::sdk
{
/*! \class stopwatch
 *  \brief Stopwatch object that measures the amount of elapsed time from a given time point.
 */
class stopwatch
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

    stopwatch(const stopwatch&) = delete;
    stopwatch(stopwatch&&) noexcept = delete;
    stopwatch& operator=(const stopwatch&) = delete;
    stopwatch& operator=(stopwatch&&) = delete;

    [[nodiscard]] tc::sdk::time_point start_time() const noexcept
    {
        return _start_time;
    }

    [[nodiscard]] tc::sdk::time_duration elapsed() const noexcept
    {
        return tc::sdk::clock::now() - _start_time;
    }

    template<class DurationT>
    [[nodiscard]] DurationT elapsed() const noexcept
    {
        static_assert(tc::sdk::is_duration<DurationT>::value, 
            "\nDurationT must be a duration type");

        return std::chrono::duration_cast<DurationT>(elapsed());
    }

    // [[nodiscard]] float GetElapsedSecondsF() const noexcept
    // {
    //     return GetElapsedSeconds<float>();
    // }

    // template<typename T>
    // [[nodiscard]] T GetElapsedSeconds() const noexcept
    // {
    //     return std::chrono::duration_cast<std::chrono::duration<T>>(elapsed()).count();
    // }

    void reset() noexcept
    {
        _start_time = tc::sdk::clock::now();
    }

private:
    tc::sdk::time_point _start_time = tc::sdk::clock::now();
};

}
