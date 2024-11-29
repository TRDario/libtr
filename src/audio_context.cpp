#include "../include/tr/audio_context.hpp"

#include <AL/alc.h>
#include <format>

namespace tr {
	ALCdevice*  openALCDeviceChecked();
	ALCcontext* createALCContextChecked(ALCdevice* device);
} // namespace tr

ALCdevice* tr::openALCDeviceChecked()
{
	auto ptr{alcOpenDevice(nullptr)};
	if (ptr == nullptr) {
		throw AudioContextCreationError{"Failed to open audio device."};
	}
	return ptr;
}

ALCcontext* tr::createALCContextChecked(ALCdevice* device)
{
	auto ptr{alcCreateContext(device, nullptr)};
	if (ptr == nullptr) {
		throw AudioContextCreationError{std::format("Failed to create audio context ({})", alcGetError(device))};
	}
	return ptr;
}

tr::AudioContext::AudioContext()
	: _device{openALCDeviceChecked()}, _context{createALCContextChecked(_device.get())}
{
}

void tr::AudioContext::DeviceDeleter::operator()(ALCdevice* ptr) const noexcept
{
	alcCloseDevice(ptr);
}

void tr::AudioContext::ContextDeleter::operator()(ALCcontext* ptr) const noexcept
{
	alcMakeContextCurrent(0);
	alcDestroyContext(ptr);
}
