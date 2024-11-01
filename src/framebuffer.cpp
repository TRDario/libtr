#include "../include/framebuffer.hpp"
#include <GL/glew.h>
#include "bitmap_to_gl_format.hpp"

namespace tr {
    GLuint createFramebuffer() noexcept;
    GLuint findBoundWriteFramebuffer() noexcept;
    GLenum getGLAttachment(Framebuffer::Slot slot) noexcept;
}


GLuint tr::createFramebuffer() noexcept
{
    GLuint id;
    glCreateFramebuffers(1, &id);
    return id;
}

GLuint tr::findBoundWriteFramebuffer() noexcept
{
    GLint id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);
    return id;
}

GLenum tr::getGLAttachment(Framebuffer::Slot slot) noexcept
{
    switch (slot) {
    case Framebuffer::Slot::DEPTH:
        return GL_DEPTH_ATTACHMENT;
    case Framebuffer::Slot::STENCIL:
        return GL_STENCIL_ATTACHMENT;
    default:
        return GL_COLOR_ATTACHMENT0 + int(slot);
    }
}

tr::BasicFramebuffer::BasicFramebuffer(GLuint id, RectI2 viewport, DepthRange depthRange) noexcept
    : _id { id }
    , _viewport { viewport }
    , _depthRange { depthRange }
{}

bool tr::BasicFramebuffer::operator==(const BasicFramebuffer& r) const noexcept
{
    return _id == r._id;
}

tr::Bitmap tr::BasicFramebuffer::readRegion(RectI2 rect, BitmapFormat format) const
{
    auto [glFormat, glType] { bitmapToGLFormat(format) };
    Bitmap bitmap { rect.size, format };
    bindRead();
    glReadPixels(rect.tl.x, rect.tl.y, rect.size.x, rect.size.y, glFormat, glType, bitmap.data());
    return bitmap;
}

void tr::BasicFramebuffer::copyRegion(RectI2 rect, Texture2D& texture, glm::ivec2 textureTL) const noexcept
{
    bindRead();
    glCopyTextureSubImage2D(((Texture&)(texture))._id.get(), 0, textureTL.x, textureTL.y, rect.tl.x, rect.tl.y, rect.size.x, rect.size.y);
}

tr::RectI2 tr::BasicFramebuffer::viewport() const noexcept
{
    return _viewport;
}

void tr::BasicFramebuffer::setViewport(RectI2 viewport) noexcept
{
    _viewport = viewport;
    if (findBoundWriteFramebuffer() == _id) {
        glViewport(viewport.tl.x, viewport.tl.y, viewport.size.x, viewport.size.y);
    }
}

tr::DepthRange tr::BasicFramebuffer::depthRange() const noexcept
{
    return _depthRange;
}

void tr::BasicFramebuffer::setDepthRange(DepthRange range) noexcept
{
    _depthRange = range;
    if (findBoundWriteFramebuffer() == _id) {
        glDepthRange(range.min, range.max);
    }
}

void tr::BasicFramebuffer::bindRead() const noexcept
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
}

void tr::BasicFramebuffer::bindWrite() const noexcept
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
}

tr::Framebuffer::Framebuffer() noexcept
    : BasicFramebuffer { createFramebuffer(), { { 0, 0 }, { 0, 0 } }, { 0.0, 1.0 } }
{}

tr::Framebuffer::Framebuffer(Framebuffer&& r) noexcept
    : BasicFramebuffer { r }
    , _attachSizes { r._attachSizes }
    , _size { r._size }
{
    r._id = 0;
}

tr::Framebuffer& tr::Framebuffer::operator=(Framebuffer&& r) noexcept
{
    Framebuffer temp { std::move(*this) };
    std::swap(_id, r._id);
    std::swap(_attachSizes, r._attachSizes);
    std::swap(_size, r._size);
    return *this;
}

tr::Framebuffer::~Framebuffer() noexcept
{
    glDeleteFramebuffers(1, &_id);
}

glm::ivec2 tr::Framebuffer::size() const noexcept
{
    return _size;
}

void tr::Framebuffer::attach(Texture1D& tex, Slot slot) noexcept
{
    glNamedFramebufferTexture(_id, getGLAttachment(slot), ((Texture&)(tex))._id.get(), 0);
    _attachSizes[int(slot)] = { tex.size(), 1 };
    clampViewport();
}

void tr::Framebuffer::attach(ArrayTexture1D& tex, int layer, Slot slot) noexcept
{
    assert(layer >= 0 && layer < tex.layers());
    glNamedFramebufferTextureLayer(_id, getGLAttachment(slot), ((Texture&)(tex))._id.get(), 0, layer);
    _attachSizes[int(slot)] = { tex.size(), 1 };
    clampViewport();
}

void tr::Framebuffer::attach(Texture2D& tex, Slot slot) noexcept
{
    glNamedFramebufferTexture(_id, getGLAttachment(slot), ((Texture&)(tex))._id.get(), 0);
    _attachSizes[int(slot)] = tex.size();
    clampViewport();
}

void tr::Framebuffer::attach(ArrayTexture2D& tex, int layer, Slot slot) noexcept
{
    assert(layer >= 0 && layer < tex.layers());
    glNamedFramebufferTextureLayer(_id, getGLAttachment(slot), ((Texture&)(tex))._id.get(), 0, layer);
    _attachSizes[int(slot)] = tex.size();
    clampViewport();
}

void tr::Framebuffer::attach(Texture3D& tex, int z, Slot slot) noexcept
{
    assert(z >= 0 && z < tex.size().z);
    glNamedFramebufferTextureLayer(_id, getGLAttachment(slot), ((Texture&)(tex))._id.get(), 0, z);
    _attachSizes[int(slot)] = { tex.size().x, tex.size().y };
    clampViewport();
}

void tr::Framebuffer::attach(Renderbuffer& buffer, Slot slot) noexcept
{
    glNamedFramebufferRenderbuffer(_id, getGLAttachment(slot), GL_RENDERBUFFER, buffer._id.get());
    _attachSizes[int(slot)] = buffer.size();
    clampViewport();
}

void tr::Framebuffer::clear(Slot slot) noexcept
{
    glNamedFramebufferTexture(_id, getGLAttachment(slot), 0, 0);
    _attachSizes[int(slot)] = EMPTY_ATTACHMENT;
    clampViewport();
}

void tr::Framebuffer::setLabel(std::string_view label) noexcept
{
    glObjectLabel(GL_FRAMEBUFFER, _id, label.size(), label.data());
}

glm::ivec2 tr::Framebuffer::calcSize() noexcept
{
    auto attachments { std::views::filter(_attachSizes, [] (glm::ivec2 attachSize) { return attachSize != EMPTY_ATTACHMENT; }) };
    if (attachments.empty()) {
        _size = { 0, 0 };
    }
    else {
        _size = *attachments.begin();
        for (auto& attach : attachments) {
            _size = { std::min(_size.x, attach.x), std::min(_size.y, attach.y) };
        }
    }
    return _size;
}

void tr::Framebuffer::clampViewport() noexcept
{
    auto size { calcSize() };
    auto newViewportSize { _viewport.size };
    if (!RectI2 { size }.contains(_viewport.tl)) {
        setViewport(RectI2 { size });
    }
    else {
        if (_viewport.tl.x + _viewport.size.x >= size.x) {
            newViewportSize.x = size.x - _viewport.tl.x;
        }
        if (_viewport.tl.y + _viewport.size.y >= size.y) {
            newViewportSize.y = size.y - _viewport.tl.y;
        }
        if (_viewport.size != newViewportSize) {
            setViewport({ _viewport.tl, newViewportSize });
        }
    }
}

tr::Backbuffer::Backbuffer(const Window& window) noexcept
    : BasicFramebuffer { 0, { {}, window.size() }, { 0.0, 1.0 } }
    , _window { window }
{}

glm::ivec2 tr::Backbuffer::size() const noexcept
{
    return _window.size();
}