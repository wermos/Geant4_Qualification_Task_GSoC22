#ifndef STATE_HPP
#define STATE_HPP

#include "vec3.hpp"

/**
 * A wrapper class around an array of 6 doubles, tailored specifically to the
 * problem of integrating the equations of motion of a charged particle under
 * the influence of E and B fields.
 *
 * I overloaded the arithmetic operators and made them work elementwise, to allow
 * for more readable code.
 *
 */
class State {
	public:
		constexpr State() noexcept : y{0, 0, 0, 0, 0, 0} {}
		constexpr State(const double y0, const double y1,
						const double y2, const double y3,
						const double y4, const double y5) noexcept :
			y{y0, y1, y2, y3, y4, y5} {}
		constexpr State(const vec3& x, const vec3& p) noexcept :
			y{x[0], x[1], x[2], p[0], p[1], p[2]} {}
		constexpr State(const State&) noexcept = default;

		constexpr vec3 getPosition() const {
			// The reason this works is a little tricky. &y[0] is a pointer to the
			// first element in the array.
			//
			// In the vec3 class, we have a constructor which takes in a pointer to
			// an array of of length at least 3, and then uses the first 3 elements
			// in that array to make a vec3. Since the first 3 elements in y are the
			// (x, y, z) coordinates of the position, passing in a pointer to the
			// 0th element (i.e. the x coordinate) gets us a vec3 of the position.
			return {&y[0]};
		}

		constexpr vec3 getMomentum() const {
			// This uses an idea similar to the one explained in the comment in the
			// getPosition() function, but instead of passing on a pointer to the
			// 0th element, we pass on a pointer to the 3rd element. Hence, the vec3
			// is constructed using y[3], y[4], and y[5], which store the (p_x, p_y,
			// p_z) coordinates of the particle.
			return {&y[3]};
		}

		constexpr void setPosition(const vec3& x) {
			// This uses the same idea explained in the comment in the getPosition()
			// function
			x.toArray(&y[0]);
		}

		constexpr void setMomentum(const vec3& p) {
			// This uses the same idea explained in the comment in the getMomentum()
			// function
			p.toArray(&y[3]);
		}

		constexpr void toArray(double* out) const {
			// Undefined behavior if the array is too small
			for (auto i = 0; i < 6; ++i) {
				out[i] = y[i];
			}
		}

		// Arithmetic operator overloads
		constexpr State operator-() const {
			return {-y[0], -y[1], -y[2], -y[3], -y[4], -y[5]}; 
		}

		constexpr const double& operator[](int i) const {
			return y[i];
		}

		constexpr double& operator[](int i) {
			return y[i];
		}

		friend constexpr State& operator+=(State& s1, const State& s2) {
			s1.y[0] += s2.y[0];
			s1.y[1] += s2.y[1];
			s1.y[2] += s2.y[2];
			s1.y[3] += s2.y[3];
			s1.y[4] += s2.y[4];
			s1.y[5] += s2.y[5];
			return s1;
		}

		friend constexpr State& operator-=(State& s1, const State& s2) {
			s1.y[0] -= s2.y[0];
			s1.y[1] -= s2.y[1];
			s1.y[2] -= s2.y[2];
			s1.y[3] -= s2.y[3];
			s1.y[4] -= s2.y[4];
			s1.y[5] -= s2.y[5];
			return s1;
		}

		friend constexpr State& operator*=(State& s, const double t) {
			s.y[0] *= t;
			s.y[1] *= t;
			s.y[2] *= t;
			s.y[3] *= t;
			s.y[4] *= t;
			s.y[5] *= t;
			return s;
		}

		friend constexpr State& operator*=(const double t, State& s) {
			return s *= t;
		}

		friend constexpr State& operator/=(State& s, const double t) {
			return s *= (1 / t);
		}

		friend constexpr State operator+(const State& s1, const State& s2) {
			State copy = s1;
			copy += s2;
			return copy;
		}

		friend constexpr State operator-(const State& s1, const State& s2) {
			State copy = s1;
			copy -= s2;
			return copy;
		}

		friend constexpr State operator*(const State& s, const double t) {
			State copy = s;
			copy *= t;
			return copy;
		}

		friend constexpr State operator*(const double t, const State& s) {
			State copy = s;
			copy *= t;
			return copy;
		}

		// The << operator was also overloaded to help with debugging purposes.
		friend std::ostream& operator<<(std::ostream& out, const State& s) {
			for (auto i = 0; i < 6; i++) {
				out << s.y[i] << " ";
			}
			return out;
		}

	private:
		double y[6];
};

#endif // STATE_HPP
