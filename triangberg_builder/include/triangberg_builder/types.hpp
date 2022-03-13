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

#ifndef COM_SAXBOPHONE_TRIANGBERG_STRUCTS_HPP
#define COM_SAXBOPHONE_TRIANGBERG_STRUCTS_HPP

#include <cstddef>

namespace com::saxbophone::triangberg {
    typedef float Unit;

    typedef Unit Degrees;

    typedef Unit Radians;

    typedef Unit Percentage;

    typedef std::size_t EdgeID;

    struct Point {
        Unit x;
        Unit y;
    };

    typedef Point Vector;

    struct Line {
        Point origin;
        Point destination;
    };
}

#endif // include guard
