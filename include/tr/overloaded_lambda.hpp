#pragma once

namespace tr {
	/** @ingroup misc
	 *  @defgroup overloaded_lambda Overloaded
	 *  Overloaded lambda helper class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Overloaded lambda helper class.
	 *
	 * @tparam Fs The lambdas/functors to inheret from.
	 ******************************************************************************************************************/
	template <class... Fs> struct Overloaded : Fs... {
		using Fs::operator()...;
	};

	/******************************************************************************************************************
	 * Deduction guide.
	 ******************************************************************************************************************/
	template <class... Fs> Overloaded(Fs...) -> Overloaded<Fs...>;

	/// @}
} // namespace tr