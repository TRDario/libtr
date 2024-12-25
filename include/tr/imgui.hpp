#pragma once
#include "event.hpp"
#include "sampler.hpp"
#include "texture.hpp"

namespace tr {
	/** @ingroup misc
	 *  @defgroup imgui Dear ImGui Integration
	 *  Functionality for Dear ImGui integration.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Namespace containing functionality for integrating Dear ImGui with libtr.
	 ******************************************************************************************************************/
	namespace ImGui {
		/**************************************************************************************************************
		 * Initializes the Dear ImGui backends needed for work with libtr.
		 **************************************************************************************************************/
		void initialize();

		/**************************************************************************************************************
		 * Prepares the Dear ImGui backends needed for work with libtr for a new frame.
		 **************************************************************************************************************/
		void newFrame();

		/**************************************************************************************************************
		 * Gets the Dear ImGui texture ID for a libtr texture.
		 *
		 * @param[in] texture The texture to get an ID for.
		 *
		 * @return A corresponding Dear ImGui texture ID.
		 **************************************************************************************************************/
		std::uint64_t getTextureID(const Texture& texture) noexcept;

		/**************************************************************************************************************
		 * Sets the sampling mode used for a texture by Dear ImGui.
		 *
		 * @param[in] texture The texture to set the sampling mode for.
		 * @param[in] minFilter The minifying filter to use.
		 * @param[in] magFilter The magniifying filter to use.
		 **************************************************************************************************************/
		void setTextureSampling(const Texture& texture, MinFilter minFilter, MagFilter magFilter) noexcept;

		/**************************************************************************************************************
		 * Processes an event for Dear ImGui.
		 *
		 * @param[in] event The event to process.
		 **************************************************************************************************************/
		void processEvent(const Event& event);

		/**************************************************************************************************************
		 * Draws a Dear ImGui frame.
		 **************************************************************************************************************/
		void draw();

		/**************************************************************************************************************
		 * Shuts the Dear ImGui backends needed for work with libtr down.
		 **************************************************************************************************************/
		void shutdown();
	} // namespace ImGui

	/// @}
} // namespace tr