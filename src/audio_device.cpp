#include "../include/audio_device.hpp"
#include <format>
#include <AL/alc.h>


tr::NamedAudioDeviceOpenError::NamedAudioDeviceOpenError(const char* name)
    : runtime_error { std::format("Failed to open audio device '{}'.", name) }
{}

std::vector<const char*> tr::availableAudioDevices()
{
    const ALCchar* strs { alcGetString(nullptr, ALC_DEVICE_SPECIFIER) };
    std::vector<const char*> names;
    for (const ALCchar* str = strs; *str != '\0'; str = str + std::strlen(str) + 1) {
        names.emplace_back(str);
    }
    return names;
}

tr::AudioDevice::AudioDevice()
    : _impl { alcOpenDevice(nullptr) }
{
    if (_impl == nullptr) {
        throw DefaultAudioDeviceOpenError {};
    }
}

tr::AudioDevice::AudioDevice(const char* name)
    : _impl { alcOpenDevice(name) }
{
    if (_impl == nullptr) {
        throw NamedAudioDeviceOpenError { name };
    }
}

void tr::AudioDevice::Deleter::operator()(ALCdevice* ptr) const noexcept
{
    alcCloseDevice(ptr);
}

const char* tr::AudioDevice::name() const noexcept
{
    return alcGetString(_impl.get(), ALC_DEVICE_SPECIFIER);
}