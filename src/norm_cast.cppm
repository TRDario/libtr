export module tr:norm_cast;

import std;
import :concepts;
import :integer;

export namespace tr {
	template <FloatingPoint To, FloatingPoint From> constexpr To norm_cast(From from) noexcept;
	template <FloatingPoint To, std::integral From> constexpr To norm_cast(From from) noexcept;
	template <std::integral To, FloatingPoint From> constexpr To norm_cast(From from) noexcept;
	template <std::integral To, std::integral From> constexpr To norm_cast(From from) noexcept;
}

// IMPLEMENTATION

template <tr::FloatingPoint To, tr::FloatingPoint From>
constexpr To tr::norm_cast(From from) noexcept
{
	return To(from);
}

template <tr::FloatingPoint To, std::integral From>
constexpr To tr::norm_cast(From from) noexcept
{
	return To(double(from) / std::numeric_limits<From>::max());
}

template <std::integral To, tr::FloatingPoint From>
constexpr To tr::norm_cast(From from) noexcept
{
	return To(double(from) * std::numeric_limits<To>::max());
}

template <std::integral To, std::integral From>
constexpr To tr::norm_cast(From from) noexcept
{   
	if constexpr (sizeof(From) == 8 || sizeof(To) == 8) {
		return To(double(from) / std::numeric_limits<From>::max() * std::numeric_limits<To>::max());
	}
	else if constexpr (std::signed_integral<To> || std::signed_integral<From>) {
		return To(Si64(from) * std::numeric_limits<To>::max() / std::numeric_limits<From>::max());
	}
	else {
		return To(Ui64(from) * std::numeric_limits<To>::max() / std::numeric_limits<From>::max());
	}
}