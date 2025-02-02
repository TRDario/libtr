#pragma once
#include "common.hpp"

namespace tr {
	/** @ingroup system
	 *  @defgroup display Display
	 *  Display querying functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Gets the size of the display.
	 *
	 * @return The size of the display in pixels.
	 ******************************************************************************************************************/
	glm::ivec2 displaySize() noexcept;

	/******************************************************************************************************************
	 * Gets the refresh rate of the display.
	 *
	 * @return The refresh rate of the display in Hz.
	 ******************************************************************************************************************/
	int refreshRate() noexcept;

	/// @}
} // namespace tr
