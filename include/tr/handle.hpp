#pragma once
#include "concepts.hpp"
#include <cassert>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace tr {
	/******************************************************************************************************************
	 * Concept that denotes a valid handle base type.
	 *
	 * To satisfy this requirement, a type must be trivially copyable and equality comparable.
	 ******************************************************************************************************************/
	template <class T>
	concept HandleType = std::is_trivially_copyable_v<T> && std::equality_comparable<T>;

	/******************************************************************************************************************
	 * Concept that denotes a valid handle deleter type for a Handle of base type @em HandleT.
	 *
	 * To satisfy this requirement, a type must be invocable with @em HandleT, be move- or copy-constructible.
	 ******************************************************************************************************************/
	template <class T, class HandleT>
	concept HandleDeleter = std::invocable<T, HandleT> && (std::move_constructible<T> || std::copy_constructible<T>);

	/******************************************************************************************************************
	 * Concept that denotes a default constructible handle deleter.
	 *
	 * To satisfy this requirement, a type must be default initializable, but not a pointer.
	 ******************************************************************************************************************/
	template <class T>
	concept DefaultConstructibleHandleDeleter = std::default_initializable<T> && !std::is_pointer_v<T>;

	/******************************************************************************************************************
	 * Tag struct used in some handle functions to suppress empty value checking.
	 ******************************************************************************************************************/
	struct NoEmptyHandleCheck {};

	/******************************************************************************************************************
	 * Tag value used in some handle functions to suppress empty value checking.
	 ******************************************************************************************************************/
	constexpr NoEmptyHandleCheck NO_EMPTY_HANDLE_CHECK{};

	/******************************************************************************************************************
	 * RAII wrapper over non-pointer handles.
	 *
	 * libtr examples of usage of this type include various OpenGL and OpenAL objects.
	 *
	 * @tparam T Underlying type used by the handle, must satisfy <em>HandleType</em>.
	 * @tparam EmptyValue A value of type T set aside for use as a sentinel of an empty handle.
	 * @tparam Deleter The deleter used by the handle, must satisfy @link HandleDeleter <em>HandleDeleter<T></em>
	 *@endlink.
	 ******************************************************************************************************************/
	template <HandleType T, T EmptyValue, HandleDeleter<T> Deleter> class Handle : private Deleter {
	  public:
		/**************************************************************************************************************
		 * Default-constructs an empty handle.
		 *
		 * Requires that @em Deleter is default constructible (pointers are not counted as default constructible).
		 **************************************************************************************************************/
		constexpr Handle() noexcept
			requires(DefaultConstructibleHandleDeleter<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * If @em EmptyValue is an expected input, Handle(T, NoEmptyHandleCheck)
		 * may be used instead.
		 *
		 * Requires that @em Deleter is default constructible (pointers are not counted as default constructible).
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 **************************************************************************************************************/
		constexpr explicit Handle(T value) noexcept
			requires(DefaultConstructibleHandleDeleter<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * If @em EmptyValue is an expected input, Handle(T, deleter, NoEmptyHandleCheck)
		 * may be used instead.
		 *
		 * Requires that @em Deleter is copy-constructible.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit Handle(T value, Deleter& deleter) noexcept(std::is_nothrow_copy_constructible_v<Deleter>)
			requires(std::copy_constructible<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * If @em EmptyValue is an expected input, Handle(T, deleter, NoEmptyHandleCheck)
		 * may be used instead.
		 *
		 * Requires that @em Deleter is copy-constructible.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit Handle(T              value,
								  const Deleter& deleter) noexcept(std::is_nothrow_copy_constructible_v<Deleter>)
			requires(std::copy_constructible<Deleter> && !NonConstReference<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * If @em EmptyValue is an expected input, Handle(T, deleter, NoEmptyHandleCheck)
		 * may be used instead.
		 *
		 * Requires that @em Deleter is move-constructible.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 * @param[in] deleter The deleter value to move.
		 **************************************************************************************************************/
		constexpr explicit Handle(T value, Deleter&& deleter) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
			requires(std::move_constructible<Deleter> && !std::is_lvalue_reference_v<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value without checking for the invalid @em EmptyValue case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Handle(T) should be used in cases where this isn't necessary.
		 *
		 * Requires that @em Deleter is default constructible (pointers are not counted as default constructible).
		 *
		 * @param[in] value The base type value that will be managed by the handle.
		 **************************************************************************************************************/
		constexpr explicit Handle(T value, NoEmptyHandleCheck) noexcept
			requires(DefaultConstructibleHandleDeleter<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter without checking for the invalid @em EmptyValue
		 * case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Handle(T, deleter) should be used in cases where this isn't necessary.
		 *
		 * Requires that @em Deleter is copy-constructible.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit Handle(T value, Deleter& deleter,
								  NoEmptyHandleCheck) noexcept(std::is_nothrow_copy_constructible_v<Deleter>)
			requires(std::copy_constructible<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter without checking for the invalid @em EmptyValue
		 * case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Handle(T, deleter) should be used in cases where this isn't necessary.
		 *
		 * Requires that @em Deleter is copy-constructible.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit Handle(T value, const Deleter& deleter,
								  NoEmptyHandleCheck) noexcept(std::is_nothrow_copy_constructible_v<Deleter>)
			requires(std::copy_constructible<Deleter> && !NonConstReference<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter without checking for the invalid @em EmptyValue
		 * case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Handle(T, deleter) should be used in cases where this isn't necessary.
		 *
		 * Requires that @em Deleter is move-constructible.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em EmptyValue, an assertion may be raised otherwise.
		 * @endparblock
		 * @param[in] deleter The deleter value to move.
		 **************************************************************************************************************/
		constexpr explicit Handle(T value, Deleter&& deleter,
								  NoEmptyHandleCheck) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
			requires(std::move_constructible<Deleter> && !std::is_lvalue_reference_v<Deleter>);

		/**************************************************************************************************************
		 * Constructs a handle by moving from another handle.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * @param[in] move
		 * @parblock
		 * The handle whose ownership over a value will be moved into the new handle.
		 *
		 * It will be left in a valid empty state afterwards.
		 * @endparblock
		 **************************************************************************************************************/
		template <HandleDeleter<T> Deleter1> constexpr Handle(Handle<T, EmptyValue, Deleter1>&& move) noexcept;

		/**************************************************************************************************************
		 * Destroys the handle.
		 *
		 * If the handle is not empty, <em>Deleter</em> will be called with the contained value.
		 **************************************************************************************************************/
		constexpr ~Handle() noexcept;

		/**************************************************************************************************************
		 * Move-assigns the handle.
		 *
		 * If any value was contained in the handle before this, <em>Deleter</em> is called with said value.
		 *
		 * @param[in] r
		 * @parblock
		 * The handle whose ownership over a value will be moved into the handle.
		 *
		 * It will be left in a valid empty state afterwards.
		 * @endparblock
		 *
		 * @return A reference to assigned-to handle.
		 **************************************************************************************************************/
		constexpr Handle& operator=(Handle&& r) noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 *
		 * Two handle references are only considered equal if they point to the same object.
		 *
		 * @param[in] r A handle reference.
		 *
		 * @return true if r is a reference to the same object as *this.
		 **************************************************************************************************************/
		constexpr bool operator==(const Handle& r) const noexcept;

		/**************************************************************************************************************
		 * Checks if the handle contains a value.
		 *
		 * @return true if the handle is not empty.
		 **************************************************************************************************************/
		constexpr bool has_value() const noexcept;

		/**************************************************************************************************************
		 * Checks if the handle contains a value, see has_value().
		return std::hash<decltype(texture._id)> {}(texture._id);
		**************************************************************************************************************/
		constexpr explicit operator bool() const noexcept;

		/**************************************************************************************************************
		 * Gets the handle's base type value.
		 *
		 * Calls to this function are only valid if the handle contains a value, an assertion may be raised otherwise.
		 *
		 * get(NoEmptyHandleCheck) may be used in cases where the empty value is an expected input.
		 *
		 * @return A reference to the handle's base type value.
		 **************************************************************************************************************/
		constexpr const T& get() const noexcept;

		/**************************************************************************************************************
		 * Gets the handle's base type value.
		 *
		 * This function circuments the notion of an "empty" handle and returns a value even if it's
		 *<em>EmptyValue</em>.
		 *
		 * get() should be used in cases where this isn't necessary.
		 *
		 * @return A reference to the handle's base type value.
		 **************************************************************************************************************/
		constexpr const T& get(NoEmptyHandleCheck) const noexcept;

		/**************************************************************************************************************
		 * Gets the handle's deleter.
		 *
		 * @return A mutable reference to the handle's deleter.
		 **************************************************************************************************************/
		constexpr Deleter& get_deleter() noexcept;

		/**************************************************************************************************************
		 * Gets the handle's deleter.
		 *
		 * @return An immutable reference to the handle's deleter.
		 **************************************************************************************************************/
		constexpr Deleter& get_deleter() const noexcept;

		/**************************************************************************************************************
		 * Releases ownership over the handle, if any.
		 *
		 * The handle will be empty after this call.
		 *
		 * @return The value previously held by the handle.
		 **************************************************************************************************************/
		constexpr T release() noexcept;

		/**************************************************************************************************************
		 * Resets the handle to an empty state.
		 *
		 * If the handle is not empty before this call, <em>Deleter</em> will be called with the contained value.
		 **************************************************************************************************************/
		constexpr void reset() noexcept;

		/**************************************************************************************************************
		 * Resets the handle to a non-empty state.
		 *
		 * If the handle is not empty before this call, <em>Deleter</em> will be called with the contained value.
		 *
		 * reset(T, NoEmptyHandleCheck) may be used in cases where the empty value is an expected input.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal <em>EmptyValue</em>, an assertion may be raised otherwise.
		 * @endparblock
		 **************************************************************************************************************/
		constexpr void reset(T value) noexcept;

		/**************************************************************************************************************
		 * Resets the handle to a new state.
		 *
		 * If the handle is not empty before this call, <em>Deleter</em> will be called with the contained value.
		 *
		 * reset(T) should be preferred in most cases.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * If it equals <em>EmptyValue</em>, the handle will be empty afterwards, otherwise it will contain a value.
		 * @endparblock
		 **************************************************************************************************************/
		constexpr void reset(T value, NoEmptyHandleCheck) noexcept;

		/**************************************************************************************************************
		 * Swaps the ownership over values between two handles.
		 *
		 * Either handle may or may not be empty, in which case the swapped handle will take on that state.
		 *
		 * @param other The handle to swap values with.
		 **************************************************************************************************************/
		constexpr void swap(Handle& other) noexcept;

	  private:
		T _base;
	};
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
struct std::hash<tr::Handle<T, EmptyValue, Deleter>> {
	constexpr auto operator()(const tr::Handle<T, EmptyValue, Deleter>& handle) const noexcept;
};

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr auto std::hash<tr::Handle<T, EmptyValue, Deleter>>::operator()(
	const tr::Handle<T, EmptyValue, Deleter>& handle) const noexcept
{
	return std::hash<T>{}(handle.get(tr::NO_EMPTY_HANDLE_CHECK));
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle() noexcept
	requires(DefaultConstructibleHandleDeleter<Deleter>)
	: _base{EmptyValue}
{
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value) noexcept
	requires(DefaultConstructibleHandleDeleter<Deleter>)
	: _base{value}
{
	assert(value != EmptyValue);
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, Deleter& deleter) noexcept(
	std::is_nothrow_copy_constructible_v<Deleter>)
	requires(std::copy_constructible<Deleter>)
	: Deleter{deleter}, _base{value}
{
	assert(value != EmptyValue);
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, const Deleter& deleter) noexcept(
	std::is_nothrow_copy_constructible_v<Deleter>)
	requires(std::copy_constructible<Deleter> && !NonConstReference<Deleter>)
	: Deleter{deleter}, _base{value}
{
	assert(value != EmptyValue);
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, Deleter&& deleter) noexcept(
	std::is_nothrow_move_constructible_v<Deleter>)
	requires(std::move_constructible<Deleter> && !std::is_lvalue_reference_v<Deleter>)
	: Deleter{std::move(deleter)}, _base{value}
{
	assert(value != EmptyValue);
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, NoEmptyHandleCheck) noexcept
	requires(DefaultConstructibleHandleDeleter<Deleter>)
	: _base{value}
{
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, Deleter& deleter, NoEmptyHandleCheck) noexcept(
	std::is_nothrow_copy_constructible_v<Deleter>)
	requires(std::copy_constructible<Deleter>)
	: Deleter{deleter}, _base{value}
{
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, const Deleter& deleter, NoEmptyHandleCheck) noexcept(
	std::is_nothrow_copy_constructible_v<Deleter>)
	requires(std::copy_constructible<Deleter> && !NonConstReference<Deleter>)
	: Deleter{deleter}, _base{value}
{
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(T value, Deleter&& deleter, NoEmptyHandleCheck) noexcept(
	std::is_nothrow_move_constructible_v<Deleter>)
	requires(std::move_constructible<Deleter> && !std::is_lvalue_reference_v<Deleter>)
	: Deleter{std::move(deleter)}, _base{value}
{
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
template <tr::HandleDeleter<T> Deleter1>
constexpr tr::Handle<T, EmptyValue, Deleter>::Handle(Handle<T, EmptyValue, Deleter1>&& move) noexcept
	: _base{std::exchange(move._base, EmptyValue)}
{
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::~Handle<T, EmptyValue, Deleter>() noexcept
{
	if (_base != EmptyValue) {
		Deleter::operator()(_base);
	}
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>& tr::Handle<T, EmptyValue, Deleter>::operator=(Handle&& r) noexcept
{
	std::ignore = Handle{std::move(*this)};
	std::swap(_base, r._base);
	return *this;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr bool tr::Handle<T, EmptyValue, Deleter>::operator==(const Handle& r) const noexcept
{
	return this == &r;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr bool tr::Handle<T, EmptyValue, Deleter>::has_value() const noexcept
{
	return _base != EmptyValue;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr tr::Handle<T, EmptyValue, Deleter>::operator bool() const noexcept
{
	return has_value();
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr const T& tr::Handle<T, EmptyValue, Deleter>::get() const noexcept
{
	assert(_base != EmptyValue);
	return _base;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr const T& tr::Handle<T, EmptyValue, Deleter>::get(NoEmptyHandleCheck) const noexcept
{
	return _base;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr Deleter& tr::Handle<T, EmptyValue, Deleter>::get_deleter() noexcept
{
	return *this;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr Deleter& tr::Handle<T, EmptyValue, Deleter>::get_deleter() const noexcept
{
	return *this;
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr T tr::Handle<T, EmptyValue, Deleter>::release() noexcept
{
	return std::exchange(_base, EmptyValue);
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr void tr::Handle<T, EmptyValue, Deleter>::reset() noexcept
{
	std::ignore = std::exchange(*this, Handle{});
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr void tr::Handle<T, EmptyValue, Deleter>::reset(T value) noexcept
{
	std::ignore = std::exchange(*this, Handle{value});
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr void tr::Handle<T, EmptyValue, Deleter>::reset(T value, NoEmptyHandleCheck) noexcept
{
	std::ignore = std::exchange(*this, Handle{value, NO_EMPTY_HANDLE_CHECK});
}

template <tr::HandleType T, T EmptyValue, tr::HandleDeleter<T> Deleter>
constexpr void tr::Handle<T, EmptyValue, Deleter>::swap(Handle& other) noexcept
{
	std::swap(_base, other._base);
}

/// @endcond
