/**
 * @file bitmap_format.hpp
 * @brief Provides a bitmap pixel format type.
 */

#pragma once

namespace tr {
	/******************************************************************************************************************
	 * Bitmap format information.
	 ******************************************************************************************************************/
	class BitmapFormat {
	  public:
		/**************************************************************************************************************
		 * The base enumerator.
		 *
		 * Its members are included in the parent class, so it's mostly an implementation detail.
		 **************************************************************************************************************/
		enum class Type {
			UNKNOWN,
			RGBA32,
			ARGB32,
			BGRA32,
			ABGR32,
			INDEX_1LSB   = 286'261'504,
			INDEX_1MSB   = 287'310'080,
			INDEX_4LSB   = 303'039'488,
			INDEX_4MSB   = 304'088'064,
			INDEX_8      = 318'769'153,
			RGB_332      = 336'660'481,
			XRGB_4444    = 353'504'258,
			XRGB_1555    = 353'570'562,
			XBGR_1555    = 357'764'866,
			ARGB_4444    = 355'602'434,
			RGBA_4444    = 356'651'010,
			ABGR_4444    = 359'796'738,
			BGRA_4444    = 360'845'314,
			ARGB_1555    = 355'667'970,
			RGBA_5551    = 356'782'082,
			ABGR_1555    = 359'862'274,
			BGRA_5551    = 360'976'386,
			RGB_565      = 353'701'890,
			BGR_565      = 357'896'194,
			RGB_24       = 386'930'691,
			BGR_24       = 390'076'419,
			XRGB_8888    = 370'546'692,
			RGBX_8888    = 371'595'268,
			XBGR_8888    = 374'740'996,
			BGRX_8888    = 375'789'572,
			ARGB_8888    = 372'645'892,
			RGBA_8888    = 373'694'468,
			ABGR_8888    = 376'840'196,
			BGRA_8888    = 377'888'772,
			ARGB_2101010 = 372'711'428
		};
		using enum Type;

		/**************************************************************************************************************
		 * Orders of elements in a bitmap format.
		 **************************************************************************************************************/
		enum class Order {
			BITMAPPED_NONE = 0,
			BITMAPPED_4321,
			BITMAPPED_1234,

			PACKED_NONE,
			PACKED_XRGB,
			PACKED_RGBX,
			PACKED_ARGB,
			PACKED_RGBA,
			PACKED_XBGR,
			PACKED_BGRX,
			PACKED_ABGR,
			PACKED_BGRA,

			ARRAY_NONE,
			ARRAY_RGB,
			ARRAY_RGBA,
			ARRAY_ARGB,
			ARRAY_BGR,
			ARRAY_BGRA,
			ARRAY_ABGR
		};

		/**************************************************************************************************************
		 * Packed bitmap format channel layouts.
		 **************************************************************************************************************/
		enum class Layout {
			NONE,
			I8_332,
			I16_4444,
			I16_1555,
			I16_5551,
			I16_565,
			I32_8888,
			I32_2101010,
			I32_1010102
		};

		/**************************************************************************************************************
		 * Constructs a bitmap format from the base enumerator.
		 *
		 * @param base The base enumerator.
		 **************************************************************************************************************/
		BitmapFormat(Type base) noexcept;

		/**************************************************************************************************************
		 * Casts the format to the base enumerator.
		 **************************************************************************************************************/
		operator Type() const noexcept;

		/**************************************************************************************************************
		 * Gets the printable name of the bitmap format.
		 *
		 * @return A C-string containing the printable name of the bitmap format.
		 **************************************************************************************************************/
		const char* name() const noexcept;

		/**************************************************************************************************************
		 * Gets the order of the bitmap format.
		 *
		 * @return The order type of the bitmap format.
		 **************************************************************************************************************/
		Order order() const noexcept;

		/**************************************************************************************************************
		 * Gets the layout of the bitmap format.
		 *
		 * @return The layout type of the bitmap format.
		 **************************************************************************************************************/
		Layout layout() const noexcept;

		/**************************************************************************************************************
		 * Gets the number of bits in a pixel.
		 *
		 * @return The number of bits in a pixel.
		 **************************************************************************************************************/
		int pixelBits() const noexcept;

		/**************************************************************************************************************
		 * Gets the number of bytes in a pixel.
		 *
		 * @return The number of bytes in a pixel.
		 **************************************************************************************************************/
		int pixelBytes() const noexcept;

		/**************************************************************************************************************
		 * Gets whether the format is indexed.
		 *
		 * @return True if the format is indexed and false otherwise.
		 **************************************************************************************************************/
		bool isIndexed() const noexcept;

		/**************************************************************************************************************
		 * Gets whether the format is packed.
		 *
		 * @return True if the format is packed and false otherwise.
		 **************************************************************************************************************/
		bool isPacked() const noexcept;

		/**************************************************************************************************************
		 * Gets whether the format is an array format.
		 *
		 * @return True if the format is an array format and false otherwise.
		 **************************************************************************************************************/
		bool isArray() const noexcept;

		/**************************************************************************************************************
		 * Gets whether the format has alpha.
		 *
		 * @return True if the format has alpha and false otherwise.
		 **************************************************************************************************************/
		bool hasAlpha() const noexcept;

	  private:
		Type _type;
	};
} // namespace tr
