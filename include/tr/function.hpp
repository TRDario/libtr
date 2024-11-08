/**
 * @file function.hpp
 * @brief Provides some functor functionality.
 */

#pragma once
#include <utility>

namespace tr {
	/******************************************************************************************************************
     * Wrapper around a function that turns it into a stateless functor.
	 *
	 * @tparam FunctionPointer a function pointer.
     ******************************************************************************************************************/
	template <auto FunctionPointer>
	struct FunctionCaller {
		/**************************************************************************************************************
    	 * Calls the function.
		 *
		 * @param us The function parameters to forward.
    	 **************************************************************************************************************/
		template <class... Ts>
		constexpr auto operator()(Ts&&... us) const noexcept(noexcept(FunctionPointer(std::forward<Ts...>(us...))));
	};
} // namespace tr

/// @cond IMPLEMENTATION

template <auto FunctionPointer>
template <class... Ts>
constexpr auto tr::FunctionCaller<FunctionPointer>::operator()(Ts&&... us) const
	noexcept(noexcept(FunctionPointer(std::forward<Ts...>(us...))))
{
	return FunctionPointer(std::forward<Ts...>(us...));
}

/// @endcond
