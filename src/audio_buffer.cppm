/**
 * @file audio_buffer.cppm
 * @brief Provides an audio buffer class.
 */

module;
#include <AL/al.h>
#include <sndfile.h>

#ifdef _WIN32
#include <windows.h>
#endif

export module tr:audio_buffer;

import std;
import :handle;
import :iostream;
import :ranges;

export namespace tr {
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
		ALuint _id; // The OpenAL ID of the buffer.

		// Constructs a audio buffer view from an OpenAL id.
		AudioBufferView(ALuint id) noexcept;
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
		struct Deleter { void operator()(ALuint id) noexcept; /**< @private */ };
		Handle<ALuint, 0, Deleter> _id;
	};
}

/// @cond IMPLEMENTATION

constexpr const char* tr::AudioBufferBadAlloc::what() const noexcept
{
    return "failed audio buffer allocation";
}

tr::AudioBufferView::AudioBufferView(ALuint id) noexcept
    : _id { id }
{}

void tr::AudioBufferView::set(std::span<const std::byte> data, AudioFormat format, int frequency)
{
    alBufferData(_id, ALenum(format), data.data(), data.size(), frequency);
    if (alGetError() == AL_OUT_OF_MEMORY) {
        throw AudioBufferBadAlloc {};
    }
}

tr::AudioBuffer::AudioBuffer()
{
    ALuint id;
    alGenBuffers(1, &id);
    if (alGetError() == AL_OUT_OF_MEMORY) {
        throw AudioBufferBadAlloc {};
    }
    _id.reset(id);
}

tr::AudioBuffer::AudioBuffer(std::span<const std::byte> data, AudioFormat format, int frequency)
{
    set(data, format, frequency);
}

tr::AudioBuffer::AudioBuffer(const std::filesystem::path& path)
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }

    SF_INFO info;
    #ifdef _WIN32
    std::unique_ptr<SNDFILE, decltype(&sf_close)> file { sf_wchar_open(path.c_str(), SFM_READ, &info), sf_close };
    #else
    std::unique_ptr<SNDFILE, decltype(&sf_close)> file { sf_open(path.c_str(), SFM_READ, &info), sf_close };
    #endif

    if (file == nullptr) {
        throw FileOpenError { path };
    }
    if (info.channels > 2) {
        throw UnsupportedAudioFile { path };
    }
    if (info.format & (SF_FORMAT_OGG | SF_FORMAT_VORBIS | SF_FORMAT_FLOAT | SF_FORMAT_DOUBLE)) {
        sf_command(file.get(), SFC_SET_SCALE_FLOAT_INT_READ, nullptr, true);
    }

    std::vector<std::int16_t> data(info.frames * info.channels);
    sf_readf_short(file.get(), data.data(), info.frames);
    set(rangeBytes(data), info.channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, info.samplerate);
}

void tr::AudioBuffer::Deleter::operator()(ALuint id) noexcept
{
    alDeleteBuffers(1, &id);
}

const char* tr::UnsupportedAudioFile::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "Unsupported audio file: '{}'", path());
    return str.c_str();
}

tr::AudioBuffer::operator AudioBufferView() const noexcept
{
    return _id.get();
}

void tr::AudioBuffer::set(std::span<const std::byte> data, AudioFormat format, int frequency)
{
    AudioBufferView(*this).set(data, format, frequency);
}

/// @endcond