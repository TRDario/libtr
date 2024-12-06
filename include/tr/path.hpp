#pragma once
#include <exception>
#include <filesystem>

namespace tr {
	/** @defgroup path Paths
	 *  Path getters.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error in getting the executable directory path.
	 ******************************************************************************************************************/
	struct ExeDirInitError : std::exception {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Error in getting the executable directory path.
	 ******************************************************************************************************************/
	struct UserDirInitError : std::exception {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Gets a path to the executable directory.
	 *
	 * This function should only be called once at the beginning of the program as it may be relatively expensive.
	 *
	 * @exception ExeDirInitError If getting the path fails.
	 * @exception std::bad_alloc If allocating the path fails.
	 *
	 * @return A fath to the executable directory.
	 ******************************************************************************************************************/
	std::filesystem::path getExeDir();

	/******************************************************************************************************************
	 * Gets a path to a safe user directory where data can be stored.
	 *
	 * This function should only be called once at the beginning of the program as it may be relatively expensive.
	 *
	 * @exception UserDirInitError If getting the path fails.
	 * @exception std::bad_alloc If allocating the path fails.
	 *
	 * @param[in] org The organization name, used so that all applications under this organization share a folder.
	 * @param[in] app The application name, used to that a specific subdirectory be made for the application.
	 *
	 * @return A path to the executable directory.
	 ******************************************************************************************************************/
	std::filesystem::path getUserDir(const char* org, const char* app);

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::ExeDirInitError::what() const noexcept
{
	return "Failed to initialize the executable directory base path.";
}

constexpr const char* tr::UserDirInitError::what() const noexcept
{
	return "Failed to initialize the user directory base path.";
}

/// @endcond
