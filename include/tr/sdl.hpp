#pragma once
#include <stdexcept>

namespace tr {
	/******************************************************************************************************************
	 * Error thrown by an internal SDL function failing.
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
} // namespace tr