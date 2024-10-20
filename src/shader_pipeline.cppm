/**
 * @file shader_pipeline.cppm
 * @brief Provides a shader program pipeline class.
 */

module;
#include <cassert>
#include <GL/glew.h>

export module tr:shader_pipeline;

import std;
import boost;
import :handle;
import :shader;

export namespace tr {
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
        ShaderPipeline(const Shader& vertexShader, boost::optional<const Shader&> tessEvalShader, boost::optional<const Shader&> tessControlShader, boost::optional<const Shader&> geometryShader, const Shader& fragmentShader) noexcept;


        /**************************************************************************************************************
	     * Sets the debug label of the pipeline.
         *
         * @param label The new label of the pipeline.
	     **************************************************************************************************************/
        void setLabel(std::string_view label) noexcept;
    private:
        struct Deleter { void operator()(GLuint id) const noexcept; /**< @private */ };
        Handle<GLuint, 0, Deleter> _id;

        void bind() const noexcept;

        friend class GLContext;
    };
}

// IMPLEMENTATION

tr::ShaderPipeline::ShaderPipeline(const Shader& vertexShader, boost::optional<const Shader&> tessEvalShader, boost::optional<const Shader&> tessControlShader, boost::optional<const Shader&> geometryShader, const Shader& fragmentShader) noexcept
{
    GLuint id;
    glCreateProgramPipelines(1, &id);
    _id.reset(id);

    assert(vertexShader.type() == Shader::Type::VERTEX);
    glUseProgramStages(_id.get(), GL_VERTEX_SHADER_BIT, vertexShader._id.get());

    if (tessEvalShader.has_value()) {
        assert(tessEvalShader->type() == Shader::Type::TESS_EVAL);
        glUseProgramStages(_id.get(), GL_TESS_EVALUATION_SHADER_BIT, tessEvalShader->_id.get());
    }

    if (tessControlShader.has_value()) {
        assert(tessControlShader->type() == Shader::Type::TESS_CONTROL);
        glUseProgramStages(_id.get(), GL_TESS_CONTROL_SHADER_BIT, tessControlShader->_id.get());
    }

    if (geometryShader.has_value()) {
        assert(geometryShader->type() == Shader::Type::GEOMETRY);
        glUseProgramStages(_id.get(), GL_GEOMETRY_SHADER_BIT, geometryShader->_id.get());
    }

    assert(fragmentShader.type() == Shader::Type::FRAGMENT);
    glUseProgramStages(_id.get(), GL_FRAGMENT_SHADER_BIT, fragmentShader._id.get());
}

void tr::ShaderPipeline::setLabel(std::string_view label) noexcept
{
    glObjectLabel(GL_PROGRAM_PIPELINE, _id.get(), label.size(), label.data());
}

void tr::ShaderPipeline::Deleter::operator()(GLuint id) const noexcept
{
    glDeleteProgramPipelines(1, &id);
}

void tr::ShaderPipeline::bind() const noexcept
{
    glBindProgramPipeline(_id.get());
}