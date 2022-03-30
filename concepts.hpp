#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#ifdef __cpp_lib_concepts
	#include <concepts> // for std::same_as
#endif

namespace Solver {
#ifdef __cpp_lib_concepts
	template <typename FunctionType>
	concept EMFunc = requires (FunctionType func, const double t) {
		{ func(t) } -> std::same_as<std::array<double, 3>>;
	};
#endif
}
#endif // CONCEPTS_HPP
