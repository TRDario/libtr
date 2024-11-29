#include "../include/tr/shader.hpp"

#include "../include/tr/ranges.hpp"

#include <GL/glew.h>
#include <glm/ext.hpp>

namespace tr {
	// Loads, compiles and links a shader program. Returns 0 on failure.
	GLuint constructProgram(std::span<const std::byte> data, ShaderType type) noexcept;
} // namespace tr

GLuint tr::constructProgram(std::span<const std::byte> data, ShaderType type) noexcept
{
	Handle<GLuint, 0, decltype([](GLuint id) { glDeleteShader(id); })>  shader{glCreateShader(GLenum(type))};
	Handle<GLuint, 0, decltype([](GLuint id) { glDeleteProgram(id); })> program{glCreateProgram()};

	glShaderBinary(1, &shader.get(), GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());
	glSpecializeShader(shader.get(), "main", 0, nullptr, nullptr);

	int compiled;
	glGetShaderiv(shader.get(), GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		return 0;
	}

	glProgramParameteri(program.get(), GL_PROGRAM_SEPARABLE, GL_TRUE);
	glAttachShader(program.get(), shader.get());
	glLinkProgram(program.get());
	glDetachShader(program.get(), shader.get());

	int linked;
	glGetProgramiv(program.get(), GL_LINK_STATUS, &linked);
	if (!linked) {
		return 0;
	}

	return program.release();
}

const char* tr::ShaderLoadError::what() const noexcept
{
	static std::string str;
	str.clear();
	format_to(back_inserter(str), "Failed to load shader: {}", path());
	return str.c_str();
}

tr::Shader::Shader(const std::filesystem::path& path, ShaderType type)
	: _type{type}
{
	auto file{openFileR(path, std::ios::binary)};
	auto data{flushBinary<std::vector<char>>(file)};
	_id.reset(constructProgram(rangeBytes(data), type), NO_EMPTY_HANDLE_CHECK);
	if (!_id.has_value()) {
		throw ShaderLoadError{path};
	}
}

tr::Shader::Shader(std::span<const std::byte> embeddedFile, ShaderType type) noexcept
	: _id{constructProgram(embeddedFile, type)}, _type{type}
{
}

void tr::Shader::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteProgram(id);
}

bool tr::operator==(const Shader& l, const Shader& r) noexcept
{
	return l._id == r._id;
}

tr::ShaderType tr::Shader::type() const noexcept
{
	return _type;
}

void tr::Shader::setUniform(int index, bool value) noexcept
{
	setUniform(index, int(value));
}

void tr::Shader::setUniform(int index, glm::bvec2 value) noexcept
{
	setUniform(index, glm::ivec2(value));
}

void tr::Shader::setUniform(int index, glm::bvec3 value) noexcept
{
	setUniform(index, glm::ivec3(value));
}

void tr::Shader::setUniform(int index, glm::bvec4 value) noexcept
{
	setUniform(index, glm::ivec4(value));
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

void tr::Shader::setUniform(int index, const glm::mat2& value) noexcept
{
	glProgramUniformMatrix2fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat2> value) noexcept
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

void tr::Shader::setUniform(int index, const glm::mat2x3& value) noexcept
{
	glProgramUniformMatrix2x3fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat2x3> value) noexcept
{
	glProgramUniformMatrix2x3fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat2x4& value) noexcept
{
	glProgramUniformMatrix2x4fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat2x4> value) noexcept
{
	glProgramUniformMatrix2x4fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat3x2& value) noexcept
{
	glProgramUniformMatrix3x2fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat3x2> value) noexcept
{
	glProgramUniformMatrix3x2fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat3x4& value) noexcept
{
	glProgramUniformMatrix3x4fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat3x4> value) noexcept
{
	glProgramUniformMatrix3x4fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat4x2& value) noexcept
{
	glProgramUniformMatrix4x2fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat4x2> value) noexcept
{
	glProgramUniformMatrix4x2fv(_id.get(), index, value.size(), false, value_ptr(value[0]));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, const glm::mat4x3& value) noexcept
{
	glProgramUniformMatrix4x3fv(_id.get(), index, 1, false, value_ptr(value));
	assert(glGetError() != GL_INVALID_OPERATION);
}

void tr::Shader::setUniform(int index, std::span<const glm::mat4x3> value) noexcept
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

void tr::Shader::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_PROGRAM, _id.get(), label.size(), label.data());
}
