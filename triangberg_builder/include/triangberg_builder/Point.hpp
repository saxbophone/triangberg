/**
 * @file
 * This is a sample public compilation unit.
 *
 * @author Your Name <your.email.address@goes.here>
 * @date Creation/Edit Date
 *
 * @copyright Copyright information goes here
 *
 * @copyright
 * Copyright information can span multiple paragraphs if needed, such as if you
 * use a well-known software license for which license header text (to be
 * placed in locations like these) are provided by the license custodians.
 *
 */

#ifndef COM_SAXBOPHONE_TRIANGBERG_POINT_HPP
#define COM_SAXBOPHONE_TRIANGBERG_POINT_HPP

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/Vector.hpp>

namespace com::saxbophone::triangberg {
    struct Point {
        Unit x;
        Unit y;
        // defaulted equality operator
        bool operator==(const Point&) const = default;
        // conversion operator to Vector
        operator Vector() const;
        // subtracting another Point from this one yields their delta as a Vector
        Vector operator-(const Point& other) const;
        // adding a Vector to this Point yields a Point translated along the Vector from this one
        Point operator+(const Vector& delta) const;
        // likewise for subtracting a vector
        Point operator-(const Vector& delta) const;
    };
}

#endif // include guard
