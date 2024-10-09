export module tr:color;

import std;
import :concepts;
import :integer;

export namespace tr {
	// Concept restricting valid color datatypes.
	template <class T>
	concept ColorDatatype = (std::integral<T> || FloatingPoint<T>) && sizeof(T) <= 4;

	// Inline namespace containing color-related structs.
	inline namespace color {
		template <ColorDatatype T> struct R {
			T r;
			constexpr bool operator==(const R& r) const noexcept = default;
		};
		template <ColorDatatype T> struct G {
			T g;
			constexpr bool operator==(const G& r) const noexcept = default;
		};
		template <ColorDatatype T> struct B {
			T b;
			constexpr bool operator==(const B& r) const noexcept = default;
		};
		template <ColorDatatype T> struct RG {
			T r, g;
			constexpr bool operator==(const RG& r) const noexcept = default;
		};
		template <ColorDatatype T> struct RGB {
			T r, g, b;
			constexpr bool operator==(const RGB& r) const noexcept = default;
		};
		template <ColorDatatype T> struct BGR {
			T b, g, r;
			constexpr bool operator==(const BGR& r) const noexcept = default;
		};
		template <ColorDatatype T> struct RGBA {
			T r, g, b, a;

			constexpr RGBA() noexcept = default;
			constexpr RGBA(T r, T g, T b, T a) noexcept
				: r { r }, g { g }, b { b }, a { a } {}
			constexpr RGBA(RGB<T> rgb, T a) noexcept
				: r { rgb.r }, g { rgb.g }, b { rgb.b }, a { a } {}

			constexpr bool operator==(const RGBA& r) const noexcept = default;
		};
		template <ColorDatatype T> struct BGRA {
			T b, g, r, a;
			constexpr bool operator==(const BGRA& r) const noexcept = default;
		};
		struct HSV { float h, s, v; };

		using RGB8 = RGB<Ui8>;
		using RGBA8 = RGBA<Ui8>;
		using RGBAF = RGBA<float>;

		struct RGB_Ui8_2_3_3 {
			Ui8 r : 2;
			Ui8 g : 3;
			Ui8 b : 3;
		};
		struct RGB_Ui16_5_6_5 {
			Ui16 r : 5;
			Ui16 g : 6;
			Ui16 b : 5;
		};
		struct BGR_Ui8_3_3_2 {
			Ui8 b : 3;
			Ui8 g : 3;
			Ui8 r : 2;
		};
		struct BGR_Ui16_5_6_5 {
			Ui16 b : 5;
			Ui16 g : 6;
			Ui16 r : 5;
		};
		struct RGBA_Ui16_4_4_4_4 {
			Ui16 r : 4;
			Ui16 g : 4;
			Ui16 b : 4;
			Ui16 a : 4;
		};
		struct RGBA_Ui16_5_5_5_1 {
			Ui16 r : 5;
			Ui16 g : 5;
			Ui16 b : 5;
			Ui16 a : 1;
		};
		struct RGBA_Ui32_10_10_10_2 {
			Ui32 r : 10;
			Ui32 g : 10;
			Ui32 b : 10;
			Ui32 a : 2;
		};
		struct ABGR_Ui16_4_4_4_4 {
			Ui16 a : 4;
			Ui16 b : 4;
			Ui16 g : 4;
			Ui16 r : 4;
		};
		struct ABGR_Ui16_1_5_5_5 {
			Ui16 a : 1;
			Ui16 b : 5;
			Ui16 g : 5;
			Ui16 r : 5;
		};
		struct ABGR_Ui32_2_10_10_10 {
			Ui32 a : 2;
			Ui32 b : 10;
			Ui32 g : 10;
			Ui32 r : 10;
		};
		struct BGRA_Ui16_4_4_4_4 {
			Ui16 b : 4;
			Ui16 g : 4;
			Ui16 r : 4;
			Ui16 a : 4;
		};
		struct BGRA_Ui16_5_5_5_1 {
			Ui16 b : 5;
			Ui16 g : 5;
			Ui16 r : 5;
			Ui16 a : 1;
		};
		struct BGRA_Ui32_10_10_10_2 {
			Ui32 b : 10;
			Ui32 g : 10;
			Ui32 r : 10;
			Ui32 a : 2;
		};
		struct ARGB_Ui16_4_4_4_4 {
			Ui16 a : 4;
			Ui16 r : 4;
			Ui16 g : 4;
			Ui16 b : 4;
		};
		struct ARGB_Ui16_1_5_5_5 {
			Ui16 a : 1;
			Ui16 r : 5;
			Ui16 g : 5;
			Ui16 b : 5;
		};
		struct ARGB_Ui32_2_10_10_10 {
			Ui32 a : 2;
			Ui32 r : 10;
			Ui32 g : 10;
			Ui32 b : 10;
		};
	}

	// Concept defining "built-in" colors.
	template <class T>
	concept BuiltinColor
		 = SpecializationOf<T, R>
		|| SpecializationOf<T, G>
		|| SpecializationOf<T, B>
		|| SpecializationOf<T, RG>
		|| SpecializationOf<T, RGB>
		|| SpecializationOf<T, BGR>
		|| SpecializationOf<T, RGBA>
		|| SpecializationOf<T, BGRA>
		|| std::same_as<T, RGB_Ui8_2_3_3>
		|| std::same_as<T, RGB_Ui16_5_6_5>
		|| std::same_as<T, BGR_Ui8_3_3_2>
		|| std::same_as<T, BGR_Ui16_5_6_5>
		|| std::same_as<T, RGBA_Ui16_4_4_4_4>
		|| std::same_as<T, RGBA_Ui16_5_5_5_1>
		|| std::same_as<T, RGBA_Ui32_10_10_10_2>
		|| std::same_as<T, ABGR_Ui16_4_4_4_4>
		|| std::same_as<T, ABGR_Ui16_1_5_5_5>
		|| std::same_as<T, ABGR_Ui32_2_10_10_10>
		|| std::same_as<T, BGRA_Ui16_4_4_4_4>
		|| std::same_as<T, BGRA_Ui16_5_5_5_1>
		|| std::same_as<T, BGRA_Ui32_10_10_10_2>
		|| std::same_as<T, ARGB_Ui16_4_4_4_4>
		|| std::same_as<T, ARGB_Ui16_1_5_5_5>
		|| std::same_as<T, ARGB_Ui32_2_10_10_10>;
}