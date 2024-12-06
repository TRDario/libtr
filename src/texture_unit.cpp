#include "../include/tr/texture_unit.hpp"
#include <GL/glew.h>
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
	for (int i = 0; i < _texUnitPool.size(); ++i) {
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

void tr::TextureUnit::setSampler(const Sampler& sampler) noexcept
{
	glBindSampler(_id.get(), sampler._id.get());
}

void tr::TextureUnit::setTexture(const Texture& texture) noexcept
{
	glBindTextures(_id.get(), 1, &texture._id.get());
}
