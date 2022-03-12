/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <triangberg/Public.hpp>

#include "Private.hpp"

namespace com::saxbophone::triangberg {
    bool library_works() {
        return PRIVATE::library_works();
    }
}
