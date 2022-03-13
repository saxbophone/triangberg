/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstddef>

#include <functional>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/Drawing.hpp>

namespace {
    using namespace com::saxbophone::triangberg;

    const Point A = { 7, 10},
                B = { 9,  7},
                C = { 5,  7},
                D = { 8,  7},
                E = {10,  4},
                F = { 6,  4},
                G = {13,  6},
                H = { 3,  5},
                J = {14, 14},
                K = { 1, 11},
                M = { 6, 20};
}

namespace com::saxbophone::triangberg {
    class Drawing::Builder {};

    Drawing::Drawing(Point, Unit, Degrees, EdgeID, Percentage, Degrees)
      : _stage(0)
      , _builder(new Builder())
      {}

    Drawing::~Drawing() = default;

    bool Drawing::is_complete() const {
        return this->_stage == 6;
    }

    void Drawing::add_triangle(std::function<std::size_t(std::size_t)>) {
        if (this->_stage < 6) {
            this->_stage++;
        }
    }

    Drawing::Shapes Drawing::get_shapes() const {
        // NOTE: this dummy implementation is hard-coded to test out rendering code
        switch (this->_stage) {
            case 0:
                return {
                    {A, B, C,}, {{A, B, C},}
                };
            case 1:
                return {
                    {A, B, D, E, F, D, C,}, {{A, B, C}, {D, E, F},}
                };
            case 2:
                return {
                    {A, B, G, E, F, D, C,}, {{A, B, C}, {D, E, F}, {B, G, E},}
                };
            case 3:
                return {
                    {A, B, G, E, F, H, C,},
                    {{A, B, C}, {D, E, F}, {B, G, E}, {C, F, H},}
                };
            case 4:
                return {
                    {A, J, G, E, F, H, C,},
                    {{A, B, C}, {D, E, F}, {B, G, E}, {C, F, H}, {A, J, G},}
                };
            case 5:
                return {
                    {A, J, G, E, F, H, K,},
                    {
                        {A, B, C}, {D, E, F}, {B, G, E}, {C, F, H}, {A, J, G},
                        {K, A, H},
                    }
                };
            case 6:
            default:
                return {
                    {M, J, G, E, F, H, K,},
                    {
                        {A, B, C}, {D, E, F}, {B, G, E}, {C, F, H}, {A, J, G},
                        {K, A, H}, {M, J, K},
                    }
                };
        }
    }
}
