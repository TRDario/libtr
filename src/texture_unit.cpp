#include "../include/tr/texture_unit.hpp"
#include "gl_call.hpp"
#include <bitset>

namespace tr {
	// Defined by the OpenGL standard.
	inline constexpr int MAX_TEX_UNITS{80};
	// Bitset where used up units are marked as bit flags.
	std::bitset<MAX_TEX_UNITS> _texUnitPool;
} // namespace tr

tr::TextureUnit::TextureUnit() noexcept
{
	assert(!_texUnitPool.all());
	for (std::size_t i = 0; i < _texUnitPool.size(); ++i) {
		if (!_texUnitPool[i]) {
			_texUnitPool[i] = true;
			_id.reset(i);
			return;
		}
	}
}

void tr::TextureUnit::Deleter::operator()(GLuint id) noexcept
{
	_texUnitPool[id] = false;
}

void tr::TextureUnit::setTexture(const Texture& texture) noexcept
{
	TR_GL_CALL(glBindTextures, _id.get(), 1, &texture._id.get());
}
