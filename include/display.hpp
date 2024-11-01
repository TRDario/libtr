/**
 * @file display.hpp
 * @brief Provides ways of getting display information.
 */

#pragma once
#include <optional>
#include "bitmap_format.hpp"
#include "geometry.hpp"

namespace tr {
	/******************************************************************************************************************
     * Display mode information.
     ******************************************************************************************************************/
	struct DisplayMode {
		/**************************************************************************************************************
		 * The size of the display mode.
		 **************************************************************************************************************/
		glm::ivec2 size;

		/**************************************************************************************************************
		 * The pixel format of the display mode.
		 **************************************************************************************************************/
		BitmapFormat format;

		/**************************************************************************************************************
		 * The refresh rate of the display mode.
		 **************************************************************************************************************/
		int refreshRate;
	};


	/******************************************************************************************************************
     * Display information.
     ******************************************************************************************************************/
	class DisplayInfo {
	public:
		/**************************************************************************************************************
		 * Creates an information object for the default display (see: DEFAULT_DISPLAY).
		 **************************************************************************************************************/
		constexpr DisplayInfo() noexcept : _id { 0 } {};

		/**************************************************************************************************************
		 * Creates an information object for a display.
		 *
		 * @param id The ID of the display. Must be in the range (0, displayCount()), otherwise a failed assertion
		 *			 may be raised.
		 **************************************************************************************************************/
		DisplayInfo(int id) noexcept;

		/**************************************************************************************************************
		 * Tries to return a display information object from global coordinates.
		 *
		 * @param globalCoords The global coordinates.
		 *
		 * @return std::nullopt if the coordinates are not in the bounds of any display, or display information of the
		 *         display the coordinates are in otherwise.
		 **************************************************************************************************************/
		static std::optional<DisplayInfo> fromCoords(glm::ivec2 globalCoords) noexcept;


		/**************************************************************************************************************
		 * Gets the bounds of the display.
		 *
		 * @return The bounds of the display.
		 **************************************************************************************************************/
		RectI2 bounds() const noexcept;

		/**************************************************************************************************************
		 * Creates a value usable by a Window constructor for centering a window on the display.
		 *
		 * @return A sentinel value used by the window's positioning function to center itself on the display.
		 **************************************************************************************************************/
		glm::ivec2 centeredPos() const noexcept;

		/**************************************************************************************************************
		 * Converts global coordinates to local coordinates on the display.
		 *
		 * @param globalCoords The global coordinates to convert.
		 *
		 * @return The corresponding local coordinates.
		 **************************************************************************************************************/
		glm::ivec2 toLocalCoords(glm::ivec2 globalCoords) const noexcept;

		/**************************************************************************************************************
		 * Converts local coordinates on the display to global coordinates.
		 *
		 * @param localCoords The local coordinates on the display to convert.
		 *
		 * @return The corresponding global coordinates.
		 **************************************************************************************************************/
		glm::ivec2 toGlobalCoords(glm::ivec2 localCoords) const noexcept;


		/**************************************************************************************************************
		 * Gets the display's display modes.
		 *
		 * @exception std::bad_alloc If allocating the vector failed.
		 *
		 * @return The modes of the display.
		 **************************************************************************************************************/
		std::vector<DisplayMode> modes() const;

		/**************************************************************************************************************
		 * Gets the display's highest-ordered display mode.
		 *
		 * @return The display's highest-ordered display mode.
		 **************************************************************************************************************/
		DisplayMode highestMode() const noexcept;
		
		/**************************************************************************************************************
		 * Gets the display's desktop display mode.
		 *
		 * @return The display's desktop display mode.
		 **************************************************************************************************************/
		DisplayMode desktopMode() const noexcept;

		/**************************************************************************************************************
		 * Tries to get the closest compatible display mode to the desired one.
		 *
		 * @param mode The mode to look for.
		 *
		 * @return The closest compatible display mode or std::nullopt if no display is close enough.
		 **************************************************************************************************************/
		std::optional<DisplayMode> closestModeTo(const DisplayMode& mode) const noexcept;
	private:
		int _id;
	};

	/******************************************************************************************************************
     * Default display information.
     ******************************************************************************************************************/
	inline constexpr DisplayInfo DEFAULT_DISPLAY {};

	/******************************************************************************************************************
     * Gets the number of available displays.
	 *
	 * @return The number of available displays.
     ******************************************************************************************************************/
	int displayCount() noexcept;
}