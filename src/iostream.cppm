/**
 * @file iostream.cppm
 * @brief Provides various input/output functionality.
 */

export module tr:iostream;

import std;
import :integer;

export namespace tr {
    /******************************************************************************************************************
	 * Concept that denotes a binaryFlush-compatible container.
     *
     * To satisfy this requirement, a container must be a contiguous range of char-sized values.
	 ******************************************************************************************************************/
	template <class T>
    concept BinaryFlushableContainer = std::ranges::contiguous_range<T> && sizeof(typename T::value_type) == sizeof(Byte);


    /******************************************************************************************************************
	 * Generic file error base exception type.
     *
     * Cannot be thrown directly as what() is a pure virtual function.
	 ******************************************************************************************************************/
    class FileError : public std::exception {
    public:
        /**************************************************************************************************************
         * Constructs a file error with a path string.
	     **************************************************************************************************************/
        FileError(std::string path) noexcept;

        /**************************************************************************************************************
         * Constructs a file error with a path.
         *
         * @exception May throw std::bad_alloc due to copying the string.
	     **************************************************************************************************************/
        FileError(const std::filesystem::path& path);


        /**************************************************************************************************************
         * Pure virtual what() function.
	     **************************************************************************************************************/
        virtual const char* what() const noexcept = 0;

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
        virtual const char* what() const noexcept;
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
        virtual const char* what() const noexcept;
    };


    /******************************************************************************************************************
	 * Opens a file for writing with extra checks.
     *
     * @exception May throw FileNotFound, FileOpenError.
     *
     * @param[in] path The file path. Must be a valid, writable file, otherwise one of FileNotFound, FileOpenError
     * may be thrown.
     * @param[in] openmode std::ostream openmode arguments.
     *
     * @return An output file stream.
	 ******************************************************************************************************************/
    std::ofstream openFileW(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::in);
    
    /******************************************************************************************************************
	 * Opens a file for reading with extra checks.
     *
     * @exception May throw FileNotFound, FileOpenError.
     *
     * @param[in] path The file path. Must be a valid, readable file, otherwise one of FileNotFound, FileOpenError
     * may be thrown.
     * @param[in] openmode std::istream openmode arguments.
     *
     * @return An input file stream.
	 ******************************************************************************************************************/
    std::ifstream openFileR(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::out);


	/******************************************************************************************************************
	 * Reads binary data into an existing object.
     *
     * @exception May throw an exception if the stream throws one.
     *
     * @param[in] is The input stream.
     * @param[out] out The output object.
	 ******************************************************************************************************************/
	template <class T>
    void readBinary(std::istream& is, T& out);

	/******************************************************************************************************************
	 * Reads binary data into a new object.
     *
     * @exception May throw an exception if the stream throws one.
     *
     * @param[in] is The input stream.
     *
     * @return A new object with the extracted value.
	 ******************************************************************************************************************/
	template <class T>
    T readBinary(std::istream& is);

	/******************************************************************************************************************
	 * Reads binary data into a contiguous range.
     *
     * @exception May throw an exception if the stream throws one.
     *
     * @param[in] is The input stream.
     * @param[out] out The output range.
	 ******************************************************************************************************************/
	template <std::ranges::contiguous_range Range>
    void readBinary(std::istream& is, Range&& out);


	/******************************************************************************************************************
	 * Flushes the rest of the stream into an output iterator.
     *
     * @exception May throw an exception if the stream throws one, or if the iterator throws one.
     *
     * @param[in] is The input stream.
     * @param[out] out The output iterator.
	 ******************************************************************************************************************/
	template <std::output_iterator<char> It>
    void flushBinary(std::istream& is, It out);

	/******************************************************************************************************************
	 * Flushes the rest of the stream into a container.
     *
     * @em Container must satisfy @em BinaryFlushableContainer.
     *
     * @exception May throw an exception if the stream throws one, or if the container construction throws one.
     *
     * @param[in] is The input stream.
     *
     * @return A container with the rest of the stream data.
	 ******************************************************************************************************************/
	template <BinaryFlushableContainer Container>
    Container flushBinary(std::istream& is);


	/******************************************************************************************************************
	 * Writes an object's binary data to stream.
     *
     * @exception May throw an exception if the stream throws one.
     *
     * @param[out] os The output stream.
     * @param[in] in The input object.
	 ******************************************************************************************************************/
	template <class T>
    void writeBinary(std::ostream& os, const T& in);

	/******************************************************************************************************************
	 * Writes a contiguous range's binary data to stream.
     *
     * @exception May throw an exception if the stream throws one.
     *
     * @param[out] os The output stream.
     * @param[in] range The input range.
	 ******************************************************************************************************************/
	template <std::ranges::contiguous_range Range>
    void writeBinary(std::ostream& os, const Range& range);

	/******************************************************************************************************************
	 * Writes a C-string to stream.
     *
     * @exception May throw an exception if the stream throws one.
     *
     * @param[out] os The output stream.
     * @param[in] cstr The input string.
	 ******************************************************************************************************************/
	inline void writeBinary(std::ostream& os, const char* cstr);
}

// IMPLEMENTATION

tr::FileError::FileError(std::string path) noexcept
    : _path { path }
{}

tr::FileError::FileError(const std::filesystem::path& path)
    : _path { path.string() }
{}

const std::string& tr::FileError::path() const noexcept
{
    return _path;
}

const char* tr::FileNotFound::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "File not found: {}", path());
    return str.c_str();
}

const char* tr::FileOpenError::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "Failed to open file: {}", path());
    return str.c_str();
}

std::ofstream tr::openFileW(const std::filesystem::path& path, std::ios::openmode openmode)
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }
    std::ofstream file { path, openmode };
    if (!file.is_open()) {
        throw FileOpenError { path };
    }
    file.exceptions(std::ios::badbit | std::ios::failbit);
    return file;
}

std::ifstream tr::openFileR(const std::filesystem::path& path, std::ios::openmode openmode)
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }
    std::ifstream file { path, openmode };
    if (!file.is_open()) {
        throw FileOpenError { path };
    }
    file.exceptions(std::ios::badbit | std::ios::failbit);
    return file;
}

template <class T>
void tr::readBinary(std::istream& is, T& out)
{
    is.read((char*)(std::addressof(out)), sizeof(T));
}

template <class T>
T tr::readBinary(std::istream& is)
{
    std::array<char, sizeof(T)> bytes;
    is.read(bytes.data(), sizeof(T));
    return std::bit_cast<T>(bytes);
}

template <std::ranges::contiguous_range Range>
void tr::readBinary(std::istream& is, Range&& out)
{
    is.read((char*)(std::addressof(*out.begin())), out.size() * sizeof(typename Range::value_type));
}

template <std::output_iterator<char> It>
void tr::flushBinary(std::istream& is, It out)
{
    std::copy(std::istreambuf_iterator<char> { is }, std::istreambuf_iterator<char> {}, out);
}

template <tr::BinaryFlushableContainer Container>
Container tr::flushBinary(std::istream& is)
{
    Container out;
    flushBinary(is, std::back_inserter(out));
    return out;
}

template <class T>
void tr::writeBinary(std::ostream& os, const T& in)
{
    os.write((const char*)(std::addressof(in)), sizeof(T));
}

template <std::ranges::contiguous_range Range>
void tr::writeBinary(std::ostream& os, const Range& range)
{
    os.write((const char*)(std::addressof(*range.begin())), range.size() * sizeof(typename Range::value_type));
}

inline void tr::writeBinary(std::ostream& os, const char* cstr)
{
    os.write(cstr, std::strlen(cstr));
}