module;
#include <AL/al.h>
#include <AL/alc.h>

export module tr:audio_device;

import std;

export namespace tr {
    std::vector<const char*> availableAudioDevices();

    class AudioDevice {
    public:
        AudioDevice() noexcept;
        AudioDevice(const char* name);

        const char* name() const noexcept;
    private:
        struct Deleter {
            void operator()(ALCdevice* device) const noexcept;
        };

        std::unique_ptr<ALCdevice, Deleter> _impl;

        friend class AudioContext;
    };
}

// IMPLEMENTATION

std::vector<const char*> tr::availableAudioDevices()
{
    const char* strs { alcGetString(nullptr, ALC_DEVICE_SPECIFIER) };
    std::vector<const char*> names;
    for (const char* str = strs; *str != '\0'; str = str + std::strlen(str) + 1) {
        names.emplace_back(str);
    }
    return names;
}

tr::AudioDevice::AudioDevice() noexcept
    : AudioDevice { nullptr }
{}

tr::AudioDevice::AudioDevice(const char* name)
    : _impl { alcOpenDevice(name) }
{
    if (_impl == nullptr) {
        throw std::runtime_error { std::format("Failed to open audio device '{}'.", name == nullptr ? "DEFAULT" : (const char*)(name)) };
    }
}

const char* tr::AudioDevice::name() const noexcept
{
    return alcGetString(_impl.get(), ALC_DEVICE_SPECIFIER);
}

void tr::AudioDevice::Deleter::operator()(ALCdevice* device) const noexcept
{
    alcCloseDevice(device);
}