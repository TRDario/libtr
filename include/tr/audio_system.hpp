#pragma once
#include "listener.hpp"
#include <memory>
#include <stdexcept>

struct ALCdevice;
struct ALCcontext;

namespace tr {
	/******************************************************************************************************************
	 * Error thrown when creating the audio system fails.
	 ******************************************************************************************************************/
	struct AudioSystemInitializationError : std::runtime_error {
		using runtime_error::runtime_error;
	};

	/******************************************************************************************************************
	 * Audio system manager required for using audio classes.
	 *
	 * Only one instance of the audio system is allowed to exist at a time.
	 ******************************************************************************************************************/
	class AudioSystem {
	  public:
		/**************************************************************************************************************
		 * Initializes the audio system.
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
	 * Gets a reference to the active audio system. This function cannot be called if the audio system isn't active.
	 *
	 * @return A reference to the audio system.
	 ******************************************************************************************************************/
	AudioSystem& audio() noexcept;
} // namespace tr
