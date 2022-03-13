/*
 * This is a sample private compilation unit.
 *
 * <Copyright information goes here>
 */

#include <cmath>
#include <cstddef>

#include <vector>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/geometry.hpp>

// are_intersecting implementation
namespace {
    using namespace com::saxbophone::triangberg;

    // derives angle of Line
    Radians get_angle(Line l) {
        Point delta = {
            l.destination.x - l.origin.x, l.destination.y - l.origin.y,
        };
        return std::atan2(delta.y, delta.x);
    }

    // modifies x and y in-place
    void rotate_point(Unit& x, Unit& y, Radians theta) {
        Unit old_x = x, old_y = y;
        // long unwinded form of matrix rotation
        x = old_x * std::cos(theta) - old_y * std::sin(theta);
        y = old_x * std::sin(theta) + old_y * std::cos(theta);
    }

    // modifies l in-place
    void rotate(Line& l, Radians theta) {
        rotate_point(l.origin.x, l.origin.y, theta);
        rotate_point(l.destination.x, l.destination.y, theta);
    }

    // modifies l in-place
    void translate_y(Line& l, Unit y_delta) {
        l.origin.y += y_delta;
        l.destination.y += y_delta;
    }

    // does l intersect i?
    bool lines_intersect(Line l, Line i) {
        // get the angle of i
        Radians theta = get_angle(i);
        // rotate both l and i by the inverse of this angle
        // this will make i normal with the x-axis
        rotate(l, -theta);
        rotate(i, -theta);
        // both ends of i should have same y coörd
        // translate both up or down the y-axis so i is level with the x-axis
        Unit delta_y = i.origin.y;
        translate_y(l, -delta_y);
        translate_y(i, -delta_y);
        // i should now run along the x-axis
        // l intersects i if one of it's y coörds is below zero and the other above
        return (l.origin.y < 0 and l.destination.y > 0) or (l.origin.y > 0 and l.destination.y < 0);
    }
}

namespace com::saxbophone::triangberg {
    Radians degrees_to_radians(Degrees) { return {}; }

    Degrees radians_to_degrees(Radians) { return {}; }

    Radians angle_between(Vector, Vector) { return {}; }

    bool are_intersecting(Line a, Line b) {
        // test both if a intersects b and b intersects a
        // --this is needed because only comparing from one POV gets false positives
        // with "near misses"
        return lines_intersect(a, b) and lines_intersect(b, a);
    }

    bool is_concave(std::vector<Point> points) {
        bool sign = false;
        for (std::size_t i = 0; i < points.size(); i++) {
            Point start = points[i];
            Point middle = points[(i + 1) % points.size()];
            Point end = points[(i + 2) % points.size()];
            // Calculating the angular normal from three points
            // https://en.wikipedia.org/wiki/Cross_product#Computational_geometry
            Unit angular_normal =
                (middle.x - start.x) * (end.y - start.y) -
                (middle.y - start.y) * (end.x - start.x);
            // the sign determines whether it's a CW or ACW bearing
            if (i == 0) {
                // for first-run, set sign
                sign = std::signbit(angular_normal);
            } else {
                // compare sign for consistency
                if (std::signbit(angular_normal) != sign) {
                    return true; // not all angles have same sign: shape is concave
                }
            }
        }
        return false; // all angles have same sign: shape is convex
    }
}
