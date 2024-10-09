export module tr:iostream;

import std;
import :integer;

export namespace tr {
    class FileError : public std::exception {
    public:
        FileError(std::string path);
        FileError(const std::filesystem::path& path);

        virtual const char* what() const noexcept = 0;
        const std::string& path() const noexcept;
    private:
        std::string _path;
    };

    struct FileNotFound : FileError {
        using FileError::FileError;
        virtual const char* what() const noexcept;
    };
    struct FileOpenError : FileError {
        using FileError::FileError;
        virtual const char* what() const noexcept;
    };

    std::ofstream openFileW(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::in);
    std::ifstream openFileR(const std::filesystem::path& path, std::ios::openmode openmode = std::ios::out);

    // Concept representing a container that flushBinary can be done into.
	template <class T> concept BinaryFlushableContainer = std::ranges::contiguous_range<T> && sizeof(typename T::value_type) == sizeof(Byte);

	// Read binary data into an existing object.
	template <class T> 						 	   void 	 readBinary (std::istream& is, T& out);
	// Reads binary data into a new object.
	template <class T> 						 	   T    	 readBinary (std::istream& is);
	// Reads raw binary data into a range.
	template <std::ranges::contiguous_range Range> void 	 readBinary (std::istream& is, Range&& out);
	// Flushes the rest of the stream into an output iterator.
	template <std::output_iterator<char> It> 	   void 	 flushBinary(std::istream& is, It out);
	// Flushes the rest of the stream into a container.
	template <BinaryFlushableContainer Container>  Container flushBinary(std::istream& is);

	// Writes an object as binary data.
	template <class T> 					           void writeBinary(std::ostream& os, const T& in);
	// Writes a range as binary data.
	template <std::ranges::contiguous_range Range> void writeBinary(std::ostream& os, const Range& range);
	// Writes a C string as binary data.
	inline 								           void writeBinary(std::ostream& os, const char* cstr);
}

// IMPLEMENTATION

tr::FileError::FileError(std::string path)
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