/**
 * @file audio_device.cppm
 * @brief Provides an audio device class.
 */

module;
#include <AL/al.h>
#include <AL/alc.h>

export module tr:audio_device;

import std;

export namespace tr {
    /******************************************************************************************************************
	 * Error thrown when opening the default audio device failed.
	 ******************************************************************************************************************/
    struct DefaultAudioDeviceOpenError : std::exception {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        constexpr virtual const char* what() const noexcept;
    };

    /******************************************************************************************************************
	 * Error thrown when opening a named audio device failed.
	 ******************************************************************************************************************/
    struct NamedAudioDeviceOpenError : std::runtime_error {
        /**************************************************************************************************************
         * Constructs a device opening error.
         *
         * @param name The name of the device.
         *
         * @exception std::bad_alloc If constructing the error string failed.
	     **************************************************************************************************************/
        NamedAudioDeviceOpenError(const char* name);
    };


    /******************************************************************************************************************
	 * Gets a list of available audio devices.
     *
     * @exception std::bad_alloc If allocating the vector failed.
     *
     * @return A vector of audio device names.
	 ******************************************************************************************************************/
    std::vector<const char*> availableAudioDevices();


    /******************************************************************************************************************
	 * Physical audio device.
	 ******************************************************************************************************************/
    class AudioDevice {
    public:
        /**************************************************************************************************************
         * Opens the default audio device.
         *
         * @exception DefaultAudioDeviceOpenError If opening the device failed.
	     **************************************************************************************************************/
        AudioDevice();

        /**************************************************************************************************************
         * Opens a named audio device.
         *
         * @param name The name of the audio device to open.
         *
         * @exception NamedAudioDeviceOpenError If opening the device failed.
	     **************************************************************************************************************/
        AudioDevice(const char* name);


        /**************************************************************************************************************
         * Gets the name of the device.
         *
         * @return A C-string with the name of the device.
	     **************************************************************************************************************/
        const char* name() const noexcept;
    private:
        struct Deleter { void operator()(ALCdevice* device) const noexcept; /**< @private */ };
        std::unique_ptr<ALCdevice, Deleter> _impl;

        friend class AudioContext;
    };
}

// IMPLEMENTATION

constexpr const char* tr::DefaultAudioDeviceOpenError::what() const noexcept
{
    return "Failed to open default audio device.";
}

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

const char* tr::AudioDevice::name() const noexcept
{
    return alcGetString(_impl.get(), ALC_DEVICE_SPECIFIER);
}

void tr::AudioDevice::Deleter::operator()(ALCdevice* device) const noexcept
{
    alcCloseDevice(device);
}