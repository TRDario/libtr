#include "../include/tr/shader_pipeline.hpp"
#include <GL/glew.h>

tr::ShaderPipeline::ShaderPipeline(const Shader& vertexShader, const Shader& fragmentShader) noexcept
{
	GLuint id;
	glCreateProgramPipelines(1, &id);
	_id.reset(id);

	assert(vertexShader.type() == ShaderType::VERTEX);
	glUseProgramStages(_id.get(), GL_VERTEX_SHADER_BIT, vertexShader._id.get());
	assert(fragmentShader.type() == ShaderType::FRAGMENT);
	glUseProgramStages(_id.get(), GL_FRAGMENT_SHADER_BIT, fragmentShader._id.get());
}

void tr::ShaderPipeline::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteProgramPipelines(1, &id);
}

void tr::ShaderPipeline::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_PROGRAM_PIPELINE, _id.get(), label.size(), label.data());
}

void tr::ShaderPipeline::bind() const noexcept
{
	glBindProgramPipeline(_id.get());
}
