#include <cstddef> // for the std::size_t data type
#include <array> // for std::array

#include "boost/numeric/odeint.hpp" // for testing purposes

#include "matplotlibcpp.h" // for graphing purposes

#include "vec3.hpp"
#include "state.hpp"

#include "rk4.hpp"
#include "leapfrog.hpp"

typedef std::array<double, 6> StateType;
// originally, this was boost::array, but then Boost documentation itself
// recommended using std::array if the project in C++11 or higher.

constexpr std::size_t numSteps = 2'00;
constexpr double speed_of_light = 299'792'458; // units: m/s
double mass = 9.109e-31; // units: kg
double charge = 1.602e-19; // units: C

std::array<double, 3> B(const double /* t */) {
	return {0, 0, 1};
}

std::array<double, 3> E(const double /* t */) {
	return {0, 0, 0};
}

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
	
	// Move position variables into dpdt array
	v.toArray(&out[0]);

	// Move totalForce into dpdt array:
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
	// This runs an 8th order integration scheme called the Adams
	// Bashforth Moulton method
	
	double currentTime = t0;

	for (auto i = 0; i < numSteps; ++i) {
		abmStepper.do_step(updateFunction, values.back(), currentTime, tempState, tStep);
		/**
		for (auto& value : tempState) {
			std::cout << value << ' ';
		}
		std::cout << '\n';
		*/
		values.push_back(tempState); // temp is copied into the std::vector
		currentTime += tStep;
	}

	return values;
}

int main() {
	namespace plt = matplotlibcpp;

	constexpr double v0 = 0.9 * speed_of_light;
	constexpr double t0 = 0;
	constexpr double tStep = 1e-12;
	constexpr vec3 dir0 = {0, 1, 0};
	constexpr vec3 initialVelocity = v0 * dir0;
	constexpr vec3 pos0 = {0, 0, 0};
	const vec3 initialMomentum = mass * initialVelocity;

	// obtaining the values using my own implementation of RK4
	State initialState(pos0, initialMomentum);

	std::vector<State> values = Solver::RK4(initialState, t0, tStep, numSteps, E, B);
	std::vector<State> leapFrogValues = Solver::LeapFrog(initialState, t0, tStep, numSteps, E, B);

	StateType initialStateBoost{pos0[0], pos0[1], pos0[2], initialMomentum[0], initialMomentum[1], initialMomentum[2]};
	std::vector<StateType> boostValues = boostResult(initialStateBoost, t0, tStep, numSteps);

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
	for (auto& elem: values) {
		leapFrog_X.push_back(elem.getPosition().x());
		leapFrog_Y.push_back(elem.getPosition().y());
		//std::cout << elem << '\n';
	}

	std::vector<double> boostX, boostY;
	boostX.reserve(numSteps + 1);
	boostY.reserve(numSteps + 1);

	for (auto& elem: boostValues) {
		boostX.push_back(elem[0]);
		boostY.push_back(elem[1]);
	}
 
	plt::figure_size(1280, 720);

	plt::plot(times, rk4_X, "r--");
	plt::plot(times, rk4_Y, "r--");

	plt::plot(times, leapFrog_X, "b--");
	plt::plot(times, leapFrog_Y, "b--");
	
	//plt::plot(times, boostX, "g--");
	//plt::plot(times, boostY, "g--");

	plt::show();
}
