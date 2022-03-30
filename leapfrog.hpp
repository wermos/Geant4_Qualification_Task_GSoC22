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
     * This function does one step of the Yoshida LeapFrog algorithm. This is analogous to the do_step
	 * function in the Boost odeint library.
     */
	State LeapFrogStepper(const State& currentState, const double t, const double tStep,
			Callable EFunc, Callable BFunc) {
		// These constants were evaluated to 15 digits of precision (which is the maximum precision
		// a double can store) using yoshida.py.
		constexpr double c1 = 0.675603595979829;
		constexpr double c2 = -0.175603595979829;
		constexpr double c3 = -0.175603595979829;
		constexpr double c4 = 0.675603595979829;

		constexpr double d1 = 1.35120719195966;
		constexpr double d2 = -1.70241438391932;
		constexpr double d3 = 1.35120719195966; 

		// Query the function which returns the value of E for the current value
		// of E and store it inside EField
		auto EField = EFunc(t);
		// Query the function which returns the value of B for the current value
		// of B and store it inside BField
		auto BField = BFunc(t);
		// converting the momentum vector into a velocity vector
		auto v = currentState.getMomentum() / mass;
		// Calculating the total force on the particle
		vec3 totalForce = charge * (EField + vec3::cross(v, BField));	
		// Calculating the acceleration from the force
		vec3 a = totalForce / mass;

		vec3 x1 = currentState.getPosition() + c1 * v * tStep;
		vec3 v1 = v + d1 * a * tStep;
		
		EField = EFunc(t + tStep);
		BField = BFunc(t + tStep);
		totalForce = charge * (EField + vec3::cross(v1, BField));	
		a = totalForce / mass;

		vec3 x2 = x1 + c2 * v1 * tStep;
		vec3 v2 = v1 + d2 * a * tStep;

		EField = EFunc(t + tStep);
		BField = BFunc(t + tStep);
		totalForce = charge * (EField + vec3::cross(v1, BField));	
		a = totalForce / mass;

		vec3 x3 = x2 + c3 * v2 * tStep;
		vec3 v3 = v2 + c3 * a * tStep;

		vec3 x4 = x3 + c4 * v3 * tStep;

		State newState;
		newState.setPosition(x4);
		newState.setMomentum(mass * v3);
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
