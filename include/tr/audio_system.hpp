/**
 * @file audio_system.hpp
 * @brief Provides an audio context class.
 */

#pragma once
#include "listener.hpp"
#include <memory>
#include <stdexcept>

struct ALCdevice;
struct ALCcontext;

namespace tr {
	/******************************************************************************************************************
	 * Error thrown when creating the audio system failed.
	 ******************************************************************************************************************/
	struct AudioSystemInitializationError : std::runtime_error {
		using runtime_error::runtime_error;
	};

	/******************************************************************************************************************
	 * Audio system manager required for using audio classes.
	 *
	 * Only one instance of the audio system can exist at a time, any further attemped constructions may trigger a
	 * failed assertion.
	 ******************************************************************************************************************/
	class AudioSystem {
	  public:
		/**************************************************************************************************************
		 * Initializes the audio system.
		 *
		 * @exception AudioSystemInitializationError If opening an audio device or creating the audio context failed.
		 **************************************************************************************************************/
		AudioSystem();

		/**************************************************************************************************************
		 * Deinitializes the audio system.
		 **************************************************************************************************************/
		~AudioSystem() noexcept;

		/**************************************************************************************************************
		 * The global audio listener.
		 **************************************************************************************************************/
		Listener listener;

	  private:
		struct DeviceDeleter {
			/// @private
			void operator()(ALCdevice* ptr) const noexcept;
		};
		struct ContextDeleter {
			/// @private
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
	 * A failed assertion may be triggered if the audio system is inactive.
	 *
	 * @return A reference to the audio system.
	 ******************************************************************************************************************/
	AudioSystem& audio() noexcept;
} // namespace tr
