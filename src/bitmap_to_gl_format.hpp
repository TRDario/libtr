#pragma once
#include "../include/tr/bitmap_format.hpp"
#include "../include/tr/common.hpp"
#include "gl_call.hpp"

namespace tr {
	inline std::pair<GLenum, GLenum> bitmapToGLFormat(BitmapFormat format) noexcept;
}

inline std::pair<GLenum, GLenum> tr::bitmapToGLFormat(BitmapFormat format) noexcept
{
	switch (static_cast<BitmapFormat::Type>(format)) {
	case BitmapFormat::INDEX_8:
		return {GL_RED, GL_UNSIGNED_BYTE};
	case BitmapFormat::RGB_332:
		return {GL_RGB, GL_UNSIGNED_BYTE_3_3_2};
	case BitmapFormat::XRGB_4444:
		return {GL_BGR, GL_UNSIGNED_SHORT_4_4_4_4_REV};
	case BitmapFormat::XRGB_1555:
		return {GL_BGR, GL_UNSIGNED_SHORT_1_5_5_5_REV};
	case BitmapFormat::XBGR_1555:
		return {GL_RGB, GL_UNSIGNED_SHORT_1_5_5_5_REV};
	case BitmapFormat::ARGB_4444:
		return {GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV};
	case BitmapFormat::RGBA_4444:
		return {GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4};
	case BitmapFormat::ABGR_4444:
		return {GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4_REV};
	case BitmapFormat::BGRA_4444:
		return {GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4};
	case BitmapFormat::ARGB_1555:
		return {GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV};
	case BitmapFormat::RGBA_5551:
		return {GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1};
	case BitmapFormat::ABGR_1555:
		return {GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV};
	case BitmapFormat::BGRA_5551:
		return {GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1};
	case BitmapFormat::RGB_565:
		return {GL_RGB, GL_UNSIGNED_SHORT_5_6_5};
	case BitmapFormat::BGR_565:
		return {GL_BGR, GL_UNSIGNED_SHORT_5_6_5};
	case BitmapFormat::RGB_24:
		return {GL_RGB, GL_UNSIGNED_BYTE};
	case BitmapFormat::BGR_24:
		return {GL_BGR, GL_UNSIGNED_BYTE};
	case BitmapFormat::XRGB_8888:
		return {GL_BGR, GL_UNSIGNED_INT_8_8_8_8_REV};
	case BitmapFormat::RGBX_8888:
		return {GL_RGB, GL_UNSIGNED_INT_8_8_8_8};
	case BitmapFormat::XBGR_8888:
		return {GL_RGB, GL_UNSIGNED_INT_8_8_8_8_REV};
	case BitmapFormat::BGRX_8888:
		return {GL_BGR, GL_UNSIGNED_INT_8_8_8_8};
	case BitmapFormat::ARGB_8888:
		return {GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV};
	case BitmapFormat::RGBA_8888:
		return {GL_RGBA, GL_UNSIGNED_INT_8_8_8_8};
	case BitmapFormat::ABGR_8888:
		return {GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV};
	case BitmapFormat::BGRA_8888:
		return {GL_BGRA, GL_UNSIGNED_INT_8_8_8_8};
	case BitmapFormat::ARGB_2101010:
		return {GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV};
	default:
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
		__assume(false);
#else // GCC, Clang
		__builtin_unreachable();
#endif
		break;
	}
}
