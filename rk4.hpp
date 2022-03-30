#ifndef RK4_HPP
#define RK4_HPP

#include <cstddef> // for the std::size_t data type
#include <array> // for std::array
#include <vector>

#include "concepts.hpp"
#include "state.hpp"
#include "vec3.hpp"

// These two constants are defined elsewhere. For now, all we need to let the
// compiler know is that they exist somewhere else
extern double mass;
extern double charge;

namespace Solver {
// These ifdef clauses conditionally use the concepts defined in concepts.hpp if the compiler
// supports C++ concepts. All the functions in this header use concepts if they are there, and
// use normal template parameters if they are not implemented in the compiler.
#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	/**
	 * This function is dedicated to evaluating the E and B functions at the current time and returning
	 * the relevant information to the RKStepper function.
	 */
	State functionEvaluator(const State& currentState, const double t, Callable EFunc, Callable BFunc) {
		// Query the function which returns the value of E for the current value
		// of E and store it inside EField
		auto EField = EFunc(t);

		// Query the function which returns the value of B for the current value
		// of B and store it inside BField
		auto BField = BFunc(t);

		// converting the momentum vector into a velocity vector
		auto v = currentState.getMomentum() / mass;

		vec3 totalForce = charge * (EField + vec3::cross(v, BField));	

		State newState;

		// We are actually storing the velocity and the acceleration in the newState variable here, using
		// the setPosition and setMomentum functions. Due to a small oversight in the design, the member
		// function names aren't true to their purpose and don't make much sense in this case.
		newState.setPosition(v);
		newState.setMomentum(totalForce);

		return newState;
	}

#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	/**
	 * This function does one step of the RK4 algorithm. This is analogous to the do_step function
	 * in the Boost odeint library.
	 */
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
	/**
	 * This function runs the entire RK4 integration scheme on the given problem. It uses the E and B
	 * fields that are passed into the function (as EFunc and BFunc).
	 *
	 * It runs numSteps of the algorithm on the problem, and then returns a vector of all the States
	 * at each point.
	 */
	std::vector<State> RK4(const State initialState, const double t0,
			const double tStep, const std::size_t numSteps, Callable EFunc,
			Callable BFunc) {
		std::vector<State> values;
		values.reserve(numSteps + 1);
		// explicitly initialize the vector with the required amount of space to prevent memory
		// reallocations

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
