#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:
        vec3() : e{0,0,0} {}
        vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

        float x() const { return e.s[0]; }
        float y() const { return e.s[1]; }
        float z() const { return e.s[2]; }

        vec3 operator-() const { return vec3(-e.s[0], -e.s[1], -e.s[2]); }
        float operator[](int i) const { return e.s[i]; }
        float& operator[](int i) { return e.s[i]; }

        vec3& operator+=(const vec3 &v) {
            e.s[0] += v.e.s[0];
            e.s[1] += v.e.s[1];
            e.s[2] += v.e.s[2];
            return *this;
        }

        vec3& operator*=(const float t) {
            e.s[0] *= t;
            e.s[1] *= t;
            e.s[2] *= t;
            return *this;
        }

        vec3& operator/=(const float t) {
            return *this *= 1/t;
        }

        float length() const {
            return sqrt(length_squared());
        }

        float length_squared() const {
            return e.s[0]*e.s[0] + e.s[1]*e.s[1] + e.s[2]*e.s[2];
        }

    public:
        cl_float3 e;
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e.s[0] << ' ' << v.e.s[1] << ' ' << v.e.s[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e.s[0] + v.e.s[0], u.e.s[1] + v.e.s[1], u.e.s[2] + v.e.s[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e.s[0] - v.e.s[0], u.e.s[1] - v.e.s[1], u.e.s[2] - v.e.s[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e.s[0] * v.e.s[0], u.e.s[1] * v.e.s[1], u.e.s[2] * v.e.s[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t*v.e.s[0], t*v.e.s[1], t*v.e.s[2]);
}

inline vec3 operator*(const vec3 &v, float t) {
    return t * v;
}

inline vec3 operator/(vec3 v, float t) {
    return (1/t) * v;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u.e.s[0] * v.e.s[0]
         + u.e.s[1] * v.e.s[1]
         + u.e.s[2] * v.e.s[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e.s[1] * v.e.s[2] - u.e.s[2] * v.e.s[1],
                u.e.s[2] * v.e.s[0] - u.e.s[0] * v.e.s[2],
                u.e.s[0] * v.e.s[1] - u.e.s[1] * v.e.s[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float pi = 3.1415926535897932385f;

// Utility Functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

#endif