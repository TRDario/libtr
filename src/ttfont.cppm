module;
#include <cassert>
#include <EnumBitmask.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

export module tr:ttfont;

import std;
import glm;
import :bitmap;
import :color;
import :handle;
import :iostream;
import :sdl;

#ifdef _WIN32
#include <windows.h>
#endif

export namespace tr {
	// SDL_TTF library RAII wrapper.
	class SDL_TTF {
	public:
		// Initializes the SDL_TTF library.
		SDL_TTF();

		Version version() const noexcept;
		Version freetypeVersion() const noexcept;
	private:
		struct Deleter { void operator()(bool) noexcept; };
        Handle<bool, false, Deleter> _handle { true };
	};

    struct TTFontLoadError : FileError {
        using FileError::FileError;
		virtual const char* what() const noexcept;
	};

	// TrueType font class.
	class TTFont {
	public:
		// Enum representing a type of font hint.
		enum class Hint {
			NORMAL,
			LIGHT,
			MONO,
			NONE,
			LIGHT_SUBPIXEL
		};
		// Enum representing a font style.
		enum class Style {
			NORMAL = 0,
			BOLD = 1,
			ITALIC = 2,
			UNDERLINE = 4,
			STRIKETHROUGH = 8
		};
		// Enum representing ttf text alignment.
		enum class Align {
			LEFT,
			CENTER,
			RIGHT
		};
		// Struct containing glyph metrics.
		struct GlyphMetrics {
			glm::ivec2 min; // Min x/y of the glyph.
			glm::ivec2 max; // Max x/y of the glyph.
			std::int32_t       adv; // The advance of the glyph.
		};

		// Loads a TTF font from file.
		TTFont(const std::filesystem::path& path, int size);
		// Loads a TTF font from file.
		// dpi - The target resolution DPI values of the font.
		TTFont(const std::filesystem::path& path, int size, glm::uvec2 dpi);

		int ascent() const noexcept;
		int descent() const noexcept;
		int height() const noexcept;
		int lineHeight() const noexcept;
		bool fixedWidth() const noexcept;

		const char* familyName() const noexcept;
		const char* styleName() const noexcept;

		int faceCount() const noexcept;

		Hint hinting() const noexcept;
		void setHinting(Hint hinting) noexcept;

		bool kerning() const noexcept;
		void setKerning(bool kerning) noexcept;

		Style style() const noexcept;
		void setStyle(Style style) noexcept;

		bool contains(std::uint32_t glyph) const noexcept;
		GlyphMetrics glyphMetrics(std::uint32_t glyph) const noexcept;

		bool resize(int size) noexcept;
		// Resizes the font with the target resolution.
		// dpi - The target resolution DPI values of the font.
		bool resizeDPI(int size, glm::uvec2 dpi) noexcept;

		glm::ivec2 textSize(const char* text) const noexcept;
        glm::ivec2 textSize(const std::string& text) const noexcept;

		// Renders an alpha-blended glyph to a 32-bit bitmap.
		Bitmap render(std::uint32_t cp, RGBA8 color) const;
		// Renders alpha-blended text to a 32-bit bitmap.
		Bitmap render(const char* text, RGBA8 color) const;
        // Renders alpha-blended text to a 32-bit bitmap.
		Bitmap render(const std::string& text, RGBA8 color) const;
		// Renders multiline, wrapped alpha-blended text to a 32-bit bitmap.
		// width - The max horizontal width of a line. 0 means only newlines break.
		Bitmap renderWrapped(const char* text, RGBA8 color, std::uint32_t width, Align align) const;
        // Renders multiline, wrapped alpha-blended text to a 32-bit bitmap.
		// width - The max horizontal width of a line. 0 means only newlines break.
		Bitmap renderWrapped(const std::string& text, RGBA8 color, std::uint32_t width, Align align) const;

        // Renders an alpha-blended glyph to a 32-bit bitmap.
		Bitmap renderOutlined(std::uint32_t cp, RGBA8 color, RGBA8 outlineColor, int outlineThickness) const;
		// Renders outlined alpha-blended text to a 32-bit bitmap.
		Bitmap renderOutlined(const char* text, RGBA8 color, RGBA8 outlineColor, int outlineThickness) const;
        // Renders outlined alpha-blended text to a 32-bit bitmap.
		Bitmap renderOutlined(const std::string& text, RGBA8 color, RGBA8 outlineColor, int outlineThickness) const;
		// Renders outlined multiline, wrapped alpha-blended text to a 32-bit bitmap.
		// width - The max horizontal width of a line. 0 means only newlines break.
		Bitmap renderWrappedOutlined(const char* text, RGBA8 color, std::uint32_t width, Align align, RGBA8 outlineColor, int outlineThickness) const;
        // Renders outlined multiline, wrapped alpha-blended text to a 32-bit bitmap.
		// width - The max horizontal width of a line. 0 means only newlines break.
		Bitmap renderWrappedOutlined(const std::string& text, RGBA8 color, std::uint32_t width, Align align, RGBA8 outlineColor, int outlineThickness) const;
	private:
		struct Deleter { void operator()(TTF_Font* font) noexcept; };
		std::unique_ptr<TTF_Font, Deleter> _impl;
	};

	DEFINE_BITMASK_OPERATORS(TTFont::Style);
}

// IMPLEMENTATION

namespace tr {
    // Fixes certain edge artifacts when rendering partially transparent text.
    void fixAlphaArtifacts(Bitmap& bitmap, std::uint8_t maxAlpha) noexcept;
}

void tr::fixAlphaArtifacts(Bitmap& bitmap, std::uint8_t maxAlpha) noexcept
{
    for (auto pixel : bitmap) {
        RGBA8 color { pixel };
        pixel = RGBA8 { color.r, color.g, color.b, std::min(color.a, maxAlpha) };
    }
}

void tr::SDL_TTF::Deleter::operator()(bool) noexcept
{
    TTF_Quit();
}

tr::SDL_TTF::SDL_TTF()
{
    if (TTF_Init() < 0) {
        throw SDLError { "Failed to initialize the SDL TrueType library" };
    }
}

tr::Version tr::SDL_TTF::version() const noexcept
{
    return std::bit_cast<Version>(*TTF_Linked_Version());
}

tr::Version tr::SDL_TTF::freetypeVersion() const noexcept
{
    int major, minor, patch;
    TTF_GetFreeTypeVersion(&major, &minor, &patch);
    return { std::uint8_t(major), std::uint8_t(minor), std::uint8_t(patch) };
}

const char* tr::TTFontLoadError::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "Failed to load TrueType font file ({}): '{}'", SDL_GetError(), path());
    return str.c_str();
}

tr::TTFont::TTFont(const std::filesystem::path& path, int size)
    : TTFont { path, size, { 0, 0 } }
{}

tr::TTFont::TTFont(const std::filesystem::path& path, int size, glm::uvec2 dpi)
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }

    #ifdef _WIN32
    _impl.reset(TTF_OpenFontDPIRW(SDL_RWFromFP(_wfopen(path.c_str(), L"r"), true), true, size, dpi.x, dpi.y));
    #else
    _impl.reset(TTF_OpenFontDPI(path.c_str(), size, dpi.x, dpi.y));
    #endif
    if (_impl == nullptr) {
        throw TTFontLoadError { path };
    }
}

void tr::TTFont::Deleter::operator()(TTF_Font* font) noexcept
{
    TTF_CloseFont(font);
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

int tr::TTFont::lineHeight() const noexcept
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
    TTF_SetFontStyle(_impl.get(), int(style));
}

bool tr::TTFont::contains(std::uint32_t cp) const noexcept
{
    return TTF_GlyphIsProvided32(_impl.get(), cp);
}

tr::TTFont::GlyphMetrics tr::TTFont::glyphMetrics(std::uint32_t cp) const noexcept
{
    assert(contains(cp));
    GlyphMetrics metrics;
    TTF_GlyphMetrics32(_impl.get(), cp, &metrics.min.x, &metrics.max.x, &metrics.min.y, &metrics.max.y, &metrics.adv);
    return metrics;
}

bool tr::TTFont::resize(int size) noexcept
{
    return !TTF_SetFontSize(_impl.get(), size);
}

bool tr::TTFont::resizeDPI(int size, glm::uvec2 dpi) noexcept
{
    return !TTF_SetFontSizeDPI(_impl.get(), size, dpi.x, dpi.y);
}

glm::ivec2 tr::TTFont::textSize(const char* text) const noexcept
{
    glm::ivec2 size;
    TTF_SizeUTF8(_impl.get(), text, &size.x, &size.y);
    return size;
}

glm::ivec2 tr::TTFont::textSize(const std::string& text) const noexcept
{
    return textSize(text.c_str());
}

tr::Bitmap tr::TTFont::render(std::uint32_t cp, RGBA8 color) const
{
    return Bitmap { TTF_RenderGlyph32_Blended(_impl.get(), cp, std::bit_cast<SDL_Color>(color)) };
}

tr::Bitmap tr::TTFont::render(const char* text, RGBA8 color) const
{
    Bitmap render { TTF_RenderUTF8_Blended(_impl.get(), text, std::bit_cast<SDL_Color>(color)) };
    if (color.a < 255) {
        fixAlphaArtifacts(render, color.a);
    }
    return render;
}

tr::Bitmap tr::TTFont::render(const std::string& text, RGBA8 color) const
{
    return render(text.c_str(), color);
}

tr::Bitmap tr::TTFont::renderWrapped(const char* text, RGBA8 color, std::uint32_t width, Align align) const
{
    TTF_SetFontWrappedAlign(_impl.get(), int(align));
    Bitmap render { TTF_RenderUTF8_Blended_Wrapped(_impl.get(), text, std::bit_cast<SDL_Color>(color), width) };
    if (color.a < 255) {
        fixAlphaArtifacts(render, color.a);
    }
    return render;
}

tr::Bitmap tr::TTFont::renderWrapped(const std::string& text, RGBA8 color, std::uint32_t width, Align align) const
{
    return renderWrapped(text.c_str(), color, width, align);
}

tr::Bitmap tr::TTFont::renderOutlined(std::uint32_t cp, RGBA8 color, RGBA8 outlineColor, int outlineThickness) const
{
    Bitmap fill { render(cp, color) };
    TTF_SetFontOutline(_impl.get(), outlineThickness);
    Bitmap outline { render(cp, outlineColor) }; 
    TTF_SetFontOutline(_impl.get(), 0);
    outline.blit({ outlineThickness, outlineThickness }, fill.sub({ {}, outline.size() - glm::ivec2 { outlineThickness * 2, outlineThickness * 2 } }));
    return outline;
}

tr::Bitmap tr::TTFont::renderOutlined(const char* text, RGBA8 color, RGBA8 outlineColor, int outlineThickness) const
{
    Bitmap fill { render(text, color) };
    TTF_SetFontOutline(_impl.get(), outlineThickness);
    Bitmap outline { render(text, outlineColor) }; 
    TTF_SetFontOutline(_impl.get(), 0);
    outline.blit({ outlineThickness, outlineThickness }, fill.sub({ {}, outline.size() - glm::ivec2 { outlineThickness * 2, outlineThickness * 2 } }));
    return outline;
}

tr::Bitmap tr::TTFont::renderOutlined(const std::string& text, RGBA8 color, RGBA8 outlineColor, int outlineThickness) const
{
    return renderOutlined(text.c_str(), color, outlineColor, outlineThickness);
}

tr::Bitmap tr::TTFont::renderWrappedOutlined(const char* text, RGBA8 color, std::uint32_t width, Align align, RGBA8 outlineColor, int outlineThickness) const
{
    Bitmap fill { renderWrapped(text, color, width == 0 ? width : width - 2 * outlineThickness, align) };
    TTF_SetFontOutline(_impl.get(), outlineThickness);
    Bitmap outline { renderWrapped(text, outlineColor, width, align) }; 
    TTF_SetFontOutline(_impl.get(), 0);
    outline.blit({ outlineThickness, outlineThickness }, fill.sub({ {}, outline.size() - glm::ivec2 { outlineThickness * 2, outlineThickness * 2 } }));
    return outline;
}

tr::Bitmap tr::TTFont::renderWrappedOutlined(const std::string& text, RGBA8 color, std::uint32_t width, Align align, RGBA8 outlineColor, int outlineThickness) const
{
    return renderWrappedOutlined(text.c_str(), color, width, align, outlineColor, outlineThickness);
}