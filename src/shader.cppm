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
import :ranges;
import :shader_buffer;
import :texture_unit;

export namespace tr {
	/******************************************************************************************************************
	 * Error thrown when loading a shader failed.
	 ******************************************************************************************************************/
	struct ShaderLoadError : FileError {
		using FileError::FileError;
		/**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		virtual const char* what() const noexcept;
	};


	/******************************************************************************************************************
	 * Shader types.
	 ******************************************************************************************************************/
	enum class ShaderType : std::uint32_t {
		VERTEX 		 = 0x8B31,
		TESS_CONTROL = 0x8E88,
		TESS_EVAL 	 = 0x8E87,
		GEOMETRY 	 = 0x8DD9,
		FRAGMENT 	 = 0x8B30,
		COMPUTE 	 = 0x91B9
	};

	class Shader {
	public:
		/**************************************************************************************************************
	     * Loads a shader from file.
		 *
		 * @exception FileNotFound If the file was not found.
		 * @exception FileOpenError If opening the file failed.
		 * @exception std::bad_alloc If copying the contents of the file to a buffer failed.
		 * @exception ShaderLoadError If loading the shader failed.
		 *
		 * @param path The path to the shader file.
		 * @param type The shader type.
	     **************************************************************************************************************/
		Shader(const std::filesystem::path& path, ShaderType type);

		/**************************************************************************************************************
	     * Loads a shader from an embedded file.
		 *
		 * If loading the shader fails, a failed assertion is triggered.
		 *
		 * @param embeddedFile An embedded SPIR-V shader file.
		 * @param type The shader type.
	     **************************************************************************************************************/
		Shader(std::span<const std::byte> embeddedFile, ShaderType type) noexcept;
		

		/**************************************************************************************************************
	     * Equality comparison operator.
	     **************************************************************************************************************/
		friend bool operator==(const Shader&, const Shader&) noexcept;


		/**************************************************************************************************************
	     * Sets the type of the shader.
         *
         * @return The shader's type.
	     **************************************************************************************************************/
		ShaderType type() const noexcept;


		/**************************************************************************************************************
	     * Sets a boolean uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, bool value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a boolean array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const bool> value);
		
		/**************************************************************************************************************
	     * Sets a bvec2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::bvec2 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a bvec2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::bvec2> value);
		
		/**************************************************************************************************************
	     * Sets a bvec3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::bvec3 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a bvec3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::bvec3> value);
		
		/**************************************************************************************************************
	     * Sets a bvec4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::bvec4 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a bvec4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::bvec4> value);

		
		/**************************************************************************************************************
	     * Sets a integer uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, int value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a integer array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const int> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a ivec2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::ivec2 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a ivec2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::ivec2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a ivec3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::ivec3 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a ivec3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::ivec3> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a ivec4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::ivec4 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a ivec4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::ivec4> value) noexcept;

		
		/**************************************************************************************************************
	     * Sets an unsigned integer uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, unsigned int value) noexcept;
		
		/**************************************************************************************************************
	     * Sets an unsigned integer array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const unsigned int> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a uvec2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::uvec2 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a uvec2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::uvec2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a uvec3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::uvec3 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a uvec3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::uvec3> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a uvec4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::uvec4 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a uvec4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::uvec4> value) noexcept;

		/**************************************************************************************************************
	     * Sets a float uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, float value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a float array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const float> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a vec2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::vec2 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a vec2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::vec2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a vec3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::vec3 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a vec3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::vec3> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a vec4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::vec4 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a vec4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::vec4> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat2& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat3& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat3> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat4& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat4> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat2x3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat2x3& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat2x3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat2x3> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat2x4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat2x4& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat2x4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat2x4> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat3x2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat3x2& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat3x2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat3x2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat3x4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat3x4& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat3x4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat3x4> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat4x2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat4x2& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat4x2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat4x2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat4x3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, const glm::mat4x3& value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a mat4x3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat4x3> value) noexcept;


		/**************************************************************************************************************
	     * Sets a double uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, double value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a double array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const double> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a dvec2 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::dvec2 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a dvec2 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::dvec2> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a dvec3 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::dvec3 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a dvec3 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::dvec3> value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a dvec4 uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, glm::dvec4 value) noexcept;
		
		/**************************************************************************************************************
	     * Sets a dvec4 array uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param value The value to set to.
	     **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::dvec4> value) noexcept;


		/**************************************************************************************************************
	     * Sets a texture sampler uniform.
         *
         * @param index The index of the uniform. A failed assertion may be triggered if the index isn't valid or the
		 *              uniform at that index is not of the right type.
		 * @param unit The texture unit to bind the uniform to.
	     **************************************************************************************************************/
		void setUniform(int index, const TextureUnit& unit) noexcept;
	
		
		/**************************************************************************************************************
	     * Sets a shader storage buffer.
         *
         * @param index The index of the buffer. A failed assertion may be triggered if the index isn't valid.
		 * @param buffer The buffer to bind.
	     **************************************************************************************************************/
		void setStorageBuffer(unsigned int index, ShaderBuffer& buffer) noexcept;


		/**************************************************************************************************************
	     * Sets the debug label of the shader.
         *
         * @param label The new label of the shader.
	     **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;
	private:
		Handle<GLuint, 0, decltype([] (GLuint id) { glDeleteProgram(id); })> _id;
		ShaderType 					   										 _type;

		friend class ShaderPipeline;
	};
}

/// @cond IMPLEMENTATION

namespace tr {
	// Loads, compiles and links a shader program. Returns 0 on failure.
	GLuint constructProgram(std::span<const std::byte> data, ShaderType type) noexcept;
}

GLuint tr::constructProgram(std::span<const std::byte> data, ShaderType type) noexcept
{
	Handle<GLuint, 0, decltype([] (GLuint id) { glDeleteShader(id); })> shader { glCreateShader(GLenum(type)) };
	Handle<GLuint, 0, decltype([] (GLuint id) { glDeleteProgram(id); })> program { glCreateProgram() };

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
	: _type { type }
{
	auto file { openFileR(path, std::ios::binary) };
	auto data { flushBinary<std::vector<char>>(file) };
	_id.reset(constructProgram(rangeBytes(data), type), NO_EMPTY_HANDLE_CHECK);
	if (!_id.has_value()) {
		throw ShaderLoadError { path };
	}
}

tr::Shader::Shader(std::span<const std::byte> embeddedFile, ShaderType type) noexcept
	: _id { constructProgram(embeddedFile, type) }
	, _type { type }
{}

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

/// @endcond