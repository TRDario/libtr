/**
 * @file hashmap.cppm
 * @brief Provides extra hashmap functionality.
 */

export module tr:hashmap;

import std;
import boost;
import :concepts;

export namespace tr {
	/// @cond
	template <Enumerator T> struct EnumHash {
		auto operator()(T arg) const noexcept;
	};

	struct StringHash {
		using is_transparent = std::true_type;

		inline auto operator()(std::string_view str) const noexcept;
	};

	struct StaticStringHash {
		using is_transparent = std::true_type;

        template <std::size_t Cap> auto operator()(const boost::static_string<Cap>& str) const noexcept;
		inline auto operator()(std::string_view str) const noexcept;
	};

	struct StringEquals {
		using is_transparent = std::true_type;

		constexpr bool operator()(std::string_view l, std::string_view r) const noexcept;
	};
	/// @endcond


	/******************************************************************************************************************
	 * Typedef for a enumerator-key hash map.
	 ******************************************************************************************************************/
	template <Enumerator Key, class Value>
	using EnumHashMap = std::unordered_map<Key, Value, EnumHash<Key>>;

	/******************************************************************************************************************
	 * Typedef for a string-key hash map.
	 ******************************************************************************************************************/
	template <class Value>
	using StringHashMap = std::unordered_map<std::string, Value, StringHash, StringEquals>;

	/******************************************************************************************************************
	 * Typedef for a static_string-key hash map.
	 ******************************************************************************************************************/
	template <std::size_t Cap, class Value>
	using StaticStringHashMap = std::unordered_map<boost::static_string<Cap>, Value, StaticStringHash, StringEquals>;
}

/// @cond IMPLEMENTATION

template <tr::Enumerator T>
auto tr::EnumHash<T>::operator()(T arg) const noexcept
{
	return std::hash<std::underlying_type_t<T>>{}(std::to_underlying(arg));
}

auto tr::StringHash::operator()(std::string_view str) const noexcept
{
	return std::hash<std::string_view>{}(str);
}

template <std::size_t Cap>
auto tr::StaticStringHash::operator()(const boost::static_string<Cap>& arg) const noexcept
{
	return std::hash<std::string_view>{}(std::string_view(arg));
}

auto tr::StaticStringHash::operator()(std::string_view str) const noexcept
{
	return std::hash<std::string_view>{}(str);
}

constexpr bool tr::StringEquals::operator()(std::string_view l, std::string_view r) const noexcept
{
	return l == r;
}

/// @endcond