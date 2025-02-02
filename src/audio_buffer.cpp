#include "../include/tr/audio_buffer.hpp"
#include <AL/al.h>
#include <sndfile.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace tr {
	struct EmbeddedAudioFile {
		std::span<const std::byte>           file;
		std::span<const std::byte>::iterator pos;
	};
	sf_count_t embeddedAudioSize(void* user_data) noexcept;
	sf_count_t embeddedAudioSeek(sf_count_t offset, int whence, void* user_data) noexcept;
	sf_count_t embeddedAudioRead(void* ptr, sf_count_t count, void* user_data) noexcept;
	sf_count_t embeddedAudioTell(void* user_data) noexcept;

	AudioBuffer loadAudio(SNDFILE* file, const SF_INFO& info);
} // namespace tr

sf_count_t tr::embeddedAudioSize(void* user_data) noexcept
{
	return (static_cast<EmbeddedAudioFile*>(user_data))->file.size();
}

sf_count_t tr::embeddedAudioSeek(sf_count_t offset, int whence, void* user_data) noexcept
{
	EmbeddedAudioFile& file{*static_cast<EmbeddedAudioFile*>(user_data)};
	switch (whence) {
	case SEEK_SET:
		file.pos = file.file.begin() + offset;
		break;
	case SEEK_CUR:
		file.pos += offset;
		break;
	case SEEK_END:
		file.pos = file.file.end() + offset;
		break;
	}
	return std::distance(file.file.begin(), file.pos);
}

sf_count_t tr::embeddedAudioRead(void* ptr, sf_count_t count, void* user_data) noexcept
{
	EmbeddedAudioFile& file{*static_cast<EmbeddedAudioFile*>(user_data)};
	count = std::max(std::min(count, std::distance(file.pos, file.file.end())), std::ptrdiff_t{0});
	if (count > 0) {
		std::memcpy(ptr, &*file.pos, count);
		file.pos += count;
	}
	return count;
}

sf_count_t tr::embeddedAudioTell(void* user_data) noexcept
{
	EmbeddedAudioFile& file{*static_cast<EmbeddedAudioFile*>(user_data)};
	return std::distance(file.file.begin(), file.pos);
}

tr::AudioBuffer tr::loadAudio(SNDFILE* file, const SF_INFO& info)
{
	AudioBuffer               buffer;
	std::vector<std::int16_t> data(info.frames * info.channels);

	if (info.format & (SF_FORMAT_OGG | SF_FORMAT_VORBIS | SF_FORMAT_FLOAT | SF_FORMAT_DOUBLE)) {
		sf_command(file, SFC_SET_SCALE_FLOAT_INT_READ, nullptr, true);
	}
	sf_readf_short(file, data.data(), info.frames);

	return AudioBuffer{data, info.channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, info.samplerate};
}

tr::AudioBufferView::AudioBufferView(ALuint id) noexcept
	: _id{id}
{
}

tr::SecondsF tr::AudioBufferView::length() const noexcept
{
	ALint sampleRate, size;
	alGetBufferi(_id, AL_FREQUENCY, &sampleRate);
	alGetBufferi(_id, AL_SIZE, &size);
	return sampleRate == 0 ? tr::SecondsF::zero() : tr::SecondsF{static_cast<double>(size) / sampleRate};
}

void tr::AudioBufferView::set(std::span<const std::int16_t> data, AudioFormat format, int frequency)
{
	alBufferData(_id, ALenum(format), data.data(), data.size(), frequency);
	switch (alGetError()) {
	case AL_NO_ERROR:
		break;
	case AL_OUT_OF_MEMORY:
		throw AudioBufferBadAlloc{};
	default:
		assert(false);
	}
}

tr::AudioBuffer::AudioBuffer()
{
	ALuint id;
	alGenBuffers(1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw AudioBufferBadAlloc{};
	}
	_id.reset(id);
}

tr::AudioBuffer::AudioBuffer(std::span<const std::int16_t> data, AudioFormat format, int frequency)
	: AudioBuffer{}
{
	set(data, format, frequency);
}

void tr::AudioBuffer::Deleter::operator()(unsigned int id) const noexcept
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

tr::SecondsF tr::AudioBuffer::length() const noexcept
{
	return AudioBufferView(*this).length();
}

void tr::AudioBuffer::set(std::span<const std::int16_t> data, AudioFormat format, int frequency)
{
	AudioBufferView(*this).set(data, format, frequency);
}

tr::AudioBuffer tr::loadEmbeddedAudio(std::span<const std::byte> data)
{
	EmbeddedAudioFile fp{data, data.begin()};
	SF_VIRTUAL_IO     io{embeddedAudioSize, embeddedAudioSeek, embeddedAudioRead, nullptr, embeddedAudioTell};
	SF_INFO           info;

	std::unique_ptr<SNDFILE, decltype(&sf_close)> file{sf_open_virtual(&io, SFM_READ, &info, &fp), sf_close};
	assert(file != nullptr);

	return loadAudio(file.get(), info);
}

tr::AudioBuffer tr::loadAudioFile(const std::filesystem::path& path)
{
	if (!is_regular_file(path)) {
		throw FileNotFound{path};
	}

	SF_INFO info;
#ifdef _WIN32
	std::unique_ptr<SNDFILE, decltype(&sf_close)> file{sf_wchar_open(path.c_str(), SFM_READ, &info), sf_close};
#else
	std::unique_ptr<SNDFILE, decltype(&sf_close)> file{sf_open(path.c_str(), SFM_READ, &info), sf_close};
#endif

	if (file == nullptr) {
		throw FileOpenError{path};
	}
	if (info.channels > 2) {
		throw UnsupportedAudioFile{path};
	}

	return loadAudio(file.get(), info);
}