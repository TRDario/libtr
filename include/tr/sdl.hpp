/**
 * @file sdl.hpp
 * @brief Provides an SDL library RAII wrapper and related functionality.
 */

#pragma once
#include "handle.hpp"

#include <format>
#include <string_view>

namespace tr {
	/******************************************************************************************************************
	 * OpenGL attributes used by SDL.
	 ******************************************************************************************************************/
	struct GLAttrs {
		/**************************************************************************************************************
		 * Whether a debug context should be used.
		 **************************************************************************************************************/
		bool debugContext = false;

		/**************************************************************************************************************
		 * The number of bits to use for backbuffer depth.
		 **************************************************************************************************************/
		std::uint8_t depthBits = 0;

		/**************************************************************************************************************
		 * The number of bits to use for backbuffer stencil.
		 **************************************************************************************************************/
		std::uint8_t stencilBits = 0;

		/**************************************************************************************************************
		 * The number of samples used around a pixel for multisampled anti-aliasing.
		 **************************************************************************************************************/
		std::uint8_t multisamples = 0;
	};

	/******************************************************************************************************************
	 * Library version information.
	 *
	 * This struct is printable via std::format.
	 ******************************************************************************************************************/
	struct Version {
		/**************************************************************************************************************
		 * The major (first) version number.
		 **************************************************************************************************************/
		std::uint8_t major;

		/**************************************************************************************************************
		 * The minor (second) version number.
		 **************************************************************************************************************/
		std::uint8_t minor;

		/**************************************************************************************************************
		 * The patch (third) version number.
		 **************************************************************************************************************/
		std::uint8_t patch;
	};

	/******************************************************************************************************************
	 * Error thrown by an SDL function failing.
	 ******************************************************************************************************************/
	struct SDLError : std::runtime_error {
		/**************************************************************************************************************
		 * Constructs an SDL error.
		 *
		 * @exception std::bad_alloc If allocating the error string failed.
		 *
		 * @param message The error message.
		 **************************************************************************************************************/
		SDLError(std::string_view message);
	};

	/******************************************************************************************************************
	 * SDL library RAII wrapper.
	 ******************************************************************************************************************/
	class SDL {
	  public:
		/**************************************************************************************************************
		 * Initializes the SDL library.
		 *
		 * @exception SDLError If initializing the library failed.
		 *
		 * @param glAttrs Attributes used for the OpenGL context.
		 **************************************************************************************************************/
		SDL(const GLAttrs& glAttrs = {});

		/**************************************************************************************************************
		 * Gets the version of the SDL library.
		 *
		 * @return A structure containing SDL version info.
		 **************************************************************************************************************/
		Version version() const noexcept;

		/**************************************************************************************************************
		 * Gets the revision string of the SDL library.
		 *
		 * @return A C-string containing SDL revision info.
		 **************************************************************************************************************/
		const char* revision() const noexcept;

		/**************************************************************************************************************
		 * Gets the platform string.
		 *
		 * @return A C-string containing platform info.
		 **************************************************************************************************************/
		const char* platform() const noexcept;

		/**************************************************************************************************************
		 * Gets the amount of system RAM.
		 *
		 * @return The amount of system RAM in megabytes.
		 **************************************************************************************************************/
		int systemRAM() const noexcept;

		/**************************************************************************************************************
		 * Gets the number of system cores.
		 *
		 * @return The number of system cores.
		 **************************************************************************************************************/
		int systemCores() const noexcept;

	  private:
		struct Deleter {
			/// @private
			void operator()(bool) const noexcept;
		};

		Handle<bool, false, Deleter> _handle{true};
	};
} // namespace tr

/// @cond IMPLEMENTATION

namespace std {
	template <> struct formatter<tr::Version> : formatter<string> {
		auto format(tr::Version v, format_context& ctx) const
		{
			return formatter<string>::format(std::format("{:d}.{:d}.{:d}", v.major, v.minor, v.patch), ctx);
		}
	};
} // namespace std

/// @endcond
