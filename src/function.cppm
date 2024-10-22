/**
 * @file function.cppm
 * @brief Provides OpenGL framebuffer classes.
 */

export module tr:function;

import std;

export namespace tr {
    /******************************************************************************************************************
     * Wrapper around a function that turns it into a stateless functor.
	 *
	 * @tparam FunctionPointer a function pointer.
     ******************************************************************************************************************/
	template<auto FunctionPointer>
	struct FunctionCaller
	{
		/**************************************************************************************************************
    	 * Calls the function.
		 *
		 * @param us The function parameters to forward.
    	 **************************************************************************************************************/
		template<class... Ts>
		constexpr auto operator()(Ts&&... us) const noexcept(noexcept(FunctionPointer(std::forward<Ts...>(us...))));
	};
}

template<auto FunctionPointer> template<class... Ts>
constexpr auto tr::FunctionCaller<FunctionPointer>::operator()(Ts&&... us) const noexcept(noexcept(FunctionPointer(std::forward<Ts...>(us...))))
{
    return FunctionPointer(std::forward<Ts...>(us...));
}