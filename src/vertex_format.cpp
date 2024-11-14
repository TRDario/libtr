#include "../include/tr/vertex_format.hpp"

#include "../include/tr/overloaded_lambda.hpp"
#include <GL/glew.h>

tr::VertexFormat::VertexFormat(std::span<const VertexAttribute> attrs) noexcept
{
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	for (int i = 0; i < attrs.size(); ++i) {
		std::visit(overloaded{[=](const VertexAttributeF& attr) {
								  glVertexArrayAttribFormat(vao, i, attr.elements, GLenum(attr.type), attr.normalized,
															attr.offset);
							  },
							  [=](const VertexAttributeD& attr) {
								  glVertexArrayAttribLFormat(vao, i, attr.elements, GL_DOUBLE, attr.offset);
							  },
							  [=](const VertexAttributeI& attr) {
								  glVertexArrayAttribIFormat(vao, i, attr.elements, GLenum(attr.type), attr.offset);
							  }},
				   attrs[i]);
		glVertexArrayAttribBinding(vao, i, 0);
		glEnableVertexArrayAttrib(vao, i);
	}
	_id.reset(vao);
}

void tr::VertexFormat::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteVertexArrays(1, &id);
}

void tr::VertexFormat::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_VERTEX_ARRAY, _id.get(), label.size(), label.data());
}

void tr::VertexFormat::bind() const noexcept
{
	glBindVertexArray(_id.get());
}
