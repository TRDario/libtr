#pragma once

namespace tr {
	/** @addtogroup bitmap
	 *  @{
	 */

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
			INDEX_1LSB   = 286261504,
			INDEX_1MSB   = 287310080,
			INDEX_4LSB   = 303039488,
			INDEX_4MSB   = 304088064,
			INDEX_8      = 318769153,
			RGB_332      = 336660481,
			XRGB_4444    = 353504258,
			XRGB_1555    = 353570562,
			XBGR_1555    = 357764866,
			ARGB_4444    = 355602434,
			RGBA_4444    = 356651010,
			ABGR_4444    = 359796738,
			BGRA_4444    = 360845314,
			ARGB_1555    = 355667970,
			RGBA_5551    = 356782082,
			ABGR_1555    = 359862274,
			BGRA_5551    = 360976386,
			RGB_565      = 353701890,
			BGR_565      = 357896194,
			RGB_24       = 386930691,
			BGR_24       = 390076419,
			XRGB_8888    = 370546692,
			RGBX_8888    = 371595268,
			XBGR_8888    = 374740996,
			BGRX_8888    = 375789572,
			ARGB_8888    = 372645892,
			RGBA_8888    = 373694468,
			ABGR_8888    = 376840196,
			BGRA_8888    = 377888772,
			ARGB_2101010 = 372711428
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
		 * @param[in] base The base enumerator.
		 **************************************************************************************************************/
		BitmapFormat(Type base) noexcept;

		/// @private
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

	/// @}
} // namespace tr
