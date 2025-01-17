#pragma once
#include "angle.hpp"
#include "concepts.hpp"
#include <cstdint>

namespace tr {
	/** @ingroup misc
	 *  @defgroup rng RNG
	 *  Random number generation functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * @brief Generates a random seed.
	 *
	 * @return A random boolean value.
	 ******************************************************************************************************************/
	std::uint64_t generateRandomSeed() noexcept;

	/******************************************************************************************************************
	 * Xorshiftr128+ random number generator.
	 ******************************************************************************************************************/
	class Xorshiftr128p {
	  public:
		/**************************************************************************************************************
		 * Initializes RNG with a value gotten from generateRandomSeed().
		 **************************************************************************************************************/
		Xorshiftr128p() noexcept;

		/**************************************************************************************************************
		 * Initializes RNG with a seed.
		 *
		 * @param seed The RNG seed.
		 **************************************************************************************************************/
		constexpr Xorshiftr128p(std::uint64_t seed) noexcept;

		/**************************************************************************************************************
		 * Advances the RNG and returns a value.
		 *
		 * @return A random 64-bit integer.
		 **************************************************************************************************************/
		constexpr std::uint64_t advance() noexcept;

	  private:
		std::uint64_t _state[2];
	};

	/******************************************************************************************************************
	 * @brief Generates a random boolean value.
	 *
	 * @param rng The random number generator to use.
	 *
	 * @return A random boolean value.
	 ******************************************************************************************************************/
	constexpr bool randb(Xorshiftr128p& rng) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random integral value.
	 *
	 * @param rng The random number generator to use.
	 *
	 * @return A random integral value.
	 ******************************************************************************************************************/
	template <std::integral T> constexpr T rand(Xorshiftr128p& rng) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random integral value in the range [0, max).
	 *
	 * @param rng The random number generator to use.
	 * @param max
	 * @parblock
	 * The upper boundary for the generated value.
	 *
	 * @pre @em max cannot be 0.
	 * @endparblock
	 *
	 * @return A random integral value in the range [0, max).
	 ******************************************************************************************************************/
	template <std::integral T> constexpr T rand(Xorshiftr128p& rng, T max) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random integral value in the range [min, max).
	 *
	 * @pre `(min - max)` cannot be 0.
	 *
	 * @param rng The random number generator to use.
	 * @param min The lower boundary for the generated value.
	 * @param max The upper boundary for the generated value.
	 *
	 * @return A random integral value in the range [min, max).
	 ******************************************************************************************************************/
	template <std::integral T> constexpr T rand(Xorshiftr128p& rng, T min, T max) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random floating point value in the range [0, 1).
	 *
	 * @param rng The random number generator to use.
	 *
	 * @return A random floating point value in the range [0, 1).
	 ******************************************************************************************************************/
	template <std::floating_point T> constexpr T rand(Xorshiftr128p& rng) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random floating point value in the range [0, max).
	 *
	 * @param rng The random number generator to use.
	 * @param max The upper boundary for the generated value.
	 *
	 * @return A random floating point value in the range [0, max).
	 ******************************************************************************************************************/
	template <std::floating_point T> constexpr T rand(Xorshiftr128p& rng, T max) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random floating point value in the range [min, max).
	 *
	 * @pre `(min - max)` cannot be 0.
	 *
	 * @param rng The random number generator to use.
	 * @param min The lower boundary for the generated value.
	 * @param max The upper boundary for the generated value.
	 *
	 * @return A random floating point value in the range [min, max).
	 ******************************************************************************************************************/
	template <std::floating_point T> constexpr T rand(Xorshiftr128p& rng, T min, T max) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random angle value in the range [0tr, 1tr).
	 *
	 * @param rng The random number generator to use.
	 *
	 * @return A random angle value in the range [0tr, 1tr).
	 ******************************************************************************************************************/
	template <SpecializationOf<Angle> T> constexpr T rand(Xorshiftr128p& rng) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random angle value in the range [0, max).
	 *
	 * @param rng The random number generator to use.
	 * @param max The upper boundary for the generated value.
	 *
	 * @return A random angle value in the range [0tr, max).
	 ******************************************************************************************************************/
	template <SpecializationOf<Angle> T> constexpr T rand(Xorshiftr128p& rng, T max) noexcept;

	/******************************************************************************************************************
	 * @brief Generates a random angle value in the range [min, max).
	 *
	 * @pre `(min - max)` cannot be 0.
	 *
	 * @param rng The random number generator to use.
	 * @param min The lower boundary for the generated value.
	 * @param max The upper boundary for the generated value.
	 *
	 * @return A random angle value in the range [min, max).
	 ******************************************************************************************************************/
	template <SpecializationOf<Angle> T> constexpr T rand(Xorshiftr128p& rng, T min, T max) noexcept;

	/// @}
} // namespace tr

#include "rng_impl.hpp"