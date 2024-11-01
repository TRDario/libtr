#include "../include/audio_context.hpp"
#include <format>
#include <AL/alc.h>


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

void tr::AudioContext::Deleter::operator()(ALCcontext* ptr) const noexcept
{
    alcMakeContextCurrent(0);
    alcDestroyContext(ptr);
}