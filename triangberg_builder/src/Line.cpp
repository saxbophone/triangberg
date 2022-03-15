/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <triangberg_builder/Line.hpp>
#include <triangberg_builder/Vector.hpp>

namespace com::saxbophone::triangberg {
    // converting Line to Vector gets the Vector delta between start and end
    Line::operator Vector() const {
        return destination - origin;
    }
}
