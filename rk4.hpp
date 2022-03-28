#ifndef RK4_HPP
#define RK4_HPP

#include <cstdlib> // for the std::size_t data type
#include <vector>
#include "state.hpp"
#include "vec3.hpp"

// These two constants are defined elsewhere. For now, all we need to let the
// compiler know is that they exist somewhere else
extern double mass;
extern double charge;

namespace Solver {
#ifdef __cpp_lib_concepts
	template <typename FunctionType>
	concept 3DFunction = requires (FunctionType func, double t) {
		func(t) -> double*;
	};
#endif

#ifdef __cpp_lib_concepts
	template <3DFunction Callable>
#else
	template <typename Callable>
#endif
	std::vector<State> RK4(const State initialState, const double t0,
			const double tStep, const std::size_t numSteps, Callable EFunc,
			Callable BFunc) {
		std::vector<State> values(numStep + 1);
		// explicitly initialize the vector with the required amount of space to
		// prevent memory reallocations

		values.push_back(initialState);

		double currentTime = t0;

		for (auto i = 0; i < numSteps; ++i) {
			State state = RKStepper(values.back(), currentTime, tStep, EFunc, BFunc);
			values.push_back(state)
			currentTime += tStep;
		}

		return values;
	}

#ifdef __cpp_lib_concepts
	template <3DFunction Callable>
#else
	template <typename Callable>
#endif
	State RKStepper(const State currentState, const double t, const double tStep,
			Callable EFunc, Callable BFunc) {
		State k1 = functionEvaluator(currentState, t, EFunc, BFunc);
		State k2 = functionEvaluator(currentState + (tStep / 2) * k1, t + (tStep / 2), EFunc, BFunc);
		State k3 = functionEvaluator(currentState + (tStep / 2) * k2, t + (tStep / 2), EFunc, BFunc);
		State k4 = functionEvaluator(currentState + tStep * k3, t + tStep, EFunc, BFunc);

		return currentState + (1.0 / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
	}

#ifdef __cpp_lib_concepts
	template <3DFunction Callable>
#else
	template <typename Callable>
#endif
	State functionEvaluator(const State& currentState, const double t, Callable EFunc, Callable BFunc) {
		// Query the function which returns the value of E for the current current
		// value of E and store it inside EField
		double EField[3];
		EFunc(t, EField);

		// Query the function which returns the value of B for the current current
		// value of B and store it inside BField
		double BField[3];
		BFunc(t, BField);

		/**
		// Slicing the currentState array into position and momentum vec3 variables
		vec3 x(currentState[0]);
		vec3 p(currentState[3]);
		vec3 v(p); // copying the contents of the momentum vector into the velocity vector

		v /= mass; // converting the momentum vector into a velocity vector
		*/

		vec3 totalForce = charge * (EField + vec3::cross(v, BField));		
		//totalForce /= mass

		State newState;

		newState.setPosition(currentState.getPosition());
		newState.setMomentum(totalForce);

		return newState;
	}
}

#endif // RK4_HPP
