#pragma once
#include "handle.hpp"
#include "iostream.hpp"

namespace tr {
	class ShaderBuffer;
	class TextureUnit;

	/** @ingroup graphics
	 *  @defgroup shader Shader
	 *  Shader class and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when loading a shader fails.
	 ******************************************************************************************************************/
	struct ShaderLoadError : FileError {
		using FileError::FileError;
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		const char* what() const noexcept override;
	};

	/******************************************************************************************************************
	 * Shader types.
	 ******************************************************************************************************************/
	enum class ShaderType : std::uint32_t {
		VERTEX   = 0x8B31,
		FRAGMENT = 0x8B30,
		// COMPUTE = 0x91B9
	};

	/******************************************************************************************************************
	 * GPU shader program.
	 *
	 * Shader is movable, but not copyable. A moved shader is left in a state where another shader can be moved into it,
	 * but is otherwise unusable.
	 ******************************************************************************************************************/
	class Shader {
	  public:
		/**************************************************************************************************************
		 * Sets the type of the shader.
		 *
		 * @return The shader's type.
		 **************************************************************************************************************/
		ShaderType type() const noexcept;

		/**************************************************************************************************************
		 * Sets a boolean uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, bool value) noexcept;

		/**************************************************************************************************************
		 * Sets a bvec2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::bvec2 value) noexcept;

		/**************************************************************************************************************
		 * Sets a bvec3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::bvec3 value) noexcept;

		/**************************************************************************************************************
		 * Sets a bvec4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::bvec4 value) noexcept;

		/**************************************************************************************************************
		 * Sets a integer uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, int value) noexcept;

		/**************************************************************************************************************
		 * Sets a integer array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const int> value) noexcept;

		/**************************************************************************************************************
		 * Sets a ivec2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::ivec2 value) noexcept;

		/**************************************************************************************************************
		 * Sets a ivec2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::ivec2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a ivec3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::ivec3 value) noexcept;

		/**************************************************************************************************************
		 * Sets a ivec3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::ivec3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a ivec4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::ivec4 value) noexcept;

		/**************************************************************************************************************
		 * Sets a ivec4 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::ivec4> value) noexcept;

		/**************************************************************************************************************
		 * Sets an unsigned integer uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, unsigned int value) noexcept;

		/**************************************************************************************************************
		 * Sets an unsigned integer array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const unsigned int> value) noexcept;

		/**************************************************************************************************************
		 * Sets a uvec2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::uvec2 value) noexcept;

		/**************************************************************************************************************
		 * Sets a uvec2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::uvec2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a uvec3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::uvec3 value) noexcept;

		/**************************************************************************************************************
		 * Sets a uvec3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::uvec3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a uvec4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::uvec4 value) noexcept;

		/**************************************************************************************************************
		 * Sets a uvec4 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::uvec4> value) noexcept;

		/**************************************************************************************************************
		 * Sets a float uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, float value) noexcept;

		/**************************************************************************************************************
		 * Sets a float array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const float> value) noexcept;

		/**************************************************************************************************************
		 * Sets a vec2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::vec2 value) noexcept;

		/**************************************************************************************************************
		 * Sets a vec2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::vec2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a vec3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::vec3 value) noexcept;

		/**************************************************************************************************************
		 * Sets a vec3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::vec3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a vec4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::vec4 value) noexcept;

		/**************************************************************************************************************
		 * Sets a vec4 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::vec4> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat2& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat3& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat4& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat4 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat4> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat2x3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat2x3& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat2x3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat2x3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat2x4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat2x4& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat2x4 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat2x4> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat3x2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat3x2& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat3x2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat3x2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat3x4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat3x4& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat3x4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat3x4> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat4x2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat4x2& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat4x2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat4x2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat4x3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, const glm::mat4x3& value) noexcept;

		/**************************************************************************************************************
		 * Sets a mat4x3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::mat4x3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a double uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, double value) noexcept;

		/**************************************************************************************************************
		 * Sets a double array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const double> value) noexcept;

		/**************************************************************************************************************
		 * Sets a dvec2 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::dvec2 value) noexcept;

		/**************************************************************************************************************
		 * Sets a dvec2 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::dvec2> value) noexcept;

		/**************************************************************************************************************
		 * Sets a dvec3 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::dvec3 value) noexcept;

		/**************************************************************************************************************
		 * Sets a dvec3 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::dvec3> value) noexcept;

		/**************************************************************************************************************
		 * Sets a dvec4 uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, glm::dvec4 value) noexcept;

		/**************************************************************************************************************
		 * Sets a dvec4 array uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] value The value to set to.
		 **************************************************************************************************************/
		void setUniform(int index, std::span<const glm::dvec4> value) noexcept;

		/**************************************************************************************************************
		 * Sets a texture sampler uniform.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the uniform.
		 *
		 * @pre The uniform at the index must be valid and of the matching type.
		 * @endparblock
		 * @param[in] unit The texture unit to bind the uniform to.
		 **************************************************************************************************************/
		void setUniform(int index, const TextureUnit& unit) noexcept;

		/**************************************************************************************************************
		 * Sets a shader storage buffer.
		 *
		 * @param[in] index
		 * @parblock
		 * The index of the buffer.
		 *
		 * @pre The buffer at the index must be valid.
		 * @endparblock
		 * @param[in] buffer The buffer to bind.
		 **************************************************************************************************************/
		void setStorageBuffer(unsigned int index, ShaderBuffer& buffer) noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the shader.
		 *
		 * @param[in] label The new label of the shader.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
		ShaderType                       _type;

		Shader(unsigned int id, ShaderType type) noexcept;

		friend class ShaderPipeline;
		friend Shader loadEmbeddedShader(std::span<const std::byte> embeddedFile, ShaderType type) noexcept;
		friend Shader loadShaderFile(const std::filesystem::path& path, ShaderType type);
	};

	/**************************************************************************************************************
	 * Loads a shader from an embedded file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @param data An embedded SPIR-V shader file.
	 * @param type The shader type.
	 *
	 * @return The loaded shader.
	 **************************************************************************************************************/
	Shader loadEmbeddedShader(std::span<const std::byte> data, ShaderType type) noexcept;

	/**************************************************************************************************************
	 * Loads a shader from an embedded file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @param range An embedded SPIR-V shader file.
	 * @param type The shader type.
	 *
	 * @return The loaded shader.
	 **************************************************************************************************************/
	template <std::ranges::contiguous_range Range> Shader loadEmbeddedShader(Range&& range, ShaderType type) noexcept
	{
		return loadEmbeddedShader(std::span<const std::byte>(rangeBytes(range)), type);
	}

	/**************************************************************************************************************
	 * Loads a shader from file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception FileNotFound If the file is not found.
	 * @exception FileOpenError If opening the file fails.
	 * @exception std::bad_alloc If copying the contents of the file to a buffer fails.
	 * @exception ShaderLoadError If loading the shader fails.
	 *
	 * @param path The path to the shader file.
	 * @param type The shader type.
	 *
	 * @return The loaded shader.
	 **************************************************************************************************************/
	Shader loadShaderFile(const std::filesystem::path& path, ShaderType type);

	/// @}
} // namespace tr
