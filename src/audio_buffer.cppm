module;
#include <AL/al.h>
#include <sndfile.h>

export module tr:audio_buffer;

import std;
import :handle;
import :integer;
import :iostream;
import :ranges;

#ifdef _WIN32
#include <windows.h>
#endif

export namespace tr {
    struct AudioBufferBadAlloc : std::bad_alloc {
        AudioBufferBadAlloc() noexcept = default;
		constexpr virtual const char* what() const noexcept { return "failed audio buffer allocation"; };
    };

	enum class AudioFormat {
		MONO8    = 0x1100, // 8-bit mono audio.
		STEREO8  = 0x1102, // 8-bit stereo audio.
		MONO16   = 0x1101, // 16-bit mono audio.
		STEREO16 = 0x1103  // 16-bit stereo audio.
	};

	// Non-owning audio buffer view.
	class AudioBufferView {
	public:
		friend bool operator==(const AudioBufferView&, const AudioBufferView&) noexcept = default;

		// Gets the frequency of the buffer data.
		int frequency() const noexcept;
		// Gets the bit depth of the buffer.
		int depth() const noexcept;
		// Gets the number of channels in the buffer.
		int channels() const noexcept;
		// Gets the size of the buffer in bytes.
		int size() const noexcept;

		// Sets the data of the buffer.
		void set(std::span<const Byte> data, AudioFormat format, int frequency);
	protected:
		/// The OpenAL ID of the buffer.
		unsigned int _id;

		// Constructs a audio buffer view from an OpenAL id.
		AudioBufferView(unsigned int id) noexcept;

		friend class AudioBuffer;
		friend class AudioSource;
	};

	// Owning audio buffer class.
	class AudioBuffer {
	public:
		// Constructs an empty audio buffer.
		AudioBuffer();
		// Constructs an initialized audio buffer.
		AudioBuffer(std::span<const Byte> data, AudioFormat format, int frequency);

		friend bool operator==(const AudioBuffer&, const AudioBuffer&) noexcept = default;

		operator AudioBufferView() const noexcept;

		// Gets the frequency of the buffer data.
		int frequency() const noexcept;
		// Gets the bit depth of the buffer.
		int depth() const noexcept;
		// Gets the number of channels in the buffer.
		int channels() const noexcept;
		// Gets the size of the buffer in bytes.
		int size() const noexcept;

		// Sets the data of the buffer.
		void set(std::span<const Byte> data, AudioFormat format, int frequency);
	private:
		struct Deleter { void operator()(unsigned int id) noexcept; };
		Handle<unsigned int, 0, Deleter> _id;
	};

    struct UnsupportedAudioFile : FileError {
        using FileError::FileError;
        virtual const char* what() const noexcept;
    };

    AudioBuffer loadAudio(const std::filesystem::path& path);
}

// IMPLEMENTATION

tr::AudioBufferView::AudioBufferView(unsigned int id) noexcept
    : _id { id }
{}

int tr::AudioBufferView::frequency() const noexcept
{
    int value;
    alGetBufferi(_id, AL_FREQUENCY, &value);
    return value;
}

int tr::AudioBufferView::depth() const noexcept
{
    int value;
    alGetBufferi(_id, AL_BITS, &value);
    return value;
}

int tr::AudioBufferView::channels() const noexcept
{
    int value;
    alGetBufferi(_id, AL_CHANNELS, &value);
    return value;
}

int tr::AudioBufferView::size() const noexcept
{
    int value;
    alGetBufferi(_id, AL_SIZE, &value);
    return value;
}

void tr::AudioBufferView::set(std::span<const Byte> data, AudioFormat format, int frequency)
{
    alBufferData(_id, ALenum(format), data.data(), data.size(), frequency);
    if (alGetError() == AL_OUT_OF_MEMORY) {
        throw AudioBufferBadAlloc {};
    }
}

tr::AudioBuffer::AudioBuffer()
{
    unsigned int id;
    alGenBuffers(1, &id);
    if (alGetError() == AL_OUT_OF_MEMORY) {
        throw AudioBufferBadAlloc {};
    }
    _id.reset(id);
}

tr::AudioBuffer::AudioBuffer(std::span<const Byte> data, AudioFormat format, int frequency)
{
    set(data, format, frequency);
}

void tr::AudioBuffer::Deleter::operator()(unsigned int id) noexcept
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

tr::AudioBuffer tr::loadAudio(const std::filesystem::path& path)
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }

    SF_INFO info;
    #ifdef _WIN32
    SNDFILE* file { sf_open_fd(_wopen(path.c_str(), _O_RDONLY, 0), SFM_READ, &info, true) };
    #else
    SNDFILE* file { sf_open(path.c_str(), SFM_READ, &info) };
    #endif

    if (file == nullptr) {
        throw FileOpenError { path };
    }
    if (info.channels > 2) {
        throw UnsupportedAudioFile { path };
    }
    if (info.format & (SF_FORMAT_OGG | SF_FORMAT_VORBIS | SF_FORMAT_FLOAT | SF_FORMAT_DOUBLE)) {
        sf_command(file, SFC_SET_SCALE_FLOAT_INT_READ, nullptr, true);
    }

    std::vector<Si16> data(info.frames * info.channels);
    sf_readf_short(file, data.data(), info.frames);
    sf_close(file);
    return AudioBuffer(asBytes(data), info.channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, info.samplerate);
}

tr::AudioBuffer::operator AudioBufferView() const noexcept
{
    return _id.get();
}

int tr::AudioBuffer::frequency() const noexcept
{
    return AudioBufferView(*this).frequency();
}

int tr::AudioBuffer::depth() const noexcept
{
    return AudioBufferView(*this).depth();
}

int tr::AudioBuffer::channels() const noexcept
{
    return AudioBufferView(*this).channels();
}

int tr::AudioBuffer::size() const noexcept
{
    return AudioBufferView(*this).size();
}

void tr::AudioBuffer::set(std::span<const Byte> data, AudioFormat format, int frequency)
{
    AudioBufferView(*this).set(data, format, frequency);
}