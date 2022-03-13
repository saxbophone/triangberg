/**
 * @file
 * Miscellaneous core geometry helper functions.
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

#ifndef COM_SAXBOPHONE_TRIANGBERG_GEOMETRY_HPP
#define COM_SAXBOPHONE_TRIANGBERG_GEOMETRY_HPP

#include <vector>

#include <triangberg_builder/types.hpp>

namespace com::saxbophone::triangberg {
    /**
     * @param d angle given in Degrees
     * @returns angle converted to Radians
     */
    Radians degrees_to_radians(Degrees d);

    /**
     * @param r angle given in Radians
     * @returns angle converted to Degrees
     * @warning Unimplemented
     */
    Degrees radians_to_degrees(Radians r);

    /**
     * @param a Vector to use as reference point
     * @param b Vector to measure the angle of
     * @returns angle of Vector b using angle of Vector a as a reference point
     */
    Radians angle_between(Vector a, Vector b);

    /**
     * @returns true if Lines a and b are intersecting
     */
    bool are_intersecting(Line a, Line b);

    /**
     * @params points vector of Points defining a polygon to test
     * @warning points must define a polygon in a clockwise or anticlockwise
     * order
     * @returns true if the shape defined by the points is concave
     */
    bool is_concave(std::vector<Point> points);
}

#endif // include guard
