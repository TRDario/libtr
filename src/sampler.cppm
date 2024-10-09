module;
#include <cassert>
#include <GL/glew.h>

export module tr:sampler;

import std;
import :color;
import :handle;
import :integer;

export namespace tr {
	// Enum representing a texture wrapping type.
	enum class Wrap {
		REPEAT        = GL_REPEAT,          // The texture is repeated.
		MIRROR_REPEAT = GL_MIRRORED_REPEAT, // The texture is repeated and mirrored.
		EDGE_CLAMP    = GL_CLAMP_TO_EDGE,   // The value of the edge pixel is used.
		BORDER_CLAMP  = GL_CLAMP_TO_BORDER  // The value of the border color is used.
	};
	// Enum representing a minifying filter type.
	enum class MinFilter {
		NEAREST       = GL_NEAREST,                // The value of the texture element that is nearest to the specified texture coordinates is used. 
		LINEAR        = GL_LINEAR,                 // The average of the four texture elements that are closest to the specified texture coordinates is used. 
		NMIP_NEAREST  = GL_NEAREST_MIPMAP_NEAREST, // Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		NMIP_LINEAR   = GL_NEAREST_MIPMAP_LINEAR,  // Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		LMIPS_NEAREST = GL_LINEAR_MIPMAP_NEAREST,  // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		LMIPS_LINEAR  = GL_LINEAR_MIPMAP_LINEAR    // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
	};
	// Enum representing a magnifying filter type.
	enum class MagFilter {
		NEAREST = GL_NEAREST, // The value of the texture element that is nearest to the specified texture coordinates is used. 
		LINEAR  = GL_LINEAR   // The average of the four texture elements that are closest to the specified texture coordinates is used. 
	};
	// Enum representing a comparison function.
	enum class Compare {
		NEVER   = GL_NEVER,    // The function always returns false.
		LESS    = GL_LESS,     // The function returns true if the value is less than the value being compared to.
		EQUAL   = GL_EQUAL,    // The function returns true if the value is equal to the value being compared to.
		LEQUAL  = GL_LEQUAL,   // The function returns true if the value is less than or equal to the value being compared to.
		GREATER = GL_GREATER,  // The function returns true if the value is greater than the value being compared to.
		NEQUAL  = GL_NOTEQUAL, // The function returns true if the value is not equal to the value being compared to.
		GEQUAL  = GL_GEQUAL,   // The function returns true if the value is greater than or equal to the value being compared to.
		ALWAYS  = GL_ALWAYS    // The function always returns true.
	};

	// Texture sampler class.
	class Sampler {
	public:
		Sampler() noexcept;

		bool operator==(const Sampler&) const noexcept = default;

		// Gets the minifying filter used by the sampler.
		MinFilter minFilter() const noexcept;
		// Gets the magnifying filter used by the sampler.
		MagFilter magFilter() const noexcept;
		// Sets the minifying filter used by the sampler.
		void setMinFilter(MinFilter filter) noexcept;
		// Sets the magnifying filter used by the sampler.
		void setMagFilter(MagFilter filter) noexcept;

		// Gets the minimum allowed level-of-detail parameter used by the sampler.
		int minLOD() const noexcept;
		// Gets the maximum allowed level-of-detail parameter used by the sampler.
		int maxLOD() const noexcept;
		// Sets the minimum allowed level-of-detail parameter used by the sampler.
		void setMinLOD(int lod) noexcept;
		// Sets the maximum allowed level-of-detail parameter used by the sampler.
		void setMaxLOD(int lod) noexcept;

		// Gets the comparison mode used by the sampler (if one is used at all).
		std::optional<Compare> comparisonMode() const noexcept;
        // Disables the use of depth comparison.
        void disableComparison() noexcept;
		// Sets the depth comparison operator of the sampler.
		// op - The operator to use.
		void setComparisonMode(Compare op) noexcept;

		// Gets the type of wrapping used for the s coordinate.
		Wrap wrapS() const noexcept;
		// Gets the type of wrapping used for the t coordinate.
		Wrap wrapT() const noexcept;
		// Gets the type of wrapping used for the r coordinate.
		Wrap wrapR() const noexcept;
		// Sets the type of wrapping used for the s coordinate.
		void setWrapS(Wrap wrap) noexcept;
		// Sets the type of wrapping used for the t coordinate.
		void setWrapT(Wrap wrap) noexcept;
		// Sets the type of wrapping used for the r coordinate.
		void setWrapR(Wrap wrap) noexcept;
		// Sets the type of wrapping used for every coordinate.
		void setWrap(Wrap wrap) noexcept;

		RGBAF borderColor() const noexcept;
		void setBorderColor(RGBAF color) noexcept;

        void setLabel(std::string_view label) noexcept;
	private:
		struct Deleter { void operator()(unsigned int id) noexcept; };
		Handle<unsigned int, 0, Deleter> _id;

		friend class TextureUnit;
	};
}

// IMPLEMENTATION

tr::Sampler::Sampler() noexcept
{
    unsigned int id;
    glCreateSamplers(1, &id);
    _id.reset(id);
}

void tr::Sampler::Deleter::operator()(unsigned int id) noexcept
{
    glDeleteSamplers(1, &id);
}

tr::MinFilter tr::Sampler::minFilter() const noexcept
{
    int glFilter;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_MIN_FILTER, &glFilter);
    return MinFilter(glFilter);
}

tr::MagFilter tr::Sampler::magFilter() const noexcept
{
    int glFilter;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_MAG_FILTER, &glFilter);
    return MagFilter(glFilter);
}

void tr::Sampler::setMinFilter(MinFilter filter) noexcept
{
    glSamplerParameteri(_id.get(), GL_TEXTURE_MIN_FILTER, int(filter));
}

void tr::Sampler::setMagFilter(MagFilter filter) noexcept
{
    glSamplerParameteri(_id.get(), GL_TEXTURE_MAG_FILTER, int(filter));
}

int tr::Sampler::minLOD() const noexcept
{
    int lod;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_MIN_LOD, &lod);
    return lod;
}

int tr::Sampler::maxLOD() const noexcept
{
    int lod;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_MAX_LOD, &lod);
    return lod;
}

void tr::Sampler::setMinLOD(int lod) noexcept
{
    glSamplerParameteri(_id.get(), GL_TEXTURE_MIN_LOD, lod);
}

void tr::Sampler::setMaxLOD(int lod) noexcept
{
    glSamplerParameteri(_id.get(), GL_TEXTURE_MAX_LOD, lod);
}

std::optional<tr::Compare> tr::Sampler::comparisonMode() const noexcept
{
    int compareMode;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_COMPARE_MODE, &compareMode);
    if (compareMode == GL_NONE) {
        return std::nullopt;
    }
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_COMPARE_FUNC, &compareMode);
    return Compare(compareMode);
}

void tr::Sampler::disableComparison() noexcept
{
    glSamplerParameteri(_id.get(), GL_TEXTURE_COMPARE_MODE, GL_NONE);
}

void tr::Sampler::setComparisonMode(Compare op) noexcept
{
    glSamplerParameteri(_id.get(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glSamplerParameteri(_id.get(), GL_TEXTURE_COMPARE_FUNC, int(op));
}

tr::Wrap tr::Sampler::wrapS() const noexcept
{
    int glWrap;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_WRAP_S, &glWrap);
    return Wrap(glWrap);
}

tr::Wrap tr::Sampler::wrapT() const noexcept
{
    int glWrap;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_WRAP_T, &glWrap);
    return Wrap(glWrap);
}

tr::Wrap tr::Sampler::wrapR() const noexcept
{
    int glWrap;
    glGetSamplerParameteriv(_id.get(), GL_TEXTURE_WRAP_R, &glWrap);
    return Wrap(glWrap);
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

tr::RGBAF tr::Sampler::borderColor() const noexcept
{
    RGBAF color;
    glGetSamplerParameterfv(_id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
    return color;
}

void tr::Sampler::setBorderColor(RGBAF color) noexcept
{
    glSamplerParameterfv(_id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::Sampler::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_SAMPLER, _id.get(), label.size(), label.data());
}