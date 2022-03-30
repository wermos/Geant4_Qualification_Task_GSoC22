#include <cstddef> // for the std::size_t data type
#include <array> // for std::array

#include "boost/numeric/odeint.hpp" // for testing purposes

#include "matplotlibcpp.h" // for graphing purposes

#include "vec3.hpp"
#include "state.hpp"

#include "rk4.hpp"
#include "leapfrog.hpp"

typedef std::array<double, 6> StateType;
// originally, this was a boost::array, but then Boost documentation itself
// recommended using std:array if the project is written in C++11 or higher.

constexpr std::size_t numSteps = 2'0;
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

	std::vector<StateType> boostValues = boostResult(initialStateBoost, t0, tStep, numSteps);

	// Getting the times for which we have the X and Y values
	std::vector<double> times;
	times.reserve(numSteps + 1);

	for (auto i = 0; i < numSteps + 1; ++i) {
		times.push_back(t0 + i * tStep);
	}

	std::vector<double> rk4_X, rk4_Y;
	rk4_X.reserve(numSteps + 1);
	rk4_Y.reserve(numSteps + 1);
	for (auto& elem: values) {
		rk4_X.push_back(elem.getPosition().x());
		rk4_Y.push_back(elem.getPosition().y());
		//std::cout << elem << '\n';
	}

	std::vector<double> leapFrog_X, leapFrog_Y;
	leapFrog_X.reserve(numSteps + 1);
	leapFrog_Y.reserve(numSteps + 1);
	for (auto& elem: leapFrogValues) {
		leapFrog_X.push_back(elem.getPosition().x());
		leapFrog_Y.push_back(elem.getPosition().y());
		//std::cout << elem << '\n';
	}

	std::vector<double> boostX, boostY;
	boostX.reserve(numSteps + 1);
	boostY.reserve(numSteps + 1);
	for (auto& elem: boostValues) {
		boostX.push_back(elem[0]); // x-coordinate
		boostY.push_back(elem[1]); // y-coordinate
	}
 
	plt::figure_size(1280, 720);

	plt::named_plot("RK4", times, rk4_X, "r-");
	plt::plot(times, rk4_Y, "r-");

	plt::named_plot("Boris Leap frog", times, leapFrog_X, "b-");
	plt::plot(times, leapFrog_Y, "b-");
	
	//plt::named_plot("Boost Adams-Bashforth-Moulton", times, boostX, "k-");
	//plt::plot(times, boostY, "k-");

	plt::title("Position w.r.t. t"); // Add graph title

	plt::legend(); // Enable legend.
	plt::show();
}
