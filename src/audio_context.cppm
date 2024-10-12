/**
 * @file audio_context.cppm
 * @brief Provides an audio context class.
 */

module;
#include <AL/al.h>
#include <AL/alc.h>

export module tr:audio_context;

import std;
import :audio_device;
import :listener;

export namespace tr {
    /******************************************************************************************************************
	 * Error thrown when creating an audio context failed.
	 ******************************************************************************************************************/
    struct AudioContextCreationError : std::runtime_error {
        /**************************************************************************************************************
         * Constructs an audio context creation error.
         *
         * @param name The error string.
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
        struct Deleter { void operator()(ALCcontext* context) noexcept; /**< @private */ };
        std::unique_ptr<ALCcontext, Deleter> _impl;
    };
}

// IMPLEMENTATION

tr::AudioContextCreationError::AudioContextCreationError(const char* error)
    : runtime_error { std::format("Failed to create audio context ({})", error) }
{}

tr::AudioContext::AudioContext(AudioDevice& device)
    : _impl { alcCreateContext(device._impl.get(), nullptr) }
{
    if (_impl == nullptr || !alcMakeContextCurrent(_impl.get())) {
        throw AudioContextCreationError { alcGetString(device._impl.get(), alcGetError(device._impl.get())) };
    }
}

void tr::AudioContext::Deleter::operator()(ALCcontext* context) noexcept
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
}