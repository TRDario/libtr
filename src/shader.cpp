#include "../include/tr/shader.hpp"
#include "../include/tr/ranges.hpp"
#include "gl_call.hpp"
#include <format>
#include <glm/ext.hpp>

namespace tr {
	// Loads, compiles and links a shader program. Returns 0 on failure.
	GLuint constructProgram(std::span<const std::byte> data, ShaderType type) noexcept;
} // namespace tr

GLuint tr::constructProgram(std::span<const std::byte> data, ShaderType type) noexcept
{
	using ShaderHandle  = Handle<GLuint, 0, decltype([](GLuint id) { TR_GL_CALL(glDeleteShader, id); })>;
	using ProgramHandle = Handle<GLuint, 0, decltype([](GLuint id) { TR_GL_CALL(glDeleteShader, id); })>;

	ShaderHandle  shader{TR_RETURNING_GL_CALL(glCreateShader, GLenum(type))};
	ProgramHandle program{TR_RETURNING_GL_CALL(glCreateProgram)};

	TR_GL_CALL(glShaderBinary, 1, &shader.get(), GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());
	TR_GL_CALL(glSpecializeShader, shader.get(), "main", 0, nullptr, nullptr);

	int compiled;
	TR_GL_CALL(glGetShaderiv, shader.get(), GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		return 0;
	}

	TR_GL_CALL(glProgramParameteri, program.get(), GL_PROGRAM_SEPARABLE, GL_TRUE);
	TR_GL_CALL(glAttachShader, program.get(), shader.get());
	TR_GL_CALL(glLinkProgram, program.get());
	TR_GL_CALL(glDetachShader, program.get(), shader.get());

	int linked;
	TR_GL_CALL(glGetProgramiv, program.get(), GL_LINK_STATUS, &linked);
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

tr::Shader::Shader(unsigned int id, ShaderType type) noexcept
	: _id{id}, _type{type}
{
}

void tr::Shader::Deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteProgram, id);
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
	TR_GL_CALL(glProgramUniform1i, _id.get(), index, value);
}

void tr::Shader::setUniform(int index, std::span<const int> value) noexcept
{
	TR_GL_CALL(glProgramUniform1iv, _id.get(), index, value.size(), value.data());
}

void tr::Shader::setUniform(int index, glm::ivec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2i, _id.get(), index, value.x, value.y);
}

void tr::Shader::setUniform(int index, std::span<const glm::ivec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2iv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::ivec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3i, _id.get(), index, value.x, value.y, value.z);
}

void tr::Shader::setUniform(int index, std::span<const glm::ivec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3iv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::ivec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4i, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::Shader::setUniform(int index, std::span<const glm::ivec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4iv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, unsigned int value) noexcept
{
	TR_GL_CALL(glProgramUniform1ui, _id.get(), index, value);
}

void tr::Shader::setUniform(int index, std::span<const unsigned int> value) noexcept
{
	TR_GL_CALL(glProgramUniform1uiv, _id.get(), index, value.size(), value.data());
}

void tr::Shader::setUniform(int index, glm::uvec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2ui, _id.get(), index, value.x, value.y);
}

void tr::Shader::setUniform(int index, std::span<const glm::uvec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2uiv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::uvec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3ui, _id.get(), index, value.x, value.y, value.z);
}

void tr::Shader::setUniform(int index, std::span<const glm::uvec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3uiv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::uvec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4ui, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::Shader::setUniform(int index, std::span<const glm::uvec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4uiv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, float value) noexcept
{
	TR_GL_CALL(glProgramUniform1f, _id.get(), index, value);
}

void tr::Shader::setUniform(int index, std::span<const float> value) noexcept
{
	TR_GL_CALL(glProgramUniform1fv, _id.get(), index, value.size(), value.data());
}

void tr::Shader::setUniform(int index, glm::vec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2f, _id.get(), index, value.x, value.y);
}

void tr::Shader::setUniform(int index, std::span<const glm::vec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2fv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::vec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3f, _id.get(), index, value.x, value.y, value.z);
}

void tr::Shader::setUniform(int index, std::span<const glm::vec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3fv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::vec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4f, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::Shader::setUniform(int index, std::span<const glm::vec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4fv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat2& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat2> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat3& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat3> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat4& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat4> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat2x3& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x3fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat2x3> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x3fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat2x4& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x4fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat2x4> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix2x4fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat3x2& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x2fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat3x2> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x2fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat3x4& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x4fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat3x4> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix3x4fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat4x2& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x2fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat4x2> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x2fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const glm::mat4x3& value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x3fv, _id.get(), index, 1, false, value_ptr(value));
}

void tr::Shader::setUniform(int index, std::span<const glm::mat4x3> value) noexcept
{
	TR_GL_CALL(glProgramUniformMatrix4x3fv, _id.get(), index, value.size(), false, value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, double value) noexcept
{
	TR_GL_CALL(glProgramUniform1d, _id.get(), index, value);
}

void tr::Shader::setUniform(int index, std::span<const double> value) noexcept
{
	TR_GL_CALL(glProgramUniform1dv, _id.get(), index, value.size(), value.data());
}

void tr::Shader::setUniform(int index, glm::dvec2 value) noexcept
{
	TR_GL_CALL(glProgramUniform2d, _id.get(), index, value.x, value.y);
}

void tr::Shader::setUniform(int index, std::span<const glm::dvec2> value) noexcept
{
	TR_GL_CALL(glProgramUniform2dv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::dvec3 value) noexcept
{
	TR_GL_CALL(glProgramUniform3d, _id.get(), index, value.x, value.y, value.z);
}

void tr::Shader::setUniform(int index, std::span<const glm::dvec3> value) noexcept
{
	TR_GL_CALL(glProgramUniform3dv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, glm::dvec4 value) noexcept
{
	TR_GL_CALL(glProgramUniform4f, _id.get(), index, value.x, value.y, value.z, value.w);
}

void tr::Shader::setUniform(int index, std::span<const glm::dvec4> value) noexcept
{
	TR_GL_CALL(glProgramUniform4dv, _id.get(), index, value.size(), value_ptr(value[0]));
}

void tr::Shader::setUniform(int index, const TextureUnit& value) noexcept
{
	setUniform(index, int(value._id.get()));
}

void tr::Shader::setStorageBuffer(unsigned int index, ShaderBuffer& buffer) noexcept
{
	buffer.bindIndexedRange(GraphicsBuffer::Target::SHADER_STORAGE_BUFFER, index, 0, buffer._headerSize + buffer._size);
}

void tr::Shader::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_PROGRAM, _id.get(), label.size(), label.data());
}

tr::Shader tr::loadEmbeddedShader(std::span<const std::byte> data, ShaderType type) noexcept
{
	return Shader{constructProgram(data, type), type};
}

tr::Shader tr::loadShaderFile(const std::filesystem::path& path, ShaderType type)
{
	auto file{openFileR(path, std::ios::binary)};
	auto data{flushBinary<std::vector<char>>(file)};
	auto id{constructProgram(rangeBytes(data), type)};
	if (id == 0) {
		throw ShaderLoadError{path};
	}
	return Shader{id, type};
}