#pragma once
#include "chrono.hpp"
#include "handle.hpp"
#include "iostream.hpp"
#include <span>

namespace tr {
	/** @addtogroup audio
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
		 * 1-channel, 8-bit audio.
		 **************************************************************************************************************/
		MONO8 = 0x11'00, // 8-bit mono audio.

		/**************************************************************************************************************
		 * 2-channel, 8-bit audio.
		 **************************************************************************************************************/
		STEREO8 = 0x11'02, // 8-bit stereo audio.

		/**************************************************************************************************************
		 * 1-channel, 16-bit audio.
		 **************************************************************************************************************/
		MONO16 = 0x11'01, // 16-bit mono audio.

		/**************************************************************************************************************
		 * 2-channel, 16-bit audio.
		 **************************************************************************************************************/
		STEREO16 = 0x11'03 // 16-bit stereo audio.
	};

	/******************************************************************************************************************
	 * Non-owning audio buffer view.
	 ******************************************************************************************************************/
	class AudioBufferView {
	  public:
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
		void set(std::span<const std::byte> data, AudioFormat format, int frequency);

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
	 * Owning audio buffer.
	 *
	 * This class cannot be instantiated before the audio system is initialized.
	 ******************************************************************************************************************/
	class AudioBuffer {
	  public:
		/**************************************************************************************************************
		 * Constructs an empty audio buffer.
		 *
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 **************************************************************************************************************/
		AudioBuffer();

		/**************************************************************************************************************
		 * Constructs an audio buffer and immediately sets it.
		 *
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] data A span over audio data.
		 * @param[in] format The format of the audio data.
		 * @param[in] frequency The frequency of the audio data.
		 **************************************************************************************************************/
		AudioBuffer(std::span<const std::byte> data, AudioFormat format, int frequency);

		/**************************************************************************************************************
		 * Loads audio data from file to a buffer.
		 *
		 * @exception FileNotFound If the file isn't found.
		 * @exception FileOpenError If opening the file fails.
		 * @exception UnsupportedAudioFile If the file is an unsupported or invalid format.
		 * @exception std::bad_alloc If allocating a buffer for reading the data fails.
		 * @exception AudioBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] path The path to an audio file.
		 **************************************************************************************************************/
		explicit AudioBuffer(const std::filesystem::path& path);

		friend bool operator==(const AudioBuffer&, const AudioBuffer&) noexcept = default;

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
		void set(std::span<const std::byte> data, AudioFormat format, int frequency);

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::AudioBufferBadAlloc::what() const noexcept
{
	return "failed audio buffer allocation";
}

/// @endcond
