#pragma once
#include "listener.hpp"
#include <memory>
#include <stdexcept>

struct ALCdevice;
struct ALCcontext;

namespace tr {
	/** @defgroup audio Audio
	 *  The audio subsystem.
	 *
	 *  An instance of AudioSystem must be created before any other functionality from this section can be used.
	 *  @{
	 *  @}
	 */

	/** @ingroup audio
	 *  @defgroup audio_system Audio System
	 *  Audio device/context manager and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when creating the audio system fails.
	 ******************************************************************************************************************/
	struct AudioSystemInitializationError : std::runtime_error {
		using runtime_error::runtime_error;
	};

	/******************************************************************************************************************
	 * Audio device and context manager required for using audio classes.
	 *
	 * Only one instance of the audio system is allowed to exist at a time.
	 ******************************************************************************************************************/
	class AudioSystem {
	  public:
		/**************************************************************************************************************
		 * Initializes the audio system.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception AudioSystemInitializationError If initializing the audio system fails.
		 **************************************************************************************************************/
		AudioSystem();

		~AudioSystem() noexcept;

		/**************************************************************************************************************
		 * The global audio listener.
		 **************************************************************************************************************/
		Listener listener;

	  private:
		struct DeviceDeleter {
			void operator()(ALCdevice* ptr) const noexcept;
		};
		struct ContextDeleter {
			void operator()(ALCcontext* ptr) const noexcept;
		};

		std::unique_ptr<ALCdevice, DeviceDeleter>   _device;
		std::unique_ptr<ALCcontext, ContextDeleter> _context;
	};

	/******************************************************************************************************************
	 * Gets whether the audio system is active.
	 *
	 * @return True if the audio system is active, and false otherwise.
	 ******************************************************************************************************************/
	bool audioActive() noexcept;

	/******************************************************************************************************************
	 * Gets a reference to the active audio system.
	 *
	 * @pre This function cannot be called if the audio system isn't active.
	 *
	 * @return A reference to the audio system.
	 ******************************************************************************************************************/
	AudioSystem& audio() noexcept;

	/// @}
} // namespace tr
