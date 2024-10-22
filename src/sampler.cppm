/**
 * @file sampler.cppm
 * @brief Provides a texture sampler type.
 */

module;
#include <cassert>
#include <GL/glew.h>

export module tr:sampler;

import std;
import :color;
import :handle;

export namespace tr {
	/******************************************************************************************************************
	 * Texture wrapping types.
	 ******************************************************************************************************************/
	enum class Wrap {
		/**************************************************************************************************************
		 * The texture is repeated.
		 **************************************************************************************************************/
		REPEAT = GL_REPEAT,

		/**************************************************************************************************************
		 * The texture is repeated and mirrored.
		 **************************************************************************************************************/
		MIRROR_REPEAT = GL_MIRRORED_REPEAT,

		/**************************************************************************************************************
		 * The value of the edge pixel is used.
		 **************************************************************************************************************/
		EDGE_CLAMP = GL_CLAMP_TO_EDGE,

		/**************************************************************************************************************
		 * The value of the border color is used.
		 **************************************************************************************************************/
		BORDER_CLAMP = GL_CLAMP_TO_BORDER
	};
	
	/******************************************************************************************************************
	 * Minifying filter types.
	 ******************************************************************************************************************/
	enum class MinFilter {
		/**************************************************************************************************************
		 * The value of the texture element that is nearest to the specified texture coordinates is used. 
		 **************************************************************************************************************/
		NEAREST = GL_NEAREST,

		/**************************************************************************************************************
		 * The average of the four texture elements that are closest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		LINEAR = GL_LINEAR,

		/**************************************************************************************************************
		 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		 **************************************************************************************************************/
		NMIP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,

		/**************************************************************************************************************
		 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		 **************************************************************************************************************/
		NMIP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,

		/**************************************************************************************************************
		 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		 **************************************************************************************************************/
		LMIPS_NEAREST = GL_LINEAR_MIPMAP_NEAREST,

		/**************************************************************************************************************
		 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
		 **************************************************************************************************************/
		LMIPS_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	/******************************************************************************************************************
	 * Magnifying filter types.
	 ******************************************************************************************************************/
	enum class MagFilter {
		/**************************************************************************************************************
		 * The value of the texture element that is nearest to the specified texture coordinates is used. 
		 **************************************************************************************************************/
		NEAREST = GL_NEAREST,

		/**************************************************************************************************************
		 * The average of the four texture elements that are closest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		LINEAR = GL_LINEAR
	};
	
	/******************************************************************************************************************
	 * Depth comparison functions.
	 ******************************************************************************************************************/
	enum class Compare {
		/**************************************************************************************************************
		 * The function always returns false.
		 **************************************************************************************************************/
		NEVER = GL_NEVER,

		/**************************************************************************************************************
		 * The function returns true if the value is less than the value being compared to.
		 **************************************************************************************************************/
		LESS = GL_LESS,

		/**************************************************************************************************************
		 * The function returns true if the value is equal to the value being compared to.
		 **************************************************************************************************************/
		EQUAL = GL_EQUAL,

		/**************************************************************************************************************
		 * The function returns true if the value is less than or equal to the value being compared to.
		 **************************************************************************************************************/
		LEQUAL = GL_LEQUAL,

		/**************************************************************************************************************
		 * The function returns true if the value is greater than the value being compared to.
		 **************************************************************************************************************/
		GREATER = GL_GREATER,

		/**************************************************************************************************************
		 * The function returns true if the value is not equal to the value being compared to.
		 **************************************************************************************************************/
		NEQUAL = GL_NOTEQUAL,

		/**************************************************************************************************************
		 * The function returns true if the value is greater than or equal to the value being compared to.
		 **************************************************************************************************************/
		GEQUAL = GL_GEQUAL,
		
		/**************************************************************************************************************
		 * The function always returns true.
		 **************************************************************************************************************/
		ALWAYS = GL_ALWAYS
	};

	/******************************************************************************************************************
	 * GPU Texture sampler.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	class Sampler {
	public:
		/**************************************************************************************************************
		 * Creates a sampler.
		 **************************************************************************************************************/
		Sampler() noexcept;


		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		friend bool operator==(const Sampler&, const Sampler&) noexcept = default;


		/**************************************************************************************************************
		 * Sets the minifying filter used by the sampler.
		 *
		 * @param filter The new minifying filter type.
		 **************************************************************************************************************/
		void setMinFilter(MinFilter filter) noexcept;
		
		/**************************************************************************************************************
		 * Sets the magnifying filter used by the sampler.
		 *
		 * @param filter The new magnifying filter type.
		 **************************************************************************************************************/
		void setMagFilter(MagFilter filter) noexcept;


		/**************************************************************************************************************
		 * Sets the minimum allowed level-of-detail parameter used by the sampler.
		 *
		 * @param lod The new minimum LOD.
		 **************************************************************************************************************/
		void setMinLOD(int lod) noexcept;
		
		/**************************************************************************************************************
		 * Sets the maximum allowed level-of-detail parameter used by the sampler.
		 *
		 * @param lod The new maximum LOD.
		 **************************************************************************************************************/
		void setMaxLOD(int lod) noexcept;


		/**************************************************************************************************************
		 * Disables the use of depth comparison.
		 **************************************************************************************************************/
        void disableComparison() noexcept;

		/**************************************************************************************************************
		 * Enables the use of depth comparison and sets the depth comparison operator of the sampler.
		 *
		 * @param op The function to use for comparison.
		 **************************************************************************************************************/
		void setComparisonMode(Compare op) noexcept;


		/**************************************************************************************************************
        * Sets the wrapping used for the s/x coordinate.
		*
		* @param wrap The new wrapping type.
        **************************************************************************************************************/
		void setWrapS(Wrap wrap) noexcept;
		
		/**************************************************************************************************************
        * Sets the wrapping used for the t/y coordinate.
		*
		* @param wrap The new wrapping type.
        **************************************************************************************************************/
		void setWrapT(Wrap wrap) noexcept;

		/**************************************************************************************************************
        * Sets the wrapping used for the r/z coordinate.
		*
		* @param wrap The new wrapping type.
        **************************************************************************************************************/
		void setWrapR(Wrap wrap) noexcept;
		
		/**************************************************************************************************************
        * Sets the wrapping used for all coordinates.
		*
		* @param wrap The new wrapping type.
        **************************************************************************************************************/
		void setWrap(Wrap wrap) noexcept;


		/**************************************************************************************************************
        * Sets the border color of the sampler (used when Wrap::BORDER_CLAMP is in use).
		*
		* @param color The border color in floating point RGBA format.
        **************************************************************************************************************/
		void setBorderColor(RGBAF color) noexcept;


		/**************************************************************************************************************
        * Sets the debug label of the sampler.
        *
        * @param label The new label of the sampler.
        **************************************************************************************************************/
        void setLabel(std::string_view label) noexcept;
	private:
		Handle<GLuint, 0, decltype([] (GLuint id) { glDeleteSamplers(1, &id); })> _id;

		friend class TextureUnit;
	};
}

/// @cond IMPLEMENTATION

tr::Sampler::Sampler() noexcept
{
    GLuint id;
    glCreateSamplers(1, &id);
    _id.reset(id);
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

/// @endcond