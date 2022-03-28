#ifndef STATE_HPP
#define STATE_HPP

#include "vec3.hpp"

class State {
	public:
		constexpr State() noexcept : y({0, 0, 0, 0, 0, 0}) {}
		constexpr State(double (&arr)[6]) noexcept :
			arr({-arr[0], -arr[1], -arr[2], -arr[3], -arr[4], -arr[5]}) {}
		constexpr State(const State&) noexcept = default;

		constexpr vec3 getPosition() const {
			// TODO: Add comment explaining why this works
			return {y[0]};
		}

		constexpr vec3 getMomentum() const {
			// TODO: Add comment explaining why this works
			return {y[3]};
		}

		constexpr void setPosition(const vec3& x) {
			x.toArray(y[0]);
		}

		constexpr void setMomentum(const vec3& p) {
			p.toArray(y[3]);
		}

		constexpr void toArray(double (&out)[3]) const {
			for (auto i = 0; i < 3; ++i) {
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

		friend constexpr State& operator+=(State& u, const State& v) {
			u.y[0] += v.y[0];
			u.y[1] += v.y[1];
			u.y[2] += v.y[2];
			return u;
		}

		friend constexpr State& operator-=(State& u, const State& v) {
			u.y[0] -= v.y[0];
			u.y[1] -= v.y[1];
			u.y[2] -= v.y[2];
			return u;
		}

		friend constexpr State& operator*=(State& v, const double t) {
			v.y[0] *= t;
			v.y[1] *= t;
			v.y[2] *= t;
			return v;
		}

		friend constexpr State& operator*=(const double t, State& v) {
			return v *= t;
		}

		friend constexpr State& operator/=(State& v, const double t) {
			return v *= (1 / t);
		}

		friend constexpr State operator+(const State& u, const State& v) {
			State copy = u;
			copy += v;
			return copy;
		}

		friend constexpr State operator-(const State& u, const State& v) {
			State copy = u;
			copy -= v;
		}

		friend std::ostream& operator<<(std::ostream& out, const State& s) {
			out << y.y[0] << " " << y.y[1] << " " << y.y[2];
			return out;
		}

	private:
		double y[6];
};

#endif // STATE_HPP
