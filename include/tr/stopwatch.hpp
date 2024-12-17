#pragma once
#include "chrono.hpp"

namespace tr {
	/** @ingroup misc
	 *  @defgroup stopwatch Stopwatch
	 *  Minimal stopwatch class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Minimal stopwatch class.
	 ******************************************************************************************************************/
	class Stopwatch {
	  public:
		/**************************************************************************************************************
		 * Constructs and starts the stopwatch.
		 **************************************************************************************************************/
		Stopwatch() noexcept;

		/**************************************************************************************************************
		 * Gets the elapsed time since the stopwatch was started.
		 *
		 * @return The elapsed time since the stopwatch was started.
		 **************************************************************************************************************/
		Duration elapsedTime() const noexcept;

		/**************************************************************************************************************
		 * Restarts the stopwatch and gets the elapsed time since it was started.
		 *
		 * @return The elapsed time since the stopwatch was started.
		 **************************************************************************************************************/
		Duration lap() noexcept;

	  private:
		TimePoint _start;
	};

	/// @}
} // namespace tr