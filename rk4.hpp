#ifndef RK4_HPP
#define RK4_HPP

#include "vec3.hpp"

// These two constants are defined elsewhere. For now, all we need to let the compiler know
// is that they exist somewhere else
extern double mass;
extern double charge;

namespace Solver {
	template <typename EFunc, typename BFunc>
	void RK(const double (&currentState)[6], const double t0, const double tStep) {
		vec3 current_x(currentState[0]);
		vec3 current_p(currentState[3]);
		double temp[6];

		functionEvaluator<EFunc, BFunc>(currentState, t, temp);
		vec3 k1(temp);

		functionEvaluator<EFunc, BFunc>(currentState, t, k1);
		functionEvaluator<EFunc, BFunc>(currentState, t, k1);
		functionEvaluator<EFunc, BFunc>(currentState, t, k1);	

	}

	template <typename EFunc, typename BFunc>
	void functionEvaluator(const double (&currentState)[6], const double t,
						   double (&out)[6]) {
		// Query the function which returns the value of E for the current current
		// value of E and store it inside EField
		double EField[3];
		EFunc(t, EField);

		// Query the function which returns the value of B for the current current
		// value of B and store it inside BField
		double BField[3];
		BFunc(t, BField);

		// Slicing the currentState array into position and momentum vec3 variables
		vec3 x(currentState[0]);
		vec3 p(currentState[3]);
		/**
		vec3 v(p); // copying the contents of the momentum vector into the velocity vector

		v /= mass; // converting the momentum vector into a velocity vector
		*/

		totalForce = charge * (EField + vec3::cross(v, BField));		
		totalForce /= mass

		x.toArray(out);
		totalForce.toArray(out[3]);
	}
}

#endif
