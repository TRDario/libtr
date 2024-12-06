#include "../include/tr/audio_system.hpp"
#include <AL/alc.h>
#include <format>

namespace tr {
	AudioSystem* _audioSystem{nullptr};

	ALCdevice*  openALCDeviceChecked();
	ALCcontext* createALCContextChecked(ALCdevice* device);
} // namespace tr

ALCdevice* tr::openALCDeviceChecked()
{
	auto ptr{alcOpenDevice(nullptr)};
	if (ptr == nullptr) {
		throw AudioSystemInitializationError{"Failed to open audio device."};
	}
	return ptr;
}

ALCcontext* tr::createALCContextChecked(ALCdevice* device)
{
	auto ptr{alcCreateContext(device, nullptr)};
	if (ptr == nullptr) {
		throw AudioSystemInitializationError{std::format("Failed to create audio context ({})", alcGetError(device))};
	}
	return ptr;
}

tr::AudioSystem::AudioSystem()
	: _device{openALCDeviceChecked()}, _context{createALCContextChecked(_device.get())}
{
	assert(!audioActive());
	_audioSystem = this;
}

tr::AudioSystem::~AudioSystem() noexcept
{
	_audioSystem = nullptr;
}

void tr::AudioSystem::DeviceDeleter::operator()(ALCdevice* ptr) const noexcept
{
	alcCloseDevice(ptr);
}

void tr::AudioSystem::ContextDeleter::operator()(ALCcontext* ptr) const noexcept
{
	alcMakeContextCurrent(0);
	alcDestroyContext(ptr);
}

bool tr::audioActive() noexcept
{
	return _audioSystem != nullptr;
}

tr::AudioSystem& tr::audio() noexcept
{
	assert(audioActive());
	return *_audioSystem;
}