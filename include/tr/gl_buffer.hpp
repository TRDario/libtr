#pragma once
#include "handle.hpp"
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace tr {
	class GLBufferMap;

	/** @addtogroup graphics
	 *  @{
	 */

	/******************************************************************************************************************
	 * Concept denoting an iterator suitable for GLBuffer::copyRegionTo.
	 ******************************************************************************************************************/
	template <class It>
	concept GLCopyOutputIterator =
		std::contiguous_iterator<It> && (std::same_as<typename std::iterator_traits<It>::value_type, std::byte> ||
										 std::same_as<typename std::iterator_traits<It>::value_type, char> ||
										 std::same_as<typename std::iterator_traits<It>::value_type, std::uint8_t>);

	/******************************************************************************************************************
	 * Error thrown when allocating a GPU buffer fails.
	 ******************************************************************************************************************/
	struct GLBufferBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Error thrown when mapping a GPU buffer fails.
	 ******************************************************************************************************************/
	struct GLMapBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Base OpenGL buffer class.
	 *
	 * This class cannot be constructed directly.
	 ******************************************************************************************************************/
	class GLBuffer {
	  public:
		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		friend bool operator==(const GLBuffer&, const GLBuffer&) noexcept;

	  protected:
		/// @cond IMPLEMENTATION
		// Enum representing an OpenGL buffer target.
		enum class Target : std::uint32_t {
			ARRAY_BUFFER              = 0x88'92,
			ATOMIC_COUNTER_BUFFER     = 0x92'C0,
			COPY_READ_BUFFER          = 0x8F'36,
			COPY_WRITE_BUFFER         = 0x8F'37,
			DISPATCH_INDIRECT_BUFFER  = 0x90'EE,
			DRAW_INDIRECT_BUFFER      = 0x8F'3F,
			ELEMENT_ARRAY_BUFFER      = 0x88'93,
			PIXEL_PACK_BUFFER         = 0x88'EB,
			PIXEL_UNPACK_BUFFER       = 0x88'EC,
			QUERY_BUFFER              = 0x91'92,
			SHADER_STORAGE_BUFFER     = 0x90'D2,
			TEXTURE_BUFFER            = 0x8C'2A,
			TRANSFORM_FEEDBACK_BUFFER = 0x8C'8E,
			UNIFORM_BUFFER            = 0x8A'11
		};
		// Enum representing buffer creation flags.
		enum class Flag : std::uint32_t {
			NONE            = 0x0,
			READABLE        = 0x1,
			WRITABLE        = 0x2,
			PERSISTENT      = 0x40,
			COHERENT        = 0x80,
			DYNAMIC_STORAGE = 0x1'00,
			CPU_STORAGE     = 0x2'00
		};
		// Enum representing a buffer format.
		enum class Format {
			R8        = 0x82'29,
			R16       = 0x82'2A,
			R_FP16    = 0x82'2D,
			R_FP32    = 0x82'2E,
			R_SI8     = 0x82'31,
			R_UI8     = 0x82'32,
			R_SI16    = 0x82'33,
			R_UI16    = 0x82'34,
			R_SI32    = 0x82'35,
			R_UI32    = 0x82'36,
			RG8       = 0x82'2B,
			RG16      = 0x82'2C,
			RG_FP16   = 0x82'2F,
			RG_FP32   = 0x82'30,
			RG_SI8    = 0x82'37,
			RG_UI8    = 0x82'38,
			RG_SI16   = 0x82'39,
			RG_UI16   = 0x82'3A,
			RG_SI32   = 0x82'3B,
			RG_UI32   = 0x82'3C,
			RGB_FP32  = 0x88'15,
			RGB_SI32  = 0x8D'83,
			RGB_UI32  = 0x8D'71,
			RGBA8     = 0x80'58,
			RGBA16    = 0x80'5B,
			RGBA_FP16 = 0x88'1A,
			RGBA_FP32 = 0x88'14,
			RGBA_SI8  = 0x8D'8E,
			RGBA_UI8  = 0x8D'7C,
			RGBA_SI16 = 0x8D'88,
			RGBA_UI16 = 0x8D'76,
			RGBA_SI32 = 0x8D'82,
			RGBA_UI32 = 0x8D'70,
		};
		// Enum representing a mapping flag.
		enum class MapFlag {
			NONE              = 0x0,
			READABLE          = 0x1,
			WRITABLE          = 0x2,
			INVALIDATE_RANGE  = 0x4,
			INVALIDATE_BUFFER = 0x8,
			EXPLICIT_FLUSH    = 0x10,
			UNSYNCHRONIZED    = 0x20,
			PERSISTENT        = 0x40,
			COHERENT          = 0x80
		};

		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
		Target                           _target;
		std::size_t                      _size;

		// Creates an uninitialized buffer.
		GLBuffer(Target target, std::size_t size, Flag flags);
		// Creates an initialized buffer.
		GLBuffer(Target target, std::span<const std::byte> data, Flag flags);

		std::size_t size() const noexcept;

		template <GLCopyOutputIterator It>
		void copyRegionTo(It out, std::size_t offset, std::size_t size) const noexcept
		{
			copyRegionBase(std::to_address(out), offset, size);
		}

		void setRegion(std::size_t offset, std::span<const std::byte> data) noexcept;

		bool        mapped() const noexcept;
		GLBufferMap mapRegion(std::size_t offset, std::size_t size, MapFlag flags);

		// Resets the buffer's target binding point.
		void resetTarget(Target newTarget) noexcept;
		// Binds the buffer to a target (or std::nullopt to bind to the default target).
		void bind(std::optional<Target> target = std::nullopt) const noexcept;
		// Binds the buffer to an indexed target (or std::nullopt to bind to the default target).
		void bindIndexed(std::optional<Target> target, std::uint32_t index) const noexcept;
		// Binds a range of the buffer to an indexed target (or std::nullopt to bind to the default target).
		void bindIndexedRange(std::optional<Target> target, std::uint32_t index, std::size_t offset,
							  std::size_t size) const noexcept;

		void setLabel(std::string_view label) noexcept;
		/// @endcond
	  private:
		void copyRegionToBase(void* ptr, std::size_t offset, std::size_t size) const noexcept;

		friend class Shader;
		friend class GLContext;
		friend class VertexBuffer;
		friend class IndexBuffer;
	};

	/******************************************************************************************************************
	 * RAII wrapper over an OpenGL buffer map.
	 *
	 * The buffer is automatically unmapped once the map goes out of scope.
	 ******************************************************************************************************************/
	class GLBufferMap {
	  public:
		/**************************************************************************************************************
		 * Casts the map into a regular byte span.
		 **************************************************************************************************************/
		operator std::span<std::byte>() const noexcept;

		/**************************************************************************************************************
		 * Casts the map into a regular byte span.
		 *
		 * @return A span of bytes. Despite not being const-qualified, they may be read-only dpeending on buffer access.
		 **************************************************************************************************************/
		std::span<std::byte> span() const noexcept;

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _buffer;
		std::span<std::byte>             _span;

		GLBufferMap(unsigned int buffer, std::span<std::byte> span) noexcept;

		friend class GLBuffer;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::GLBufferBadAlloc::what() const noexcept
{
	return "OpenGL buffer allocation error";
}

constexpr const char* tr::GLMapBadAlloc::what() const noexcept
{
	return "OpenGL buffer map allocation error";
}

/// @endcond
