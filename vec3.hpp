#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <array>
#include <ostream>

// A 3D vector class
class vec3 {
	public:
		constexpr vec3() noexcept : m_e{0, 0, 0} {}
		constexpr vec3(double e0, double e1, double e2) noexcept : m_e{e0, e1, e2} {}

		// This constructor is required to make the position and momentum vec3's from
		// the pointers to elements in the State class. I.e., the getPosition() and
		// getMomentum() functions use this constructor.
		constexpr vec3(const double* array) noexcept : m_e{array[0], array[1], array[2]} {}

		// This constructor is required for the implicit conversion of a std::array<double, 3>
		// to a vec3. This is useful in performing implicit conversions from return type of the
		// E and B fields into a vec3 automatically (i.e. without an explicit type cast).
		constexpr vec3(const std::array<double, 3> arr) noexcept : m_e{arr[0], arr[1], arr[2]} {}

		constexpr double x() const {
			return m_e[0];
		}

		constexpr double y() const {
			return m_e[1];
		}

		constexpr double z() const {
			return m_e[2];
		}

		constexpr void toArray(double* out) const {
			// Writes the contents of the vec3 into an array of doubles.
			// Undefined behavior if out is too small, i.e. if the length of out
			// is less than 3.
			for (auto i = 0; i < 3; ++i) {
				out[i] = m_e[i];
			}
		}

		// Arithmetic operator overloads
		constexpr vec3 operator-() const {
			return {-m_e[0], -m_e[1], -m_e[2]};
		}

		constexpr const double& operator[](int i) const {
			return m_e[i];
		}

		constexpr double& operator[](int i) {
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

		friend constexpr vec3& operator*=(vec3& v, const double t) {
			v.m_e[0] *= t;
			v.m_e[1] *= t;
			v.m_e[2] *= t;
			return v;
		}

		friend constexpr vec3& operator*=(const double t, vec3& v) {
			return v *= t;
		}

		friend constexpr vec3& operator/=(vec3& v, const double t) {
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

		friend constexpr vec3 operator*(const vec3& v, const double t) {
			vec3 copy = v;
			copy *= t;
			return copy;
		}

		friend constexpr vec3 operator*(const double t, const vec3& v) {
			vec3 copy = v;
			copy *= t;
			return copy;
		}

		friend constexpr vec3 operator/(const vec3& v, const double t) {
			vec3 copy = v;
			copy /= t;
			return copy;
		}

		// The << operator was also overloaded to help with debugging purposes.
		friend std::ostream& operator<<(std::ostream& out, const vec3& v) {
			out << v.m_e[0] << " " << v.m_e[1] << " " << v.m_e[2];
			return out;
		}

		// Standard vector operations
		constexpr static double dot(const vec3& u, const vec3& v) {
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

		constexpr double length() const {
			return std::sqrt(m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2]);
		}
		
		constexpr double lengthSquared() const {
			return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
		}

	private:
		double m_e[3];
};

#endif // VEC3_HPP
