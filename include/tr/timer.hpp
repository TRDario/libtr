#pragma once
#include "chrono.hpp"
#include <functional>
#include <thread>

namespace tr {
	/** @ingroup misc
	 *  @defgroup timer Timer
	 *  Minimal callback timer class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Minimal callback timer class.
	 *
	 * The timer runs a callback function on a separate thread at a regular interval until the timer is destroyed.
	 *
	 * The accuracy of the underlying system functions can be as low as 1ms, and although Timer attempts to correct for
	 * errors, it's not advised to use it for applications with <1ms resolution.
	 *
	 * Timer is default-constructible, non-copyable, and movable.
	 ******************************************************************************************************************/
	class Timer {
	  public:
		// The callback function signature expected by the timer.
		using Callback = std::function<void()>;

		/**************************************************************************************************************
		 * Constructs an inactive timer.
		 **************************************************************************************************************/
		Timer() noexcept = default;

		/**************************************************************************************************************
		 * Constructs an active timer.
		 *
		 * This constructor offers a strong exception guarantee.
		 *
		 * @exception std::system_error If launching a thread for the timer failed.
		 * @exception std::bad_alloc If allocating the callback function failed.
		 *
		 * @param interval The interval between ticks. Must be greater than 0, otherwise the thread will exit
		 *                 immediately.
		 * @param callback The callback function to call on every tick.
		 **************************************************************************************************************/
		template <class Rep, class Period, class CallbackT>
		Timer(const std::chrono::duration<Rep, Period>& interval, CallbackT&& callback);

		/**************************************************************************************************************
		 * Move constructs a timer.
		 *
		 * If @em r was previously in an active state, the thread it was managing will be transferred to the new
		 *timer, leaving @em r in an inactive state.
		 *
		 * @param r The timer to move from.
		 **************************************************************************************************************/
		Timer(Timer&& r) noexcept = default;

		/**************************************************************************************************************
		 * Stops and destroys the timer.
		 **************************************************************************************************************/
		~Timer() noexcept;

		/**************************************************************************************************************
		 * Move-assigns a timer.
		 *
		 * If the left-hand timer was previously in an active state, the thread it was managing will be closed as-if by
		 * destructor.
		 *
		 * If @em r was previously in an active state, the thread it was managing will be transferred to the new timer,
		 * leaving @em r in an inactive moved-from state.
		 *
		 * @param r The timer to move from.
		 **************************************************************************************************************/
		Timer& operator=(Timer&& r) noexcept;

		/**************************************************************************************************************
		 * Reports whether the timer is active.
		 *
		 * @return true If the timer is managing a ticking thread, or false otherwise.
		 **************************************************************************************************************/
		bool active() const noexcept;

	  private:
		std::unique_ptr<bool> _active; // Dynamically allocated so the thread isn't interrupted when moving.
		std::thread           _thread;

		static void thread(bool& active, Duration interval, Callback callback) noexcept;
	};
} // namespace tr

/// @cond

template <class Rep, class Period, class CallbackT>
tr::Timer::Timer(const std::chrono::duration<Rep, Period>& interval, CallbackT&& callback)
	: _active{std::make_unique<bool>(true)}
	, _thread{thread, std::ref(*_active), std::chrono::duration_cast<Duration>(interval),
			  Callback{std::forward<CallbackT>(callback)}}
{
}

/// @endcond