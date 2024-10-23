/**
 * @file audio_stream.cppm
 * @brief Provides an audio source class.
 */

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
    /******************************************************************************************************************
	 * Error thrown when allocating an audio source fails.
	 ******************************************************************************************************************/
    struct AudioSourceBadAlloc : std::bad_alloc {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        virtual constexpr const char* what() const noexcept;
    };


    /******************************************************************************************************************
	 * Audio source position origin types.
	 ******************************************************************************************************************/
	enum class AudioOrigin : bool {
        /**************************************************************************************************************
         * Absolute coordinates.
	     **************************************************************************************************************/
		ABSOLUTE,

        /**************************************************************************************************************
         * Coordinates relative to the listener's position.
	     **************************************************************************************************************/
		LISTENER
	};

    /******************************************************************************************************************
	 * Audio source states.
	 ******************************************************************************************************************/
	enum class AudioState : std::uint8_t {
        /**************************************************************************************************************
         * The source has not been played yet.
	     **************************************************************************************************************/
		INITIAL,

        /**************************************************************************************************************
         * The source is playing.
	     **************************************************************************************************************/
		PLAYING,

        /**************************************************************************************************************
         * The source is paused.
	     **************************************************************************************************************/
		PAUSED,

        /**************************************************************************************************************
         * The source stopped playing.
	     **************************************************************************************************************/
		STOPPED
	};

    /******************************************************************************************************************
	 * 0-length direction vector representing an omnidirectional audio source.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 OMNIDIRECTIONAL { 0, 0, 0 };

    /******************************************************************************************************************
	 * Sentinel value that tells the unqueueBuffers function to unqueue all processed buffers.
	 ******************************************************************************************************************/
	inline constexpr std::size_t UNQUEUE_PROCESSED { std::numeric_limits<std::size_t>().max() };


	/******************************************************************************************************************
	 * Audio source.
	 ******************************************************************************************************************/
	class AudioSource {
	public:
        /**************************************************************************************************************
         * Constructs an audio source.
         *
         * @exception AudioSourceBadAlloc If allocating the audio source failed.
	     **************************************************************************************************************/
		AudioSource();

        /**************************************************************************************************************
         * Constructs an audio source with a pre-set buffer.
         *
         * @exception AudioSourceBadAlloc If allocating the audio source failed.
         *
         * @param buffer The buffer to attach to the source.
	     **************************************************************************************************************/
		AudioSource(AudioBufferView buffer);


        /**************************************************************************************************************
         * Equality comparison operator.
	     **************************************************************************************************************/
		friend bool operator==(const AudioSource&, const AudioSource&) noexcept = default;


        /**************************************************************************************************************
         * Gets the pitch of the source.
         *
         * @return The pitch multiplier of the source.
	     **************************************************************************************************************/
		float pitch() const noexcept;
		
        /**************************************************************************************************************
         * Sets the pitch (and speed) of the source.
         *
         * @param pitch The pitch multiplier of the source, clamped to [0.5, 2.0].
	     **************************************************************************************************************/
		void setPitch(float pitch) noexcept;


		/**************************************************************************************************************
         * Gets the gain of the source.
         *
         * @return The gain multiplier of the source.
	     **************************************************************************************************************/
		float gain() const noexcept;
		
        /**************************************************************************************************************
         * Sets the gain of the source.
         *
         * @param gain The gain multiplier of the source, clamped to a non-negative value.
	     **************************************************************************************************************/
		void setGain(float gain) noexcept;


        /**************************************************************************************************************
         * Gets the distance where the source will no longer be attenuated any further.
         *
         * @return The maximum distance of the source.
	     **************************************************************************************************************/
		float maxDistance() const noexcept;

        /**************************************************************************************************************
         * Sets the distance where the source will no longer be attenuated any further.
         *
         * @param maxDistance The maximum distance of the source, clamped to a non-negative value.
	     **************************************************************************************************************/
		void setMaxDistance(float maxDistance) noexcept;


        /**************************************************************************************************************
         * Gets the distance rolloff factor of the source.
         *
         * @return The distance rolloff factor of the source.
	     **************************************************************************************************************/
		float rolloff() const noexcept;

        /**************************************************************************************************************
         * Sets the distance rolloff factor of the source.
         *
         * @param rolloff The distance rolloff factor of the source, clamped to a non-negative value.
	     **************************************************************************************************************/
		void setRolloff(float rolloff) noexcept;


        /**************************************************************************************************************
         * Gets the reference distance of the source, where there is no attenuation.
         *
         * @return Gets the reference distance of the source, where there is no attenuation.
	     **************************************************************************************************************/
		float referenceDistance() const noexcept;

        /**************************************************************************************************************
         * Sets the reference distance of the source, where there is no attenuation.
         *
         * @param referenceDistance The new reference distance of the source, clamped to a non-negative value.
	     **************************************************************************************************************/
		void setReferenceDistance(float referenceDistance) noexcept;


        /**************************************************************************************************************
         * Gets the minimum allowed gain multiplier for the source.
         *
         * @return The minimum allowed gain multiplier for the source.
	     **************************************************************************************************************/
		float minGain() const noexcept;

        /**************************************************************************************************************
         * Sets the minimum allowed gain multiplier for the source.
         *
         * @param minGain The new minimum allowed gain multiplier, clamped to [0.0, maxGain()].
	     **************************************************************************************************************/
		void setMinGain(float minGain) noexcept;


		/**************************************************************************************************************
         * Gets the maximum allowed gain multiplier for the source.
         *
         * @return The maximum allowed gain multiplier for the source.
	     **************************************************************************************************************/
		float maxGain() const noexcept;

		/**************************************************************************************************************
         * Sets the maximum allowed gain multiplier for the source.
         *
         * @param maxGain The new maximum allowed gain multiplier, clamped to [minGain(), 1.0].
	     **************************************************************************************************************/
		void setMaxGain(float maxGain) noexcept;


        /**************************************************************************************************************
         * Gets the gain multiplier applied when the listener is outside the source's outer cone angle.
         *
         * @return The outer cone gain multiplier.
	     **************************************************************************************************************/
		float outerConeGain() const noexcept;

		/**************************************************************************************************************
         * Sets the gain multiplier applied when the listener is outside the source's outer cone angle.
         *
         * @param outGain The new gain multiplier, clamped to [0.0, 1.0].
	     **************************************************************************************************************/
		void setOuterConeGain(float outGain) noexcept;


		/**************************************************************************************************************
         * Gets the width of the inner cone of the source (where no direction attenuation is done).
         *
         * @return The width of the inner cone of the source.
	     **************************************************************************************************************/
		AngleF innerConeWidth() const noexcept;

		/**************************************************************************************************************
         * Sets the width of the inner cone of the source (where no direction attenuation is done).
         *
         * @param inConeW The new width, clamped to [0.0, outerConeWidth()].
	     **************************************************************************************************************/
		void setInnerConeWidth(AngleF inConeW) noexcept;


        /**************************************************************************************************************
         * Gets the width of the outer cone of the source (where direction attenuation is done).
         *
         * @return The width of the outer cone of the source.
	     **************************************************************************************************************/
		AngleF outerConeWidth() const noexcept;

		/**************************************************************************************************************
         * Sets the width of the outer cone of the source (where direction attenuation is done).
         *
         * @param outConeW The new width, clamped to [innerConeWidth(), 360 degrees].
	     **************************************************************************************************************/
		void setOuterConeWidth(AngleF outConeW) noexcept;


		/**************************************************************************************************************
         * Gets the position of the source.
         *
         * @return The position vector of the audio source.
	     **************************************************************************************************************/
		glm::vec3 position() const noexcept;

		/**************************************************************************************************************
         * Sets the position of the source.
         *
         * @param position The position of the source.
	     **************************************************************************************************************/
		void setPosition(const glm::vec3& position) noexcept;


		/**************************************************************************************************************
         * Gets the velocity of the source.
         *
         * @return The velocity vector of the audio source.
	     **************************************************************************************************************/
		glm::vec3 velocity() const noexcept;

		/**************************************************************************************************************
         * Sets the velocity of the source.
         *
         * @param velocity The velocity of the source.
	     **************************************************************************************************************/
		void setVelocity(const glm::vec3& velocity) noexcept;


		/**************************************************************************************************************
         * Gets the direction of the source cone.
         *
         * @return The direction vector of the audio source.
	     **************************************************************************************************************/
		glm::vec3 direction() const noexcept;

        /**************************************************************************************************************
         * Sets the direction of the source cone.
         *
         * @param direction The direction of the source cone. Can also be OMNIDRECTIONAL.
	     **************************************************************************************************************/
		void setDirection(const glm::vec3& direction) noexcept;


        /**************************************************************************************************************
         * Gets the origin of the source's position.
         *
         * @return The origin of the audio source.
	     **************************************************************************************************************/
		AudioOrigin origin() const noexcept;

		/**************************************************************************************************************
         * Sets the origin of the source's position.
         *
         * @param type The new origin type.
	     **************************************************************************************************************/
		void setOrigin(AudioOrigin type) noexcept;


        /**************************************************************************************************************
         * Gets whether the source is looping.
         *
         * @return True if the source is looping, and false otherwise.
	     **************************************************************************************************************/
		bool looping() const noexcept;

		/**************************************************************************************************************
         * Sets whether the source is looping.
         *
         * @param looping Whether the source should loop.
	     **************************************************************************************************************/
		void setLooping(bool looping) noexcept;


		/**************************************************************************************************************
         * Gets the state of the audio source.
         *
         * @return The state of the audio source.
	     **************************************************************************************************************/
		AudioState state() const noexcept;

		/**************************************************************************************************************
         * Plays the source.
	     **************************************************************************************************************/
		void play() noexcept;
        
		/**************************************************************************************************************
         * Pauses the source.
	     **************************************************************************************************************/
		void pause() noexcept;

		/**************************************************************************************************************
         * Stops the source and rewinds it to the beginning.
	     **************************************************************************************************************/
		void stop() noexcept;


        /**************************************************************************************************************
         * Gets the buffer the source is currently using for playback.
         *
         * @return A view over the playback buffer, or nullopt if the source is empty/streamed.
	     **************************************************************************************************************/
		std::optional<AudioBufferView> buffer() const noexcept;

        /**************************************************************************************************************
         * Sets a buffer for the source to use.
         *
         * Calling this function is not allowed while the source is playing/paused.
         *
         * @param buffer The buffer to use, or nullopt to unset any set/queued buffers.
	     **************************************************************************************************************/
		void setBuffer(std::optional<AudioBufferView> buffer) noexcept;


        /**************************************************************************************************************
         * Gets the number of queued buffers.
         *
         * @return The number of queued buffers.
	     **************************************************************************************************************/
		std::size_t queuedBuffers() const noexcept;

		/**************************************************************************************************************
         * Gets the number of processed queued buffers.
         *
         * @return The number of processed queued buffers.
	     **************************************************************************************************************/
		std::size_t processedBuffers() const noexcept;


        /**************************************************************************************************************
         * Queues a buffer for streaming.
         *
         * @param buffer The buffer to attach. It must be of the same audio format as any other queued buffers.
	     **************************************************************************************************************/
		void queueBuffer(AudioBufferView buffer) noexcept;

		/**************************************************************************************************************
         * Queues buffers for streaming.
         *
         * @param buffers The buffers to attach. They must be of the same audio format as any other queued buffers.
	     **************************************************************************************************************/
		void queueBuffers(std::span<AudioBufferView> buffers) noexcept;

        /**************************************************************************************************************
         * Removes at most max (or all processed buffers with UNQUEUE_PROCESSED) of buffers from the source's queue.
         *
         * @exception std::bad_alloc If allocating the return vector failed.
         *
         * @param max The number of buffers to unqueue.
         *
         * @return A vector containing the removed buffers.
	     **************************************************************************************************************/
		std::vector<AudioBufferView> unqueueBuffers(std::size_t max = UNQUEUE_PROCESSED);


        /**************************************************************************************************************
         * Gets the source's playback position within the current buffer.
         *
         * @return The source's playback position within the current buffer in seconds.
	     **************************************************************************************************************/
		SecondsF offset() const noexcept;

		/**************************************************************************************************************
         * Sets the source's playback position within the current buffer.
         *
         * @param offset The new playback position within the current buffer in seconds.
	     **************************************************************************************************************/
		void setOffset(SecondsF offset) noexcept;
	private:
		Handle<ALuint, 0, decltype([] (ALuint id) { alDeleteSources(1, &id); })> _id;

		friend class AudioStream;
	};
}

/// @cond IMPLEMENTATION

constexpr const char* tr::AudioSourceBadAlloc::what() const noexcept
{
    return "failed audio source allocation";
}

tr::AudioSource::AudioSource()
{
    ALuint id;
    alGenSources(1, &id);
    if (alGetError() == AL_OUT_OF_MEMORY) {
        throw AudioSourceBadAlloc {};
    }
    _id.reset(id);
}

tr::AudioSource::AudioSource(AudioBufferView buffer)
{
    setBuffer(buffer);
}

float tr::AudioSource::pitch() const noexcept
{
    float pitch;
    alGetSourcef(_id.get(), AL_PITCH, &pitch);
    return pitch;
}

void tr::AudioSource::setPitch(float pitch) noexcept
{
    alSourcef(_id.get(), AL_PITCH, std::clamp(pitch, 0.5f, 2.0f));
}

float tr::AudioSource::gain() const noexcept
{
    float gain;
    alGetSourcef(_id.get(), AL_GAIN, &gain);
    return gain;
}

void tr::AudioSource::setGain(float gain) noexcept
{
    alSourcef(_id.get(), AL_GAIN, std::min(gain, 0.0f));
}

float tr::AudioSource::maxDistance() const noexcept
{
    float maxDistance;
    alGetSourcef(_id.get(), AL_MAX_DISTANCE, &maxDistance);
    return maxDistance;
}

void tr::AudioSource::setMaxDistance(float maxDistance) noexcept
{
    alSourcef(_id.get(), AL_MAX_DISTANCE, std::min(maxDistance, 0.0f));
}

float tr::AudioSource::rolloff() const noexcept
{
    float rolloff;
    alGetSourcef(_id.get(), AL_ROLLOFF_FACTOR, &rolloff);
    return rolloff;
}

void tr::AudioSource::setRolloff(float rolloff) noexcept
{
    alSourcef(_id.get(), AL_ROLLOFF_FACTOR, std::min(rolloff, 0.0f));
}

float tr::AudioSource::referenceDistance() const noexcept
{
    float referenceDistance;
    alGetSourcef(_id.get(), AL_REFERENCE_DISTANCE, &referenceDistance);
    return referenceDistance;
}

void tr::AudioSource::setReferenceDistance(float referenceDistance) noexcept
{
    alSourcef(_id.get(), AL_REFERENCE_DISTANCE, std::min(referenceDistance, 0.0f));
}

float tr::AudioSource::minGain() const noexcept
{
    float minGain;
    alGetSourcef(_id.get(), AL_MIN_GAIN, &minGain);
    return minGain;
}

void tr::AudioSource::setMinGain(float minGain) noexcept
{
    alSourcef(_id.get(), AL_MIN_GAIN, std::clamp(minGain, 0.0f, maxGain()));
}

float tr::AudioSource::maxGain() const noexcept
{
    float maxGain;
    alGetSourcef(_id.get(), AL_MAX_GAIN, &maxGain);
    return maxGain;
}

void tr::AudioSource::setMaxGain(float maxGain) noexcept
{
    alSourcef(_id.get(), AL_MAX_GAIN, std::clamp(maxGain, minGain(), 1.0f));
}

float tr::AudioSource::outerConeGain() const noexcept
{
    float outGain;
    alGetSourcef(_id.get(), AL_CONE_OUTER_GAIN, &outGain);
    return outGain;
}

void tr::AudioSource::setOuterConeGain(float outGain) noexcept
{
    alSourcef(_id.get(), AL_CONE_OUTER_GAIN, std::clamp(outGain, 0.0f, 1.0f));
}

tr::AngleF tr::AudioSource::innerConeWidth() const noexcept
{
    float inConeW;
    alGetSourcef(_id.get(), AL_CONE_INNER_ANGLE, &inConeW);
    return degs(inConeW);
}

void tr::AudioSource::setInnerConeWidth(AngleF inConeW) noexcept
{
    alSourcef(_id.get(), AL_CONE_INNER_ANGLE, std::clamp(inConeW, 0.0_degf, outerConeWidth()).degs());
}

tr::AngleF tr::AudioSource::outerConeWidth() const noexcept
{
    float outConeW;
    alGetSourcef(_id.get(), AL_CONE_OUTER_ANGLE, &outConeW);
    return degs(outConeW);;
}

void tr::AudioSource::setOuterConeWidth(AngleF outConeW) noexcept
{
    alSourcef(_id.get(), AL_CONE_OUTER_ANGLE, std::clamp(outConeW, innerConeWidth(), 360_degf).degs());
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
    assert(state() != AudioState::PLAYING && state() != AudioState::PAUSED);

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

void tr::AudioSource::queueBuffer(AudioBufferView buffer) noexcept
{
    alSourceQueueBuffers(_id.get(), 1, &buffer._id);
    assert(alGetError() != AL_INVALID_OPERATION);
}

void tr::AudioSource::queueBuffers(std::span<AudioBufferView> buffers) noexcept
{
    alSourceQueueBuffers(_id.get(), buffers.size(), (unsigned int*)(buffers.data()));
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

/// @endcond