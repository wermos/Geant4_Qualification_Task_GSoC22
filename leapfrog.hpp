#ifndef LEAPFROG_HPP
#define LEAPFROG_HPP

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
     * This function does one step of the Boris LeapFrog algorithm. This is analogous to the do_step
	 * function in the Boost odeint library.
     */
	State LeapFrogStepper(const State& currentState, const double t, const double tStep,
			Callable EFunc, Callable BFunc) {
		// Query the function which returns the value of E for the current value
		// of E and store it inside EField
		vec3 EField = EFunc(t);
		// Query the function which returns the value of B for the current value
		// of B and store it inside BField
		vec3 BField = BFunc(t);

		vec3 h = (charge / (2 * mass)) * BField * tStep;
		vec3 s = (2 * h) / (1 + h.lengthSquared());

		// converting the momentum vector into a velocity vector
		auto v = currentState.getMomentum() / mass;

		vec3 v_minus = v + (charge / (2 * mass)) * EField * tStep;
		vec3 v_prime = v_minus + vec3::cross(v_minus, h);
		vec3 v_plus = v_minus + vec3::cross(v_prime, s);
		vec3 final_v = v_plus + (charge / (2 * mass)) * EField * tStep;
		
		State newState;

		newState.setPosition(currentState.getPosition() + final_v * tStep);
		newState.setMomentum(mass * final_v);

		return newState;
	}

#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	/**
     * This function runs the entire Yohsida integration scheme on the given problem. It uses the E
	 * and B fields that are passed into the function (as EFunc and BFunc).
     *
     * It runs numSteps of the algorithm on the problem, and then returns a vector of all the States
     * at each point.
     */
	std::vector<State> LeapFrog(const State initialState, const double t0,
			const double tStep, const std::size_t numSteps, Callable EFunc,
			Callable BFunc) {
		std::vector<State> values;
		values.reserve(numSteps + 1);
		// explicitly initialize the vector with the required amount of space to prevent memory
		// reallocations

		values.push_back(initialState);

		double currentTime = t0;

		for (auto i = 0; i < numSteps; ++i) {
			State state = LeapFrogStepper(values.back(), currentTime, tStep, EFunc, BFunc);
			values.push_back(state);
			currentTime += tStep;
		}

		return values;
	}
}
#endif // LEAPFROG_HPP
