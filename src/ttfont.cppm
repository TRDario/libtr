/**
 * @file ttfont.cppm
 * @brief Provides TrueType font functionality.
 */

module;
#include <cassert>
#include <EnumBitmask.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef _WIN32
#include <windows.h>
#endif

export module tr:ttfont;

import std;
import glm;
import :bitmap;
import :color;
import :function;
import :handle;
import :iostream;
import :sdl;

export namespace tr {
    /******************************************************************************************************************
	 * Error thrown when TrueType font loading failed.
	 ******************************************************************************************************************/
    struct TTFontLoadError : FileError {
        using FileError::FileError;

        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		virtual const char* what() const noexcept;
	};

    /******************************************************************************************************************
	 * Error thrown when TrueType font resizing failed.
	 ******************************************************************************************************************/
    struct TTFontResizeError : SDLError {
        /******************************************************************************************************************
	     * Constructs the error.
	     ******************************************************************************************************************/
        TTFontResizeError();
	};


	/******************************************************************************************************************
	 * SDL_TTF library RAII wrapper.
	 ******************************************************************************************************************/
	class SDL_TTF {
	public:
		/**************************************************************************************************************
	     * Initializes the SDL_TTF library.
         *
         * @exception SDLError If initializing the library failed.
	     **************************************************************************************************************/
		SDL_TTF();


        /**************************************************************************************************************
	     * Gets the version of the SDL_TTF library.
         *
         * @return The version of the SDL_TTF library.
	     **************************************************************************************************************/
		Version version() const noexcept;

        /**************************************************************************************************************
	     * Gets the version of the FreeType library.
         *
         * @return The version of the FreeType library.
	     **************************************************************************************************************/
		Version freetypeVersion() const noexcept;
	private:
        Handle<bool, false, decltype([] (bool) { TTF_Quit(); })> _handle { true };
	};


	/**************************************************************************************************************
	 * TrueType font.
	 **************************************************************************************************************/
	class TTFont {
	public:
        /**************************************************************************************************************
	     * Font hinting types.
	     **************************************************************************************************************/
		enum class Hint {
            /**********************************************************************************************************
	         * Normal hinting.
	         **********************************************************************************************************/
			NORMAL,

            /**********************************************************************************************************
	         * Lighter hinting.
	         **********************************************************************************************************/
			LIGHT,

            /**********************************************************************************************************
	         * Mono hinting.
	         **********************************************************************************************************/
			MONO,

            /**********************************************************************************************************
	         * Disabled hinting.
	         **********************************************************************************************************/
			NONE,

            /**********************************************************************************************************
	         * Light hinting with subpixel rendering.
	         **********************************************************************************************************/
			LIGHT_SUBPIXEL
		};
        
		/**************************************************************************************************************
	     * Font style types. May be ORed together.
	     **************************************************************************************************************/
		enum class Style {
            /**********************************************************************************************************
	         * Normal font.
	         **********************************************************************************************************/
			NORMAL = 0,

            /**********************************************************************************************************
	         * Bold font.
	         **********************************************************************************************************/
			BOLD = 1,

            /**********************************************************************************************************
	         * Italic font.
	         **********************************************************************************************************/
			ITALIC = 2,

            /**********************************************************************************************************
	         * Underlined font.
	         **********************************************************************************************************/
			UNDERLINE = 4,

            /**********************************************************************************************************
	         * Striked font.
	         **********************************************************************************************************/
			STRIKETHROUGH = 8
		};

        /**************************************************************************************************************
	     * Wrapped text alignment.
	     **************************************************************************************************************/
		enum class WrapAlignment {
            /**********************************************************************************************************
	         * The text is left-aligned.
	         **********************************************************************************************************/
			LEFT,

            /**********************************************************************************************************
	         * The text is center-aligned.
	         **********************************************************************************************************/
			CENTER,

            /**********************************************************************************************************
	         * The text is right-aligned.
	         **********************************************************************************************************/
			RIGHT
		};

        /**************************************************************************************************************
	     * Font glyph metrics.
	     **************************************************************************************************************/
		struct Glyph {
            /**********************************************************************************************************
	         * Min x/y of the glyph.
	         **********************************************************************************************************/
			glm::ivec2 min;

            /**********************************************************************************************************
	         * Max x/y of the glyph.
	         **********************************************************************************************************/
			glm::ivec2 max;

            /**********************************************************************************************************
	         * The advance of the glyph in pixels.
	         **********************************************************************************************************/
			std::int32_t adv;
		};

        /**************************************************************************************************************
	     * Font text measurement result.
	     **************************************************************************************************************/
        struct MeasureResult {
            /**********************************************************************************************************
	         * The text that fit in the width.
	         **********************************************************************************************************/
            std::string_view text;

            /**********************************************************************************************************
	         * The width of the fit string in pixels.
	         **********************************************************************************************************/
            int width;
        };


        /**************************************************************************************************************
	     * Loads a font from file.
         *
         * @exception FileNotFound If the file was not found.
         * @exception TTFontLoadError If loading the font failed.
         *
         * @param path The path to the font file.
         * @param size The point size of the font.
         * @param dpi The target resolution of the font.
	     **************************************************************************************************************/
		TTFont(const std::filesystem::path& path, int size, glm::uvec2 dpi = { 72, 72 });

        /**************************************************************************************************************
	     * Loads a font from file.
         *
         * A failed assertion may be triggered if loading the font failed.
         *
         * @param path The path to the font file.
         * @param size The point size of the font.
         * @param dpi The target resolution of the font.
	     **************************************************************************************************************/
        TTFont(std::span<const std::byte> embeddedFile, int size, glm::uvec2 dpi = { 72, 72 }) noexcept;


        /**************************************************************************************************************
	     * Gets the ascent of the font.
         *
         * @return The ascent of the font in pixels.
	     **************************************************************************************************************/
		int ascent() const noexcept;

        /**************************************************************************************************************
	     * Gets the descent of the font.
         *
         * @return The descent of the font in pixels.
	     **************************************************************************************************************/
		int descent() const noexcept;

        /**************************************************************************************************************
	     * Gets the height of the font.
         *
         * @return The height of the font in pixels.
	     **************************************************************************************************************/
		int height() const noexcept;

        /**************************************************************************************************************
	     * Gets the distance between two lines.
         *
         * @return The distance between lines in pixels.
	     **************************************************************************************************************/
		int lineSkip() const noexcept;

        /**************************************************************************************************************
	     * Gets whether the font is fixed width.
         *
         * @return True if the font is fixed width, and false otherwise.
	     **************************************************************************************************************/
		bool fixedWidth() const noexcept;


        /**************************************************************************************************************
	     * Gets the font's family name.
         *
         * @return A C-string containing the font's family name.
	     **************************************************************************************************************/
		const char* familyName() const noexcept;

        /**************************************************************************************************************
	     * Gets the font's style name.
         *
         * @return A C-string containing the font's style name.
	     **************************************************************************************************************/
		const char* styleName() const noexcept;


        /**************************************************************************************************************
	     * Gets the number of font faces.
         *
         * @return The number of font faces.
	     **************************************************************************************************************/
		int faceCount() const noexcept;


        /**************************************************************************************************************
	     * Gets the hinting of the font.
         *
         * @return The hinting type of the font.
	     **************************************************************************************************************/
		Hint hinting() const noexcept;

        /**************************************************************************************************************
	     * Sets the hinting of the font.
         *
         * @param hinting The new hinting type of the font.
	     **************************************************************************************************************/
		void setHinting(Hint hinting) noexcept;


        /**************************************************************************************************************
	     * Gets whether kerning is enabled.
         *
         * @return True if kerning is enabled, and false otherwise.
	     **************************************************************************************************************/
		bool kerning() const noexcept;

        /**************************************************************************************************************
	     * Sets whether kerning is enabled.
         *
         * @param kerning Whether kerning should be enabled.
	     **************************************************************************************************************/
		void setKerning(bool kerning) noexcept;


        /**************************************************************************************************************
	     * Gets the style of the font.
         *
         * @return The style of the font.
	     **************************************************************************************************************/
		Style style() const noexcept;

        /**************************************************************************************************************
	     * Sets the style of the font.
         *
         * @param style The new style of the font.
	     **************************************************************************************************************/
		void setStyle(Style style) noexcept;


        /**************************************************************************************************************
	     * Gets the outline thickness of the font.
         *
         * @return The outline thickness of the font.
	     **************************************************************************************************************/
        int outline() const noexcept;

        /**************************************************************************************************************
	     * Sets the outline thickness of the font.
         *
         * @param thickness The new outline thickness of the font.
	     **************************************************************************************************************/
        void setOutline(int thickness) noexcept;


        /**************************************************************************************************************
	     * Gets the wrap alignment of the font.
         *
         * @return The wrap alignment of the font.
	     **************************************************************************************************************/
        WrapAlignment wrapAlignment() const noexcept;

        /**************************************************************************************************************
	     * Sets the wrap alignment of the font.
         *
         * @param alignment The new wrap alignment of the font.
	     **************************************************************************************************************/
        void setWrapAlignment(WrapAlignment alignment) noexcept;


        /**************************************************************************************************************
	     * Gets whether a glyph is contained in the font.
         *
         * @param glyph The glyph to check for.
         *
         * @return True if the glyph is contained in the font, and false otherwise.
	     **************************************************************************************************************/
		bool contains(std::uint32_t glyph) const noexcept;

        /**************************************************************************************************************
	     * Gets the metrics of a glyph.
         *
         * @param glyph The glyph to get the metrics of. If the glyph isn't contained in the font, a failed assertion
         *              may be triggered.
         *
         * @return The metrics of the glyph.
	     **************************************************************************************************************/
		Glyph glyph(std::uint32_t glyph) const noexcept;


        /**************************************************************************************************************
	     * Resizes the font.
         *
         * @exception TTFontResizeError If resizing the font failed.
         *
         * @param size The new point size of the font.
         * @param dpi The new DPI of the font.
	     **************************************************************************************************************/
		void resize(int size, glm::uvec2 dpi = { 72, 72 });


        /**************************************************************************************************************
	     * Gets the size of a line of text.
         *
         * @param text The line of text to get the size of.
         *
         * @return The size of the bitmap required to hold the text.
	     **************************************************************************************************************/
		glm::ivec2 textSize(const char* text) const noexcept;

        /**************************************************************************************************************
	     * Measures how much of a line of text can fit within a certain width.
         *
         * @param text The line of text to get the measure of.
         * @param maxWidth The maximum width that the text can occupy.
         *
         * @return The string that can be contained in the width, and the width of that string.
	     **************************************************************************************************************/
        MeasureResult measure(const char* text, int maxWidth) const noexcept;


		// Renders an alpha-blended glyph to a 32-bit bitmap.

        /**************************************************************************************************************
	     * Renders a glyph.
         *
         * @exception BitmapBadAlloc If rendering the bitmap failed.
         *
         * @param cp The unicode codepoint to render.
         * @param color The color of the glyph.
         *
         * @return A bitmap holding the glyph.
	     **************************************************************************************************************/
		Bitmap render(std::uint32_t cp, RGBA8 color) const;

		/**************************************************************************************************************
	     * Renders a line of text.
         *
         * @exception BitmapBadAlloc If rendering the bitmap failed.
         *
         * @param text The line of text to render (\n not supported).
         * @param color The color of the text.
         *
         * @return A bitmap holding the text.
	     **************************************************************************************************************/
		Bitmap render(const char* text, RGBA8 color) const;

        /**************************************************************************************************************
	     * Renders wrapped text.
         *
         * @exception BitmapBadAlloc If rendering the bitmap failed.
         *
         * @param text The line of text to render (\n supported).
         * @param color The color of the text.
         * @param width The bounding width of the text.
         *
         * @return A bitmap holding the text.
	     **************************************************************************************************************/
		Bitmap renderWrapped(const char* text, RGBA8 color, std::uint32_t width) const;
	private:
		std::unique_ptr<TTF_Font, FunctionCaller<&TTF_CloseFont>> _impl;
	};

    /// @cond IMPLEMENTATION
	DEFINE_BITMASK_OPERATORS(TTFont::Style);
    /// @endcond
}

/// @cond IMPLEMENTATION

namespace tr {
    // Fixes certain edge artifacts when rendering partially transparent text.
    void fixAlphaArtifacts(Bitmap& bitmap, std::uint8_t maxAlpha) noexcept;
}

void tr::fixAlphaArtifacts(Bitmap& bitmap, std::uint8_t maxAlpha) noexcept
{
    // We know the bitmap is ARGB_8888.
    auto it { (std::uint8_t*)(bitmap.data()) };
    for (int y = 0; y < bitmap.size().y; ++y) {
        for (int x = 0; x < bitmap.size().x; ++x) {
            it[x * 4] = std::min(it[x * 4], maxAlpha);
        }
        it += bitmap.pitch();
    }
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

tr::TTFontResizeError::TTFontResizeError()
    : SDLError { "Failed to resize font" }
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

tr::TTFont::TTFont(std::span<const std::byte> embeddedFile, int size, glm::uvec2 dpi) noexcept
    : _impl { TTF_OpenFontDPIRW(SDL_RWFromConstMem(embeddedFile.data(), embeddedFile.size()), true, size, dpi.x, dpi.y) }
{
    assert(_impl != nullptr);
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

int tr::TTFont::outline() const noexcept
{
    return TTF_GetFontOutline(_impl.get());
}

void tr::TTFont::setOutline(int thickness) noexcept
{
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
    if (TTF_SetFontSizeDPI(_impl.get(), size, dpi.x, dpi.y) < 0) {
        throw TTFontResizeError {};
    }
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
    return { std::string_view { text, std::size_t(count) }, extent };
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

tr::Bitmap tr::TTFont::renderWrapped(const char* text, RGBA8 color, std::uint32_t width) const
{
    Bitmap render { TTF_RenderUTF8_Blended_Wrapped(_impl.get(), text, std::bit_cast<SDL_Color>(color), width) };
    if (color.a < 255) {
        fixAlphaArtifacts(render, color.a);
    }
    return render;
}

/// @endcond