#include "../include/tr/ttfont.hpp"
#include <SDL2/SDL_ttf.h>

namespace tr {
	// Fixes certain edge artifacts when rendering partially transparent text.
	void fixAlphaArtifacts(Bitmap& bitmap, std::uint8_t maxAlpha) noexcept;
} // namespace tr

void tr::fixAlphaArtifacts(Bitmap& bitmap, std::uint8_t maxAlpha) noexcept
{
	// We know the bitmap is ARGB_8888.
	auto it{(std::uint8_t*)(bitmap.data())};
	for (int y = 0; y < bitmap.size().y; ++y) {
		for (int x = 0; x < bitmap.size().x; ++x) {
			it[x * 4] = std::min(it[x * 4], maxAlpha);
		}
		it += bitmap.pitch();
	}
}

tr::TTFont::TTFont(const std::filesystem::path& path, int size, glm::uvec2 dpi)
{
	if (!is_regular_file(path)) {
		throw FileNotFound{path};
	}

#ifdef _WIN32
	_impl.reset(TTF_OpenFontDPIRW(SDL_RWFromFP(_wfopen(path.c_str(), L"r"), true), true, size, dpi.x, dpi.y));
#else
	_impl.reset(TTF_OpenFontDPI(path.c_str(), size, dpi.x, dpi.y));
#endif
	if (_impl == nullptr) {
		throw TTFontLoadError{path};
	}
}

tr::TTFont::TTFont(std::span<const std::byte> embeddedFile, int size, glm::uvec2 dpi) noexcept
	: _impl{TTF_OpenFontDPIRW(SDL_RWFromConstMem(embeddedFile.data(), embeddedFile.size()), true, size, dpi.x, dpi.y)}
{
	assert(_impl != nullptr);
}

void tr::TTFont::Deleter::operator()(TTF_Font* ptr) const noexcept
{
	TTF_CloseFont(ptr);
}

int tr::TTFont::ascent() const noexcept
{
	return TTF_FontAscent(_impl.get());
}

int tr::TTFont::descent() const noexcept
{
	return TTF_FontDescent(_impl.get());
}

int tr::TTFont::height() const noexcept
{
	return TTF_FontHeight(_impl.get());
}

int tr::TTFont::lineSkip() const noexcept
{
	return TTF_FontLineSkip(_impl.get());
}

bool tr::TTFont::fixedWidth() const noexcept
{
	return TTF_FontFaceIsFixedWidth(_impl.get());
}

const char* tr::TTFont::familyName() const noexcept
{
	return TTF_FontFaceFamilyName(_impl.get());
}

const char* tr::TTFont::styleName() const noexcept
{
	return TTF_FontFaceStyleName(_impl.get());
}

int tr::TTFont::faceCount() const noexcept
{
	return TTF_FontFaces(_impl.get());
}

tr::TTFont::Hint tr::TTFont::hinting() const noexcept
{
	return Hint(TTF_GetFontHinting(_impl.get()));
}

void tr::TTFont::setHinting(Hint hinting) noexcept
{
	if (hinting == this->hinting()) {
		return;
	}
	TTF_SetFontHinting(_impl.get(), int(hinting));
}

bool tr::TTFont::kerning() const noexcept
{
	return TTF_GetFontKerning(_impl.get());
}

void tr::TTFont::setKerning(bool kerning) noexcept
{
	TTF_SetFontKerning(_impl.get(), kerning);
}

tr::TTFont::Style tr::TTFont::style() const noexcept
{
	return Style(TTF_GetFontStyle(_impl.get()));
}

void tr::TTFont::setStyle(Style style) noexcept
{
	if (style == this->style()) {
		return;
	}
	TTF_SetFontStyle(_impl.get(), int(style));
}

int tr::TTFont::outline() const noexcept
{
	return TTF_GetFontOutline(_impl.get());
}

void tr::TTFont::setOutline(int thickness) noexcept
{
	if (thickness == outline()) {
		return;
	}
	TTF_SetFontOutline(_impl.get(), thickness);
}

tr::TTFont::WrapAlignment tr::TTFont::wrapAlignment() const noexcept
{
	return WrapAlignment(TTF_GetFontWrappedAlign(_impl.get()));
}

void tr::TTFont::setWrapAlignment(WrapAlignment alignment) noexcept
{
	TTF_SetFontWrappedAlign(_impl.get(), int(alignment));
}

bool tr::TTFont::contains(std::uint32_t cp) const noexcept
{
	return TTF_GlyphIsProvided32(_impl.get(), cp);
}

tr::TTFont::Glyph tr::TTFont::glyph(std::uint32_t cp) const noexcept
{
	assert(contains(cp));
	Glyph glyph;
	TTF_GlyphMetrics32(_impl.get(), cp, &glyph.min.x, &glyph.max.x, &glyph.min.y, &glyph.max.y, &glyph.adv);
	return glyph;
}

void tr::TTFont::resize(int size, glm::uvec2 dpi)
{
	if (size == _size && dpi == _dpi) {
		return;
	}

	if (TTF_SetFontSizeDPI(_impl.get(), size, dpi.x, dpi.y) < 0) {
		throw TTFontError{"Failed to resize font"};
	}
	_size = size;
	_dpi  = dpi;
}

glm::ivec2 tr::TTFont::textSize(const char* text) const noexcept
{
	glm::ivec2 size;
	TTF_SizeUTF8(_impl.get(), text, &size.x, &size.y);
	return size;
}

tr::TTFont::MeasureResult tr::TTFont::measure(const char* text, int maxWidth) const noexcept
{
	int extent, count;
	TTF_MeasureUTF8(_impl.get(), text, maxWidth, &extent, &count);
	return {std::string_view{text, std::size_t(count)}, extent};
}

tr::Bitmap tr::TTFont::render(std::uint32_t cp, RGBA8 color) const
{
	auto ptr{TTF_RenderGlyph32_Blended(_impl.get(), cp, std::bit_cast<SDL_Color>(color))};
	if (ptr == nullptr) {
		throw TTFontError{"Failed to render codepoint"};
	}
	return Bitmap{ptr};
}

tr::Bitmap tr::TTFont::render(const char* text, RGBA8 color) const
{
	assert(!std::string_view{text}.empty());
	auto ptr{TTF_RenderUTF8_Blended(_impl.get(), text, std::bit_cast<SDL_Color>(color))};
	if (ptr == nullptr) {
		throw TTFontError{"Failed to render text"};
	}
	Bitmap bitmap{ptr};
	if (color.a < 255) {
		fixAlphaArtifacts(bitmap, color.a);
	}
	return bitmap;
}

tr::Bitmap tr::TTFont::renderWrapped(const char* text, RGBA8 color, std::uint32_t width) const
{
	assert(!std::string_view{text}.empty());
	auto ptr{TTF_RenderUTF8_Blended_Wrapped(_impl.get(), text, std::bit_cast<SDL_Color>(color), width)};
	if (ptr == nullptr) {
		throw TTFontError{"Failed to render wrapped text"};
	}
	Bitmap bitmap{ptr};
	if (color.a < 255) {
		fixAlphaArtifacts(bitmap, color.a);
	}
	return bitmap;
}
