#include "../include/tr/timer.hpp"

using namespace std::chrono_literals;

tr::Timer::Timer(Duration interval, const Callback& callback)
	: _active{std::make_unique<bool>(true)}, _thread{thread, *_active, interval, callback}
{
}

tr::Timer::Timer(Duration interval, Callback&& callback)
	: _active{std::make_unique<bool>(true)}, _thread{thread, *_active, interval, std::move(callback)}
{
}

tr::Timer::~Timer() noexcept
{
	if (_active != nullptr) {
		*_active = false;
		if (_thread.joinable()) {
			_thread.join();
		}
	}
}

tr::Timer& tr::Timer::operator=(Timer&& r) noexcept
{
	std::ignore = Timer{std::move(*this)};
	_active     = std::move(r._active);
	_thread     = std::move(r._thread);
	return *this;
}

bool tr::Timer::active() const noexcept
{
	return _active != nullptr;
}

void tr::Timer::thread(bool& active, Duration interval, Callback callback) noexcept
{
	try {
		tr::TimePoint prev{tr::Clock::now()};
		tr::Duration  totalError{};

		std::this_thread::sleep_for(interval);
		while (active) {
			atomic_thread_fence(std::memory_order::relaxed);
			const auto now{tr::Clock::now()};
			atomic_thread_fence(std::memory_order::relaxed);

			const tr::Duration lastInterval{now - prev};
			totalError += lastInterval - interval;

			tr::Duration nextInterval{interval - totalError};
			while (nextInterval <= 0s) {
				callback();
				nextInterval += interval;
				totalError -= interval;
			}
			callback();

			prev = now;
			std::this_thread::sleep_until(now + nextInterval);
		}
	}
	catch (...) {
		// Exit gracefully if an exception occurs.
		active = false;
		return;
	}
}