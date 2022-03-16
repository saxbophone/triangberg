/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cmath>
#include <cstddef>

#include <functional>
#include <memory>
#include <set>
#include <vector>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/geometry.hpp>
#include <triangberg_builder/Drawing.hpp>
#include <triangberg_builder/Line.hpp>
#include <triangberg_builder/Point.hpp>
#include <triangberg_builder/Vector.hpp>

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
        Vertex(Point position) : Vertex(position, true) {}

        Vertex(Point position, bool eligible) : _position(position), _eligible(eligible) {}

        ~Vertex();
        // Vertex needs to know about every Triangle that uses it
        void add_triangle(std::shared_ptr<Triangle> triangle);

        Point get_position() const {
            return this->_position;
        }

        bool is_eligible() const {
            return this->_eligible;
        }

        std::size_t connected_triangles_count() const {
            std::size_t count = 0;
            // every non-null triangle pointer counts
            for (const auto& t : this->_triangles) {
                if (!t.expired()) {
                    count++;
                }
            }
            return count;
        }

        // returns true if these two vertices share any common triangles
        bool common_to(const Vertex& other) const {
            for (const auto& t : this->_triangles) {
                if (other._triangles.contains(t)) {
                    return true;
                }
            }
            return false;
        }

    private:
        Point _position;
        std::set<std::weak_ptr<Triangle>, std::owner_less<std::weak_ptr<Triangle>>> _triangles;
        bool _eligible;
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
            // NOTE: this Vertex is not eligible for starting any more new Triangles
            std::shared_ptr<Vertex> first_vertex = std::make_shared<Vertex>(first_point, false);
            this->_vertices.push_back(first_vertex);
            // for the second vertex we just follow the vector of the first edge from the first vertex
            Point second_point = first_point + first_edge;
            std::shared_ptr<Vertex> second_vertex = std::make_shared<Vertex>(second_point);
            this->_vertices.push_back(second_vertex);
            // for the final vertex we need to subtend the first_edge by 60° around the first_point
            std::shared_ptr<Vertex> third_vertex = std::make_shared<Vertex>(
                subtend_point_from_vector(
                    first_point, first_edge,
                    degrees_to_radians(60)
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
            Vector first_edge = second_point - first_point;
            // subtend this vector about the first point to find the third point
            std::shared_ptr<Vertex> third_vertex = std::make_shared<Vertex>(
                subtend_point_from_vector(
                    first_point, first_edge,
                    degrees_to_radians(60)
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
        Line get_edge(std::size_t id) {
            return Line {
                get_vertex(id)->get_position(),
                get_vertex((id + 1) % this->_vertices.size())->get_position()
            };
        }
        std::size_t get_id() const {
            return this->_id;
        }
        // determines whether this Triangle intersects with any in the given vector
        bool intersects_with(std::vector<std::shared_ptr<Triangle>> others) {
            for (auto t : others) {
                for (std::size_t i = 0; i < 3; i++) {
                    for (std::size_t j = 0; j < 3; j++) {
                        Line our_edge = this->get_edge(i);
                        Line their_edge = t->get_edge(j);
                        // XXX: the intersection code is a bit false-positivy
                        // XXX: skip checking intersections when a vertice is shared
                        if (
                            our_edge.origin == their_edge.origin or
                            our_edge.origin == their_edge.destination or
                            our_edge.destination == their_edge.origin or
                            our_edge.destination == their_edge.destination
                        ) {
                            continue;
                        }
                        if (are_intersecting(our_edge, their_edge)) {
                            // std::cout << "INTERSECT " << t->get_id() << " : (" << our_edge.origin.x << ", "
                            // << our_edge.origin.y << ")->(" << our_edge.destination.x << ", "
                            // << our_edge.destination.y << ") with (" << their_edge.origin.x << ", "
                            // << their_edge.origin.y << ")->(" << their_edge.destination.x << ", "
                            // << their_edge.destination.y << ")" << std::endl;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

    private:
        std::size_t _id; // tracking identity explicitly is better than pointers
        std::vector<std::shared_ptr<Vertex>> _vertices;
    };

    Vertex::~Vertex() = default;

    // implemented out-of-class to avoid error due to incomplete type Triangle
    void Vertex::add_triangle(std::shared_ptr<Triangle> triangle) {
        if (triangle) {
            this->_triangles.insert(triangle);
        }
    }
}

namespace com::saxbophone::triangberg {
    class Drawing::Builder {
    public:
        Builder(
            Point origin,
            Unit size,
            Degrees rotation,
            EdgeID branch_edge,
            Percentage branch_point,
            Degrees branch_angle,
            Vector screen_size
        )
          : _triangles{std::make_shared<Triangle>(0, origin, rotation, size)}
          , _branch_edge(branch_edge)
          , _branch_point(branch_point)
          , _branch_angle(branch_angle)
          , _screen_size(screen_size)
          {
            this->_triangles.back()->update_references();
        }

        void add_second_triangle(Unit size) {
            Line branch_line = this->_triangles[0]->get_edge(this->_branch_edge);
            // get the vector of the line
            Vector line_vector = branch_line;
            // create a scaled version of that vector
            Vector scaled_vector = line_vector * this->_branch_point;
            // add this to the origin to get the position of new triangle's first vertex
            Point first_point = branch_line.origin + scaled_vector;
            // oh my..! this is way more involved than anticipated:
            // we need to:
            // - get a vector running opposite to that of the branch edge
            Vector opposite = line_vector * -1;
            // - scale it up to the same size as needed for the new triangle
            // scale for actual size as in triangle diameter
            // the radius of the triangle is the circumscribed circle radius (R)
            // R = a / sqrt(3)
            // a = sqrt(3) * R
            // with a as side length
            Unit scale = (std::sqrt(3) * size) / opposite.length();
            Vector scaled_opposite = opposite * scale;
            // - rotate it so it has the requested angle between it and branch edge
            Point branching_edge = subtend_point_from_vector(
                first_point, scaled_opposite, degrees_to_radians(this->_branch_angle)
            );
            // only then can we make the second triangle by passing it the branch point
            // the vector that describes the first edge
            this->_triangles.push_back(
                std::make_shared<Triangle>(1, first_point, branching_edge - first_point)
            );
            this->_triangles.back()->update_references();
        }

        bool add_next_triangle() {
            auto next_triangles = this->get_possible_next_triangles();
            if (next_triangles.size() > 0) {
                this->_triangles.push_back(next_triangles.back());
                this->_triangles.back()->update_references();
                return true;
            }
            return false;
        }

        std::vector<Shape> get_shapes() const {
            std::vector<Shape> shapes;
            for (const auto& t : this->_triangles) {
                shapes.push_back(t->get_shape());
            }
            return shapes;
        }

        // returns a vector of all possible new Triangles we could place
        std::vector<std::shared_ptr<Triangle>> get_possible_next_triangles() {
            std::vector<std::shared_ptr<Triangle>> candidates;
            // rules:
            // - vertices must be from different triangles
            // - ignore ineligible vertices
            // - resulting triangle must not intersect any other (not yet implemented)
            for (std::size_t i = 0; i < this->_triangles.size(); i++) {
                for (std::size_t j = 0; j < this->_triangles.size(); j++) {
                    // if (i == j) {
                    //     continue;
                    // }
                    for (std::size_t iv = 0; iv < 3; iv++) {
                        std::shared_ptr<Vertex> i_vertex = this->_triangles[i]->get_vertex(iv);
                        // skip early if vertex ineligible
                        if (not i_vertex->is_eligible()) {
                            continue;
                        }
                        for (std::size_t jv = 0; jv < 3; jv++) {
                            std::shared_ptr<Vertex> j_vertex = this->_triangles[j]->get_vertex(jv);
                            // skip early if vertex ineligible
                            if (not j_vertex->is_eligible()) {
                                continue;
                            }
                            // skip when it's the same triangle
                            if (i_vertex->common_to(*j_vertex)) {
                                continue;
                            }
                            // skip vertex-pairs where neither have only one triangle
                            // if (i_vertex->connected_triangles_count() == 1 or j_vertex->connected_triangles_count() == 1) {
                                // make the candidate Triangle
                                std::shared_ptr<Triangle> candidate = std::make_shared<Triangle>(
                                    this->_triangles.size(), i_vertex, j_vertex
                                );
                                // check that at least one of the candidate's vertices is on-screen
                                std::size_t off_screen = 0;
                                for (const auto& vertex : candidate->get_shape()) {
                                    if (
                                        vertex.x < 0 or
                                        vertex.x > this->_screen_size.x or
                                        vertex.y < 0 or
                                        vertex.y > this->_screen_size.y
                                    ) {
                                        off_screen++;
                                    }
                                }
                                // TODO: OR that at least one of its edges intersects the screen bounds
                                if (off_screen < 3) {
                                    // add it if it doesn't intersect any of the others
                                    if (not candidate->intersects_with(this->_triangles)) {
                                        // XXX: here, we *would* check if the resultant triangle would overlap
                                        // instead, we'll just assume that it doesn't for now...
                                        candidates.push_back(candidate);
                                    }
                                }
                            // }
                        }
                    }
                }
            }
            return candidates;
        }

    private:
        std::vector<std::shared_ptr<Triangle>> _triangles;
        EdgeID _branch_edge;
        Percentage _branch_point;
        Degrees _branch_angle;
        Vector _screen_size;
    };

    Drawing::Drawing(
        Point origin,
        Unit size,
        Degrees rotation,
        EdgeID branch_edge,
        Percentage branch_point,
        Degrees branch_angle,
        Vector screen_size
    ) : _builder(
            new Builder(
                origin,
                size,
                rotation,
                branch_edge,
                branch_point,
                branch_angle,
                screen_size
            )
        )
      , _started(false)
      , _can_add_more(true)
      {}

    Drawing::~Drawing() = default;

    bool Drawing::is_complete() const {
        return not this->_can_add_more;
    }

    void Drawing::add_triangle(std::function<std::size_t(std::size_t)>) {
        if (not this->_started) {
            // add second triangle at an angle and partway on an edge
            this->_builder->add_second_triangle(10);
            this->_started = true;
        } else if (this->_can_add_more) {
            this->_can_add_more = this->_builder->add_next_triangle();
        }
    }

    Drawing::Shapes Drawing::get_shapes() const {
        return {
            {}, this->_builder->get_shapes()
        };
    }
}
