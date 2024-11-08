#include "../include/tr/audio_stream.hpp"

#include "../include/tr/ranges.hpp"

#include <AL/al.h>
#include <AL/alext.h>
#include <boost/container/static_vector.hpp>
#include <forward_list>
#include <mutex>
#include <sndfile.h>
#include <thread>

#ifdef _WIN32
	#include <windows.h>
#endif

namespace tr {
	// The size of the buffers used in streams.
	inline constexpr int								   AUDIO_STREAM_BUFFER_SIZE {16'384};

	// List of active streams.
	std::forward_list<std::reference_wrapper<AudioStream>> _audioStreams;
	// Mutex protecting _audioStreams.
	std::mutex											   _audioStreamsMutex;
	// Audio stream refreshing thread.
	std::thread											   _audioStreamsThread;
	// Audio stream refreshing thread activity flag.
	std::atomic_bool									   _audioStreamsThreadExited {true};
	// Audio stream refreshing thread function.
	void												   audioStreamThread() noexcept;
} // namespace tr

void tr::audioStreamThread() noexcept
{
	try {
		_audioStreamsThreadExited = false;
		for (; !_audioStreams.empty(); std::this_thread::sleep_for(std::chrono::milliseconds(10))) {
			std::lock_guard listLock {_audioStreamsMutex};
			for (AudioStream& stream : _audioStreams) {
				std::lock_guard streamLock {stream._mutex};
				auto			buffers {stream.unqueueBuffers()};
				for (auto& buffer : buffers) {
					stream.refillBuffer(*std::ranges::find(stream._buffers, buffer));
					stream.queueBuffer(buffer);
					if (sf_seek(stream._file.get(), 0, SF_SEEK_CUR) == stream._length) {
						break;
					}
				}
			}
		}
	}
	catch (...) {
	}
	_audioStreamsThreadExited = true;
}

tr::AudioStream::AudioStream()
	: _length {0}
	, _channels {1}
	, _sampleRate {1}
	, _looping {false}
	, _loopStart {0}
	, _loopEnd {0}
{}

tr::AudioStream::AudioStream(const std::filesystem::path& path)
{
	if (!is_regular_file(path)) {
		throw FileNotFound {path};
	}

	SF_INFO info;
#ifdef _WIN32
	_file.reset(sf_wchar_open(path.c_str(), SFM_READ, &info));
#else
	_file.reset(sf_open(path.c_str(), SFM_READ, &info));
#endif
	if (_file == nullptr) {
		throw FileOpenError {path};
	}

	_length		= info.frames;
	_sampleRate = info.samplerate;
	_loopEnd	= _length;
	_channels	= info.channels;
	if (_channels > 2) {
		throw UnsupportedAudioFile {path};
	}
	else if (_channels == 2) {
		alSourcei(AudioSource::_id.get(), AL_DIRECT_CHANNELS_SOFT, 1);
	}

	if (info.format & (SF_FORMAT_OGG | SF_FORMAT_VORBIS | SF_FORMAT_FLOAT | SF_FORMAT_DOUBLE)) {
		sf_command(_file.get(), SFC_SET_SCALE_FLOAT_INT_READ, nullptr, true);
	}

	bool threadInactive {_audioStreams.empty() || _audioStreamsThreadExited};
	if (threadInactive) {
		if (_audioStreamsThread.joinable()) {
			_audioStreamsThread.join();
		}
		_audioStreams.push_front(*this);
		_audioStreamsThread = std::thread {audioStreamThread};
	}
	else {
		std::lock_guard lock {_audioStreamsMutex};
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
		std::lock_guard lock {_audioStreamsMutex};
		_audioStreams.remove(*this);
		if (_audioStreams.empty() && _audioStreamsThread.joinable()) {
			_audioStreamsThread.join();
		}
	}
}

tr::AudioStream& tr::AudioStream::operator=(AudioStream&& r) noexcept
{
	if (!empty()) {
		std::ignore = AudioStream {std::move(*this)};
	}

	std::lock_guard lock {_audioStreamsMutex};
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
	std::lock_guard lock {_mutex};
	_looping = looping;
}

void tr::AudioStream::play()
{
	if (empty()) {
		return;
	}

	std::lock_guard lock {_mutex};
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
	if (empty()) {
		return;
	}

	std::lock_guard lock {_mutex};
	AudioSource::stop();
	sf_seek(_file.get(), _loopStart, SF_SEEK_SET);
}

tr::SecondsF tr::AudioStream::length() const noexcept
{
	return SecondsF {float(_length) / _sampleRate};
}

tr::SecondsF tr::AudioStream::offset() const noexcept
{
	if (empty()) {
		return SecondsF::zero();
	}

	std::lock_guard lock {_mutex};
	const auto		state {this->state()};

	if (state == AudioState::INITIAL || state == AudioState::STOPPED) {
		return SecondsF {sf_seek(_file.get(), 0, SF_SEEK_CUR) / float(_sampleRate)};
	}

	auto& buffer {*std::ranges::find(_buffers, AudioSource::buffer())};
	return SecondsF {buffer.startFileOffset / float(_sampleRate)} + AudioSource::offset();
}

void tr::AudioStream::setOffset(SecondsF off)
{
	if (empty()) {
		return;
	}

	auto state {AudioSource::state()};
	{
		std::lock_guard lock {_mutex};

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
	return SecondsF {float(_loopStart) / _sampleRate};
}

tr::SecondsF tr::AudioStream::loopEnd() const noexcept
{
	return SecondsF {float(_loopEnd) / _sampleRate};
}

void tr::AudioStream::setLoopPoints(SecondsF start, SecondsF end) noexcept
{
	if (empty()) {
		return;
	}

	start = std::clamp(start, START, length());
	end	  = std::clamp(end, START, length());
	assert(start < end);

	std::unique_lock lock {_mutex};
	_loopStart = start.count() * _sampleRate;
	_loopEnd   = end.count() * _sampleRate;
}

void tr::AudioStream::refillBuffer(Buffer& buffer)
{
	static boost::container::static_vector<std::int16_t, AUDIO_STREAM_BUFFER_SIZE * 2> dataBuffer;
	buffer.startFileOffset = sf_seek(_file.get(), 0, SF_SEEK_CUR);

	auto samplesLeft {_loopEnd - buffer.startFileOffset};
	auto samplesToRead {std::min<int>(AUDIO_STREAM_BUFFER_SIZE, samplesLeft)};
	dataBuffer.resize(samplesToRead * _channels);
	sf_readf_short(_file.get(), dataBuffer.data(), samplesToRead);
	buffer.set(rangeBytes(dataBuffer), _channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, _sampleRate);

	if (samplesToRead <= AUDIO_STREAM_BUFFER_SIZE && looping()) {
		sf_seek(_file.get(), _loopStart, SF_SEEK_SET);
	}
}
