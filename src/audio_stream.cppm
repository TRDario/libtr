module;
#include <cassert>
#include <AL/alext.h>
#include <sndfile.h>

export module tr:audio_stream;

import std;
import boost;
import :audio_buffer;
import :audio_source;
import :chrono;
import :iostream;

#ifdef _WIN32
#include <windows.h>
#endif

export namespace tr {
	// Class representing a streamed audio source.
	class AudioStream : private AudioSource {
	public:
		// Sentinel value representing the beginning of the streamed file.
		static constexpr SecondsF BEG { SecondsF::zero() };
		// Sentinel value representing the beginning of the streamed file.
		static constexpr int BEG_SAMPLE { 0 };
		// Sentinel value representing the end of the streamed file.
		static constexpr SecondsF END { SecondsF::max() };
		// Sentinel value representing the end of the streamed file.
		static constexpr int END_SAMPLE { std::numeric_limits<int>::max() };

		// Loads an audio stream or throws an error otherwise.
		explicit AudioStream(const std::filesystem::path& path);
		AudioStream(AudioStream&& rhs) noexcept;
		~AudioStream() noexcept;

		AudioStream& operator=(AudioStream&& rhs) noexcept;

		friend bool operator==(const AudioStream& lhs, const AudioStream& rhs) noexcept;

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

		bool looping() const noexcept;
		void setLooping(bool looping);

		void play();
		void stop();

		SecondsF length() const noexcept;
		int lengthSamples() const noexcept;

		// Gets the stream's playback position within the file.
		SecondsF offset() const;
		// Gets the stream's playback postition within the file.
		int offsetSamples() const;
		// Sets the stream's playback position within the file.
		void setOffset(SecondsF off);
		// Sets the stream's playback position within the file.
		void setOffset(int off);

		// Gets the starting loop point.
		SecondsF loopStart() const noexcept;
		// Gets the ending loop point.
		SecondsF loopEnd() const noexcept;
		// Gets the starting loop point.
		int loopStartSamples() const noexcept;
		// Gets the ending loop point.
		int loopEndSamples() const noexcept;
		// Sets the loop points.
		void setLoopPoints(SecondsF start, SecondsF end);
		// Sets the loop points.
		void setLoopPoints(int start, int end);
	private:
        struct LoopPoint {
            int bufferOffset;
            int fileOffset;
        };
		struct Buffer : AudioBuffer {
			std::vector<LoopPoint> loopPoints;
		};
		struct FileCloser {
            void operator()(SNDFILE* file) noexcept;
        };

		std::array<Buffer, 4> 		         _buffers;
		std::unique_ptr<SNDFILE, FileCloser> _file;
		int 					             _length;
		int 					             _channels;
		int 					             _sampleRate;
		bool 					             _looping;
		int 					             _loopStart;
		int						             _loopEnd;
		mutable std::mutex 		             _mutex;

		// Refills the internal stream buffers.
		void refillBuffers(std::span<AudioBufferView> buffers);
		void refillAllBuffers();
		void refillFreeBuffers();
		// Clears the audio stream's internals.
		void clear();

        friend void audioStreamThread();
	};
}

// IMPLEMENTATION

namespace tr {
    inline constexpr int AUDIO_STREAM_BUFFER_SIZE { 8192 };                     // The size of the buffers used in streams.
    std::forward_list<std::reference_wrapper<AudioStream>> _audioStreams;       // List of active streams.
    std::mutex                                             _audioStreamsMutex;  // Mutex protecting g_audioStreams.
    std::thread                                            _audioStreamsThread; // Audio stream refreshing thread.
    void audioStreamThread();
}

void tr::audioStreamThread()
{
    for (; !_audioStreams.empty(); std::this_thread::sleep_for(std::chrono::milliseconds(10))) {
        std::unique_lock lock { _audioStreamsMutex };
        std::ranges::for_each(_audioStreams, &tr::AudioStream::refillFreeBuffers);
    }
}

tr::AudioStream::AudioStream(const std::filesystem::path& path)
    : _looping { false }
    , _loopStart { BEG_SAMPLE }
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }

    SF_INFO info;
    #ifdef _WIN32
    _file.reset(sf_open_fd(_wopen(path.c_str(), _O_RDONLY, 0), SFM_READ, &info, true));
    #else
    _file.reset(sf_open(path.c_str(), SFM_READ, &info));
    #endif
    
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
    
    std::unique_lock lock { _audioStreamsMutex };
    bool threadInactive { _audioStreams.empty() };
    _audioStreams.push_front(*this);
    if (threadInactive) {
        _audioStreamsThread = std::thread { audioStreamThread };
    }
}

tr::AudioStream::AudioStream(AudioStream&& rhs) noexcept
    : AudioSource { std::move((AudioSource&)(rhs)) }
    , _buffers { std::move(rhs._buffers) }
    , _file { std::move(rhs._file) }
    , _length { rhs._length }
    , _channels { rhs._channels }
    , _sampleRate { rhs._sampleRate }
    , _looping { rhs._looping }
    , _loopStart { rhs._loopStart } 
    , _loopEnd { rhs._loopEnd }
{
    std::unique_lock lock { _audioStreamsMutex };
    *std::ranges::find(_audioStreams, rhs) = *this;
}

tr::AudioStream::~AudioStream() noexcept
{
    clear();
}

void tr::AudioStream::FileCloser::operator()(SNDFILE* file) noexcept
{
    sf_close(file);
}

tr::AudioStream& tr::AudioStream::operator=(AudioStream&& rhs) noexcept
{
    clear();
    std::unique_lock lock { rhs._mutex };
    _buffers = std::move(rhs._buffers);
    (AudioSource&)(*this) = std::move((AudioSource&)(rhs));
    _file = std::move(rhs._file);
    _length = rhs._length;
    _channels = rhs._channels;
    _sampleRate = rhs._sampleRate;
    _looping = rhs._looping;
    _loopStart = rhs._loopStart;
    _loopEnd = rhs._loopEnd;

    std::unique_lock lock2 { _audioStreamsMutex };
    *std::ranges::find(_audioStreams, rhs) = *this;

    return *this;
}

bool tr::operator==(const AudioStream& lhs, const AudioStream& rhs) noexcept
{
    return (const AudioSource&)(lhs) == (const AudioSource&)(rhs);
}

bool tr::AudioStream::looping() const noexcept
{
    return _looping;
}

void tr::AudioStream::setLooping(bool looping)
{
    std::unique_lock lock { _mutex };
    _looping = looping;
}

void tr::AudioStream::play()
{
    if (state() == AudioState::INITIAL || state() == AudioState::STOPPED) {
        refillAllBuffers();
    }
    AudioSource::play();
}

void tr::AudioStream::stop()
{
    AudioSource::stop();
    std::unique_lock lock { _mutex };
    sf_seek(_file.get(), _loopStart, SF_SEEK_SET);
}

tr::SecondsF tr::AudioStream::length() const noexcept
{
    return SecondsF(float(lengthSamples()) / _sampleRate);
}

int tr::AudioStream::lengthSamples() const noexcept
{
    return _length;
}

tr::SecondsF tr::AudioStream::offset() const
{
    return SecondsF(float(offsetSamples()) / _sampleRate);
}

int tr::AudioStream::offsetSamples() const
{
    std::unique_lock lock { _mutex };
    // There is an edge case where this returns end() so just fetch until it isn't.
    auto buffer { _buffers.end() };
    while (buffer == _buffers.end()) {
        buffer = std::ranges::find(_buffers, AudioSource::buffer());
    }
    auto bufferOffset { AudioSource::offsetSamples() - AUDIO_STREAM_BUFFER_SIZE * AudioSource::processedBuffers() };
    auto& lastLoopPoint { *std::ranges::find_if(buffer->loopPoints | std::views::reverse, [&] (auto& l) { return l.bufferOffset <= bufferOffset; }) };
    return lastLoopPoint.fileOffset + bufferOffset - lastLoopPoint.bufferOffset;
}

void tr::AudioStream::setOffset(SecondsF off)
{
    setOffset(off.count() * _sampleRate);
}

void tr::AudioStream::setOffset(int off)
{
    auto state { AudioSource::state() };
    std::unique_lock lock { _mutex };
    if (state == AudioState::INITIAL || state == AudioState::STOPPED) {
        AudioSource::play();
    }
    AudioSource::stop();

    sf_seek(_file.get(), off, SF_SEEK_SET);

    switch (state) {
    case AudioState::PLAYING:
        play();
        break;
    case AudioState::PAUSED:
        pause();
        break;
    case AudioState::INITIAL:
    case AudioState::STOPPED:
        break;
    }
}

tr::SecondsF tr::AudioStream::loopStart() const noexcept
{
    return SecondsF(float(loopStartSamples()) / _sampleRate);
}

tr::SecondsF tr::AudioStream::loopEnd() const noexcept
{
    return SecondsF(float(loopEndSamples()) / _sampleRate);
}

int tr::AudioStream::loopStartSamples() const noexcept
{
    return _loopStart;
}

int tr::AudioStream::loopEndSamples() const noexcept
{
    return _loopEnd;
}

void tr::AudioStream::setLoopPoints(SecondsF start, SecondsF end)
{
    setLoopPoints(start.count() * _sampleRate, end.count() * _sampleRate);
}

void tr::AudioStream::setLoopPoints(int start, int end)
{
    if (end == END_SAMPLE) {
        end = _length;
    }
    assert(start < end && start >= 0 && end <= _length);

    std::unique_lock lock { _mutex };
    _loopStart = start;
    _loopEnd = end;
}

void tr::AudioStream::refillAllBuffers()
{
    std::unique_lock lock { _mutex };
    std::ignore = AudioSource::unqueueBuffers();
    std::array<AudioBufferView, 4> buffers { { get<0>(_buffers), get<1>(_buffers), get<2>(_buffers), get<3>(_buffers) } };
    return refillBuffers(buffers);
}

void tr::AudioStream::refillFreeBuffers()
{
    std::unique_lock lock { _mutex };
    auto buffers { AudioSource::unqueueBuffers() };
    return refillBuffers(buffers);
}

void tr::AudioStream::refillBuffers(std::span<AudioBufferView> buffers)
{
    static boost::container::static_vector<std::int16_t, AUDIO_STREAM_BUFFER_SIZE * 2> dataBuffer;
    boost::container::static_vector<AudioBufferView, 4> queueBuffers;
    for (auto& buffer : buffers) {
        auto& loopPoints { std::ranges::find(_buffers, buffer)->loopPoints };
        auto fileOffset { sf_seek(_file.get(), 0, SF_SEEK_CUR) };
        auto samplesLeft { _loopEnd - fileOffset };

        queueBuffers.emplace_back(buffer);
        loopPoints.clear();
        loopPoints.emplace_back(0, int(fileOffset));

        if (samplesLeft >= AUDIO_STREAM_BUFFER_SIZE || !looping()) {
            int samples { std::min<int>(AUDIO_STREAM_BUFFER_SIZE, samplesLeft) };
            dataBuffer.resize(samples * _channels);
            sf_readf_short(_file.get(), dataBuffer.data(), samples);
        }
        else if (looping()) {
            dataBuffer.resize(AUDIO_STREAM_BUFFER_SIZE * _channels);
            decltype(samplesLeft) bufferLeft;
            for (auto it = dataBuffer.begin(); it != dataBuffer.end(); it += std::min(samplesLeft, bufferLeft) * _channels) {
                samplesLeft = _loopEnd - sf_seek(_file.get(), 0, SF_SEEK_CUR);
                bufferLeft = (dataBuffer.end() - it) / _channels;

                sf_readf_short(_file.get(), std::to_address(it), samplesLeft);
                if (samplesLeft <= bufferLeft) {
                    sf_seek(_file.get(), _loopStart, SF_SEEK_SET);
                    loopPoints.emplace_back(int(it - dataBuffer.begin()) / _channels, _loopStart);
                }
            }
        }
        buffer.set(rangeBytes(dataBuffer), _channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, _sampleRate);
    }

    if (!queueBuffers.empty()) {
        AudioSource::queueBuffers(queueBuffers);
    }
}

void tr::AudioStream::clear()
{
    if (_file != nullptr) {
        std::unique_lock lock { _audioStreamsMutex };
        _audioStreams.remove(*this);
        if (_audioStreams.empty() && _audioStreamsThread.joinable()) {
            _audioStreamsThread.join();
        }
    }
}