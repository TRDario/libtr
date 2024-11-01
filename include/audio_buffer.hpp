/**
 * @file audio_buffer.hpp
 * @brief Provides an audio buffer class.
 */

#pragma once
#include <span>
#include "handle.hpp"
#include "iostream.hpp"


namespace tr {
    /******************************************************************************************************************
	 * Error thrown when allocating an audio buffer fails.
	 ******************************************************************************************************************/
    struct AudioBufferBadAlloc : std::bad_alloc {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
    };

    /******************************************************************************************************************
	 * Error thrown when an trying to load an unsupported audio file.
	 ******************************************************************************************************************/
    struct UnsupportedAudioFile : FileError {
        using FileError::FileError;

        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        virtual const char* what() const noexcept;
    };


    /******************************************************************************************************************
	 * Audio data format.
	 ******************************************************************************************************************/
	enum class AudioFormat {
        /**************************************************************************************************************
         * 1-channel, 8-bit audio.
         **************************************************************************************************************/
		MONO8 = 0x1100, // 8-bit mono audio.

        /**************************************************************************************************************
         * 2-channel, 8-bit audio.
         **************************************************************************************************************/
		STEREO8 = 0x1102, // 8-bit stereo audio.

        /**************************************************************************************************************
         * 1-channel, 16-bit audio.
         **************************************************************************************************************/
		MONO16 = 0x1101, // 16-bit mono audio.

        /**************************************************************************************************************
         * 2-channel, 16-bit audio.
         **************************************************************************************************************/
		STEREO16 = 0x1103  // 16-bit stereo audio.
	};


	/******************************************************************************************************************
	 * Non-owning audio buffer view.
	 ******************************************************************************************************************/
	class AudioBufferView {
	public:
        /**************************************************************************************************************
         * Equality comparison operator.
         **************************************************************************************************************/
		friend bool operator==(const AudioBufferView&, const AudioBufferView&) noexcept = default;

		/**************************************************************************************************************
         * Sets the data of the buffer.
         *
         * @exception AudioBufferBadAlloc If allocating the buffer failed.
         *
         * @param data A span over audio data.
         * @param format The format of the audio data.
         * @param frequency The frequency of the audio data.
         **************************************************************************************************************/
		void set(std::span<const std::byte> data, AudioFormat format, int frequency);
	protected:
        /// @cond IMPLEMENTATION
		unsigned int _id; // The OpenAL ID of the buffer.

		// Constructs a audio buffer view from an OpenAL id.
		AudioBufferView(unsigned int id) noexcept;
        /// @endcond

		friend class AudioBuffer;
		friend class AudioSource;
	};

	/******************************************************************************************************************
	 * Owning audio buffer.
     *
     * An audio context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	class AudioBuffer {
	public:
		/**************************************************************************************************************
         * Constructs an empty audio buffer.
         *
         * @exception AudioBufferBadAlloc If allocating the buffer failed.
         **************************************************************************************************************/
		AudioBuffer();
		
        /**************************************************************************************************************
         * Constructs an audio buffer containing audio data.
         *
         * @exception AudioBufferBadAlloc If allocating the buffer failed.
         *
         * @param data A span over audio data.
         * @param format The format of the audio data.
         * @param frequency The frequency of the audio data.
         **************************************************************************************************************/
		AudioBuffer(std::span<const std::byte> data, AudioFormat format, int frequency);

        /**************************************************************************************************************
         * Loads audio data from file to a buffer.
         *
         * @exception FileNotFound If the file wasn't found.
         * @exception FileOpenError If opening the file failed.
         * @exception UnsupportedAudioFile If the file is an unsupported or invalid format.
         * @exception std::bad_alloc If allocating a buffer for reading the data failed.
         * @exception AudioBufferBadAlloc If allocating the buffer failed.
         *
         * @param path The path to an audio file.
         **************************************************************************************************************/
		explicit AudioBuffer(const std::filesystem::path& path);


        /**************************************************************************************************************
         * Equality comparison operator.
         **************************************************************************************************************/
		friend bool operator==(const AudioBuffer&, const AudioBuffer&) noexcept = default;


        /**************************************************************************************************************
         * Casts the audio buffer to an audio buffer view.
         **************************************************************************************************************/
		operator AudioBufferView() const noexcept;


		/**************************************************************************************************************
         * Sets the data of the buffer.
         *
         * @exception AudioBufferBadAlloc If allocating the buffer failed.
         *
         * @param data A span over audio data.
         * @param format The format of the audio data.
         * @param frequency The frequency of the audio data.
         **************************************************************************************************************/
		void set(std::span<const std::byte> data, AudioFormat format, int frequency);
	private:
        struct Deleter {
            void operator()(unsigned int id) const noexcept;
        };
        
		Handle<unsigned int, 0, Deleter> _id;
	};
}

/// @cond IMPLEMENTATION

constexpr const char* tr::AudioBufferBadAlloc::what() const noexcept
{
    return "failed audio buffer allocation";
}

/// @endcond
