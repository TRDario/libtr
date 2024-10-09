module;
#include <AL/al.h>
#include <AL/alc.h>

export module tr:audio_context;

import std;
import :audio_device;
import :listener;

export namespace tr {
    class AudioContext {
    public:
        AudioContext(AudioDevice& device);

        Listener listener;
    private:
        struct Deleter { void operator()(ALCcontext* context) noexcept; };
        std::unique_ptr<ALCcontext, Deleter> _impl;
    };
}

// IMPLEMENTATION

tr::AudioContext::AudioContext(AudioDevice& device)
{
    _impl.reset(alcCreateContext(device._impl.get(), nullptr));
    if (_impl == nullptr) {
        throw std::runtime_error { std::format("Failed to create audio context ({}).", alcGetString(device._impl.get(), alcGetError(device._impl.get()))) };
    }
    if (!alcMakeContextCurrent(_impl.get())) {
        throw std::runtime_error { std::format("Failed to make audio context current ({}).", alcGetString(device._impl.get(), alcGetError(device._impl.get()))) };
    }
}

void tr::AudioContext::Deleter::operator()(ALCcontext* context) noexcept
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
}