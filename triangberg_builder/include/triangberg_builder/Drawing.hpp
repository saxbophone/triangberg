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

#ifndef COM_SAXBOPHONE_TRIANGBERG_DRAWING_HPP
#define COM_SAXBOPHONE_TRIANGBERG_DRAWING_HPP

#include <cstddef>

#include <functional>
#include <memory>
#include <vector>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/Point.hpp>

namespace com::saxbophone::triangberg {
    /**
     * @brief Main class for constructing Triangberg Drawings
     */
    class Drawing {
    public:
        typedef std::vector<Point> Shape;

        /**
         * @brief The shapes that make up the visual representation of this
         * Drawing
         */
        struct Shapes {
            Shape silhouette; // the outline of the entire drawing
            std::vector<Shape> triangles; // all the triangles in the drawing
        };

        /**
         * @brief Constructs new Drawing object with given parameters
         * @param origin x/y centre of initial triangle in the drawing
         * @param size size of initial triangle, measured as distance from
         * vertices to centre
         * @param rotation orientation of initial triangle, measured as angle
         * difference between flat base of the triangle and the x-axis
         * @param branch_edge which edge of the initial triangle to branch off
         * the next triangle from
         * @param branch_point how far along the chosen edge to place the branch
         * point
         * @note branch_point range is `0% < x < 100%`
         * @param branch_angle angle between branch edge and first edge of new
         * triangle
         * @note branch_angle range is `0° < x < 120°`
         * @todo Add branch size (size of branched triangle) --missed out.
         */
        Drawing(
            Point origin,
            Unit size,
            Degrees rotation,
            EdgeID branch_edge,
            Percentage branch_point,
            Degrees branch_angle
        );

        ~Drawing();

        /**
         * @returns whether this Drawing is complete (i.e. no more triangles
         * can be added to it)
         */
        bool is_complete() const;

        /**
         * @brief Adds a new triangle to the Drawing in a suitable place,
         * assuming the drawing is not complete.
         * @param segment_picker callback function used to allow specifying
         * which segment to pick, when there are more than one suitable segments
         * from which to add the new triangle. This callback is passed the
         * number of valid segments to pick from, and should return an index
         * in the range `0..n-1` for the segment it has picked.
         */
        void add_triangle(
            std::function<std::size_t(std::size_t)> segment_picker
        );

        /**
         * @returns All the shapes that make up the drawing in its current state
         * @note This information can be used directly to draw a 2D visual of
         * this Drawing.
         */
        Shapes get_shapes() const;

    private:
        class Builder; // forward-declaration of helper class for implementation
        std::unique_ptr<Builder> _builder;
        bool _started;
        bool _can_add_more;
    };
}

#endif // include guard
