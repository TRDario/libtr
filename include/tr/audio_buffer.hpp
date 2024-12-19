#pragma once
#include "chrono.hpp"
#include "handle.hpp"
#include "iostream.hpp"
#include "ranges.hpp"
#include <span>

namespace tr {
	/** @ingroup audio
	 *  @defgroup audio_buffer Audio Buffer
	 *  Audio buffer class and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when allocating an audio buffer fails.
	 ******************************************************************************************************************/
	struct AudioBufferBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Error thrown when an trying to load an unsupported audio file.
	 ******************************************************************************************************************/
	struct UnsupportedAudioFile : FileError {
		using FileError::FileError;

		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Audio data format.
	 ******************************************************************************************************************/
	enum class AudioFormat {
		/**************************************************************************************************************
		 * 1-channel, 16-bit audio.
		 **************************************************************************************************************/
		MONO16 = 0x1101, // 16-bit mono audio.

		/**************************************************************************************************************
		 * 2-channel, 16-bit audio.
		 **************************************************************************************************************/
		STEREO16 = 0x1103 // 16-bit stereo audio.
	};

	/******************************************************************************************************************
	 * Non-owning audio buffer view.
	 ******************************************************************************************************************/
	class AudioBufferView {
	  public:
		/**************************************************************************************************************
		 * Equality comparison operator.
		 *
		 * Returns true if the views are to the same buffer, and false otherwise.
		 **************************************************************************************************************/
		friend bool operator==(const AudioBufferView&, const AudioBufferView&) noexcept = default;

		/**************************************************************************************************************
		 * Gets the length of the buffer.
		 *
		 * @return The length of the buffer in seconds.
		 **************************************************************************************************************/
		tr::SecondsF length() const noexcept;

		/**************************************************************************************************************
		 * Sets the data of the buffer.
		 *
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] data A span over the audio data.
		 * @param[in] format The format of the audio data.
		 * @param[in] frequency The frequency of the audio data.
		 **************************************************************************************************************/
		void set(std::span<const std::int16_t> data, AudioFormat format, int frequency);

	  protected:
		/// @cond IMPLEMENTATION
		unsigned int _id; // The OpenAL ID of the buffer.

		// Constructs a audio buffer view from an OpenAL id.
		AudioBufferView(unsigned int id) noexcept;
		/// @endcond

		friend class AudioBuffer;
		friend class AudioSource;
	};

	/******************************************************************************************************************
	 * Owning audio data buffer.
	 *
	 * AudioBuffer is default-constructible, non-copyable, and movable.
	 ******************************************************************************************************************/
	class AudioBuffer {
	  public:
		/**************************************************************************************************************
		 * Constructs an empty audio buffer.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 **************************************************************************************************************/
		AudioBuffer();

		/**************************************************************************************************************
		 * Constructs an audio buffer and immediately sets it.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] data A span over audio data.
		 * @param[in] format The format of the audio data.
		 * @param[in] frequency The frequency of the audio data.
		 **************************************************************************************************************/
		AudioBuffer(std::span<const std::int16_t> data, AudioFormat format, int frequency);

		/**************************************************************************************************************
		 * Move constructs an audio buffer.
		 *
		 * @param r The buffer to move from. @em r will be left in a state where other buffers can be moved into it,
		 *          but is otherwise unusable.
		 **************************************************************************************************************/
		AudioBuffer(AudioBuffer&& r) noexcept = default;

		/**************************************************************************************************************
		 * Destroys the audio buffer.
		 *
		 * @pre The audio buffer must not be in use.
		 **************************************************************************************************************/
		~AudioBuffer() noexcept = default;

		/**************************************************************************************************************
		 * Move assigns an audio buffer.
		 *
		 * @pre The assigned-to audio buffer must not be in use, as its previous data will be freed.
		 *
		 * @param r The buffer to move from. @em r will be left in a state where other buffers can be moved into it,
		 *          but is otherwise unusable.
		 *
		 * @return A reference to the assigned-to buffer.
		 **************************************************************************************************************/
		AudioBuffer& operator=(AudioBuffer&& r) noexcept = default;

		/**************************************************************************************************************
		 * Casts the audio buffer to an audio buffer view.
		 **************************************************************************************************************/
		operator AudioBufferView() const noexcept;

		/**************************************************************************************************************
		 * Gets the length of the buffer.
		 *
		 * @return The length of the buffer in seconds.
		 **************************************************************************************************************/
		tr::SecondsF length() const noexcept;

		/**************************************************************************************************************
		 * Sets the data of the buffer.
		 *
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] data A span over audio data.
		 * @param[in] format The format of the audio data.
		 * @param[in] frequency The frequency of the audio data.
		 **************************************************************************************************************/
		void set(std::span<const std::int16_t> data, AudioFormat format, int frequency);

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
	};

	/******************************************************************************************************************
	 * Loads audio data from an embedded file to a buffer.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception AudioBufferBadAlloc If allocating the buffer fails.
	 *
	 * @param[in] data
	 * @parblock
	 * The embedded file data.
	 *
	 * @pre @em data is assumed to always be a valid audio file.
	 * @endparblock
	 *
	 * @return An audio buffer loaded with data from the embedded file.
	 ******************************************************************************************************************/
	AudioBuffer loadEmbeddedAudio(std::span<const std::byte> data);

	/******************************************************************************************************************
	 * Loads audio data from an embedded file to a buffer.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception AudioBufferBadAlloc If allocating the buffer fails.
	 *
	 * @param[in] range
	 * @parblock
	 * The embedded file range.
	 *
	 * @pre @em range is assumed to always be a valid audio file.
	 * @endparblock
	 *
	 * @return An audio buffer loaded with data from the embedded file.
	 ******************************************************************************************************************/
	template <std::ranges::contiguous_range Range> AudioBuffer loadEmbeddedAudio(Range&& range)
	{
		return loadEmbeddedAudio(std::span<const std::byte>(rangeBytes(range)));
	}

	/******************************************************************************************************************
	 * Loads audio data from file to a buffer.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception FileNotFound If the file isn't found.
	 * @exception FileOpenError If opening the file fails.
	 * @exception UnsupportedAudioFile If the file is an unsupported or invalid format.
	 * @exception std::bad_alloc If allocating a buffer for reading the data fails.
	 * @exception AudioBufferBadAlloc If allocating the buffer fails.
	 *
	 * @param[in] path The path to an audio file.
	 *
	 * @return An audio buffer loaded with data from the file.
	 ******************************************************************************************************************/
	AudioBuffer loadAudioFile(const std::filesystem::path& path);

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::AudioBufferBadAlloc::what() const noexcept
{
	return "failed audio buffer allocation";
}

/// @endcond