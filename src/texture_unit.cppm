/**
 * @file texture_unit.cppm
 * @brief Provides an OpenGL texture unit interface class.
 */

module;
#include <cassert>
#include <GL/glew.h>

export module tr:texture_unit;

import std;
import :handle;
import :sampler;
import :texture;

export namespace tr {
    /******************************************************************************************************************
	 * A texture unit onto which texture and samplers can be bound.
     *
     * OpenGL mandates that 80 <em>or more</em> texture units be provided, but libtr currently locks it at 80 for maximum
     * compatility with as many devices as possible.
     *
     * Texture units are not actual OpenGL objects and so cannot be labeled.
	 ******************************************************************************************************************/
    class TextureUnit {
    public:
        /**************************************************************************************************************
        * Constructs a new texture unit.
        *
        * This function may trigger a failed assertion if the limit of 80 simultaneous texture units is surpassed.
        **************************************************************************************************************/
        TextureUnit() noexcept;


        /**************************************************************************************************************
        * Binds a sampler to the texture unit.
        *
        * A sampler can be bound to multiple texture units simultaneously.
        *
        * @param[in] sampler A sampler object.
        **************************************************************************************************************/
        void setSampler(const Sampler& sampler) noexcept;

        /**************************************************************************************************************
        * Binds a texture to the texture unit.
        *
        * A texture can be bound to multiple texture units simultaneously.
        *
        * @param[in] texture Any type of texture object.
        **************************************************************************************************************/
        void setTexture(const Texture& texture) noexcept;
    private:
        struct Deleter { void operator()(unsigned int id) noexcept; /**< @private */ };
        Handle<unsigned int, -1U, Deleter> _id;

        friend class Shader;
    };
}

// IMPLEMENTATION

namespace tr {
    /// @private
    // Defined by the OpenGL standard.
    inline constexpr int MAX_TEX_UNITS { 80 }; 

    /// @private
    // Bitset where used up units are marked as bit flags.
    std::bitset<MAX_TEX_UNITS> _texUnitPool;   
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