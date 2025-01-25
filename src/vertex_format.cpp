#include "../include/tr/overloaded_lambda.hpp"
#include "../include/tr/vertex_format.hpp"
#include "gl_call.hpp"

tr::VertexFormat::VertexFormat(std::span<const VertexAttribute> attrs) noexcept
{
	GLuint vao;
	TR_GL_CALL(glCreateVertexArrays, 1, &vao);
	for (std::size_t i = 0; i < attrs.size(); ++i) {
		std::visit(Overloaded{[=](const VertexAttributeF& attr) {
								  TR_GL_CALL(glVertexArrayAttribFormat, vao, i, attr.elements,
											 static_cast<GLenum>(attr.type), attr.normalized, attr.offset);
							  },
							  [=](const VertexAttributeD& attr) {
								  TR_GL_CALL(glVertexArrayAttribLFormat, vao, i, attr.elements, GL_DOUBLE, attr.offset);
							  },
							  [=](const VertexAttributeI& attr) {
								  TR_GL_CALL(glVertexArrayAttribIFormat, vao, i, attr.elements,
											 static_cast<GLenum>(attr.type), attr.offset);
							  }},
				   attrs[i]);
		TR_GL_CALL(glVertexArrayAttribBinding, vao, i, 0);
		TR_GL_CALL(glEnableVertexArrayAttrib, vao, i);
	}
	_id.reset(vao);
}

void tr::VertexFormat::Deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteVertexArrays, 1, &id);
}

void tr::VertexFormat::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_VERTEX_ARRAY, _id.get(), label.size(), label.data());
}

void tr::VertexFormat::bind() const noexcept
{
	TR_GL_CALL(glBindVertexArray, _id.get());
}
