/**
 * @file overloaded_lambda.hpp
 * @brief Provides an overloaded lambda helper class.
 */

#pragma once

namespace tr {
	/******************************************************************************************************************
	 * Overloaded lambda helper class.
	 *
	 * @tparam Fs The lambdas/functors to inheret from.
	 ******************************************************************************************************************/
	template <class... Fs> struct overloaded : Fs... {
		using Fs::operator()...;
	};

	/******************************************************************************************************************
	 * Deduction guide.
	 ******************************************************************************************************************/
	template <class... Fs> overloaded(Fs...) -> overloaded<Fs...>;
} // namespace tr