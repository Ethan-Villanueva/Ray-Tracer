#pragma once

#include <array>
#include <cmath>
#include <iostream>

class vec3 {
    std::array<double, 3> coordinates;

    public:
        vec3() : coordinates({0, 0, 0}) {}
        vec3(double x, double y, double z) : coordinates({x, y, z}) {}

        double getX() const { return coordinates.at(0); }
        double getY() const { return coordinates.at(1); }
        double getZ() const { return coordinates.at(2); }

        // Utility functions
        double length() const {
            return std::sqrt(length_squared());
        }
        double length_squared() const {
            return coordinates.at(0)*coordinates.at(0)
                 + coordinates.at(1)*coordinates.at(1)
                 + coordinates.at(2)*coordinates.at(2);
        }

    private:
        // Operator overloads
        vec3 operator-() const {
            return vec3(-coordinates.at(0), -coordinates.at(1), -coordinates.at(2));
        }

        double operator[](int index) const{
            return coordinates.at(index);
        }
        double& operator[](int index){
            return coordinates.at(index);
        }

        vec3& operator+=(const vec3 &v) {
            coordinates.at(0) += v.coordinates.at(0);
            coordinates.at(1) += v.coordinates.at(1);
            coordinates.at(2) += v.coordinates.at(2);
            return *this;
        }
        vec3& operator*=(const double t) {
            coordinates.at(0) *= t;
            coordinates.at(1) *= t;
            coordinates.at(2) *= t;
            return *this;
        }
        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }
};

// Stream output
std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.getX() << " " << v.getY() << " " << v.getZ();
}

// Binary operators
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.getX() + v.getX(),
                u.getY() + v.getY(),
                u.getZ() + v.getZ());
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.getX() - v.getX(),
                u.getY() - v.getY(),
                u.getZ() - v.getZ());
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.getX() * v.getX(),
                u.getY() * v.getY(),
                u.getZ() * v.getZ());
}

// Scalar operators
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.getX(),
                t * v.getY(),
                t * v.getZ());
}
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3 &v, double t) {
    return (1/t) * v;
}

// Vector maths
inline double dot(const vec3 &u, const vec3 &v) {
    return u.getX() * v.getX()
         + u.getY() * v.getY()
         + u.getZ() * v.getZ();
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.getY() * v.getZ() - u.getZ() * v.getY(),
                u.getX() * v.getZ() + u.getZ() * v.getX(),
                u.getX() * v.getY() - u.getY() * v.getX());
}

inline vec3 unit_vector(const vec3 &v) {
    return v / v.length();
}

using point3 = vec3;
using colour = vec3;