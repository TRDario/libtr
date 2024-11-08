/**
 * @file shader_pipeline.hpp
 * @brief Provides a shader program pipeline class.
 */

#pragma once
#include "shader.hpp"

#include <boost/optional.hpp>
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
         * @param tessEvalShader An optional tesselation evaluation shader.
         * @param tessControlShader An optional tesselation control shader.
         * @param geometryShader An optional geometry shader.
         * @param fragmentShader The fragment shader.
	     **************************************************************************************************************/
		ShaderPipeline(
			const Shader&                  vertexShader,
			boost::optional<const Shader&> tessEvalShader,
			boost::optional<const Shader&> tessControlShader,
			boost::optional<const Shader&> geometryShader,
			const Shader&                  fragmentShader
		) noexcept;

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

		void                             bind() const noexcept;

		friend class GLContext;
	};
} // namespace tr
