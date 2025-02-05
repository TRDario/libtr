#pragma once
#include "common.hpp"

namespace tr {
	/** @ingroup audio
	 *  @defgroup listener Listener
	 *  Global audio listener.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Listener orientation.
	 ******************************************************************************************************************/
	struct Orientation {
		/**************************************************************************************************************
		 * View vector.
		 **************************************************************************************************************/
		glm::vec3 view;

		/**************************************************************************************************************
		 * Up vector.
		 **************************************************************************************************************/
		glm::vec3 up;
	};

	/******************************************************************************************************************
	 * Global audio listener.
	 ******************************************************************************************************************/
	class Listener {
	  public:
		/**************************************************************************************************************
		 * Gets the master gain.
		 *
		 * @return The master gain factor.
		 **************************************************************************************************************/
		float gain() const noexcept;

		/**************************************************************************************************************
		 * Sets the master gain.
		 *
		 * @param gain The new gain factor (mul/div 2 = +/- 6.0dB).
		 **************************************************************************************************************/
		void setGain(float gain) noexcept;

		/**************************************************************************************************************
		 * Gets the position of the listener.
		 *
		 * @return The position of the listener.
		 **************************************************************************************************************/
		glm::vec3 position() const noexcept;

		/**************************************************************************************************************
		 * Sets the position of the listener.
		 *
		 * @param position The new position of the listener.
		 **************************************************************************************************************/
		void setPosition(const glm::vec3& position) noexcept;

		/**************************************************************************************************************
		 * Gets the velocity of the listener.
		 *
		 * @return The velocity of the listener.
		 **************************************************************************************************************/
		glm::vec3 velocity() const noexcept;

		/**************************************************************************************************************
		 * Sets the velocity of the listener.
		 *
		 * @param velocity The new velocity of the listener.
		 **************************************************************************************************************/
		void setVelocity(const glm::vec3& velocity) noexcept;

		/**************************************************************************************************************
		 * Gets the orientation of the listener.
		 *
		 * @return The orientation of the listener.
		 **************************************************************************************************************/
		Orientation orientation() const noexcept;

		/**************************************************************************************************************
		 * Sets the orientation of the listener.
		 *
		 * @param orientation The new orientation of the listener.
		 **************************************************************************************************************/
		void setOrientation(Orientation orientation) noexcept;

	  private:
		Listener() noexcept = default;

		friend class AudioSystem;
	};

	/// @}
} // namespace tr
