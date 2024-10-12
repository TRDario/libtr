/**
 * @file benchmark.cppm
 * @brief Provides a simple benchmarking class.
 */

module;
#include <cassert>

export module tr:benchmark;

import std;
import :chrono;

export namespace tr {
	/******************************************************************************************************************
    * Simple benchmarking class.
    *
    * The benchmark stores timestamps going up to 5s back for calculations like average/min/max.
    *******************************************************************************************************************/
	class Benchmark {
	public:
        /******************************************************************************************************************
        * Shorthand for the deque type used by the benchmark.
        *******************************************************************************************************************/
		using Deque = std::deque<std::pair<TimePoint, Duration>>;

		
        /******************************************************************************************************************
        * Default-constructs an empty benchmark.
        *******************************************************************************************************************/
		Benchmark() noexcept;


		/******************************************************************************************************************
        * Starts a new measurement.
        *
        * If a previous measurement was started but not ended, a failed assertion may be raised.
        *******************************************************************************************************************/
		void start() noexcept;

		/******************************************************************************************************************
        * Stops a measurement.
        *
        * @exception std::bad_alloc If the deque failed to allocate memory for the measurement.
        *
        * If a measurement was not started, a failed assertion may be raised.
        *******************************************************************************************************************/
		void stop();


		/******************************************************************************************************************
        * Clears all previous measurements from the queue.
        *
        * If a measurement was started but not stopped, it is cancelled.
        *******************************************************************************************************************/
		void clear() noexcept;


        /******************************************************************************************************************
        * Gets the duration of the latest measurement.
        *
        * @return The duration of the latest measurement, or 0s if no measurements are in the queue.
        *******************************************************************************************************************/
		Duration latest() const noexcept;

        /******************************************************************************************************************
        * Gets the duration of the shortest available measurement.
        *
        * @return The duration of the shortest available measurement, or 0s if no measurements are in the queue.
        *******************************************************************************************************************/
		Duration min() const noexcept;

        /******************************************************************************************************************
        * Gets the duration of the longest available measurement.
        *
        * @return The duration of the longest available measurement, or 0s if no measurements are in the queue.
        *******************************************************************************************************************/
		Duration max() const noexcept;

        /******************************************************************************************************************
        * Gets the average duration of the available measurements.
        *
        * @return The average duration of the available measurements, or 0s if no measurements are in the queue.
        *******************************************************************************************************************/
		Duration average() const noexcept;

        /******************************************************************************************************************
        * Gets the average number of measurements per second.
        *
        * @return The average number of measurements per second, or 0 if no measurements are in the queue.
        *******************************************************************************************************************/
		double fps() const noexcept;

        /******************************************************************************************************************
        * Gets the available measurements.
        *
        * @return A reference to the measurement queue.
        *******************************************************************************************************************/
		const Deque& measurements() const noexcept;
	private:
		Deque 	  _durations;  // The measurement deque.
		TimePoint _startPoint; // The start point of the latest started (but not ended) measurement.
	};
}

// IMPLEMENTATION

using namespace std::chrono_literals;

tr::Benchmark::Benchmark() noexcept
{}

void tr::Benchmark::start() noexcept
{
    assert(_startPoint == TimePoint {});
    atomic_thread_fence(std::memory_order::relaxed);
    _startPoint = Clock::now();
    atomic_thread_fence(std::memory_order::relaxed);
}

void tr::Benchmark::stop()
{
    assert(_startPoint != TimePoint {});

    atomic_thread_fence(std::memory_order::relaxed);
    auto now { Clock::now() };
    auto it { std::ranges::upper_bound(_durations, now - 5s, std::less {}, &Deque::value_type::first) };
    if (it != _durations.end()) {
        _durations.erase(_durations.begin(), it);
    }
    _durations.emplace_back(_startPoint, now - _startPoint);
    _startPoint = TimePoint {};
    atomic_thread_fence(std::memory_order::relaxed);
}

void tr::Benchmark::clear() noexcept
{
    _startPoint = TimePoint {};
    _durations.clear();
}

tr::Duration tr::Benchmark::latest() const noexcept
{
    return !_durations.empty() ? _durations.back().second : Duration { 0 };
}

tr::Duration tr::Benchmark::min() const noexcept
{
    return !_durations.empty() ? *std::ranges::min_element(std::views::values(_durations)) : Duration { 0 };
}

tr::Duration tr::Benchmark::max() const noexcept
{
    return !_durations.empty() ? *std::ranges::max_element(std::views::values(_durations)) : Duration { 0 };
}

tr::Duration tr::Benchmark::average() const noexcept
{
    if (_durations.empty()) {
        return Duration { 0 };
    }
    else {
        auto durations { std::views::values(_durations) };
        return std::accumulate(durations.begin(), durations.end(), Duration { 0 }) / int(_durations.size());
    }
}

double tr::Benchmark::fps() const noexcept
{
    return !_durations.empty() ? _durations.size() / duration_cast<SecondsD>(_startPoint - _durations.front().first).count() : 0;
}

const tr::Benchmark::Deque& tr::Benchmark::measurements() const noexcept
{
    return _durations;
}