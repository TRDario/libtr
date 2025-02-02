#include "../include/tr/benchmark.hpp"

using namespace std::chrono_literals;

void tr::Benchmark::start() noexcept
{
	assert(_startPoint == TimePoint{});
	atomic_thread_fence(std::memory_order::relaxed);
	_startPoint = Clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

void tr::Benchmark::stop()
{
	assert(_startPoint != TimePoint{});

	atomic_thread_fence(std::memory_order::relaxed);
	const TimePoint now{Clock::now()};
	_durations.emplace_back(_startPoint, now - _startPoint);
	const auto it{std::ranges::upper_bound(_durations, now - 2.5s, std::less{}, &Deque::value_type::first)};
	if (it != _durations.end()) {
		_durations.erase(_durations.begin(), it);
	}
	_startPoint = TimePoint{};
	atomic_thread_fence(std::memory_order::relaxed);
}

void tr::Benchmark::clear() noexcept
{
	_startPoint = TimePoint{};
	_durations.clear();
}

tr::Duration tr::Benchmark::latest() const noexcept
{
	return !_durations.empty() ? _durations.back().second : Duration{0};
}

tr::Duration tr::Benchmark::min() const noexcept
{
	return !_durations.empty() ? *std::ranges::min_element(std::views::values(_durations)) : Duration{0};
}

tr::Duration tr::Benchmark::max() const noexcept
{
	return !_durations.empty() ? *std::ranges::max_element(std::views::values(_durations)) : Duration{0};
}

tr::Duration tr::Benchmark::average() const noexcept
{
	if (_durations.empty()) {
		return Duration{0};
	}
	else {
		const auto durations{std::views::values(_durations)};
		return std::accumulate(durations.begin(), durations.end(), Duration{0}) / _durations.size();
	}
}

double tr::Benchmark::fps() const noexcept
{
	return !_durations.empty() ? _durations.size() / 2.5 : 0;
}

const tr::Benchmark::Deque& tr::Benchmark::measurements() const noexcept
{
	return _durations;
}
