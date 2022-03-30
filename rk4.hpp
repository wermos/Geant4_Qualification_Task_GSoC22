#ifndef RK4_HPP
#define RK4_HPP

#include <cstddef> // for the std::size_t data type
#include <array> // for std::array
#include <vector>

#ifdef __cpp_lib_concepts
	#include <concepts> // for std::same_as
#endif

#include "concepts.hpp"
#include "state.hpp"
#include "vec3.hpp"

// These two constants are defined elsewhere. For now, all we need to let the
// compiler know is that they exist somewhere else
extern double mass;
extern double charge;

namespace Solver {
#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	State functionEvaluator(const State& currentState, const double t, Callable EFunc, Callable BFunc) {
		// Query the function which returns the value of E for the current value
		// of E and store it inside EField
		auto EField = EFunc(t);

		// Query the function which returns the value of B for the current value
		// of B and store it inside BField
		auto BField = BFunc(t);

		auto v = currentState.getMomentum() / mass;
		// converting the momentum vector into a velocity vector

		vec3 totalForce = charge * (EField + vec3::cross(v, BField));	

		State newState;

		newState.setPosition(v);
		newState.setMomentum(totalForce);

		return newState;
	}

#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	State RKStepper(const State& currentState, const double t, const double tStep,
			Callable EFunc, Callable BFunc) {
		State k1 = functionEvaluator(currentState, t, EFunc, BFunc);
		State k2 = functionEvaluator(currentState + (tStep / 2) * k1, t + (tStep / 2), EFunc, BFunc);
		State k3 = functionEvaluator(currentState + (tStep / 2) * k2, t + (tStep / 2), EFunc, BFunc);
		State k4 = functionEvaluator(currentState + tStep * k3, t + tStep, EFunc, BFunc);

		return currentState + (1.0 / 6) * tStep * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
	}

#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	std::vector<State> RK4(const State initialState, const double t0,
			const double tStep, const std::size_t numSteps, Callable EFunc,
			Callable BFunc) {
		std::vector<State> values;
		values.reserve(numSteps + 1);
		// explicitly initialize the vector with the required amount of space to
		// prevent memory reallocations

		values.push_back(initialState);

		double currentTime = t0;

		for (auto i = 0; i < numSteps; ++i) {
			State state = RKStepper(values.back(), currentTime, tStep, EFunc, BFunc);
			values.push_back(state);
			currentTime += tStep;
		}

		return values;
	}
}
#endif // RK4_HPP
