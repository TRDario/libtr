module;
#include <cassert>

export module tr:handle;

import std;

export namespace tr {
	// RAII wrapper over non-pointer handles.
	template <class T, T EmptyValue, std::invocable<T> Deleter> class Handle {
	public:
		constexpr 		   Handle() 			 noexcept;
		constexpr explicit Handle(T object) 	 noexcept;
		constexpr 		   Handle(Handle&& move) noexcept;
		constexpr 		  ~Handle() 			 noexcept;

		constexpr Handle& operator=(Handle&& r) noexcept;

		constexpr bool operator==(const Handle&) const noexcept = default;

		constexpr bool has_value() const noexcept;
		constexpr explicit operator bool() const noexcept;

		constexpr const T& get() const noexcept;

		constexpr void reset(T object = EmptyValue) noexcept;
		constexpr void swap (Handle& other) 		noexcept;
	private:
		T _handle;
	};
}

// IMPLEMENTATION

namespace tr {
	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr Handle<T, EmptyValue, Deleter>::Handle() noexcept
		: _handle { EmptyValue }
	{}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr Handle<T, EmptyValue, Deleter>::Handle(T object) noexcept
		: _handle { object }
	{}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr Handle<T, EmptyValue, Deleter>::Handle(Handle&& move) noexcept
	{
		std::swap(_handle, move._handle);
	}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr Handle<T, EmptyValue, Deleter>::~Handle() noexcept
	{
		if (_handle != EmptyValue) {
			Deleter{}(_handle);
		}
	}
	
	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr Handle<T, EmptyValue, Deleter>& Handle<T, EmptyValue, Deleter>::operator=(Handle&& r) noexcept
	{
		std::ignore = Handle { std::move(*this) };
		std::swap(_handle, r._handle);
		return *this;
	}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr bool Handle<T, EmptyValue, Deleter>::has_value() const noexcept
	{
		return _handle != EmptyValue;
	}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr Handle<T, EmptyValue, Deleter>::operator bool() const noexcept
	{
		return has_value();
	}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr const T& Handle<T, EmptyValue, Deleter>::get() const noexcept
	{
		assert(_handle != EmptyValue);
		return _handle;
	}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr void Handle<T, EmptyValue, Deleter>::reset(T object) noexcept
	{
		std::ignore = Handle { std::move(*this) };
		_handle = object;
	}

	template <class T, T EmptyValue, std::invocable<T> Deleter>
	constexpr void Handle<T, EmptyValue, Deleter>::swap(Handle& other) noexcept
	{
		std::swap(_handle, other._handle);
	}
}