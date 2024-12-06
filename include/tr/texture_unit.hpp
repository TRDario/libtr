#pragma once
#include "sampler.hpp"
#include "texture.hpp"

namespace tr {
	/** @addtogroup graphics
	 *  @{
	 */

	/******************************************************************************************************************
	 * A texture unit onto which texture and samplers can be bound.
	 *
	 * OpenGL mandates that 80 <em>or more</em> texture units be provided, but libtr currently locks it at 80 for
	 * maximum compatility with as many devices as possible.
	 *
	 * Texture units are not actual OpenGL objects and so cannot be labeled.
	 ******************************************************************************************************************/
	class TextureUnit {
	  public:
		/**************************************************************************************************************
		 * Constructs a new texture unit.
		 *
		 * No more than 80 texture units can exist simultaneously.
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
		struct Deleter {
			void operator()(unsigned int id) noexcept;
		};

		Handle<unsigned int, -1U, Deleter> _id;

		friend class Shader;
	};

	/// @}
} // namespace tr
