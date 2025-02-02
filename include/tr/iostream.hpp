#pragma once
#include "concepts.hpp"

namespace tr {
	/** @ingroup misc
	 *  @defgroup stream Streams
	 *  Custom I/O stream functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Concept that denotes a binaryFlush-compatible iterator.
	 *
	 * To satisfy this requirement, a container must be an output iterator to a char variant or std::byte.
	 ******************************************************************************************************************/
	template <class T>
	concept BinaryFlushableIterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
									  std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;

	/******************************************************************************************************************
	 * Generic file error base exception type.
	 *
	 * Cannot be thrown directly as what() is a pure virtual function.
	 ******************************************************************************************************************/
	class FileError : public std::exception {
	  public:
		/**************************************************************************************************************
		 * Constructs a file error with a path string.
		 *
		 * @param[in] path A path string.
		 **************************************************************************************************************/
		FileError(const char* path) noexcept;

		/**************************************************************************************************************
		 * Constructs a file error with a path string.
		 *
		 * @param[in] path A path string.
		 **************************************************************************************************************/
		FileError(std::string path) noexcept;

		/**************************************************************************************************************
		 * Constructs a file error with a path.
		 *
		 * @param[in] path A filesystem path.
		 *
		 * @exception std::bad_alloc If copying the string fails.
		 **************************************************************************************************************/
		FileError(const std::filesystem::path& path);

		/**************************************************************************************************************
		 * Pure virtual what() function.
		 **************************************************************************************************************/
		const char* what() const noexcept override = 0;

		/**************************************************************************************************************
		 * Gets the path string.
		 *
		 * @return A reference to the path string.
		 **************************************************************************************************************/
		const std::string& path() const noexcept;

	  private:
		std::string _path;
	};

	/******************************************************************************************************************
	 * File not found exception.
	 ******************************************************************************************************************/
	struct FileNotFound : FileError {
		using FileError::FileError;

		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		const char* what() const noexcept override;
	};

	/******************************************************************************************************************
	 * File opening error exception.
	 ******************************************************************************************************************/
	struct FileOpenError : FileError {
		using FileError::FileError;

		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		const char* what() const noexcept override;
	};

	/******************************************************************************************************************
	 * Opens a file for writing with extra checks.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception FileOpenError If the file at path cannot be opened.
	 *
	 * @param[in] path The file path. Must be a valid, writable file.
	 * @param[in] openmode std::ostream openmode arguments.
	 *
	 * @return An output file stream.
	 ******************************************************************************************************************/
	std::ofstream openFileW(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::in);

	/******************************************************************************************************************
	 * Opens a file for reading with extra checks.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception FileNotFound If the file path does not lead to a regular file.
	 * @exception FileOpenError If the file at path cannot be opened.
	 *
	 * @param[in] path The file path. Must be a valid, readable file.
	 * @param[in] openmode std::istream openmode arguments.
	 *
	 * @return An input file stream.
	 ******************************************************************************************************************/
	std::ifstream openFileR(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::out);

	/******************************************************************************************************************
	 * Reads binary data into an existing object.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[in] is The input stream.
	 * @param[out] out The output object.
	 ******************************************************************************************************************/
	template <StandardLayout T> void readBinary(std::istream& is, T& out);

	/******************************************************************************************************************
	 * Reads binary data into a new object.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[in] is The input stream.
	 *
	 * @return A new object with the extracted value.
	 ******************************************************************************************************************/
	template <StandardLayout T> T readBinary(std::istream& is);

	/******************************************************************************************************************
	 * Reads binary data into a contiguous range.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[in] is The input stream.
	 * @param[out] out The output range.
	 ******************************************************************************************************************/
	template <StandardLayoutRange Range> void readBinaryRange(std::istream& is, Range&& out);

	/******************************************************************************************************************
	 * Flushes the rest of the stream into an output iterator.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[in] is The input stream.
	 * @param[out] out The output iterator.
	 ******************************************************************************************************************/
	template <tr::BinaryFlushableIterator It> void flushBinary(std::istream& is, It out);

	/******************************************************************************************************************
	 * Flushes the rest of the stream into a vector of bytes.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 * @exception std::bad_alloc If vector construction fails.
	 *
	 * @param[in] is The input stream.
	 *
	 * @return A byte vector with the rest of the stream data.
	 ******************************************************************************************************************/
	std::vector<std::byte> flushBinary(std::istream& is);

	/******************************************************************************************************************
	 * Writes an object's binary data to stream.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[out] os The output stream.
	 * @param[in] in The input object.
	 ******************************************************************************************************************/
	template <StandardLayout T> void writeBinary(std::ostream& os, const T& in);

	/******************************************************************************************************************
	 * Writes a contiguous range's binary data to stream.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[out] os The output stream.
	 * @param[in] range The input range.
	 ******************************************************************************************************************/
	template <StandardLayoutRange Range> void writeBinaryRange(std::ostream& os, const Range& range);

	/******************************************************************************************************************
	 * Writes a C-string to stream.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[out] os The output stream.
	 * @param[in] cstr The input string.
	 ******************************************************************************************************************/
	inline void writeBinary(std::ostream& os, const char* cstr);

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::StandardLayout T> void tr::readBinary(std::istream& is, T& out)
{
	is.read(reinterpret_cast<char*>(std::addressof(out)), sizeof(T));
}

template <tr::StandardLayout T> T tr::readBinary(std::istream& is)
{
	std::array<char, sizeof(T)> bytes;
	is.read(bytes.data(), sizeof(T));
	return std::bit_cast<T>(bytes);
}

template <tr::StandardLayoutRange Range> void tr::readBinaryRange(std::istream& is, Range&& out)
{
	is.read(reinterpret_cast<char*>(std::addressof(*out.begin())), out.size() * sizeof(typename Range::value_type));
}

template <tr::BinaryFlushableIterator It> void tr::flushBinary(std::istream& is, It out)
{
	while (!is.eof()) {
		if constexpr (std::output_iterator<It, char>) {
			*out++ = static_cast<char>(is.get());
		}
		else if constexpr (std::output_iterator<It, signed char>) {
			*out++ = static_cast<signed char>(is.get());
		}
		else if constexpr (std::output_iterator<It, unsigned char>) {
			*out++ = static_cast<unsigned char>(is.get());
		}
		else {
			*out++ = static_cast<std::byte>(is.get());
		}
	}
}

template <tr::StandardLayout T> void tr::writeBinary(std::ostream& os, const T& in)
{
	os.write(reinterpret_cast<const char*>(std::addressof(in)), sizeof(T));
}

template <tr::StandardLayoutRange Range> void tr::writeBinaryRange(std::ostream& os, const Range& range)
{
	os.write(reinterpret_cast<const char*>(std::addressof(*range.begin())),
			 range.size() * sizeof(typename Range::value_type));
}

inline void tr::writeBinary(std::ostream& os, const char* cstr)
{
	os.write(cstr, std::strlen(cstr));
}

/// @endcond
