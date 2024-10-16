module;
#include <cassert>
#include <AL/al.h>
#include <AL/alext.h>

export module tr:audio_source;

import std;
import glm;
import :angle;
import :audio_buffer;
import :chrono;
import :handle;
import :ranges;

export namespace tr {
	// Enum representing a source position origin type.
	enum class AudioOrigin : bool {
		ABSOLUTE, // Absolute coordinates
		LISTENER  // Coordinates relative to the listener's position
	};
	// Enum representing an audio source state.
	enum class AudioState : std::uint8_t {
		INITIAL, // The source has not been played yet.
		PLAYING, // The source is playing.
		PAUSED,  // The source is paused.
		STOPPED  // The source stopped playing.
	};
	// 0-length direction vector representing an omnidirectional audio source.
	inline constexpr glm::vec3 OMNIDIRECTIONAL { 0, 0, 0 };
	// Sentinel value that tells the unqueueBuffers function to unqueue all processed buffers.
	inline constexpr std::size_t UNQUEUE_PROCESSED { std::numeric_limits<std::size_t>().max() };

	// OpenAL audio source class.
	class AudioSource {
	public:
		AudioSource();
		// Constructs an audio source with a pre-set buffer.
		AudioSource(AudioBufferView buffer);

		friend bool operator==(const AudioSource&, const AudioSource&) noexcept = default;

		// Gets the pitch of the source
		float pitch() const noexcept;
		// Sets the pitch of the source.
		void setPitch(float pitch) noexcept;

		// Gets the gain of the source.
		float gain() const noexcept;
		// Sets the gain of the source.
		void setGain(float gain) noexcept;

		// Gets the distance where the source will no longer be attenuated any further.
		float maxDistance() const noexcept;
		// Sets the distance where the source will no longer be attenuated any further.
		void setMaxDistance(float maxDistance) noexcept;

		// Gets the distance rolloff factor of the source.
		float rolloff() const noexcept;
		// Sets the distance rolloff factor of the source.
		void setRolloff(float rolloff) noexcept;

		// Gets the reference distance of the source, where there is no attenuation.
		float referenceDistance() const noexcept;
		// Sets the reference distance of the source, where there is no attenuation.
		void setReferenceDistance(float referenceDistance) noexcept;

		// Gets the minimum allowed gain multiplier for the source.
		float minGain() const noexcept;
		// Sets the minimum allowed gain multiplier for the source.
		void setMinGain(float minGain) noexcept;

		// Gets the maximum allowed gain multiplier for the source.
		float maxGain() const noexcept;
		// Sets the maximum allowed gain multiplier for the source.
		void setMaxGain(float maxGain) noexcept;

		// Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
		float outerConeGain() const noexcept;
		// Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
		void setOuterConeGain(float outGain) noexcept;

		// Gets the width of the inner cone of the source (where no direction attenuation is done).
		AngleF innerConeWidth() const noexcept;
		// Sets the width of the inner cone of the source (where no direction attenuation is done).
		void setInnerConeWidth(AngleF inConeW) noexcept;

		// Gets the width of the outer cone of the source (where direction attenuation is done).
		AngleF outerConeWidth() const noexcept;
		// Sets the width of the outer cone of the source (where direction attenuation is done).
		void setOuterConeWidth(AngleF outConeW) noexcept;

		// Gets the position of the source.
		glm::vec3 position() const noexcept;
		// Gets the direction of the source cone.
		void setPosition(const glm::vec3& position) noexcept;

		// Gets the velocity of the source.
		glm::vec3 velocity() const noexcept;
		// Gets the direction of the source cone.
		void setVelocity(const glm::vec3& velocity) noexcept;

		// Gets the direction of the source cone.
		glm::vec3 direction() const noexcept;
		// Gets the direction of the source cone.
		void setDirection(const glm::vec3& direction) noexcept;

		// Gets the origin of the source's position coordinates.
		AudioOrigin origin() const noexcept;
		// Sets the origin of the source's position coordinates.
		void setOrigin(AudioOrigin type) noexcept;

		// Gets whether the source is looping.
		bool looping() const noexcept;
		// Sets whether the source is looping.
		void setLooping(bool looping) noexcept;

		// Gets the state of the source.
		AudioState state() const noexcept;
		// Starts playing the source.
		void play() noexcept;
		// Pauses the source.
		void pause() noexcept;
		// Stops the source and rewinds it back to an initial state.
		void stop() noexcept;

		// Gets the buffer the source is currently using for playback.
		std::optional<AudioBufferView> buffer() const noexcept;
		// Sets a buffer for the source to use.
		void setBuffer(std::optional<AudioBufferView> buf) noexcept;

		// Gets the number of queued buffers.
		std::size_t queuedBuffers() const noexcept;
		// Gets the number of processed queued buffers.
		std::size_t processedBuffers() const noexcept;

		// Sets a queue of buffers for the source to use in a sequence.
		void queueBuffers(std::span<AudioBufferView> bufs) noexcept;
		// Removes at most max (or all processed buffers with UNQUEUE_PROCESSED) of buffers from the source's queue.
		// References to the unqueued buffers are returned.
		std::vector<AudioBufferView> unqueueBuffers(std::size_t max = UNQUEUE_PROCESSED);

		// Gets the source's playback position within the current buffer.
		SecondsF offset() const noexcept;
		// Sets the source's playback position within the current buffer.
		void setOffset(SecondsF off) noexcept;

		// Gets the source's playback position within the current buffer.
		int offsetSamples() const noexcept;
		// Sets the source's playback position within the current buffer.
		void setOffset(int off) noexcept;
	private:
		struct Deleter { void operator()(ALuint id) noexcept; };

		// The OpenAL ID of the source.
		Handle<ALuint, 0, Deleter> _id;

		friend class AudioStream;
	};
}

// IMPLEMENTATION

tr::AudioSource::AudioSource()
{
    ALuint id;
    alGenSources(1, &id);
    if (alGetError() == AL_OUT_OF_MEMORY) {
        throw std::bad_alloc {};
    }
    _id.reset(id);
}

tr::AudioSource::AudioSource(AudioBufferView buffer)
{
    setBuffer(buffer);
}

void tr::AudioSource::Deleter::operator()(ALuint id) noexcept
{
    alDeleteSources(1, &id);
}

float tr::AudioSource::pitch() const noexcept
{
    float pitch;
    alGetSourcef(_id.get(), AL_PITCH, &pitch);
    return pitch;
}

void tr::AudioSource::setPitch(float pitch) noexcept
{
    assert(pitch >= 0.5f && pitch <= 2.0f);
    alSourcef(_id.get(), AL_PITCH, pitch);
}

float tr::AudioSource::gain() const noexcept
{
    float gain;
    alGetSourcef(_id.get(), AL_GAIN, &gain);
    return gain;
}

void tr::AudioSource::setGain(float gain) noexcept
{
    assert(gain >= 0.0f);
    alSourcef(_id.get(), AL_GAIN, gain);
}

float tr::AudioSource::maxDistance() const noexcept
{
    float maxDistance;
    alGetSourcef(_id.get(), AL_MAX_DISTANCE, &maxDistance);
    return maxDistance;
}

void tr::AudioSource::setMaxDistance(float maxDistance) noexcept
{
    assert(maxDistance >= 0.0f);
    alSourcef(_id.get(), AL_MAX_DISTANCE, maxDistance);
}

float tr::AudioSource::rolloff() const noexcept
{
    float rolloff;
    alGetSourcef(_id.get(), AL_ROLLOFF_FACTOR, &rolloff);
    return rolloff;
}

void tr::AudioSource::setRolloff(float rolloff) noexcept
{
    assert(rolloff >= 0.0f);
    alSourcef(_id.get(), AL_ROLLOFF_FACTOR, rolloff);
}

float tr::AudioSource::referenceDistance() const noexcept
{
    float referenceDistance;
    alGetSourcef(_id.get(), AL_REFERENCE_DISTANCE, &referenceDistance);
    return referenceDistance;
}

void tr::AudioSource::setReferenceDistance(float referenceDistance) noexcept
{
    assert(referenceDistance >= 0.0f);
    alSourcef(_id.get(), AL_REFERENCE_DISTANCE, referenceDistance);
}

float tr::AudioSource::minGain() const noexcept
{
    float minGain;
    alGetSourcef(_id.get(), AL_MIN_GAIN, &minGain);
    return minGain;
}

void tr::AudioSource::setMinGain(float minGain) noexcept
{
    assert(minGain >= 0.0f && minGain <= 1.0f);
    alSourcef(_id.get(), AL_MIN_GAIN, minGain);
}

float tr::AudioSource::maxGain() const noexcept
{
    float maxGain;
    alGetSourcef(_id.get(), AL_MAX_GAIN, &maxGain);
    return maxGain;
}

void tr::AudioSource::setMaxGain(float maxGain) noexcept
{
    assert(maxGain >= 0.0f && maxGain <= 1.0f);
    alSourcef(_id.get(), AL_MAX_GAIN, maxGain);
}

float tr::AudioSource::outerConeGain() const noexcept
{
    float outGain;
    alGetSourcef(_id.get(), AL_CONE_OUTER_GAIN, &outGain);
    return outGain;
}

void tr::AudioSource::setOuterConeGain(float outGain) noexcept
{
    assert(outGain >= 0.0f && outGain <= 1.0f);
    alSourcef(_id.get(), AL_CONE_OUTER_GAIN, outGain);
}

tr::AngleF tr::AudioSource::innerConeWidth() const noexcept
{
    float inConeW;
    alGetSourcef(_id.get(), AL_CONE_INNER_ANGLE, &inConeW);
    return degs(inConeW);
}

void tr::AudioSource::setInnerConeWidth(AngleF inConeW) noexcept
{
    assert(inConeW >= 0.0_degf && inConeW <= 360.0_degf);
    alSourcef(_id.get(), AL_CONE_INNER_ANGLE, inConeW.degs());
}

tr::AngleF tr::AudioSource::outerConeWidth() const noexcept
{
    float outConeW;
    alGetSourcef(_id.get(), AL_CONE_OUTER_ANGLE, &outConeW);
    return degs(outConeW);;
}

void tr::AudioSource::setOuterConeWidth(AngleF outConeW) noexcept
{
    assert(outConeW >= 0.0_degf && outConeW <= 360.0_degf);
    alSourcef(_id.get(), AL_CONE_OUTER_ANGLE, outConeW.degs());
}

glm::vec3 tr::AudioSource::position() const noexcept
{
    glm::vec3 position;
    alGetSourcefv(_id.get(), AL_POSITION, value_ptr(position));
    return position;
}

void tr::AudioSource::setPosition(const glm::vec3& position) noexcept
{
    alSourcefv(_id.get(), AL_POSITION, value_ptr(position));
}

glm::vec3 tr::AudioSource::velocity() const noexcept
{
    glm::vec3 velocity;
    alGetSourcefv(_id.get(), AL_VELOCITY, value_ptr(velocity));
    return velocity;
}

void tr::AudioSource::setVelocity(const glm::vec3& velocity) noexcept
{
    alSourcefv(_id.get(), AL_VELOCITY, value_ptr(velocity));
}

glm::vec3 tr::AudioSource::direction() const noexcept
{
    glm::vec3 direction;
    alGetSourcefv(_id.get(), AL_DIRECTION, value_ptr(direction));
    return direction;
}

void tr::AudioSource::setDirection(const glm::vec3& direction) noexcept
{
    alSourcefv(_id.get(), AL_DIRECTION, value_ptr(direction));
}

tr::AudioOrigin tr::AudioSource::origin() const noexcept
{
    int type;
    alGetSourcei(_id.get(), AL_SOURCE_RELATIVE, &type);
    return AudioOrigin(type);
}

void tr::AudioSource::setOrigin(AudioOrigin type) noexcept
{
    alSourcei(_id.get(), AL_SOURCE_RELATIVE, int(type));
}

bool tr::AudioSource::looping() const noexcept
{
    int looping;
    alGetSourcei(_id.get(), AL_LOOPING, &looping);
    return bool(looping);
}

void tr::AudioSource::setLooping(bool value) noexcept
{
    alSourcei(_id.get(), AL_LOOPING, int(value));
}

tr::AudioState tr::AudioSource::state() const noexcept
{
    int state;
    alGetSourcei(_id.get(), AL_SOURCE_STATE, &state);
    switch (state) {
    case AL_INITIAL:
        return AudioState::INITIAL;
    case AL_PLAYING:
        return AudioState::PLAYING;
    case AL_PAUSED:
        return AudioState::PAUSED;
    case AL_STOPPED:
        return AudioState::STOPPED;
    default:
        return AudioState::INITIAL;
    }
}

void tr::AudioSource::play() noexcept
{
    alSourcePlay(_id.get());
}

void tr::AudioSource::pause() noexcept
{
    alSourcePause(_id.get());
}

void tr::AudioSource::stop() noexcept
{
    alSourceStop(_id.get());
}

std::optional<tr::AudioBufferView> tr::AudioSource::buffer() const noexcept
{
    int id;
    alGetSourcei(_id.get(), AL_BUFFER, &id);
    return id != 0 ? std::optional<AudioBufferView> { { (unsigned int)(id) } } : std::nullopt;
}

void tr::AudioSource::setBuffer(std::optional<AudioBufferView> buffer) noexcept
{
    alSourcei(_id.get(), AL_BUFFER, buffer ? buffer->_id : 0);
    if (buffer.has_value()) {
        ALint channels;
        alGetBufferi(buffer->_id, AL_CHANNELS, &channels);
        alSourcei(_id.get(), AL_DIRECT_CHANNELS_SOFT, buffer ? channels == 2 : false);
    }
}

std::size_t tr::AudioSource::queuedBuffers() const noexcept
{
    int queued;
    alGetSourcei(_id.get(), AL_BUFFERS_QUEUED, &queued);
    return queued;
}

std::size_t tr::AudioSource::processedBuffers() const noexcept
{
    int processed;
    alGetSourcei(_id.get(), AL_BUFFERS_PROCESSED, &processed);
    return processed;
}

void tr::AudioSource::queueBuffers(std::span<AudioBufferView> abufs) noexcept
{
    alSourceQueueBuffers(_id.get(), abufs.size(), (unsigned int*)(abufs.data()));
    assert(alGetError() != AL_INVALID_OPERATION);
}

std::vector<tr::AudioBufferView> tr::AudioSource::unqueueBuffers(std::size_t max)
{
    std::vector<unsigned int> ids(std::min(max, processedBuffers()));
    if (!ids.empty()) {
        alSourceUnqueueBuffers(_id.get(), ids.size(), ids.data());
    }
    return ids | std::views::transform([] (auto id) { return AudioBufferView { id }; }) | std::ranges::to<std::vector>();
}

tr::SecondsF tr::AudioSource::offset() const noexcept
{
    float offset;
    alGetSourcef(_id.get(), AL_SEC_OFFSET, &offset);
    return SecondsF(offset);
}

void tr::AudioSource::setOffset(SecondsF offset) noexcept
{
    alSourcef(_id.get(), AL_SEC_OFFSET, offset.count());
}


int tr::AudioSource::offsetSamples() const noexcept
{
    int offset;
    alGetSourcei(_id.get(), AL_SAMPLE_OFFSET, &offset);
    return offset;
}

void tr::AudioSource::setOffset(int offset) noexcept
{
    alSourcei(_id.get(), AL_SAMPLE_OFFSET, offset);
}