#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <ostream>

class vec3 {
	public:
		constexpr vec3() noexcept : m_e{0, 0, 0} {}
		constexpr vec3(Float e0, Float e1, Float e2) noexcept : m_e{e0, e1, e2} {}

		constexpr Float x() const {
			return m_e[0];
		}

		constexpr Float y() const {
			return m_e[1];
		}

		constexpr Float z() const {
			return m_e[2];
		}

		constexpr vec3 operator-() const {
			return vec3(-m_e[0], -m_e[1], -m_e[2]);
		}

		constexpr const Float& operator[](int i) const {
			return m_e[i];
		}

		constexpr Float& operator[](int i) {
			return m_e[i];
		}

		friend constexpr vec3& operator+=(vec3& u, const vec3& v) {
			u.m_e[0] += v.m_e[0];
			u.m_e[1] += v.m_e[1];
			u.m_e[2] += v.m_e[2];
			return u;
		}

		friend constexpr vec3& operator-=(vec3& u, const vec3& v) {
			u.m_e[0] -= v.m_e[0];
			u.m_e[1] -= v.m_e[1];
			u.m_e[2] -= v.m_e[2];
			return u;
		}

		friend constexpr vec3& operator*=(vec3& v, const Float t) {
			v.m_e[0] *= t;
			v.m_e[1] *= t;
			v.m_e[2] *= t;
			return v;
		}

		friend constexpr vec3& operator*=(const Float t, vec3& v) {
			return v *= t;
		}

		friend constexpr vec3& operator/=(vec3& v, const Float t) {
			return v *= (1 / t);
		}

		friend constexpr vec3 operator+(const vec3& u, const vec3& v) {
			vec3 copy = u;
			copy += v;
			return copy;
		}

		friend constexpr vec3 operator-(const vec3& u, const vec3& v) {
			vec3 copy = u;
			copy -= v;
			return copy;
		}

		friend constexpr vec3 operator*(const vec3& v, const Float t) {
			vec3 copy = v;
			copy *= t;
			return copy;
		}

		friend constexpr vec3 operator*(const Float t, const vec3& v) {
			vec3 copy = v;
			copy *= t;
			return copy;
		}

		friend constexpr vec3 operator/(const vec3& v, const Float t) {
			vec3 copy = v;
			copy /= t;
			return copy;
		}

		constexpr static Float dot(const vec3& u, const vec3& v) {
			return u.m_e[0] * v.m_e[0]
				 + u.m_e[1] * v.m_e[1]
				 + u.m_e[2] * v.m_e[2];
		}

		constexpr static vec3 cross(const vec3& u, const vec3& v) {
			return vec3(u.m_e[1] * v.m_e[2] - u.m_e[2] * v.m_e[1],
						u.m_e[2] * v.m_e[0] - u.m_e[0] * v.m_e[2],
						u.m_e[0] * v.m_e[1] - u.m_e[1] * v.m_e[0]);
		}

		constexpr static vec3 unitVector(vec3 v) {
			return v / v.length();
		}

		constexpr Float length() const {
			return std::sqrt(m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2]);
		}

		friend std::ostream& operator<<(std::ostream& out, const vec3& v) {
			out << v.m_e[0] << " " << v.m_e[1] << " " << v.m_e[2];
			return out;
		}

	private:
		Float m_e[3];
};

#endif // VEC3_HPP
