export module tr:color_cast;

import std;
import :color;
import :concepts;
import :norm_cast;

namespace tr {
	consteval std::size_t umax(std::uint8_t bits) noexcept
	{
		return (std::size_t { 1 } << bits) - 1;
	}

	export template <BuiltinColor To, SpecializationOf<R> From>
	constexpr To color_cast(const From& from) noexcept
	{
		if constexpr (SpecializationOf<To, G> || SpecializationOf<To, B>) {
			return { 0 };
		}
		else {
			using T = decltype(To::r);
			constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

			if constexpr (SpecializationOf<To, R>) {
				return { norm_cast<T>(from.r) };
			}
			else if constexpr (SpecializationOf<To, RG>) {
				return { norm_cast<T>(from.r), 0 };
			}
			else if constexpr (SpecializationOf<To, RGB>) {
				return { norm_cast<T>(from.r), 0, 0 };
			}
			else if constexpr (SpecializationOf<To, BGR>) {
				return { 0, 0, norm_cast<T>(from.r) };
			}
			else if constexpr (SpecializationOf<To, RGBA>) {
				return { norm_cast<T>(from.r), 0, 0, norm_cast<T>(1.0) };
			}
			else if constexpr (SpecializationOf<To, BGRA>) {
				return { 0, 0, norm_cast<T>(from.r), norm_cast<T>(1.0) };
			}
			else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
				return { std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
				return { std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
				return { 0, 0, std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
				return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
				return { std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), 0, 0, std::uint16_t(umax(4)) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
				return { std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0, std::uint16_t(umax(1)) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
				return { std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), 0, 0, std::uint32_t(umax(2)) };
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
				return { std::uint16_t(umax(4)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
				return { std::uint16_t(umax(1)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
				return { std::uint32_t(umax(2)), 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
				return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), std::uint16_t(umax(4)) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
				return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), std::uint16_t(umax(1)) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
				return { 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), std::uint32_t(umax(2)) };
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
				return { std::uint16_t(umax(4)), std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
				return { std::uint16_t(umax(1)), std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
				return { std::uint32_t(umax(2)), std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), 0, 0 };
			}
		}
	}

	export template <BuiltinColor To, SpecializationOf<G> From>
	constexpr To color_cast(const From& from) noexcept
	{
		if constexpr (SpecializationOf<To, R> || SpecializationOf<To, B>) {
			return { 0 };
		}
		else {
			using T = decltype(To::g);
			constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

			if constexpr (SpecializationOf<To, G>) {
				return { norm_cast<T>(from.g) };
			}
			else if constexpr (SpecializationOf<To, RG>) {
				return { 0, norm_cast<T>(from.g) };
			}
			else if constexpr (SpecializationOf<To, RGB> || SpecializationOf<To, RGB>) {
				return { 0, norm_cast<T>(from.g), 0 };
			}
			else if constexpr (SpecializationOf<To, RGBA>) {
				return { 0, norm_cast<T>(from.g), 0, norm_cast<T>(1.0) };
			}
			else if constexpr (SpecializationOf<To, BGRA>) {
				return { 0, norm_cast<T>(from.g), 0, norm_cast<T>(1.0) };
			}
			else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
				return { 0, std::uint8_t(color_cast<G<std::uint8_t>>(from).g * umax(3) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
				return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(6) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
				return { 0, std::uint8_t(color_cast<G<std::uint8_t>>(from).g * umax(3) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
				return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(6) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
				return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0, std::uint16_t(umax(4)) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
				return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0, std::uint16_t(umax(1)) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
				return { 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0, std::uint32_t(umax(2)) };
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
				return { std::uint16_t(umax(4)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
				return { std::uint16_t(umax(1)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
				return { std::uint32_t(umax(2)), 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
				return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0, std::uint16_t(umax(4)) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
				return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0, std::uint16_t(umax(1)) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
				return { 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0, std::uint32_t(umax(2)) };
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
				return { std::uint16_t(umax(4)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
				return { std::uint16_t(umax(1)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0 };
			}
			else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
				return { std::uint32_t(umax(2)), 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0 };
			}
		}
	}

	export template <BuiltinColor To, SpecializationOf<B> From>
	constexpr To color_cast(const From& from) noexcept
	{
		if constexpr (SpecializationOf<To, R> || SpecializationOf<To, G> || SpecializationOf<To, RG>) {
			return { 0 };
		}
		else {
			using T = decltype(To::b);
			constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

			if constexpr (SpecializationOf<To, B>) {
				return { norm_cast<T>(from.b) };
			}
			else if constexpr (SpecializationOf<To, RGB>) {
				return { 0, 0, norm_cast<T>(from.b) };
			}
			else if constexpr (SpecializationOf<To, BGR>) {
				return { norm_cast<T>(from.b), 0, 0 };
			}
			else if constexpr (SpecializationOf<To, RGBA>) {
				return { 0, 0, norm_cast<T>(from.b), norm_cast<T>(1.0) };
			}
			else if constexpr (SpecializationOf<To, BGRA>) {
				return { norm_cast<T>(from.b), 0, 0, norm_cast<T>(1.0) };
			}
			else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
				return { 0, 0, std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
				return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
				return { std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
				return { std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
				return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), std::uint16_t(umax(4)) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
				return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), std::uint16_t(umax(1)) };
			}
			else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
				return { 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), std::uint32_t(umax(2)) };
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
				return { std::uint16_t(umax(4)), std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
				return { std::uint16_t(umax(1)), std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
				return { std::uint32_t(umax(2)), std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), 0, 0 };
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
				return { std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), 0, 0, std::uint16_t(umax(4)) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
				return { std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0, std::uint16_t(umax(1)) };
			}
			else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
				return { std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), 0, 0, std::uint32_t(umax(2)) };
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
				return { std::uint16_t(umax(4)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
				return { std::uint16_t(umax(1)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX) };
			}
			else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
				return { std::uint32_t(umax(2)), 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX) };
			}
		}
	}

	export template <BuiltinColor To, SpecializationOf<RG> From>
	constexpr To color_cast(const From& from) noexcept
	{
		if constexpr (SpecializationOf<To, B>) {
			return { 0 };
		}
		else {
			using T = decltype(To::r);
			constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

			if constexpr (SpecializationOf<To, R>) {
				return { norm_cast<T>(from.r) };
			}
			else if constexpr (SpecializationOf<To, RG>) {
				return { norm_cast<T>(from.r), norm_cast<T>(from.g) };
			}
			else if constexpr (SpecializationOf<To, RGB>) {
				return { norm_cast<T>(from.r), norm_cast<T>(from.g), 0 };
			}
			else if constexpr (SpecializationOf<To, BGR>) {
				return { 0, norm_cast<T>(from.g), norm_cast<T>(from.r) };
			}
			else if constexpr (SpecializationOf<To, RGBA>) {
				return { norm_cast<T>(from.r), norm_cast<T>(from.g), 0, norm_cast<T>(1.0) };
			}
			else if constexpr (SpecializationOf<To, BGRA>) {
				return { 0, norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(1.0) };
			}
			else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
				return {
					std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
					std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
					0
				};
			}
			else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
				return {
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
					0
				};
			}
			else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
				return {
					0,
					std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
					std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)
				};
			}
			else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
				return {
					0,
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
				};
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
				return {
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
					0,
					std::uint16_t(umax(4))
				};
			}
			else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
				return {
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
					0,
					std::uint16_t(umax(1))
				};
			}
			else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
				return {
					std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
					0,
					std::uint32_t(umax(2))
				};
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
				return {
					std::uint16_t(umax(4)),
					0,
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)
				};
			}
			else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
				return {
					std::uint16_t(umax(1)),
					0,
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
				};
			}
			else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
				return {
					std::uint32_t(umax(2)),
					0,
					std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
					std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)
				};
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
				return {
					0,
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					std::uint16_t(umax(4))
				};
			}
			else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
				return {
					0,
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					std::uint16_t(umax(1))
				};
			}
			else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
				return {
					0,
					std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
					std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					std::uint32_t(umax(2))
				};
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
				return {
					std::uint16_t(umax(4)),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
					0
				};
			}
			else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
				return {
					std::uint16_t(umax(1)),
					std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
					std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
					0
				};
			}
			else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
				return {
					std::uint32_t(umax(2)),
					std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
					std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
					0
				};
			}
		}
	}

	export template <BuiltinColor To, class From> requires SpecializationOf<From, RGB> || SpecializationOf<From, BGR>
	constexpr To color_cast(const From& from) noexcept
	{
		using T = decltype(To::r);
		constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

		if constexpr (SpecializationOf<To, R>) {
			return { norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RG>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g) };
		}
		else if constexpr (SpecializationOf<To, RGB>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b) };
		}
		else if constexpr (SpecializationOf<To, BGR>) {
			return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RGBA>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b), norm_cast<T>(1.0) };
		}
		else if constexpr (SpecializationOf<To, BGRA>) {
			return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(1.0) };
		}
		else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
			return {
				std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
			return {
				std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				std::uint16_t(umax(4))
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(umax(1))
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				std::uint32_t(umax(2))
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(umax(4)),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
			return {
				std::uint16_t(umax(1)),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
			return {
				std::uint32_t(umax(2)),
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(umax(4))
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(umax(1))
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(umax(2))
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(umax(4)),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
			return {
				std::uint16_t(umax(1)),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
			return {
				std::uint32_t(umax(2)),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)
			};
		}
	}

	export template <BuiltinColor To, class From> requires SpecializationOf<From, RGBA> || SpecializationOf<From, BGRA>
	constexpr To color_cast(const From& from) noexcept
	{
		using T = decltype(To::r);
		constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

		if constexpr (SpecializationOf<To, R>) {
			return { norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RG>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g) };
		}
		else if constexpr (SpecializationOf<To, RGB>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b) };
		}
		else if constexpr (SpecializationOf<To, BGR>) {
			return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RGBA>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b), norm_cast<T>(from.a) };
		}
		else if constexpr (SpecializationOf<To, BGRA>) {
			return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(from.a) };
		}
		else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
			return {
				std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
			return {
				std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint16_t>(from.a) * umax(1) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(1) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint16_t>(from.a) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint16_t>(from.a) * umax(1) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(1) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)
			};
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const RGB_Ui8_2_3_3& from) noexcept
	{
		if constexpr (std::same_as<To, RGB8>) {
			return { std::uint8_t(from.r * 255 / umax(2)), std::uint8_t(from.g * 255 / umax(3)), std::uint8_t(from.b * 255 / umax(3)), };
		}
		else {
			return color_cast<To>(color_cast<RGB8>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const RGB_Ui16_5_6_5& from) noexcept
	{
		if constexpr (std::same_as<To, RGB8>) {
			return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(6)), std::uint8_t(from.b * 255 / umax(5)), };
		}
		else {
			return color_cast<To>(color_cast<RGB8>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const BGR_Ui8_3_3_2& from) noexcept
	{
		if constexpr (std::same_as<To, RGB8>) {
			return { std::uint8_t(from.r * 255 / umax(2)), std::uint8_t(from.g * 255 / umax(3)), std::uint8_t(from.b * 255 / umax(3)), };
		}
		else {
			return color_cast<To>(color_cast<RGB8>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const BGR_Ui16_5_6_5& from) noexcept
	{
		if constexpr (std::same_as<To, RGB8>) {
			return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(6)), std::uint8_t(from.b * 255 / umax(5)), };
		}
		else {
			return color_cast<To>(color_cast<RGB8>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const RGBA_Ui16_4_4_4_4& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const RGBA_Ui16_5_5_5_1& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const RGBA_Ui32_10_10_10_2& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
			return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const ABGR_Ui16_4_4_4_4& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const ABGR_Ui16_1_5_5_5& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const ABGR_Ui32_2_10_10_10& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
			return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const BGRA_Ui16_4_4_4_4& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const BGRA_Ui16_5_5_5_1& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const BGRA_Ui32_10_10_10_2& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
			return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const ARGB_Ui16_4_4_4_4& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const ARGB_Ui16_1_5_5_5& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
			return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
		}
	}

	export template <BuiltinColor To> constexpr To color_cast(const ARGB_Ui32_2_10_10_10& from) noexcept
	{
		if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
			return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
		}
		else {
			return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
		}
	}

	export template <class T> struct ColorCaster {
		static constexpr bool IMPL_NOT_SPECIALIZED { true };

		static constexpr RGBAF toIntermediate(const T& from) noexcept = delete;
		static constexpr T fromIntermediate(const RGBAF& from) noexcept = delete;
	};

	// Concept defining a custom color.
	export template <class T> concept CustomColor = !requires { ColorCaster<T>::IMPL_NOT_SPECIALIZED; };

	export template <BuiltinColor To, CustomColor From> constexpr To color_cast(const From& from) noexcept
	{
		return color_cast<To>(ColorCaster<From>::toIntermediate(from));
	}

	export template <CustomColor To, BuiltinColor From> constexpr To color_cast(const From& from) noexcept
	{
		return ColorCaster<To>::fromIntermediate(color_cast<RGBAF>(from));
	}

	export template <CustomColor To, CustomColor From> constexpr To color_cast(const From& from) noexcept
	{
		return ColorCaster<To>::fromIntermediate(ColorCaster<From>::toIntermediate(from));
	}

	export template <> struct ColorCaster<HSV> {
		static constexpr RGBAF toIntermediate(const HSV& from) noexcept
		{
			constexpr auto constexprAbs = [] (float arg) {
				return arg >= 0 ? arg : -arg;
			};
			constexpr auto eucMod = [] (float arg, float modulo) {
				const float mod { arg - modulo * std::int64_t(arg / modulo) };
				return mod >= 0 ? mod : mod + modulo;
			};

			const float h { eucMod(from.h, 360.0f) };
			const float c { from.v * from.s };
			const float m { from.v - c };
			const float x { c * (1 - constexprAbs(eucMod((h / 60.0f), 2.0f) - 1)) + m };

			if (h < 60) {
				return { from.v, x, m, 1.0f };
			}
			else if (h < 120) {
				return { x, from.v, m, 1.0f };
			}
			else if (h < 180) {
				return { m, from.v, x, 1.0f };
			}
			else if (h < 240) {
				return { m, x, from.v, 1.0f };
			}
			else if (h < 300) {
				return { x, m, from.v, 1.0f };
			}
			else {
				return { from.v, m, x, 1.0f };
			}
		}

		static constexpr HSV fromIntermediate(const RGBAF& from) noexcept
		{
			constexpr auto eucMod = [] (float arg, float modulo) {
				const float mod { arg - modulo * std::int64_t(arg / modulo) };
				return mod >= 0 ? mod : mod + modulo;
			};

			HSV hsv {};
			hsv.v = std::max({ from.r, from.g, from.b });
			const float delta { hsv.v - std::min({ from.r, from.g, from.b }) };

			if (delta == 0) {
				hsv.h = 0;
			}
			else if (hsv.v == from.r) {
				hsv.h = 60.0f * eucMod((from.g - from.b) / delta, 6.0f);
			}
			else if (hsv.v == from.g) {
				hsv.h = 60.0f * ((from.b - from.r) / delta + 2.0f);
			}
			else {
				hsv.h = 60.0f * ((from.r - from.g) / delta + 4.0f);
			}

			hsv.s = hsv.v != 0 ? delta / hsv.v : 0;

			return hsv;
		}
	};
}