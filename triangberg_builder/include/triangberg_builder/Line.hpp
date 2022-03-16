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

#ifndef COM_SAXBOPHONE_TRIANGBERG_LINE_HPP
#define COM_SAXBOPHONE_TRIANGBERG_LINE_HPP

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/Point.hpp>
#include <triangberg_builder/Vector.hpp>

namespace com::saxbophone::triangberg {
    struct Line {
        Point origin;
        Point destination;
        // converting Line to Vector gets the Vector delta between start and end
        operator Vector() const;
    };
}

#endif // include guard
