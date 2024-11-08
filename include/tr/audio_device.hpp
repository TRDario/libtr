/**
 * @file audio_device.hpp
 * @brief Provides an audio device class.
 */

#pragma once
#include <exception>
#include <stdexcept>
#include <memory>
#include <vector>

struct ALCdevice;

namespace tr {
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
        struct Deleter {
            /// @private
            void operator()(ALCdevice* ptr) const noexcept;
        };
        std::unique_ptr<ALCdevice, Deleter> _impl;

        friend class AudioContext;
    };
}

/// @cond IMPLEMENTATION

constexpr const char* tr::DefaultAudioDeviceOpenError::what() const noexcept
{
    return "Failed to open default audio device.";
}

/// @endcond
