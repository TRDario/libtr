#pragma once
#include "angle.hpp"
#include "audio_buffer.hpp"
#include "chrono.hpp"
#include <glm/glm.hpp>

namespace tr {
	/** @addtogroup audio
	 *  @{
	 */

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
	inline constexpr glm::vec3 OMNIDIRECTIONAL{0, 0, 0};

	/******************************************************************************************************************
	 * Sentinel value that tells the unqueueBuffers function to unqueue all processed buffers.
	 ******************************************************************************************************************/
	inline constexpr std::size_t UNQUEUE_PROCESSED{std::numeric_limits<std::size_t>().max()};

	/******************************************************************************************************************
	 * Audio source.
	 ******************************************************************************************************************/
	class AudioSource {
	  public:
		/**************************************************************************************************************
		 * Constructs an audio source.
		 *
		 * @exception AudioSourceBadAlloc If allocating the audio source fails.
		 **************************************************************************************************************/
		AudioSource();

		/**************************************************************************************************************
		 * Constructs an audio source with a pre-set buffer.
		 *
		 * @exception AudioSourceBadAlloc If allocating the audio source fails.
		 *
		 * @param[in] buffer The buffer to attach to the source.
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
		 * @param[in] pitch The pitch multiplier of the source, clamped to [0.5, 2.0].
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
		 * @param[in] gain The gain multiplier of the source, clamped to a non-negative value.
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
		 * @param[in] maxDistance The maximum distance of the source, clamped to a non-negative value.
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
		 * @param[in] rolloff The distance rolloff factor of the source, clamped to a non-negative value.
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
		 * @param[in] referenceDistance The new reference distance of the source, clamped to a non-negative value.
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
		 * @param[in] minGain The new minimum allowed gain multiplier, clamped to [0.0, maxGain()].
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
		 * @param[in] maxGain The new maximum allowed gain multiplier, clamped to [minGain(), 1.0].
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
		 * @param[in] outGain The new gain multiplier, clamped to [0.0, 1.0].
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
		 * @param[in] inConeW The new width, clamped to [0.0, outerConeWidth()].
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
		 * @param[in] outConeW The new width, clamped to [innerConeWidth(), 360 degrees].
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
		 * @param[in] position The position of the source.
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
		 * @param[in] velocity The velocity of the source.
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
		 * @param[in] direction The direction of the source cone. Can also be OMNIDRECTIONAL.
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
		 * @param[in] type The new origin type.
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
		 * @param[in] looping Whether the source should loop.
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
		 * @param[in] buffer The buffer to use, or nullopt to unset any set/queued buffers.
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
		 * @param[in] buffer The buffer to attach. It must be of the same audio format as any other queued buffers.
		 **************************************************************************************************************/
		void queueBuffer(AudioBufferView buffer) noexcept;

		/**************************************************************************************************************
		 * Queues buffers for streaming.
		 *
		 * @param[in] buffers The buffers to attach. They must be of the same audio format as any other queued buffers.
		 **************************************************************************************************************/
		void queueBuffers(std::span<AudioBufferView> buffers) noexcept;

		/**************************************************************************************************************
		 * Removes a buffer from the source's queue.
		 *
		 * This function cannot be called if no buffers are able to be unqueued.
		 *
		 * @return A view over the unqueued buffer.
		 **************************************************************************************************************/
		AudioBufferView unqueueBuffer() const noexcept;

		/**************************************************************************************************************
		 * Gets the source's playback position within the current buffer.
		 *
		 * @return The source's playback position within the current buffer in seconds.
		 **************************************************************************************************************/
		SecondsF offset() const noexcept;

		/**************************************************************************************************************
		 * Sets the source's playback position within the current buffer.
		 *
		 * @param[in] offset The new playback position within the current buffer in seconds.
		 **************************************************************************************************************/
		void setOffset(SecondsF offset) noexcept;

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;

		friend class AudioStream;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::AudioSourceBadAlloc::what() const noexcept
{
	return "failed audio source allocation";
}

/// @endcond
