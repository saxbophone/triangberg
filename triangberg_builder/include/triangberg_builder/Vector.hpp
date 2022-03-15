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

#ifndef COM_SAXBOPHONE_TRIANGBERG_VECTOR_HPP
#define COM_SAXBOPHONE_TRIANGBERG_VECTOR_HPP

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/Point.hpp>

namespace com::saxbophone::triangberg {
    struct Vector {
        Unit x;
        Unit y;
        // conversion operator to Point
        operator Point() const;
        // adding Vectors to Vectors yields sum as Vectors
        Vector operator+(const Vector& other) const;
        // likewise for subtraction
        Vector operator-(const Vector& other) const;
        // multiplying Vector by scalar multiples the Vector components elementwise
        Vector operator*(Unit scalar) const;
    };
}

#endif // include guard
