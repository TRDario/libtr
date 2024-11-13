#include "../include/tr/audio_buffer.hpp"

#include "../include/tr/ranges.hpp"

#include <AL/al.h>
#include <format>
#include <sndfile.h>

#ifdef _WIN32
#include <windows.h>
#endif

tr::AudioBufferView::AudioBufferView(ALuint id) noexcept
	: _id{id}
{
}

void tr::AudioBufferView::set(std::span<const std::byte> data, AudioFormat format, int frequency)
{
	alBufferData(_id, ALenum(format), data.data(), data.size(), frequency);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw AudioBufferBadAlloc{};
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

tr::AudioBuffer::AudioBuffer(std::span<const std::byte> data, AudioFormat format, int frequency)
{
	set(data, format, frequency);
}

tr::AudioBuffer::AudioBuffer(const std::filesystem::path& path)
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
	if (info.format & (SF_FORMAT_OGG | SF_FORMAT_VORBIS | SF_FORMAT_FLOAT | SF_FORMAT_DOUBLE)) {
		sf_command(file.get(), SFC_SET_SCALE_FLOAT_INT_READ, nullptr, true);
	}

	std::vector<std::int16_t> data(info.frames * info.channels);
	sf_readf_short(file.get(), data.data(), info.frames);
	set(rangeBytes(data), info.channels == 2 ? AudioFormat::STEREO16 : AudioFormat::MONO16, info.samplerate);
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

void tr::AudioBuffer::set(std::span<const std::byte> data, AudioFormat format, int frequency)
{
	AudioBufferView(*this).set(data, format, frequency);
}
