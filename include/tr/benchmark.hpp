#pragma once
#include "chrono.hpp"
#include <deque>

namespace tr {
	/** @ingroup misc
	 *  @defgroup benchmark Benchmark
	 *  Minimal benchmarking class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Minimal benchmarking class.
	 *
	 * The benchmark stores intervals between calls to start() and stop() going up to 2.5s back for calculations like
	 * average/min/max.
	 *******************************************************************************************************************/
	class Benchmark {
	  public:
		/**************************************************************************************************************
		 * Shorthand for the deque type used by the benchmark.
		 **************************************************************************************************************/
		using Deque = std::deque<std::pair<TimePoint, Duration>>;

		Benchmark() noexcept = default;

		/**************************************************************************************************************
		 * Starts a new measurement.
		 *
		 * This function cannot be called if a previous measurement was started but not ended.
		 **************************************************************************************************************/
		void start() noexcept;

		/**************************************************************************************************************
		 * Stops a measurement.
		 *
		 * @exception std::bad_alloc If an internal allocation fails.
		 *
		 * This function cannot be called if a measurement was not started.
		 **************************************************************************************************************/
		void stop();

		/**************************************************************************************************************
		 * Clears all previous measurements from the queue.
		 *
		 * If a measurement was started but not stopped, it is cancelled.
		 **************************************************************************************************************/
		void clear() noexcept;

		/**************************************************************************************************************
		 * Gets the duration of the latest measurement.
		 *
		 * @return The duration of the latest measurement, or 0s if no measurements are in the queue.
		 **************************************************************************************************************/
		Duration latest() const noexcept;

		/**************************************************************************************************************
		 * Gets the duration of the shortest available measurement.
		 *
		 * @return The duration of the shortest available measurement, or 0s if no measurements are in the queue.
		 **************************************************************************************************************/
		Duration min() const noexcept;

		/**************************************************************************************************************
		 * Gets the duration of the longest available measurement.
		 *
		 * @return The duration of the longest available measurement, or 0s if no measurements are in the queue.
		 **************************************************************************************************************/
		Duration max() const noexcept;

		/**************************************************************************************************************
		 * Gets the average duration of the available measurements.
		 *
		 * @return The average duration of the available measurements, or 0s if no measurements are in the queue.
		 **************************************************************************************************************/
		Duration average() const noexcept;

		/**************************************************************************************************************
		 * Gets the average number of measurements per second.
		 *
		 * @return The average number of measurements per second, or 0 if no measurements are in the queue.
		 **************************************************************************************************************/
		double fps() const noexcept;

		/**************************************************************************************************************
		 * Gets the available measurements.
		 *
		 * @return A reference to the measurement queue.
		 **************************************************************************************************************/
		const Deque& measurements() const noexcept;

	  private:
		Deque     _durations;  // The measurement deque.
		TimePoint _startPoint; // The start point of the latest started (but not ended) measurement.
	};

	/// @}
} // namespace tr
