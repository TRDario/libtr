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

tr::OwningShaderPipeline::OwningShaderPipeline(Shader vertexShader, Shader fragmentShader) noexcept
	: _vertexShader{std::move(vertexShader)}
	, _fragmentShader{std::move(fragmentShader)}
	, _pipeline{_vertexShader, _fragmentShader}
{
}

tr::OwningShaderPipeline::operator const ShaderPipeline&() const noexcept
{
	return _pipeline;
}

tr::Shader& tr::OwningShaderPipeline::vertexShader() noexcept
{
	return _vertexShader;
}

const tr::Shader& tr::OwningShaderPipeline::vertexShader() const noexcept
{
	return _vertexShader;
}

tr::Shader& tr::OwningShaderPipeline::fragmentShader() noexcept
{
	return _fragmentShader;
}

const tr::Shader& tr::OwningShaderPipeline::fragmentShader() const noexcept
{
	return _fragmentShader;
}

void tr::OwningShaderPipeline::setLabel(std::string_view label) noexcept
{
	_pipeline.setLabel(label);
}