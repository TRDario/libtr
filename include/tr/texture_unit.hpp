#pragma once
#include "handle.hpp"

namespace tr {
	class Texture;

	/** @ingroup graphics
	 *  @defgroup texture_unit Texture Unit
	 *  Texture unit class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * A texture unit onto which textures can be bound.
	 *
	 * @remark OpenGL mandates that 80 <em>or more</em> texture units be provided, but libtr currently locks it at 80
	 *         for maximum compatility with as many devices as possible.
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
