module;
#include <cassert>
#include <GL/glew.h>

export module tr:shader;

import std;
import glm;
import :gl_buffer;
import :handle;
import :hashmap;
import :iostream;
import :shader_buffer;
import :texture_unit;

export namespace tr {
	struct ShaderCompilationError : std::runtime_error {
		ShaderCompilationError(std::string_view log);
	};
	struct InvalidShaderCacheFile : FileError {
		using FileError::FileError;
		virtual const char* what() const noexcept;
	};
	class UnknownShaderFileExtension : std::exception {
	public:
		UnknownShaderFileExtension(std::string extension) noexcept;
		virtual const char* what() const noexcept;
	private:
		std::string _extension;
	};

	class Shader {
	public:
		// Enum representing a shader type.
		enum class Type : std::uint32_t {
			VERTEX 		 = 0x8B31,
			TESS_CONTROL = 0x8E88,
			TESS_EVAL 	 = 0x8E87,
			GEOMETRY 	 = 0x8DD9,
			FRAGMENT 	 = 0x8B30,
			COMPUTE 	 = 0x91B9
		};
		
		Shader(const char* source, Type type);
		Shader(const std::string& source, Type type);
		// Depending on the path extension loads a shader source or cache file.
		Shader(const std::filesystem::path& path);

		friend bool operator==(const Shader& lhs, const Shader& rhs) noexcept;

		// Gets the type of the shader.
		Type type() const noexcept;

		// Sets a boolean uniform.
		void setUniform(int index, bool value) noexcept;
		// Sets a boolean array uniform.
		void setUniform(int index, std::span<const bool> value);
		// Sets a bvec2 uniform.
		void setUniform(int index, glm::bvec2 value) noexcept;
		// Sets a bvec2 array uniform.
		void setUniform(int index, std::span<const glm::bvec2> value);
		// Sets a bvec3 uniform.
		void setUniform(int index, glm::bvec3 value) noexcept;
		// Sets a bvec3 array uniform.
		void setUniform(int index, std::span<const glm::bvec3> value);
		// Sets a bvec4 uniform.
		void setUniform(int index, glm::bvec4 value) noexcept;
		// Sets a bvec4 array uniform.
		void setUniform(int index, std::span<const glm::bvec4> value);

		// Sets a i32 uniform.
		void setUniform(int index, int value) noexcept;
		// Sets a i32 array uniform.
		void setUniform(int index, std::span<const int> value) noexcept;
		// Sets a i32vec2 uniform.
		void setUniform(int index, glm::ivec2 value) noexcept;
		// Sets a i32vec2 array uniform.
		void setUniform(int index, std::span<const glm::ivec2> value) noexcept;
		// Sets a i32vec3 uniform.
		void setUniform(int index, glm::ivec3 value) noexcept;
		// Sets a i32vec3 array uniform.
		void setUniform(int index, std::span<const glm::ivec3> value) noexcept;
		// Sets a i32vec4 uniform.
		void setUniform(int index, glm::ivec4 value) noexcept;
		// Sets a i32vec4 array uniform.
		void setUniform(int index, std::span<const glm::ivec4> value) noexcept;

		// Sets a std::uint32_t uniform.
		void setUniform(int index, unsigned int value) noexcept;
		// Sets a std::uint32_t array uniform.
		void setUniform(int index, std::span<const unsigned int> value) noexcept;
		// Sets a u32vec2 uniform.
		void setUniform(int index, glm::uvec2 value) noexcept;
		// Sets a u32vec2 array uniform.
		void setUniform(int index, std::span<const glm::uvec2> value) noexcept;
		// Sets a u32vec3 uniform.
		void setUniform(int index, glm::uvec3 value) noexcept;
		// Sets a u32vec3 array uniform.
		void setUniform(int index, std::span<const glm::uvec3> value) noexcept;
		// Sets a u32vec4 uniform.
		void setUniform(int index, glm::uvec4 value) noexcept;
		// Sets a u32vec4 array uniform.
		void setUniform(int index, std::span<const glm::uvec4> value) noexcept;

		// Sets a float uniform.
		void setUniform(int index, float value) noexcept;
		// Sets a float array uniform.
		void setUniform(int index, std::span<const float> value) noexcept;
		// Sets a vec2 uniform.
		void setUniform(int index, glm::vec2 value) noexcept;
		// Sets a vec2 array uniform.
		void setUniform(int index, std::span<const glm::vec2> value) noexcept;
		// Sets a glm::vec3 uniform.
		void setUniform(int index, glm::vec3 value) noexcept;
		// Sets a glm::vec3 array uniform.
		void setUniform(int index, std::span<const glm::vec3> value) noexcept;
		// Sets a vec4 uniform.
		void setUniform(int index, glm::vec4 value) noexcept;
		// Sets a vec4 array uniform.
		void setUniform(int index, std::span<const glm::vec4> value) noexcept;
		// Sets a mat2 uniform.
		void setUniform(int index, const glm::mat<2, 2, float>& value) noexcept;
		// Sets a mat2 array uniform.
		void setUniform(int index, std::span<const glm::mat<2, 2, float>> value) noexcept;
		// Sets a mat3 uniform.
		void setUniform(int index, const glm::mat3& value) noexcept;
		// Sets a mat3 array uniform.
		void setUniform(int index, std::span<const glm::mat3> value) noexcept;
		// Sets a mat4 uniform.
		void setUniform(int index, const glm::mat4& value) noexcept;
		// Sets a mat4 array uniform.
		void setUniform(int index, std::span<const glm::mat4> value) noexcept;
		// Sets a mat2x3 uniform.
		void setUniform(int index, const glm::mat<2, 3, float>& value) noexcept;
		// Sets a mat2x3 array uniform.
		void setUniform(int index, std::span<const glm::mat<2, 3, float>> value) noexcept;
		// Sets a mat2x4 uniform.
		void setUniform(int index, const glm::mat<2, 4, float>& value) noexcept;
		// Sets a mat2x4 array uniform.
		void setUniform(int index, std::span<const glm::mat<2, 4, float>> value) noexcept;
		// Sets a mat3x2 uniform.
		void setUniform(int index, const glm::mat<3, 2, float>& value) noexcept;
		// Sets a mat3x2 array uniform.
		void setUniform(int index, std::span<const glm::mat<3, 2, float>> value) noexcept;
		// Sets a mat3x4 uniform.
		void setUniform(int index, const glm::mat<3, 4, float>& value) noexcept;
		// Sets a mat3x4 array uniform.
		void setUniform(int index, std::span<const glm::mat<3, 4, float>> value) noexcept;
		// Sets a mat4x2 uniform.
		void setUniform(int index, const glm::mat<4, 2, float>& value) noexcept;
		// Sets a mat4x2 array uniform.
		void setUniform(int index, std::span<const glm::mat<4, 2, float>> value) noexcept;
		// Sets a mat4x3 uniform.
		void setUniform(int index, const glm::mat<4, 3, float>& value) noexcept;
		// Sets a mat4x3 array uniform.
		void setUniform(int index, std::span<const glm::mat<4, 3, float>> value) noexcept;

		// Sets a double uniform.
		void setUniform(int index, double value) noexcept;
		// Sets a double array uniform.
		void setUniform(int index, std::span<const double> value) noexcept;
		// Sets a dvec2 uniform.
		void setUniform(int index, glm::dvec2 value) noexcept;
		// Sets a dvec2 array uniform.
		void setUniform(int index, std::span<const glm::dvec2> value) noexcept;
		// Sets a dvec3 uniform.
		void setUniform(int index, glm::dvec3 value) noexcept;
		// Sets a dvec3 array uniform.
		void setUniform(int index, std::span<const glm::dvec3> value) noexcept;
		// Sets a dvec4 uniform.
		void setUniform(int index, glm::dvec4 value) noexcept;
		// Sets a dvec4 array uniform.
		void setUniform(int index, std::span<const glm::dvec4> value) noexcept;

		// Sets a texture unit uniform.
		void setUniform(int index, const TextureUnit& unit) noexcept;
	
		// Sets a storage buffer.
		void setStorageBuffer(unsigned int index, ShaderBuffer& buffer) noexcept;

		// Returns true if the cache was successfully saved or false on error.
		bool saveCache(const std::filesystem::path& path) noexcept;

		void setLabel(std::string_view label) noexcept;
	private:
		struct Deleter { void operator()(GLuint id) noexcept; };
		Handle<GLuint, 0, Deleter> _id;
		Type 					   _type;

		void loadCacheFile(const std::filesystem::path& path);

		friend class GLContext;
	};
}

// IMPLEMENTATION

namespace tr {
	std::string shaderLog(unsigned int program);
	Shader::Type deduceShaderType(const std::filesystem::path& extension);
}

std::string tr::shaderLog(unsigned int program)
{
	int logSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
	std::string log(logSize, '\0');
	glGetProgramInfoLog(program, logSize, nullptr, log.data());
	return log;
}

tr::Shader::Type tr::deduceShaderType(const std::filesystem::path& extension)
{
	if (extension == ".vert") {
		return Shader::Type::VERTEX;
	}
	else if (extension == ".frag") {
		return Shader::Type::FRAGMENT;
	}
	else if (extension == ".tesc") {
		return Shader::Type::TESS_CONTROL;
	}
	else if (extension == ".tese") {
		return Shader::Type::TESS_EVAL;
	}
	else if (extension == ".geom") {
		return Shader::Type::GEOMETRY;
	}
	else if (extension == ".comp") {
		return Shader::Type::COMPUTE;
	}
	else {
		throw UnknownShaderFileExtension { extension };
	}
}

tr::ShaderCompilationError::ShaderCompilationError(std::string_view log)
	: runtime_error { std::format("Failed to compile/link shader program: {}", log) }
{}

tr::UnknownShaderFileExtension::UnknownShaderFileExtension(std::string extension) noexcept
	: _extension { extension }
{}

const char* tr::InvalidShaderCacheFile::what() const noexcept
{
	static std::string str;
	str.clear();
	format_to(back_inserter(str), "Invalid shader cache file: '{}'", path());
    return str.c_str();
}

const char* tr::UnknownShaderFileExtension::what() const noexcept
{
	static std::string str;
	str.clear();
    format_to(back_inserter(str), "Unknown shader file extension: '{}'", _extension);
    return str.c_str();
}

tr::Shader::Shader(const char* source, Type type)
	: _id { glCreateShaderProgramv(GLenum(type), 1, &source) }
	, _type { type }
{
	int linked;
	glGetProgramiv(_id.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		throw ShaderCompilationError { shaderLog(_id.get()) };
	}
}

tr::Shader::Shader(const std::string& source, Type type)
	: Shader { source.c_str(), type }
{}

tr::Shader::Shader(const std::filesystem::path& path)
{
	auto extension { path.extension() };
	if (extension == ".cache") {
		loadCacheFile(path);
	}
	else {
		auto type { deduceShaderType(extension) };
		auto file { openFileR(path) };
		*this = Shader { flushBinary<std::string>(file), type };
	}
}

void tr::Shader::loadCacheFile(const std::filesystem::path& path)
{
	auto file { openFileR(path) };

	auto magic { readBinary<std::array<char, 4>>(file) };
	if (std::string_view { magic.data(), 4 } != "TRSC") {
		throw InvalidShaderCacheFile { path };
	}
	readBinary(file, _type);
	auto format { readBinary<std::uint32_t>(file) };
	auto data { flushBinary<std::vector<char>>(file) };

	_id.reset(glCreateProgram());
	glProgramBinary(_id.get(), format, data.data(), data.size());
	int linked;
	glGetProgramiv(_id.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		throw ShaderCompilationError { shaderLog(_id.get()) };
	}
}

void tr::Shader::Deleter::operator()(GLuint id) noexcept
{
	glDeleteProgram(id);
}

bool tr::operator==(const Shader& lhs, const Shader& rhs) noexcept
{
	return lhs._id == rhs._id;
}

tr::Shader::Type tr::Shader::type() const noexcept
{
	return _type;
}

void tr::Shader::setUniform(int index, bool value) noexcept
{
	setUniform(index, int(value));
}

void tr::Shader::setUniform(int index, std::span<const bool> value)
{
	auto converted { value | std::views::transform([] (bool v) -> int { return v; }) | std::ranges::to<std::vector>() };
	setUniform(index, converted);
}

void tr::Shader::setUniform(int index, glm::bvec2 value)  noexcept
{
	setUniform(index, glm::ivec2(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::bvec2> value)
{
	auto converted { value | std::views::transform([] (auto& v) -> glm::ivec2 { return v; }) | std::ranges::to<std::vector>() };
	setUniform(index, converted);
}

void tr::Shader::setUniform(int index, glm::bvec3 value) noexcept
{
	setUniform(index, glm::ivec3(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::bvec3> value)
{
	auto converted { value | std::views::transform([] (auto& v) -> glm::ivec3 { return v; }) | std::ranges::to<std::vector>() };
	setUniform(index, converted);
}

void tr::Shader::setUniform(int index, glm::bvec4 value) noexcept
{
	setUniform(index, glm::ivec4(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::bvec4> value)
{
	auto converted { value | std::views::transform([] (auto& v) -> glm::ivec4 { return v; }) | std::ranges::to<std::vector>() };
	setUniform(index, converted);
}

void tr::Shader::setUniform(int index, int value) noexcept
{
	glProgramUniform1i(_id.get(), index, value);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const int> value) noexcept
{
	glProgramUniform1iv(_id.get(), index, value.size(), value.data());
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::ivec2 value) noexcept
{
	glProgramUniform2i(_id.get(), index, value.x, value.y);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::ivec2> value) noexcept
{
	glProgramUniform2iv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::ivec3 value) noexcept
{
	glProgramUniform3i(_id.get(), index, value.x, value.y, value.z);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::ivec3> value) noexcept
{
	glProgramUniform3iv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::ivec4 value) noexcept
{
	glProgramUniform4i(_id.get(), index, value.x, value.y, value.z, value.w);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::ivec4> value) noexcept
{
	glProgramUniform4iv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}


void tr::Shader::setUniform(int index, unsigned int value) noexcept
{
	glProgramUniform1ui(_id.get(), index, value);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const unsigned int> value) noexcept
{
	glProgramUniform1uiv(_id.get(), index, value.size(), value.data());
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::uvec2 value) noexcept
{
	glProgramUniform2ui(_id.get(), index, value.x, value.y);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::uvec2> value) noexcept
{
	glProgramUniform2uiv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::uvec3 value) noexcept
{
	glProgramUniform3ui(_id.get(), index, value.x, value.y, value.z);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::uvec3> value) noexcept
{
	glProgramUniform3uiv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::uvec4 value) noexcept
{
	glProgramUniform4ui(_id.get(), index, value.x, value.y, value.z, value.w);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::uvec4> value) noexcept
{
	glProgramUniform4uiv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, float value) noexcept
{
	glProgramUniform1f(_id.get(), index, value);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const float> value) noexcept
{
	glProgramUniform1fv(_id.get(), index, value.size(), value.data());
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::vec2 value) noexcept
{
	glProgramUniform2f(_id.get(), index, value.x, value.y);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::vec2> value) noexcept
{
	glProgramUniform2fv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::vec3 value) noexcept
{
	glProgramUniform3f(_id.get(), index, value.x, value.y, value.z);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::vec3> value) noexcept
{
	glProgramUniform3fv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::vec4 value) noexcept
{
	glProgramUniform4f(_id.get(), index, value.x, value.y, value.z, value.w);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::vec4> value) noexcept
{
	glProgramUniform4fv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<2, 2, float>& value) noexcept
{
	glProgramUniformMatrix2fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<2, 2, float>> value) noexcept
{
	glProgramUniformMatrix2fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat3& value) noexcept
{
	glProgramUniformMatrix3fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat3> value) noexcept
{
	glProgramUniformMatrix3fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat4& value) noexcept
{
	glProgramUniformMatrix4fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat4> value) noexcept
{
	glProgramUniformMatrix4fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<2, 3, float>& value) noexcept
{
	glProgramUniformMatrix2x3fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<2, 3, float>> value) noexcept
{
	glProgramUniformMatrix2x3fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<2, 4, float>& value) noexcept
{
	glProgramUniformMatrix2x4fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<2, 4, float>> value) noexcept
{
	glProgramUniformMatrix2x4fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<3, 2, float>& value) noexcept
{
	glProgramUniformMatrix3x2fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<3, 2, float>> value) noexcept
{
	glProgramUniformMatrix3x2fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<3, 4, float>& value) noexcept
{
	glProgramUniformMatrix3x4fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<3, 4, float>> value) noexcept
{
	glProgramUniformMatrix3x4fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<4, 2, float>& value) noexcept
{
	glProgramUniformMatrix4x2fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<4, 2, float>> value) noexcept
{
	glProgramUniformMatrix4x2fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat<4, 3, float>& value) noexcept
{
	glProgramUniformMatrix4x3fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat<4, 3, float>> value) noexcept
{
	glProgramUniformMatrix4x3fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, double value) noexcept
{
	glProgramUniform1d(_id.get(), index, value);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const double> value) noexcept
{
	glProgramUniform1dv(_id.get(), index, value.size(), value.data());
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::dvec2 value) noexcept
{
	glProgramUniform2d(_id.get(), index, value.x, value.y);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::dvec2> value) noexcept
{
	glProgramUniform2dv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::dvec3 value) noexcept
{
	glProgramUniform3d(_id.get(), index, value.x, value.y, value.z);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::dvec3> value) noexcept
{
	glProgramUniform3dv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, glm::dvec4 value) noexcept
{
	glProgramUniform4f(_id.get(), index, value.x, value.y, value.z, value.w);
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::dvec4> value) noexcept
{
	glProgramUniform4dv(_id.get(), index, value.size(), value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const TextureUnit& value) noexcept
{
	setUniform(index, int(value._id.get()));
}

void tr::Shader::setStorageBuffer(unsigned int index, ShaderBuffer& buffer) noexcept
{
	buffer.bindIndexedRange(GLBuffer::Target::SHADER_STORAGE_BUFFER, index, 0, buffer._headerSize + buffer._size);
	assert(glGetError() == GL_NO_ERROR);
}

bool tr::Shader::saveCache(const std::filesystem::path& path) noexcept
{
	std::ofstream file { path };
	if (!file.is_open()) {
		return false;
	}

	int size;
	std::uint32_t format;
	glGetProgramiv(_id.get(), GL_PROGRAM_BINARY_LENGTH, &size);
	std::vector<char> data(size);
	glGetProgramBinary(_id.get(), size, nullptr, &format, data.data());
	writeBinary(file, "TRSC");
	writeBinary(file, _type);
	writeBinary(file, format);
	writeBinaryRange(file, data);
	return true;
}

void tr::Shader::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_PROGRAM, _id.get(), label.size(), label.data());
}