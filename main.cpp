#include <cstddef> // for the std::size_t data type
#include <array> // for std::array
#include <cmath> // for std::fabs
#include <iostream>

#include "boost/numeric/odeint.hpp" // for testing purposes

#include "matplotlibcpp.h" // for graphing purposes

#include "vec3.hpp"
#include "state.hpp"

#include "rk4.hpp"
#include "leapfrog.hpp"

typedef std::array<double, 6> StateType;
// originally, this was a boost::array, but then Boost documentation itself
// recommended using std:array if the project is written in C++11 or higher.

constexpr std::size_t numSteps = 40'000;
constexpr double speed_of_light = 299'792'458; // units: m/s
double mass = 9.109e-31; // units: kg
double charge = 1.602e-19; // units: C

std::array<double, 3> B(const double /* t */) {
	// Since the B field in the test case doesn't depend on t, we don't need
	// to name the parameter variable.
	return {0, 0, 1};
}

std::array<double, 3> E(const double /* t */) {
	// Since the E field in the test case doesn't depend on t, we don't need
	// to name the parameter variable.
	return {0, 0, 0};
}

// This is analogous to the functionEvaluator in the rk4.hpp file, but
// this one doesn't have any template parameters (though they could be
// added relatively easily). This is the update function for the reference
// Boost odeint solver.
void updateFunction(const StateType& y, StateType& out, const double t) {
	// Query the function which returns the value of E for the current
	// value of E and store it inside EField
	auto EField = E(t);

	// Query the function which returns the value of B for the current
	// value of B and store it inside BField
	auto BField = B(t);

	vec3 v(y[3], y[4], y[5]);
	v /= mass;
	// converting the momentum vector into a velocity vector

	vec3 totalForce = charge * (EField + vec3::cross(v, BField));
	
	// Move position variables into out array
	v.toArray(&out[0]);

	// Move totalForce into out array:
	totalForce.toArray(&out[3]);
}

std::vector<StateType> boostResult(const StateType initialState, const double t0,
								   const double tStep, const std::size_t numSteps) {
	using namespace boost::numeric::odeint;

	StateType tempState;
	// we will use this variable for getting the new values out of the stepper

	std::vector<StateType> values;
	values.reserve(numSteps + 1);
	// explicitly initialize the vector with the required amount of space to
	// prevent memory reallocations
	
	values.push_back(initialState);

	adams_bashforth_moulton<8, StateType> abmStepper;
	// This runs an 8th order integration scheme called the Adams-Bashforth-Moulton method
	
	double currentTime = t0;

	for (auto i = 0; i < numSteps; ++i) {
		abmStepper.do_step(updateFunction, values.back(), currentTime, tempState, tStep);
		values.push_back(tempState); // temp is copied into the std::vector
		currentTime += tStep;
	}

	return values;
}

vec3 getPosition(StateType state) {
	// The objective is to print the position vector, given the values.
	// To achieve this, we just convert the raw numbers into a vec3 because
	// that class has opertor<< overloaded.
	return {state[0], state[1], state[2]};
}

double getMomentum(StateType state) {
	// A helper function for getting the momentum from the state, for Boost
	return state[3] * state[3] + state[4] * state[4] + state[5] * state[5];
}

int main() {
	namespace plt = matplotlibcpp;

	// Setting up the initial values
	constexpr double v0 = 0.9 * speed_of_light;
	constexpr double t0 = 0;
	constexpr double tStep = 8.93e-12;
	constexpr vec3 dir0 = {0, 1, 0};
	constexpr vec3 initialVelocity = v0 * dir0;
	constexpr vec3 pos0 = {0, 0, 0};
	const vec3 initialMomentum = mass * initialVelocity;

	// Setting up the initial state for my solvers
	State initialState(pos0, initialMomentum);
	// Setting up the initial state for the Boost solver
	StateType initialStateBoost{pos0[0], pos0[1], pos0[2], initialMomentum[0], initialMomentum[1], initialMomentum[2]};

	// obtaining the values using my own implementation of RK4
	std::vector<State> values = Solver::RK4(initialState, t0, tStep, numSteps, E, B);
	// obtaining the values using my own implementation of the Boris leapfrog algorithm
	std::vector<State> leapFrogValues = Solver::LeapFrog(initialState, t0, tStep, numSteps, E, B);
	// obtaining the values using Boost 
	std::vector<StateType> boostValues = boostResult(initialStateBoost, t0, tStep, numSteps);


	for (auto i = 1; i < leapFrogValues.size(); ++i) {
		// We start at i = 1 to avoid the false negative at i = 0
		double posX = leapFrogValues[i].getPosition().x();
		double posY = leapFrogValues[i].getPosition().y();
		double posZ = leapFrogValues[i].getPosition().z();
		if (std::fabs(posX) < 1e-5 && std::fabs(posY) < 1e-5 && std::fabs(posZ) < 1e-5) {
			// The idea is that, the particle starts at the origin. If we see the time taken
			// for it to come back to the origin (allowing for some error due to loss in
			// precision), this will be the time period.
			std::cout << "Time period: " << i * tStep << '\n';
			break;
		}
	}

	double radius; // This will store the radius of curvature of the circle
	for (auto i = 0; i < leapFrogValues.size(); ++i) {
		double posX = leapFrogValues[i].getPosition().x();
		double posY = leapFrogValues[i].getPosition().y();
		if (std::fabs(posX) < 1e-5 && std::fabs(posY) >= 1e-5) {
			// The electron is travelling in the XY plane, and so it will go through (0, 0) and
			// (0, 2 * r). Allowing for some precision issues, this is what we are checking for in
			// the if clause: When is the x coordinate approximately 0, but the y coordinate is
			// non-zero?
			radius = std::fabs(posY) / 2; // because radius cannot be negative
			break;
		}
	}
	std::cout << "Radius of curvature: " << radius << "\n\n";

	std::cout << "Position after 10 turns:\n";
	std::cout << "RK4:\t\t(" << values[10].getPosition() << ")\n";
	std::cout << "Leapfrog:\t(" << leapFrogValues[10].getPosition() << ")\n";
	std::cout << "Boost:\t\t(" << getPosition(boostValues[10]) << ")\n\n";

	std::cout << "Position after 100 turns:\n";
	std::cout << "RK4:\t\t(" << values[100].getPosition() << ")\n";
	std::cout << "Leapfrog:\t(" << leapFrogValues[100].getPosition() << ")\n";
	std::cout << "Boost:\t\t(" << getPosition(boostValues[100]) << ")\n\n";

	std::cout << "Position after 1,000 turns:\n";
	std::cout << "RK4:\t\t(" << values[1'000].getPosition() << ")\n";
	std::cout << "Leapfrog:\t(" << leapFrogValues[1'000].getPosition() << ")\n";
	std::cout << "Boost:\t\t(" << getPosition(boostValues[1'000]) << ")\n\n";

	std::cout << "Position after 10,000 turns:\n";
	std::cout << "RK4:\t\t(" << values[10'000].getPosition() << ")\n";
	std::cout << "Leapfrog:\t(" << leapFrogValues[10'000].getPosition() << ")\n";
	std::cout << "Boost:\t\t(" << getPosition(boostValues[10'000]) << ")\n\n";

	std::cout << "Momentum after 10 turns:\n";
	std::cout << "RK4:\t\t" << values[10].getMomentum().length() << "\n";
	std::cout << "Leapfrog:\t" << leapFrogValues[10].getMomentum().length() << "\n";
	std::cout << "Boost:\t\t" << getMomentum(boostValues[10]) << "\n\n";

	std::cout << "Momentum after 100 turns:\n";
	std::cout << "RK4:\t\t" << values[100].getMomentum().length() << "\n";
	std::cout << "Leapfrog:\t" << leapFrogValues[100].getMomentum().length() << "\n";
	std::cout << "Boost:\t\t" << getMomentum(boostValues[100]) << "\n\n";

	std::cout << "Momentum after 1,000 turns:\n";
	std::cout << "RK4:\t\t" << values[1'000].getMomentum().length() << "\n";
	std::cout << "Leapfrog:\t" << leapFrogValues[1'000].getMomentum().length() << "\n";
	std::cout << "Boost:\t\t" << getMomentum(boostValues[1'000]) << "\n\n";

	std::cout << "Momentum after 10,000 turns:\n";
	std::cout << "RK4:\t\t" << values[10'000].getMomentum().length() << "\n";
	std::cout << "Leapfrog:\t" << leapFrogValues[10'000].getMomentum().length() << "\n";
	std::cout << "Boost:\t\t" << getMomentum(boostValues[10'000]) << "\n\n";

	constexpr double trueMomentum = 2.458e-22;

	std::vector<std::size_t> numTurns;
	numTurns.reserve(10'000);
	for (auto i = 0; i <= 10'000; ++i) {
		numTurns.push_back(i);
	}
 
	std::vector<double> boostMomentum, leapFrogMomentum, RK4Momentum;
	boostMomentum.reserve(10'001);
	leapFrogMomentum.reserve(10'001);
	RK4Momentum.reserve(10'001);
	for (auto i = 0; i <= 40'000; i += 4) {
		boostMomentum.push_back(std::fabs(trueMomentum - getMomentum(boostValues[i])));
		leapFrogMomentum.push_back(std::fabs(trueMomentum - leapFrogValues[i].getMomentum().length()));
		RK4Momentum.push_back(std::fabs(trueMomentum - values[i].getMomentum().length()));
	}

	plt::figure_size(1920, 1080);

	plt::named_plot("RK4", numTurns, RK4Momentum, "r-");

	plt::named_plot("Boris Leap frog", numTurns, leapFrogMomentum, "b-");
	
	plt::named_plot("Boost Adams-Bashforth-Moulton", numTurns, boostMomentum, "k-");

	plt::title("Deviation of Momentum w.r.t. number of turns"); // Add graph title

	plt::legend(); // Enable legend.
	plt::show();

}
