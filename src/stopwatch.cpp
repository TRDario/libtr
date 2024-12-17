#include "../include/tr/stopwatch.hpp"
#include <atomic>

tr::Stopwatch::Stopwatch() noexcept
{
	atomic_thread_fence(std::memory_order::relaxed);
	_start = Clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

tr::Duration tr::Stopwatch::elapsedTime() const noexcept
{
	atomic_thread_fence(std::memory_order::relaxed);
	const auto now{Clock::now()};
	atomic_thread_fence(std::memory_order::relaxed);
	return now - _start;
}

tr::Duration tr::Stopwatch::lap() noexcept
{
	const auto start = _start;
	atomic_thread_fence(std::memory_order::relaxed);
	_start = Clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
	return _start - start;
}