#include "../include/tr/sampler.hpp"
#include <GL/glew.h>

tr::Sampler::Sampler() noexcept
{
	GLuint id;
	glCreateSamplers(1, &id);
	_id.reset(id);
}

void tr::Sampler::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteSamplers(1, &id);
}

void tr::Sampler::setMinFilter(MinFilter filter) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_MIN_FILTER, GLint(filter));
}

void tr::Sampler::setMagFilter(MagFilter filter) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_MAG_FILTER, GLint(filter));
}

void tr::Sampler::setMinLOD(int lod) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_MIN_LOD, lod);
}

void tr::Sampler::setMaxLOD(int lod) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_MAX_LOD, lod);
}

void tr::Sampler::disableComparison() noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_COMPARE_MODE, GL_NONE);
}

void tr::Sampler::setComparisonMode(Compare op) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glSamplerParameteri(_id.get(), GL_TEXTURE_COMPARE_FUNC, GLint(op));
}

void tr::Sampler::setWrapS(Wrap wrap) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_WRAP_S, GLenum(wrap));
}

void tr::Sampler::setWrapT(Wrap wrap) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_WRAP_T, GLenum(wrap));
}

void tr::Sampler::setWrapR(Wrap wrap) noexcept
{
	glSamplerParameteri(_id.get(), GL_TEXTURE_WRAP_R, GLenum(wrap));
}

void tr::Sampler::setWrap(Wrap wrap) noexcept
{
	setWrapS(wrap);
	setWrapT(wrap);
	setWrapR(wrap);
}

void tr::Sampler::setBorderColor(RGBAF color) noexcept
{
	glSamplerParameterfv(_id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::Sampler::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_SAMPLER, _id.get(), label.size(), label.data());
}
