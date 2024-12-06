#pragma once
#include <cassert>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <string_view>

namespace tr {
	/******************************************************************************************************************
	 * Unicode codepoint const iterator for UTF-8 string views.
	 *
	 * The iterator fulfills the @em ForwardIterator requirements.
	 ******************************************************************************************************************/
	class Utf8ConstIt {
	  public:
		/**************************************************************************************************************
		 * @em ForwardIterator typedef requirement.
		 **************************************************************************************************************/
		using value_type = std::uint32_t;

		/**************************************************************************************************************
		 * @em ForwardIterator typedef requirement.
		 **************************************************************************************************************/
		using difference_type = std::ptrdiff_t;

		/**************************************************************************************************************
		 * Default-constructs an iterator.
		 *
		 * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to
		 * it.
		 **************************************************************************************************************/
		constexpr Utf8ConstIt() noexcept = default;

		/**************************************************************************************************************
		 * Constructs an iterator pointing to a UTF-8 sequence in memory.
		 *
		 * @param[in] ptr A pointer to a valid UTF-8 char sequence.
		 **************************************************************************************************************/
		constexpr Utf8ConstIt(const char* ptr) noexcept;

		/**************************************************************************************************************
		 * Compares two iterators.
		 **************************************************************************************************************/
		constexpr friend auto operator<=>(const Utf8ConstIt&, const Utf8ConstIt&) = default;

		/**************************************************************************************************************
		 * Compares two iterators for equality.
		 **************************************************************************************************************/
		constexpr friend bool operator==(const Utf8ConstIt&, const Utf8ConstIt&) = default;

		/**************************************************************************************************************
		 * Converts the iterator to a const char pointer to the beginning of its UTF-8 character.
		 **************************************************************************************************************/
		explicit constexpr operator const char*() const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator.
		 *
		 * The iterator must be in a dereferencable state (i.e., not in a default-constructed state or an iterator
		 * created by utf8End()) for a call to this function to be valid.
		 *
		 * @return A Unicode codepoint.
		 **************************************************************************************************************/
		constexpr std::uint32_t operator*() const noexcept;

		/**************************************************************************************************************
		 * Pre-increments the iterator.
		 *
		 * The iterator must be in a dereferencable state (i.e., not in a default-constructed state or an iterator
		 * created by utf8End()) for a call to this function to be valid.
		 *
		 * @return A reference to the incremented iterator.
		 **************************************************************************************************************/
		constexpr Utf8ConstIt& operator++() noexcept;

		/**************************************************************************************************************
		 * Post-increments the iterator.
		 *
		 * The iterator must be in a dereferencable state (i.e., not in a default-constructed state or an iterator
		 * created by utf8End()) for a call to this function to be valid.
		 *
		 * @return An iterator with the prior state of the incremented iterator.
		 **************************************************************************************************************/
		constexpr Utf8ConstIt operator++(int) noexcept;

	  private:
		const char* _impl; // Pointer to a valid UTF-8 char sequence.
	};

	/******************************************************************************************************************
	 * Creates a beginning codepoint iterator for a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return An iterator to the first Unicode character of the string if @em str is non-empty,
	 * or utf8End() if @em str is empty.
	 ******************************************************************************************************************/
	constexpr Utf8ConstIt utf8Begin(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Creates an ending codepoint iterator for a UTF-8 UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return A sentinel iterator marking the end of the string,
	 ******************************************************************************************************************/
	constexpr Utf8ConstIt utf8End(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Creates a codepoint iterator range pair for a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return A range as if produced by @code { utf8Begin(str), utf8End(str) } @endcode
	 ******************************************************************************************************************/
	constexpr std::ranges::subrange<Utf8ConstIt> utf8Range(std::string_view str) noexcept;

	/******************************************************************************************************************
	 * Counts the number of unicode characters in a UTF-8 string view.
	 *
	 * @param[in] str A string view that either contains valid UTF-8 data or is empty.
	 *
	 * @return The length as if produced by @code std::distance(utf8Begin(str), utf8End(str)) @endcode
	 ******************************************************************************************************************/
	constexpr std::size_t utf8Length(std::string_view str) noexcept;
} // namespace tr

/// @cond IMPLEMENTATION

constexpr tr::Utf8ConstIt::Utf8ConstIt(const char* ptr) noexcept
	: _impl{ptr}
{
	assert(_impl != nullptr);
}

constexpr tr::Utf8ConstIt::operator const char*() const noexcept
{
	return _impl;
}

constexpr std::uint32_t tr::Utf8ConstIt::operator*() const noexcept
{
	assert(_impl != nullptr);

	if (std::uint8_t(*_impl) < 0x80) {
		return *_impl;
	}
	else if (std::uint8_t(*_impl) < 0xE0) {
		return ((_impl[0] & 0x1F) << 6) + (_impl[1] & 0x3F);
	}
	else if (std::uint8_t(*_impl) < 0xF0) {
		return ((_impl[0] & 0xF) << 12) + ((_impl[1] & 0x3F) << 6) + (_impl[2] & 0x3F);
	}
	else {
		return ((_impl[0] & 0x7) << 18) + ((_impl[1] & 0x3F) << 12) + ((_impl[2] & 0x3F) << 6) + +(_impl[3] & 0x3F);
	}
}

constexpr tr::Utf8ConstIt& tr::Utf8ConstIt::operator++() noexcept
{
	assert(_impl != nullptr);

	if (std::uint8_t(*_impl) < 0x80) {
		++_impl;
	}
	else if (std::uint8_t(*_impl) < 0xE0) {
		_impl += 2;
	}
	else if (std::uint8_t(*_impl) < 0xF0) {
		_impl += 3;
	}
	else {
		_impl += 4;
	}
	return *this;
}

constexpr tr::Utf8ConstIt tr::Utf8ConstIt::operator++(int) noexcept
{
	auto prev{*this};
	++(*this);
	return prev;
}

constexpr tr::Utf8ConstIt tr::utf8Begin(std::string_view str) noexcept
{
	if (str.empty()) {
		return {};
	}
	return str.data();
}

constexpr tr::Utf8ConstIt tr::utf8End(std::string_view str) noexcept
{
	if (str.empty()) {
		return {};
	}
	return str.data() + str.size();
}

constexpr std::ranges::subrange<tr::Utf8ConstIt> tr::utf8Range(std::string_view str) noexcept
{
	return {utf8Begin(str), utf8End(str)};
}

constexpr std::size_t tr::utf8Length(std::string_view str) noexcept
{
	return std::distance(utf8Begin(str), utf8End(str));
}

/// @endcond
