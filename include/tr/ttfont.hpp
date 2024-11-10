/**
 * @file ttfont.hpp
 * @brief Provides TrueType font functionality.
 */

#pragma once
#include "bitmap.hpp"

struct _TTF_Font;

namespace tr {
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
		struct Deleter {
			/// @private
			void operator()(bool) const noexcept;
		};

		Handle<bool, false, Deleter> _handle {true};
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
			NORMAL        = 0,

			/**********************************************************************************************************
	         * Bold font.
	         **********************************************************************************************************/
			BOLD          = 1,

			/**********************************************************************************************************
	         * Italic font.
	         **********************************************************************************************************/
			ITALIC        = 2,

			/**********************************************************************************************************
	         * Underlined font.
	         **********************************************************************************************************/
			UNDERLINE     = 4,

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
			glm::ivec2   min;

			/**********************************************************************************************************
	         * Max x/y of the glyph.
	         **********************************************************************************************************/
			glm::ivec2   max;

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
			int              width;
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
		TTFont(const std::filesystem::path& path, int size, glm::uvec2 dpi = {72, 72});

		/**************************************************************************************************************
	     * Loads an embedded font file.
         *
         * A failed assertion may be triggered if loading the font failed.
         *
         * @param embeddedFile The embedded font file.
         * @param size The point size of the font.
         * @param dpi The target resolution of the font.
	     **************************************************************************************************************/
		TTFont(std::span<const std::byte> embeddedFile, int size, glm::uvec2 dpi = {72, 72}) noexcept;

		/**************************************************************************************************************
	     * Gets the ascent of the font.
         *
         * @return The ascent of the font in pixels.
	     **************************************************************************************************************/
		int           ascent() const noexcept;

		/**************************************************************************************************************
	     * Gets the descent of the font.
         *
         * @return The descent of the font in pixels.
	     **************************************************************************************************************/
		int           descent() const noexcept;

		/**************************************************************************************************************
	     * Gets the height of the font.
         *
         * @return The height of the font in pixels.
	     **************************************************************************************************************/
		int           height() const noexcept;

		/**************************************************************************************************************
	     * Gets the distance between two lines.
         *
         * @return The distance between lines in pixels.
	     **************************************************************************************************************/
		int           lineSkip() const noexcept;

		/**************************************************************************************************************
	     * Gets whether the font is fixed width.
         *
         * @return True if the font is fixed width, and false otherwise.
	     **************************************************************************************************************/
		bool          fixedWidth() const noexcept;

		/**************************************************************************************************************
	     * Gets the font's family name.
         *
         * @return A C-string containing the font's family name.
	     **************************************************************************************************************/
		const char*   familyName() const noexcept;

		/**************************************************************************************************************
	     * Gets the font's style name.
         *
         * @return A C-string containing the font's style name.
	     **************************************************************************************************************/
		const char*   styleName() const noexcept;

		/**************************************************************************************************************
	     * Gets the number of font faces.
         *
         * @return The number of font faces.
	     **************************************************************************************************************/
		int           faceCount() const noexcept;

		/**************************************************************************************************************
	     * Gets the hinting of the font.
         *
         * @return The hinting type of the font.
	     **************************************************************************************************************/
		Hint          hinting() const noexcept;

		/**************************************************************************************************************
	     * Sets the hinting of the font.
         *
         * @param hinting The new hinting type of the font.
	     **************************************************************************************************************/
		void          setHinting(Hint hinting) noexcept;

		/**************************************************************************************************************
	     * Gets whether kerning is enabled.
         *
         * @return True if kerning is enabled, and false otherwise.
	     **************************************************************************************************************/
		bool          kerning() const noexcept;

		/**************************************************************************************************************
	     * Sets whether kerning is enabled.
         *
         * @param kerning Whether kerning should be enabled.
	     **************************************************************************************************************/
		void          setKerning(bool kerning) noexcept;

		/**************************************************************************************************************
	     * Gets the style of the font.
         *
         * @return The style of the font.
	     **************************************************************************************************************/
		Style         style() const noexcept;

		/**************************************************************************************************************
	     * Sets the style of the font.
         *
         * @param style The new style of the font.
	     **************************************************************************************************************/
		void          setStyle(Style style) noexcept;

		/**************************************************************************************************************
	     * Gets the outline thickness of the font.
         *
         * @return The outline thickness of the font.
	     **************************************************************************************************************/
		int           outline() const noexcept;

		/**************************************************************************************************************
	     * Sets the outline thickness of the font.
         *
         * @param thickness The new outline thickness of the font.
	     **************************************************************************************************************/
		void          setOutline(int thickness) noexcept;

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
		void          setWrapAlignment(WrapAlignment alignment) noexcept;

		/**************************************************************************************************************
	     * Gets whether a glyph is contained in the font.
         *
         * @param glyph The glyph to check for.
         *
         * @return True if the glyph is contained in the font, and false otherwise.
	     **************************************************************************************************************/
		bool          contains(std::uint32_t glyph) const noexcept;

		/**************************************************************************************************************
	     * Gets the metrics of a glyph.
         *
         * @param glyph The glyph to get the metrics of. If the glyph isn't contained in the font, a failed assertion
         *              may be triggered.
         *
         * @return The metrics of the glyph.
	     **************************************************************************************************************/
		Glyph         glyph(std::uint32_t glyph) const noexcept;

		/**************************************************************************************************************
	     * Resizes the font.
         *
         * @exception TTFontResizeError If resizing the font failed.
         *
         * @param size The new point size of the font.
         * @param dpi The new DPI of the font.
	     **************************************************************************************************************/
		void          resize(int size, glm::uvec2 dpi = {72, 72});

		/**************************************************************************************************************
	     * Gets the size of a line of text.
         *
         * @param text The line of text to get the size of.
         *
         * @return The size of the bitmap required to hold the text.
	     **************************************************************************************************************/
		glm::ivec2    textSize(const char* text) const noexcept;

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
		Bitmap        render(std::uint32_t cp, RGBA8 color) const;

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
		Bitmap        render(const char* text, RGBA8 color) const;

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
		Bitmap        renderWrapped(const char* text, RGBA8 color, std::uint32_t width) const;

	private:
		struct Deleter {
			/// @private
			void operator()(_TTF_Font* ptr) const noexcept;
		};

		std::unique_ptr<_TTF_Font, Deleter> _impl;
	};

	/// @cond IMPLEMENTATION
	DEFINE_BITMASK_OPERATORS(TTFont::Style);
	/// @endcond
} // namespace tr