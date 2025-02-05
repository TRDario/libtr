#pragma once
#include "color.hpp"
#include "iostream.hpp"
#include "sdl.hpp"

struct TTF_Font;

namespace tr {
	class Bitmap;

	/** @ingroup system
	 *  @defgroup ttfont Font
	 *  TrueType font class and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when TrueType font loading fails.
	 ******************************************************************************************************************/
	struct TTFontLoadError : FileError {
		using FileError::FileError;

		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		const char* what() const noexcept override;
	};

	/******************************************************************************************************************
	 * Error thrown when a TrueType font operation fails.
	 ******************************************************************************************************************/
	struct TTFontError : SDLError {
		using SDLError::SDLError;
	};

	/******************************************************************************************************************
	 * TrueType font.
	 ******************************************************************************************************************/
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
		 * Sets the hinting of the font. Doing this clears the glyph cache.
		 *
		 * Doing this clears the glyph cache unless the value matches the current configuration, in which case nothing
		 * happens and the cache isn't cleared.
		 *
		 * @param[in] hinting The new hinting type of the font.
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
		 * @param[in] kerning Whether kerning should be enabled.
		 **************************************************************************************************************/
		void setKerning(bool kerning) noexcept;

		/**************************************************************************************************************
		 * Gets the style of the font.
		 *
		 * @return The style of the font.
		 **************************************************************************************************************/
		Style style() const noexcept;

		/**************************************************************************************************************
		 * Sets the style of the font. Doing this clears the glyph cache.
		 *
		 * Doing this clears the glyph cache unless the style matches the current configuration, in which case nothing
		 * happens and the cache isn't cleared.
		 *
		 * @param[in] style The new style of the font.
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
		 * Doing this clears the glyph cache unless the value matches the current configuration, in which case nothing
		 * happens and the cache isn't cleared.
		 *
		 * @param[in] thickness The new outline thickness of the font.
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
		 * @param[in] alignment The new wrap alignment of the font.
		 **************************************************************************************************************/
		void setWrapAlignment(WrapAlignment alignment) noexcept;

		/**************************************************************************************************************
		 * Gets whether a glyph is contained in the font.
		 *
		 * @param[in] glyph The glyph to check for.
		 *
		 * @return True if the glyph is contained in the font, and false otherwise.
		 **************************************************************************************************************/
		bool contains(std::uint32_t glyph) const noexcept;

		/**************************************************************************************************************
		 * Gets the metrics of a glyph.
		 *
		 * @param[in] glyph The glyph to get the metrics of. The glyph must be contained in the font.
		 *
		 * @return The metrics of the glyph.
		 **************************************************************************************************************/
		Glyph glyph(std::uint32_t glyph) const noexcept;

		/**************************************************************************************************************
		 * Resizes the font.
		 *
		 * Doing this clears the glyph cache, however, if the size and dpi match the current configuration,
		 * nothing happens and the cache isn't cleared.
		 *
		 * @exception TTFontError If resizing the font fails.
		 *
		 * @param[in] size The new point size of the font.
		 * @param[in] dpi The new DPI of the font.
		 **************************************************************************************************************/
		void resize(int size, glm::uvec2 dpi = {72, 72});

		/**************************************************************************************************************
		 * Gets the size of a line of text.
		 *
		 * @param[in] text The line of text to get the size of.
		 *
		 * @return The size of the bitmap required to hold the text.
		 **************************************************************************************************************/
		glm::ivec2 textSize(const char* text) const noexcept;

		/**************************************************************************************************************
		 * Measures how much of a line of text can fit within a certain width.
		 *
		 * @param[in] text The line of text to get the measure of.
		 * @param[in] maxWidth The maximum width that the text can occupy.
		 *
		 * @return The string that can be contained in the width, and the width of that string.
		 **************************************************************************************************************/
		MeasureResult measure(const char* text, int maxWidth) const noexcept;

		/**************************************************************************************************************
		 * Renders a glyph.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TTFontError If rendering the bitmap fails.
		 *
		 * @param[in] cp
		 * @parblock
		 * The unicode codepoint to render.
		 *
		 * @pre @em cp must not be a whitespace character.
		 * @endparblock
		 * @param[in] color The color of the glyph.
		 *
		 * @return A bitmap holding the glyph.
		 **************************************************************************************************************/
		Bitmap render(std::uint32_t cp, RGBA8 color) const;

		/**************************************************************************************************************
		 * Renders a line of text.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TTFontError If rendering the bitmap fails.
		 *
		 * @param[in] text
		 * @parblock
		 * The line of text to render (\\n not supported).
		 *
		 * @pre @em text must not be composed entirely of whitespace characters.
		 * @endparblock
		 * @param[in] color The color of the text.
		 *
		 * @return A bitmap holding the text.
		 **************************************************************************************************************/
		Bitmap render(const char* text, RGBA8 color) const;

		/**************************************************************************************************************
		 * Renders wrapped text.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TTFontError If rendering the bitmap fails.
		 *
		 * @param[in] text
		 * @parblock
		 * The line of text to render (\\n supported).
		 *
		 * @pre @em text must not be composed entirely of whitespace characters.
		 * @endparblock
		 * @param[in] color The color of the text.
		 * @param[in] width The bounding width of the text.
		 *
		 * @return A bitmap holding the text.
		 **************************************************************************************************************/
		Bitmap renderWrapped(const char* text, RGBA8 color, std::uint32_t width) const;

	  private:
		struct Deleter {
			void operator()(TTF_Font* ptr) const noexcept;
		};

		std::unique_ptr<TTF_Font, Deleter> _impl;
		int                                _size;
		glm::uvec2                         _dpi;

		TTFont(TTF_Font* impl, int size, glm::uvec2 dpi) noexcept;

		friend TTFont loadEmbeddedTTFont(std::span<const std::byte> data, int size, glm::uvec2 dpi);
		friend TTFont loadTTFontFile(const std::filesystem::path& path, int size, glm::uvec2 dpi);
	};

	/******************************************************************************************************************
	 * Loads an embedded font file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception TTFontLoadError If loading the font fails.
	 *
	 * @param[in] data The embedded font file.
	 * @param[in] size The point size of the font.
	 * @param[in] dpi The target resolution of the font.
	 ******************************************************************************************************************/
	TTFont loadEmbeddedTTFont(std::span<const std::byte> data, int size, glm::uvec2 dpi = {72, 72});

	/******************************************************************************************************************
	 * Loads an embedded font file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception TTFontLoadError If loading the font fails.
	 *
	 * @param[in] range The embedded font file.
	 * @param[in] size The point size of the font.
	 * @param[in] dpi The target resolution of the font.
	 ******************************************************************************************************************/
	template <std::ranges::contiguous_range Range>
	TTFont loadEmbeddedTTFont(Range&& range, int size, glm::uvec2 dpi = {72, 72})
	{
		return loadEmbeddedTTFont(std::span<const std::byte>{rangeBytes(range)}, size, dpi);
	};

	/******************************************************************************************************************
	 * Loads a font from file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception FileNotFound If the file is not found.
	 * @exception TTFontLoadError If loading the font fails.
	 *
	 * @param[in] path The path to the font file.
	 * @param[in] size The point size of the font.
	 * @param[in] dpi The target resolution of the font.
	 ******************************************************************************************************************/
	TTFont loadTTFontFile(const std::filesystem::path& path, int size, glm::uvec2 dpi = {72, 72});

	/// @cond IMPLEMENTATION
	DEFINE_BITMASK_OPERATORS(TTFont::Style);
	/// @endcond

	/// @}
} // namespace tr
