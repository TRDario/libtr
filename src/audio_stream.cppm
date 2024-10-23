/**
 * @file audio_stream.cppm
 * @brief Provides an audio stream class.
 */

module;
#include <cassert>
#include <AL/alext.h>
#include <sndfile.h>

#ifdef _WIN32
#include <windows.h>
#endif

export module tr:audio_stream;

import std;
import boost;
import :audio_buffer;
import :audio_source;
import :chrono;
import :concepts;
import :function;
import :iostream;

export namespace tr {
	/******************************************************************************************************************
	 * Streamed audio source.
	 ******************************************************************************************************************/
    class AudioStream : private AudioSource {
	public:
		/**************************************************************************************************************
	     * Sentinel value representing the beginning of the streamed file.
	     **************************************************************************************************************/
		static constexpr SecondsF START { SecondsF::zero() };

		/**************************************************************************************************************
	     * Sentinel value representing the end of the streamed file.
	     **************************************************************************************************************/
		static constexpr SecondsF END { SecondsF::max() };


		/**************************************************************************************************************
	     * Default-constructs an audio stream.
		 *
		 * A stream constructed like this will be empty.
		 *
		 * @exception AudioSourceBadAlloc If allocating the audio source failed.
		 * @exception AudioBufferBadAlloc If allocating the audio buffers failed.
	     **************************************************************************************************************/
		AudioStream();

		/**************************************************************************************************************
	     * Loads an audio stream.
         *
		 * @exception AudioSourceBadAlloc If allocating the audio source failed.
		 * @exception AudioBufferBadAlloc If allocating the audio buffers failed.
         * @exception FileNotFound If the file was not found.
         * @exception UnsupportedAudioFile If the file format is unsupported or invalid.
         *
         * @param file The audio file to load.
	     **************************************************************************************************************/
		explicit AudioStream(const std::filesystem::path& path);

		/**************************************************************************************************************
	     * Move-constructs an audio stream.
		 *
		 * @param r The stream to move. It will be left empty afterwards.
	     **************************************************************************************************************/
		AudioStream(AudioStream&& r) noexcept;

		/**************************************************************************************************************
	     * Destroys the audio stream.
	     **************************************************************************************************************/
		~AudioStream() noexcept;


		/**************************************************************************************************************
	     * Move-assigns to an audio stream.
		 *
		 * @param r The stream to move. It will be left empty afterwards.
		 *
		 * @return A reference to the assigned audio stream.
	     **************************************************************************************************************/
		AudioStream& operator=(AudioStream&& r) noexcept;


        /**************************************************************************************************************
	     * Equality comparison operator.
	     **************************************************************************************************************/
		friend bool operator==(const AudioStream&, const AudioStream&) noexcept;


        using AudioSource::pitch;
		using AudioSource::setPitch;
		using AudioSource::gain;
		using AudioSource::setGain;
		using AudioSource::maxDistance;
		using AudioSource::setMaxDistance;
		using AudioSource::rolloff;
		using AudioSource::setRolloff;
		using AudioSource::referenceDistance;
		using AudioSource::setReferenceDistance;
		using AudioSource::minGain;
		using AudioSource::setMinGain;
		using AudioSource::maxGain;
		using AudioSource::setMaxGain;
		using AudioSource::outerConeGain;
		using AudioSource::setOuterConeGain;
		using AudioSource::innerConeWidth;
		using AudioSource::setInnerConeWidth;
		using AudioSource::outerConeWidth;
		using AudioSource::setOuterConeWidth;
		using AudioSource::position;
		using AudioSource::setPosition;
		using AudioSource::velocity;
		using AudioSource::setVelocity;
		using AudioSource::direction;
		using AudioSource::setDirection;
		using AudioSource::origin;
		using AudioSource::setOrigin;
		using AudioSource::state;


		/**************************************************************************************************************
	     * Gets whether the stream is empty.
         *
         * @return True if the stream is empty, and false otherwise.
	     **************************************************************************************************************/
        bool empty() const noexcept;


		/**************************************************************************************************************
	     * Gets whether the stream is looping.
         *
         * @return True if the stream is looping, and false otherwise.
	     **************************************************************************************************************/
		bool looping() const noexcept;

        /**************************************************************************************************************
	     * Sets whether the stream is looping.
         *
         * @param looping Whether the stream should loop.
	     **************************************************************************************************************/
		void setLooping(bool looping) noexcept;


		/**************************************************************************************************************
	     * Plays the audio stream.
		 *
		 * If the audio stream is empty, nothing happens.
		 *
		 * @exception AudioBufferBadAlloc If a stopped source is played and refreshing the internal buffers fails.
	     **************************************************************************************************************/
		void play();

        /**************************************************************************************************************
	     * Stops the audio stream.
		 *
		 * If the audio stream is empty, nothing happens.
	     **************************************************************************************************************/
		void stop() noexcept;


		/**************************************************************************************************************
	     * Gets the length of the audio stream.
         *
         * @return The length of the audio stream in seconds.
	     **************************************************************************************************************/
		SecondsF length() const noexcept;


        /**************************************************************************************************************
	     * Gets the stream's playback position within the file.
         *
         * @return The stream's playback position within the file in seconds.
	     **************************************************************************************************************/
		SecondsF offset() const noexcept;

		/**************************************************************************************************************
	     * Sets the stream's playback position within the file.
		 *
		 * If the audio stream is empty, nothing happens.
		 *
		 * @exception AudioBufferBadAlloc If refreshing the internal buffers fails.
         *
         * @param off The new offset in seconds. This value will be clamped to a valid range.
	     **************************************************************************************************************/
		void setOffset(SecondsF off);


		/**************************************************************************************************************
	     * Gets the stream's starting loop point.
         *
         * @return The stream's starting loop point within the file in seconds.
	     **************************************************************************************************************/
		SecondsF loopStart() const noexcept;

		/**************************************************************************************************************
	     * Gets the stream's ending loop point.
         *
         * @return The stream's ending loop point within the file in seconds.
	     **************************************************************************************************************/
		SecondsF loopEnd() const noexcept;

		/**************************************************************************************************************
	     * Sets the stream's loop points.
		 *
		 * If the audio stream is empty, nothing happens.
         *
         * @param start The starting loop point (special value: START). This value will be clamped to a valid range.
		 * @param end The ending loop point (special value: END). This value will be clamped to a valid range.
	     **************************************************************************************************************/
		void setLoopPoints(SecondsF start, SecondsF end) noexcept;
    private:
		// Extends an audio buffer with a tag indicating where its data starts in the audio file.
		struct Buffer : AudioBuffer {
			int startFileOffset; ///< @private
		};

        std::array<Buffer, 4> 		                        _buffers;
		std::unique_ptr<SNDFILE, FunctionCaller<&sf_close>> _file;
		int 					                            _length;
		int 					                            _channels;
		int 					                            _sampleRate;
		bool 					                            _looping;
		int 					                            _loopStart;
		int						                            _loopEnd;
		mutable std::mutex 		                            _mutex;

		void refillBuffer(Buffer& buffer);

		friend void audioStreamThread() noexcept;
    };
}

/// @cond IMPLEMENTATION

namespace tr {
    // The size of the buffers used in streams.
    inline constexpr int AUDIO_STREAM_BUFFER_SIZE { 16384 };

    // List of active streams.
    std::forward_list<std::reference_wrapper<AudioStream>> _audioStreams;
    // Mutex protecting _audioStreams.
    std::mutex _audioStreamsMutex;
    // Audio stream refreshing thread.
    std::thread _audioStreamsThread; 
	// Audio stream refreshing thread activity flag.
	std::atomic_bool _audioStreamsThreadExited { true };
    // Audio stream refreshing thread function.
    void audioStreamThread() noexcept;
}

void tr::audioStreamThread() noexcept
{
    try {
		_audioStreamsThreadExited = false;
		for (; !_audioStreams.empty(); std::this_thread::sleep_for(std::chrono::milliseconds(10))) {
			std::lock_guard listLock { _audioStreamsMutex };
			for (AudioStream& stream : _audioStreams) {
				std::lock_guard streamLock { stream._mutex };

				auto buffers { stream.unqueueBuffers() };
				for (auto& buffer : buffers) {
					stream.refillBuffer(*std::ranges::find(stream._buffers, buffer));
					stream.queueBuffer(buffer);
					if (sf_seek(stream._file.get(), 0, SF_SEEK_CUR) == stream._length) {
						break;
					}
				}
			}
		}
	} catch (...) {}
	_audioStreamsThreadExited = true;
}

tr::AudioStream::AudioStream()
	: _length { 0 }
	, _channels { 1 }
	, _sampleRate { 1 }
	, _looping { false }
	, _loopStart { 0 }
	, _loopEnd { 0 }
{}

tr::AudioStream::AudioStream(const std::filesystem::path& path)
{
	if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }

    SF_INFO info;
    #ifdef _WIN32
    _file.reset(sf_wchar_open(path.c_str(), SFM_READ, &info));
    #else
    _file.reset(sf_open(path.c_str(), SFM_READ, &info));
    #endif
	if (_file == nullptr) {
		throw FileOpenError { path };
	}

	_length = info.frames;
    _sampleRate = info.samplerate;
    _loopEnd = _length;
    _channels = info.channels;
    if (_channels > 2) {
        throw UnsupportedAudioFile { path };
    }
    else if (_channels == 2) {
        alSourcei(AudioSource::_id.get(), AL_DIRECT_CHANNELS_SOFT, 1);
    }

    if (info.format & (SF_FORMAT_OGG | SF_FORMAT_VORBIS | SF_FORMAT_FLOAT | SF_FORMAT_DOUBLE)) {
        sf_command(_file.get(), SFC_SET_SCALE_FLOAT_INT_READ, nullptr, true);
    }

	bool threadInactive { _audioStreams.empty() || _audioStreamsThreadExited };
	if (threadInactive) {
		if (_audioStreamsThread.joinable()) {
			_audioStreamsThread.join();
		}
		_audioStreams.push_front(*this);
		_audioStreamsThread = std::thread { audioStreamThread };
	}
	else {
		std::lock_guard lock { _audioStreamsMutex };
    	_audioStreams.push_front(*this);
	}
}

tr::AudioStream::AudioStream(AudioStream&& r) noexcept
{
	*this = std::move(r);
}

tr::AudioStream::~AudioStream() noexcept
{
	if (!empty()) {
        std::lock_guard lock { _audioStreamsMutex };
        _audioStreams.remove(*this);
        if (_audioStreams.empty() && _audioStreamsThread.joinable()) {
            _audioStreamsThread.join();
        }
    }
}

tr::AudioStream& tr::AudioStream::operator=(AudioStream&& r) noexcept
{
	if (!empty()) {
		std::ignore = AudioStream { std::move(*this) };
	}

	std::lock_guard lock { _audioStreamsMutex };
	std::swap((AudioSource&)(*this), (AudioSource&)(r));
	std::swap(_buffers, r._buffers);
	std::swap(_file, r._file);
	std::swap(_length, r._length);
	std::swap(_channels, r._channels);
	std::swap(_sampleRate, r._sampleRate);
	std::swap(_looping, r._looping);
	std::swap(_loopStart, r._loopStart);
	std::swap(_loopEnd, r._loopEnd);
	*std::ranges::find(_audioStreams, r) = *this;

	return *this;
}

bool tr::operator==(const AudioStream& l, const AudioStream& r) noexcept
{
	return &l == &r;
}

bool tr::AudioStream::empty() const noexcept
{
    return _file == nullptr;
}

bool tr::AudioStream::looping() const noexcept
{
    return _looping;
}

void tr::AudioStream::setLooping(bool looping) noexcept
{
    std::lock_guard lock { _mutex };
    _looping = looping;
}

void tr::AudioStream::play()
{
	if (empty()) return;

	std::lock_guard lock { _mutex };
    if (state() == AudioState::INITIAL || state() == AudioState::STOPPED) {
		AudioSource::setBuffer(std::nullopt);
		for (auto& buffer : _buffers) {
			refillBuffer(buffer);
			AudioSource::queueBuffer(buffer);
			if (sf_seek(_file.get(), 0, SF_SEEK_CUR) == _length) {
				break;
			}
		}
    }
    AudioSource::play();
}

void tr::AudioStream::stop() noexcept
{
	if (empty()) return;

	std::lock_guard lock { _mutex };
    AudioSource::stop();
    sf_seek(_file.get(), _loopStart, SF_SEEK_SET);
}

tr::SecondsF tr::AudioStream::length() const noexcept
{
    return SecondsF { float(_length) / _sampleRate };
}

tr::SecondsF tr::AudioStream::offset() const noexcept
{
	if (empty()) {
		return SecondsF::zero();
	}

	std::lock_guard lock { _mutex };
	const auto state { this->state() };

	if (state == AudioState::INITIAL || state == AudioState::STOPPED) {
		return SecondsF { sf_seek(_file.get(), 0, SF_SEEK_CUR) / float(_sampleRate) };
	}

	auto& buffer { *std::ranges::find(_buffers, AudioSource::buffer()) };
	return SecondsF { buffer.startFileOffset / float(_sampleRate) } + AudioSource::offset();
}

void tr::AudioStream::setOffset(SecondsF off)
{
	if (empty()) return;

	auto state { AudioSource::state() };
	{
		std::lock_guard lock { _mutex };

		if (off <= SecondsF::zero()) {
			sf_seek(_file.get(), 0, SF_SEEK_SET);
		}
		else if (off >= length()) {
			sf_seek(_file.get(), 0, SF_SEEK_END);
		}
		else {
			sf_seek(_file.get(), off.count() * _sampleRate, SF_SEEK_SET);
		}
	}

	AudioSource::stop();
    switch (state) {
    case AudioState::PLAYING:
		play();
        break;
    case AudioState::PAUSED:
		play();
        pause();
        break;
    case AudioState::INITIAL:
    case AudioState::STOPPED:
        break;
    }
}

tr::SecondsF tr::AudioStream::loopStart() const noexcept
{
    return SecondsF { float(_loopStart) / _sampleRate };
}

tr::SecondsF tr::AudioStream::loopEnd() const noexcept
{
    return SecondsF { float(_loopEnd) / _sampleRate };
}

void tr::AudioStream::setLoopPoints(SecondsF start, SecondsF end) noexcept
{
	if (empty()) return;

	start = std::clamp(start, START, length());
	end = std::clamp(end, START, length());
    assert(start < end);

    std::unique_lock lock { _mutex };
    _loopStart = start.count() * _sampleRate;
    _loopEnd = end.count() * _sampleRate;
}

void tr::AudioStream::refillBuffer(Buffer& buffer)
{
    static boost::container::static_vector<std::int16_t, AUDIO_STREAM_BUFFER_SIZE * 2> dataBuffer;
	buffer.startFileOffset = sf_seek(_file.get(), 0, SF_SEEK_CUR);

	auto samplesLeft { _loopEnd - buffer.startFileOffset };
	auto samplesToRead { std::min<int>(AUDIO_STREAM_BUFFER_SIZE, samplesLeft) };
	dataBuffer.resize(samplesToRead * _channels);
	sf_readf_short(_file.get(), dataBuffer.data(), samplesToRead);
	buffer.set(rangeBytes(dataBuffer), _channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, _sampleRate);

	if (samplesToRead <= AUDIO_STREAM_BUFFER_SIZE && looping()) {
		sf_seek(_file.get(), _loopStart, SF_SEEK_SET);
	}
}

/// @endcond