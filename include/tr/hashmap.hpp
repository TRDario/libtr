#pragma once
#include "concepts.hpp"

namespace tr {
	/// @cond IMPLEMENTATION
	template <Enumerator T> struct EnumHash {
		auto operator()(T arg) const noexcept;
	};

	struct StringHash {
		using is_transparent = std::true_type;

		inline auto operator()(std::string_view str) const noexcept;
	};

	struct StringEquals {
		using is_transparent = std::true_type;

		constexpr bool operator()(std::string_view l, std::string_view r) const noexcept;
	};
	/// @endcond

	/** @ingroup misc
	 *  @defgroup hash_map Hash Maps
	 *  Custom unordered_map functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Typedef for a enumerator-key hash map.
	 ******************************************************************************************************************/
	template <Enumerator Key, class Value> using EnumHashMap = std::unordered_map<Key, Value, EnumHash<Key>>;

	/******************************************************************************************************************
	 * Typedef for a string-key hash map.
	 ******************************************************************************************************************/
	template <class Value> using StringHashMap = std::unordered_map<std::string, Value, StringHash, StringEquals>;

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::Enumerator T> auto tr::EnumHash<T>::operator()(T arg) const noexcept
{
	return std::hash<std::underlying_type_t<T>>{}(std::underlying_type_t<T>(arg));
}

auto tr::StringHash::operator()(std::string_view str) const noexcept
{
	return std::hash<std::string_view>{}(str);
}

constexpr bool tr::StringEquals::operator()(std::string_view l, std::string_view r) const noexcept
{
	return l == r;
}

/// @endcond
