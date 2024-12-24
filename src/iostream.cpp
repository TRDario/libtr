#include "../include/tr/iostream.hpp"
#include <format>

tr::FileError::FileError(const char* path) noexcept
	: _path{path}
{
}

tr::FileError::FileError(std::string path) noexcept
	: _path{path}
{
}

tr::FileError::FileError(const std::filesystem::path& path)
	: _path{path.string()}
{
}

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
	std::ofstream file{path, openmode};
	if (!file.is_open()) {
		throw FileOpenError{path};
	}
	file.exceptions(std::ios::badbit | std::ios::failbit);
	return file;
}

std::ifstream tr::openFileR(const std::filesystem::path& path, std::ios::openmode openmode)
{
	if (!is_regular_file(path)) {
		throw FileNotFound{path};
	}
	std::ifstream file{path, openmode};
	if (!file.is_open()) {
		throw FileOpenError{path};
	}
	file.exceptions(std::ios::badbit | std::ios::failbit);
	return file;
}
