/**
 * @file audio_context.hpp
 * @brief Provides an audio context class.
 */

#pragma once
#include "audio_device.hpp"
#include "listener.hpp"

struct ALCcontext;

namespace tr {
	/******************************************************************************************************************
	 * Error thrown when creating an audio context failed.
	 ******************************************************************************************************************/
	struct AudioContextCreationError : std::runtime_error {
		/**************************************************************************************************************
		 * Constructs an audio context creation error.
		 *
		 * @param error The error string.
		 *
		 * @exception std::bad_alloc If constructing the error string failed.
		 **************************************************************************************************************/
		AudioContextCreationError(const char* error);
	};

	/******************************************************************************************************************
	 * OpenAL audio context.
	 ******************************************************************************************************************/
	class AudioContext {
	  public:
		/**************************************************************************************************************
		 * Creates an audio context on an audio device.
		 *
		 * @exception AudioContextCreationError If creating the audio context failed.
		 *
		 * @param device The device to open the context on.
		 **************************************************************************************************************/
		AudioContext(AudioDevice& device);

		/**************************************************************************************************************
		 * The global audio listener.
		 **************************************************************************************************************/
		Listener listener;

	  private:
		struct Deleter {
			/// @private
			void operator()(ALCcontext* ptr) const noexcept;
		};

		std::unique_ptr<ALCcontext, Deleter> _impl;
	};
} // namespace tr
