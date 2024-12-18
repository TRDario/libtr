#pragma once
#include "shader.hpp"
#include <string_view>

namespace tr {
	/** @ingroup graphics
	 *  @defgroup shader_pipeline Shader Pipelines
	 *  Shader pipeline classes.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Shader program pipeline.
	 *
	 * OwningShaderPipeline is non-copyable and moveable.
	 ******************************************************************************************************************/
	class ShaderPipeline {
	  public:
		/**************************************************************************************************************
		 * Creates a shader pipeline.
		 *
		 * All shaders must actually be of the type they're passed as.
		 *
		 * @param[in] vertexShader The vertex shader.
		 * @param[in] fragmentShader The fragment shader.
		 **************************************************************************************************************/
		ShaderPipeline(const Shader& vertexShader, const Shader& fragmentShader) noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the pipeline.
		 *
		 * @param[in] label The new label of the pipeline.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;

		void bind() const noexcept;

		friend class GraphicsContext;
	};

	/******************************************************************************************************************
	 * Shader program pipeline that owns its shaders.
	 *
	 * OwningShaderPipeline is non-copyable and moveable.
	 ******************************************************************************************************************/
	class OwningShaderPipeline {
	  public:
		/**************************************************************************************************************
		 * Creates an owning shader pipeline.
		 *
		 * All shaders must actually be of the type they're passed as.
		 *
		 * @param[in] vertexShader The vertex shader that will be moved into the pipeline.
		 * @param[in] fragmentShader The fragment shader that will be moved into the pipeline.
		 **************************************************************************************************************/
		OwningShaderPipeline(Shader&& vertexShader, Shader&& fragmentShader) noexcept;

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
		 * @param[in] label The new label of the pipeline.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		Shader         _vertexShader;
		Shader         _fragmentShader;
		ShaderPipeline _pipeline;
	};

	/// @}
} // namespace tr