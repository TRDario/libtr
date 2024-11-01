#include "../include/tr/audio_source.hpp"
#include <glm/ext.hpp>
#include <AL/al.h>
#include <AL/alext.h>


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

void tr::AudioSource::Deleter::operator()(unsigned int id) const noexcept
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
    const auto n { std::min(max, processedBuffers()) };

    std::vector<AudioBufferView> buffers;
    buffers.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        ALuint id;
        alSourceUnqueueBuffers(_id.get(), 1, &id);
        buffers.push_back(id);
    }
    return buffers;
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