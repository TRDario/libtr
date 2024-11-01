#include "../include/vertex_format.hpp"
#include <GL/glew.h>


tr::VertexFormat::VertexFormat(std::span<const VertexAttribute> attrs) noexcept
{
    struct AttribFormatter {
        GLuint vao;
        int    i;

        void operator()(const VertexAttributeF& attr) {
            glVertexArrayAttribFormat(vao, i, attr.elements, GLenum(attr.type), attr.normalized, attr.offset);
        }
        void operator()(const VertexAttributeD& attr) {
            glVertexArrayAttribLFormat(vao, i, attr.elements, GL_DOUBLE, attr.offset);
        }
        void operator()(const VertexAttributeI& attr) {
            glVertexArrayAttribIFormat(vao, i, attr.elements, GLenum(attr.type), attr.offset);
        }
    };

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    for (int i = 0; i < attrs.size(); ++i) {
        std::visit(AttribFormatter { vao, i }, attrs[i]);
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