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
#ifdef __cpp_lib_concepts
	template <typename Callable> requires EMFunc<Callable>
#else
	template <typename Callable>
#endif
	State LeapFrogStepper(const State& currentState, const double t, const double tStep,
			Callable EFunc, Callable BFunc) {
		/**
		

		vec3 newPosition = currentState.getPosition() + v * tStep + (1.0 / 2) * a * tStep * tStep;
		//vec3 newMomentum = currentState.getMomentum() + v * tStep + (1.0 / 2) * a * tStep * tStep;
		vec3 newVelocity = v + (1.0 / 2) * (a + ____) * tStep;
		*/
		
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
	std::vector<State> LeapFrog(const State initialState, const double t0,
			const double tStep, const std::size_t numSteps, Callable EFunc,
			Callable BFunc) {
		std::vector<State> values;
		values.reserve(numSteps + 1);
		// explicitly initialize the vector with the required amount of space to
		// prevent memory reallocations

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
