/**
 * @file audio_context.hpp
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
	 * Error thrown when creating an audio context failed.
	 ******************************************************************************************************************/
	struct AudioContextCreationError : std::runtime_error {
		using runtime_error::runtime_error;
	};

	/******************************************************************************************************************
	 * OpenAL audio context.
	 ******************************************************************************************************************/
	class AudioContext {
	  public:
		/**************************************************************************************************************
		 * Creates an audio context.
		 *
		 * @exception AudioContextCreationError If opening an audio device or creating the audio context failed.
		 **************************************************************************************************************/
		AudioContext();

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
} // namespace tr
