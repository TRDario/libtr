export module tr:hashmap;

import std;
import boost;
import :concepts;
import :integer;

export namespace tr {
	template <Enumerator T> struct EnumHash {
		auto operator()(T arg) const noexcept;
	};

	struct StringHash {
		using is_transparent = std::true_type;
		inline auto operator()(std::string_view str) const noexcept;
	};

	struct StaticStringHash {
		using is_transparent = std::true_type;
        template <Size Cap> auto operator()(const boost::static_string<Cap>& str) const noexcept;
		inline 				auto operator()(std::string_view str) 			      const noexcept;
	};

	struct StringEquals {
		using is_transparent = std::true_type;
		constexpr bool operator()(std::string_view l, std::string_view r) const noexcept;
	};

	template <Enumerator Key, class Value>
	using EnumHashMap = std::unordered_map<Key, Value, EnumHash<Key>>;

	template <class Value>
	using StringHashMap = std::unordered_map<std::string, Value, StringHash, StringEquals>;

	template <Size Cap, class Value>
	using StaticStringHashMap = std::unordered_map<boost::static_string<Cap>, Value, StaticStringHash, StringEquals>;
}

// TEMPLATE IMPLEMENTATION
namespace tr {
	template <Enumerator T>
	auto EnumHash<T>::operator()(T arg) const noexcept
	{
		return std::hash<std::underlying_type_t<T>>{}(std::to_underlying(arg));
	}

	auto StringHash::operator()(std::string_view str) const noexcept
	{
		return std::hash<std::string_view>{}(str);
	}

	template <Size Cap>
	auto StaticStringHash::operator()(const boost::static_string<Cap>& arg) const noexcept
	{
		return std::hash<std::string_view>{}(std::string_view(arg));
	}

	auto StaticStringHash::operator()(std::string_view str) const noexcept
	{
		return std::hash<std::string_view>{}(str);
	}

	constexpr bool StringEquals::operator()(std::string_view l, std::string_view r) const noexcept
	{
		return l == r;
	}
}