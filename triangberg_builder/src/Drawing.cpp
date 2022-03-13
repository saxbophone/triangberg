/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstddef>

#include <functional>
#include <memory>
#include <vector>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/geometry.hpp>
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

    class Triangle; // forward-declaration
    class Vertex {
    public:
        Vertex(Point position) : _position(position) {}
        // Vertex needs to know about every Triangle that uses it
        void add_triangle(std::shared_ptr<Triangle> triangle) {
            if (triangle) {
                this->_triangles.push_back(triangle);
            }
        }

        Point get_position() const {
            return this->_position;
        }

    private:
        Point _position;
        std::vector<std::weak_ptr<Triangle>> _triangles;
    };

    class Triangle : public std::enable_shared_from_this<Triangle> {
    public:
        // ctor for initial triangle
        Triangle(std::size_t id, Point centre, Degrees orientation, Unit size) : _id(id) {
            // subtend a vertical upwards-pointing line around centre point
            std::shared_ptr<Vertex> first_vertex = std::make_shared<Vertex>(
                subtend_point_from_vector(
                    centre, {0, -size},
                    degrees_to_radians(orientation)
                )
            );
            this->_vertices.push_back(first_vertex);
            // subtend another two lines at intervals of 120°
            for (std::size_t i = 1; i < 3; i++) {
                std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>(
                    subtend_point_from_vector(
                        centre, {0, -size},
                        degrees_to_radians(orientation + 120 * i)
                    )
                );
                this->_vertices.push_back(vertex);
            }
        }
        // ctor for first triangle added (with vertex bound to a point on edge of first triangle)
        Triangle(std::size_t id, Point first_point, Vector first_edge) : _id(id) {
            // first vertex is given for us
            std::shared_ptr<Vertex> first_vertex = std::make_shared<Vertex>(first_point);
            this->_vertices.push_back(first_vertex);
            // for the second vertex we just follow the vector of the first edge from the first vertex
            Point second_point = {
                first_point.x + first_edge.x, first_point.y + first_edge.y
            };
            std::shared_ptr<Vertex> second_vertex = std::make_shared<Vertex>(second_point);
            this->_vertices.push_back(second_vertex);
            // for the final vertex we need to subtend the first_edge by 60° around the first_point
            std::shared_ptr<Vertex> third_vertex = std::make_shared<Vertex>(
                subtend_point_from_vector(
                    first_point, first_edge,
                    degrees_to_radians(60) // TODO: vary sign for collisions
                )
            );
            this->_vertices.push_back(third_vertex);
        }
        // ctor used for all other triangles, whose vertices are shared with existing triangles
        Triangle(std::size_t id, std::shared_ptr<Vertex> first, std::shared_ptr<Vertex> second) : _id(id) {
            // add the first two vertices
            this->_vertices.insert(this->_vertices.end(), {first, second});
            // work out what the vector of the first edge is
            Point first_point = first->get_position();
            Point second_point = second->get_position();
            Vector first_edge = {
                second_point.x - first_point.x, second_point.y - first_point.y
            };
            // subtend this vector about the first point to find the third point
            std::shared_ptr<Vertex> third_vertex = std::make_shared<Vertex>(
                subtend_point_from_vector(
                    first_point, first_edge,
                    degrees_to_radians(60) // TODO: vary sign for collisions
                )
            );
            this->_vertices.push_back(third_vertex);
        }
        // needed to update references to this Triangle from its Vertices (can't be done in ctor)
        void update_references() {
            for (auto& vertex : this->_vertices) {
                vertex->add_triangle(shared_from_this());
            }
        }
        // returns the Shape of this Triangle
        Drawing::Shape get_shape() const {
            Drawing::Shape shape;
            for (auto vertex : this->_vertices) {
                shape.push_back(vertex->get_position());
            }
            return shape;
        }
        std::shared_ptr<Vertex> get_vertex(std::size_t id) {
            return this->_vertices[id];
        }

    private:
        std::size_t _id; // tracking identity explicitly is better than pointers
        std::vector<std::shared_ptr<Vertex>> _vertices;
    };
}

#include <iostream>

namespace com::saxbophone::triangberg {
    class Drawing::Builder {};

    Drawing::Drawing(
        Point origin,
        Unit size,
        Degrees rotation,
        EdgeID branch_edge,
        Percentage branch_point,
        Degrees branch_angle
    ) : _stage(0)
      , _builder(new Builder())
      {
        std::shared_ptr<Triangle> t = std::make_shared<Triangle>(0, origin, rotation, size);
        t->update_references();
        for (auto point : t->get_shape()) {
            std::cout << point.x << ", " << point.y << std::endl;
        }
        std::shared_ptr<Triangle> u = std::make_shared<Triangle>(1, Point{400, 305}, Vector{10, 10});
        u->update_references();
        for (auto point : u->get_shape()) {
            std::cout << point.x << ", " << point.y << std::endl;
        }
        std::shared_ptr<Triangle> v = std::make_shared<Triangle>(2, t->get_vertex(1), u->get_vertex(1));
        v->update_references();
        for (auto point : v->get_shape()) {
            std::cout << point.x << ", " << point.y << std::endl;
        }
    }

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
