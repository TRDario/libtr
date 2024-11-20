/**
 * @file shader_pipeline.hpp
 * @brief Provides a shader program pipeline class.
 */

#pragma once
#include "shader.hpp"
#include <string_view>

namespace tr {
	/******************************************************************************************************************
	 * A shader program pipeline.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	class ShaderPipeline {
	  public:
		/**************************************************************************************************************
		 * Creates a shader pipeline.
		 *
		 * All shaders must actually be of the type they're passed as, otherwise a failed assertion may be triggered.
		 *
		 * @param vertexShader The vertex shader.
		 * @param fragmentShader The fragment shader.
		 **************************************************************************************************************/
		ShaderPipeline(const Shader& vertexShader, const Shader& fragmentShader) noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the pipeline.
		 *
		 * @param label The new label of the pipeline.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		struct Deleter {
			/// @private
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;

		void bind() const noexcept;

		friend class GLContext;
	};

	/******************************************************************************************************************
	 * A shader program pipeline that owns its shaders.
	 ******************************************************************************************************************/
	class OwningShaderPipeline {
	  public:
		/**************************************************************************************************************
		 * Creates an owning shader pipeline.
		 *
		 * @param vertexShader The vertex shader.
		 * @param fragmentShader The fragment shader.
		 **************************************************************************************************************/
		OwningShaderPipeline(Shader vertexShader, Shader fragmentShader) noexcept;

		/**************************************************************************************************************
		 * Gets the base pipeline object.
		 *
		 * @return A reference to the base pipeline object.
		 **************************************************************************************************************/
		operator const ShaderPipeline&() const noexcept;

		/**************************************************************************************************************
		 * Gets the vertex shader.
		 *
		 * @return A reference to the vertex shader.
		 **************************************************************************************************************/
		Shader& vertexShader() noexcept;

		/**************************************************************************************************************
		 * Gets the vertex shader.
		 *
		 * @return A constant reference to the vertex shader.
		 **************************************************************************************************************/
		const Shader& vertexShader() const noexcept;

		/**************************************************************************************************************
		 * Gets the fragment shader.
		 *
		 * @return A reference to the fragment shader.
		 **************************************************************************************************************/
		Shader& fragmentShader() noexcept;

		/**************************************************************************************************************
		 * Gets the fragment shader.
		 *
		 * @return A constant reference to the fragment shader.
		 **************************************************************************************************************/
		const Shader& fragmentShader() const noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the pipeline.
		 *
		 * @param label The new label of the pipeline.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		Shader _vertexShader;
		Shader _fragmentShader;
		ShaderPipeline _pipeline;
	};
} // namespace tr