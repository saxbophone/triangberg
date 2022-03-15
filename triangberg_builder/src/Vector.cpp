/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cmath>

#include <triangberg_builder/Point.hpp>
#include <triangberg_builder/Vector.hpp>

namespace com::saxbophone::triangberg {
    // conversion operator to Point
    Vector::operator Point() const {
        return {x, y};
    }
    // adding Vectors to Vectors yields sum as Vectors
    Vector Vector::operator+(const Vector& other) const {
        return {x + other.x, y + other.y};
    }
    // likewise for subtraction
    Vector Vector::operator-(const Vector& other) const {
        return {x - other.x, y - other.y};
    }
    // multiplying Vector by scalar multiples the Vector components elementwise
    Vector Vector::operator*(Unit scalar) const {
        return {x * scalar, y * scalar};
    }
    // get length of the vector (aka magnitude)
    Unit Vector::length() const {
        return std::sqrt(x*x + y*y);
    }
}
