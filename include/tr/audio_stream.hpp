/**
 * @file audio_stream.hpp
 * @brief Provides an audio stream class.
 */

#pragma once
#include "audio_source.hpp"

#include <mutex>

struct sf_private_tag;

namespace tr {
	/******************************************************************************************************************
	 * Streamed audio source.
	 ******************************************************************************************************************/
	class AudioStream : private AudioSource {
	public:
		/**************************************************************************************************************
	     * Sentinel value representing the beginning of the streamed file.
	     **************************************************************************************************************/
		static constexpr SecondsF START {SecondsF::zero()};

		/**************************************************************************************************************
	     * Sentinel value representing the end of the streamed file.
	     **************************************************************************************************************/
		static constexpr SecondsF END {SecondsF::max()};

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
         * @param path The path to the audio file to load.
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
		friend bool  operator==(const AudioStream&, const AudioStream&) noexcept;

		using AudioSource::direction;
		using AudioSource::gain;
		using AudioSource::innerConeWidth;
		using AudioSource::maxDistance;
		using AudioSource::maxGain;
		using AudioSource::minGain;
		using AudioSource::origin;
		using AudioSource::outerConeGain;
		using AudioSource::outerConeWidth;
		using AudioSource::pitch;
		using AudioSource::position;
		using AudioSource::referenceDistance;
		using AudioSource::rolloff;
		using AudioSource::setDirection;
		using AudioSource::setGain;
		using AudioSource::setInnerConeWidth;
		using AudioSource::setMaxDistance;
		using AudioSource::setMaxGain;
		using AudioSource::setMinGain;
		using AudioSource::setOrigin;
		using AudioSource::setOuterConeGain;
		using AudioSource::setOuterConeWidth;
		using AudioSource::setPitch;
		using AudioSource::setPosition;
		using AudioSource::setReferenceDistance;
		using AudioSource::setRolloff;
		using AudioSource::setVelocity;
		using AudioSource::state;
		using AudioSource::velocity;

		/**************************************************************************************************************
	     * Gets whether the stream is empty.
         *
         * @return True if the stream is empty, and false otherwise.
	     **************************************************************************************************************/
		bool     empty() const noexcept;

		/**************************************************************************************************************
	     * Gets whether the stream is looping.
         *
         * @return True if the stream is looping, and false otherwise.
	     **************************************************************************************************************/
		bool     looping() const noexcept;

		/**************************************************************************************************************
	     * Sets whether the stream is looping.
         *
         * @param looping Whether the stream should loop.
	     **************************************************************************************************************/
		void     setLooping(bool looping) noexcept;

		/**************************************************************************************************************
	     * Plays the audio stream.
		 *
		 * If the audio stream is empty, nothing happens.
		 *
		 * @exception AudioBufferBadAlloc If a stopped source is played and refreshing the internal buffers fails.
	     **************************************************************************************************************/
		void     play();

		/**************************************************************************************************************
	     * Stops the audio stream.
		 *
		 * If the audio stream is empty, nothing happens.
	     **************************************************************************************************************/
		void     stop() noexcept;

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
		void     setOffset(SecondsF off);

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
		void     setLoopPoints(SecondsF start, SecondsF end) noexcept;

	private:
		// Extends an audio buffer with a tag indicating where its data starts in the audio file.
		struct Buffer : AudioBuffer {
			/// @private
			int startFileOffset;
		};

		struct FileCloser {
			/// @private
			void operator()(sf_private_tag* file) const noexcept;
		};

		std::array<Buffer, 4>                       _buffers;
		std::unique_ptr<sf_private_tag, FileCloser> _file;
		int                                         _length;
		int                                         _channels;
		int                                         _sampleRate;
		bool                                        _looping;
		int                                         _loopStart;
		int                                         _loopEnd;
		mutable std::mutex                          _mutex;

		void                                        refillBuffer(Buffer& buffer);

		friend void                                 audioStreamThread() noexcept;
	};
} // namespace tr
