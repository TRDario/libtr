module;
#include <cassert>
#include <GL/glew.h>

export module tr:texture_unit;

import std;
import :handle;
import :sampler;
import :texture;

export namespace tr {
    // A unit onto which samplers and textures are bound.
    class TextureUnit {
    public:
        TextureUnit() noexcept;

        void setSampler(const Sampler& sampler) noexcept;
        void setTexture(const Texture& texture) noexcept;
    private:
        struct Deleter { void operator()(unsigned int id) noexcept; };
        Handle<unsigned int, -1U, Deleter> _id;

        friend class Shader;
    };
}

// IMPLEMENTATION

namespace tr {
    inline constexpr int MAX_TEX_UNITS { 80 }; // Defined by the OpenGL standard.
    std::bitset<MAX_TEX_UNITS> _texUnitPool;   // Bitset where used up units are marked as bit flags.
}

tr::TextureUnit::TextureUnit() noexcept
{
    assert(!_texUnitPool.all());
    for (int i = 0; i < _texUnitPool.size(); ++i) {
        if (!_texUnitPool[i]) {
            _texUnitPool[i] = true;
            _id.reset(i);
            return;
        }
    }
}

void tr::TextureUnit::Deleter::operator()(unsigned int id) noexcept
{
    _texUnitPool[id] = false;
}

void tr::TextureUnit::setSampler(const Sampler& sampler) noexcept
{
    glBindSampler(_id.get(), sampler._id.get());
}

void tr::TextureUnit::setTexture(const Texture& texture) noexcept
{
    glBindTextures(_id.get(), 1, &texture._id.get());
}