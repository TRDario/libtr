#pragma once
#include "common.hpp"

namespace tr {
	/******************************************************************************************************************
	 * @ingroup system
	 *
	 * Base error type for all SDL operation errors.
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