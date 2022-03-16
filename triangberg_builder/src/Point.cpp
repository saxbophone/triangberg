/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <triangberg_builder/Point.hpp>
#include <triangberg_builder/Vector.hpp>

namespace com::saxbophone::triangberg {
    // conversion operator to Vector
    Point::operator Vector() const {
        return {x, y};
    }
    // subtracting another Point from this one yields their delta as a Vector
    Vector Point::operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }
    // adding a Vector to this Point yields a Point translated along the Vector from this one
    Point Point::operator+(const Vector& delta) const {
        return {x + delta.x, y + delta.y};
    }
    // likewise for subtracting a vector
    Point Point::operator-(const Vector& delta) const {
        return {x - delta.x, y - delta.y};
    }
}
