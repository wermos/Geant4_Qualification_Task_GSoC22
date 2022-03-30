#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

// These ifdef tests are feature test macros (which are guaranteed to exist
// in any ISO C++ conforming implementation), testing to see if C++ concepts
// are implemented in the compiler. If they are not, then this code is not 
// included at all. If C++ concepts are there in the compiler, then the
// following code is inserted.
//
// C++ concepts make template error messages much more readable than before.
// Since I chose to take in the E and B functions as template parameters,
// I decided to use C++ concepts (if they are available) to make error messages
// (if the compiler emits any) more readable.
#ifdef __cpp_lib_concepts
	#include <concepts> // for std::same_as
#endif

namespace Solver {
/**
 * This header defines a C++ concept for any C++ function which can act as a
 * function describing the Electric and Magnetic Fields.
 *
 * For the purposes of this assignment, I chose to make it so that the E and B
 * fields can vary with time, but not with spatial coordinates.
 *
 * Allowing for spatially as well as time-dependent E and B field will not
 * require too much of a modification to the existing code. However, since the
 * assignment did not require us to generalize the project as much as possible,
 * the current version of the project as it stands only allows for time-varying
 * E and B fields.
 * 
 * The current concept requires E and B functions to depend on only one parameter,
 * which must be of double type. This is supposed to be the time at which we want
 * the values of the E and B field. The return type is required to be a std::array
 * of doubles of length 3.
 */
#ifdef __cpp_lib_concepts
	template <typename FunctionType>
	concept EMFunc = requires (FunctionType func, const double t) {
		{ func(t) } -> std::same_as<std::array<double, 3>>;
	};
#endif
}
#endif // CONCEPTS_HPP
