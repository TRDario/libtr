#pragma once
#include "bitmap_format.hpp"
#include "geometry.hpp"
#include <optional>

namespace tr {
	/** @ingroup system
	 *  @defgroup display Display
	 *  Display querying functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Display mode datatype.
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
	 * Sentinel for getting the highest available display mode for a display.
	 ******************************************************************************************************************/
	inline constexpr int HIGHEST_MODE{-1};

	/******************************************************************************************************************
	 * Sentinel for getting the desktop display mode for a display.
	 ******************************************************************************************************************/
	inline constexpr int DESKTOP_MODE{-2};

	/******************************************************************************************************************
	 * Display information querying object.
	 ******************************************************************************************************************/
	class DisplayInfo {
	  public:
		/**************************************************************************************************************
		 * Queries the bounds of the display.
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
		 * @param[in] globalCoords The global coordinates to convert.
		 *
		 * @return The corresponding local coordinates.
		 **************************************************************************************************************/
		glm::ivec2 toLocalCoords(glm::ivec2 globalCoords) const noexcept;

		/**************************************************************************************************************
		 * Converts local coordinates on the display to global coordinates.
		 *
		 * @param[in] localCoords The local coordinates on the display to convert.
		 *
		 * @return The corresponding global coordinates.
		 **************************************************************************************************************/
		glm::ivec2 toGlobalCoords(glm::ivec2 localCoords) const noexcept;

		/**************************************************************************************************************
		 * Gets the number of display modes available on the display.
		 *
		 * @return The number of display modes available on the display.
		 **************************************************************************************************************/
		int displayModeCount() const noexcept;

		/**************************************************************************************************************
		 * Gets one of the display's display modes.
		 *
		 * @param[in] mode
		 * @parblock
		 * The index of the display mode, HIGHEST_MODE, or DESKTOP_MODE.
		 *
		 * @pre If an index, @em mode must be in the range [0, displayModeCount()).
		 * @endparblock
		 *
		 * @return The requested display modes.
		 **************************************************************************************************************/
		DisplayMode displayMode(int mode) const noexcept;

		/**************************************************************************************************************
		 * Tries to get the closest compatible display mode to the desired one.
		 *
		 * @param[in] mode The mode to look for.
		 *
		 * @return The closest compatible display mode or std::nullopt if no display is close enough.
		 **************************************************************************************************************/
		std::optional<DisplayMode> closestModeTo(const DisplayMode& mode) const noexcept;

	  private:
		int _id;

		DisplayInfo(int id) noexcept;

		friend DisplayInfo display(int display) noexcept;
	};

	/******************************************************************************************************************
	 * Gets the number of available displays.
	 *
	 * @return The number of available displays.
	 ******************************************************************************************************************/
	int displayCount() noexcept;

	/******************************************************************************************************************
	 * Gets a display querying object.
	 *
	 * @param display
	 * @parblock
	 * The index of the display to query.
	 *
	 * @pre @em display must be in the range [0, displayCount()).
	 * @endparblock
	 *
	 * @return A querying object for the desired display.
	 ******************************************************************************************************************/
	DisplayInfo display(int display = 0) noexcept;

	/******************************************************************************************************************
	 * Tries to determine a display from global coordinates.
	 *
	 * @param[in] globalCoords The global coordinates.
	 *
	 * @return A querying object for the display in which the global coordinates are located, or std::nullopt if the
	 *         coordinates are not in the bounds of any display.
	 ******************************************************************************************************************/
	std::optional<DisplayInfo> display(glm::ivec2 globalCoords) noexcept;

	/// @}
} // namespace tr
