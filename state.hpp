#ifndef STATE_HPP
#define STATE_HPP

#include "vec3.hpp"

class State {
	public:
		constexpr State() noexcept : y{0, 0, 0, 0, 0, 0} {}
		constexpr State(const double y0, const double y1,
						const double y2, const double y3,
						const double y4, const double y5) noexcept :
			y{y0, y1, y2, y3, y4, y5} {}
		constexpr State(const double* arr) noexcept :
			y{arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]} {}
		constexpr State(const vec3& x, const vec3& p) noexcept :
			y{x[0], x[1], x[2], p[0], p[1], p[2]} {}
		constexpr State(const State&) noexcept = default;

		constexpr vec3 getPosition() const {
			// TODO: Add comment explaining why this works
			return {&y[0]};
		}

		constexpr vec3 getMomentum() const {
			// TODO: Add comment explaining why this works
			return {&y[3]};
		}

		constexpr void setPosition(const vec3& x) {
			x.toArray(&y[0]);
		}

		constexpr void setMomentum(const vec3& p) {
			p.toArray(&y[3]);
		}

		constexpr void toArray(double* out) const {
			// Undefined behavior if the array is too small
			for (auto i = 0; i < 6; ++i) {
				out[i] = y[i];
			}
		}

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
