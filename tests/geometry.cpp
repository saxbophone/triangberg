#include <algorithm>
#include <vector>

#include <catch2/catch.hpp>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/geometry.hpp>

using namespace com::saxbophone::triangberg;

TEST_CASE("degrees_to_radians", "[geometry]") {
    auto angles = GENERATE(
        table<Degrees, Radians>(
            {
                {   0,  0.0},
                {   1,  0.01745329252},
                {   2,  0.03490658504},
                {   3,  0.05235987756},
                {   5,  0.0872664626},
                {  45,  0.7853981634},
                {  90,  1.570796327},
                { 135,  2.35619449},
                { 180,  3.141592654},
                {  75,  1.308996939},
                { -75, -1.308996939},
                { -45, -0.7853981634},
                { -80, -1.396263402},
                { -20, -0.3490658504},
                {  15,  0.2617993878},
                {-100, -1.745329252},
            }
        )
    );

    CHECK(degrees_to_radians(std::get<0>(angles)) == Approx(std::get<1>(angles)));
}

TEST_CASE("radians_to_degrees", "[geometry][.][unimplemented]") {
}

TEST_CASE("angle_between", "[geometry]") {
    auto vectors = GENERATE(
        table<Vector, Vector, Radians>(
            {
                // some simple vectors, unit-vectors mostly
                {{ 0,  1}, { 0,  1}, 0.0},
                {{ 0,  1}, { 1,  1}, -0.7853981634},
                {{ 0,  1}, { 1,  0}, -1.570796327},
                {{ 0,  1}, { 0, -1}, 3.141592654},
                {{ 0,  1}, {-1,  1}, 0.7853981634},
                {{ 0,  1}, {-1,  0}, 1.570796327},
                {{ 0,  1}, {-1, -1}, 2.35619449},
                {{ 1,  1}, { 1,  1}, 0.0},
                {{ 1,  1}, { 0,  1}, 0.7853981634},
                {{ 1,  1}, { 1,  0}, -0.7853981634},
                {{ 1,  1}, {-1,  0}, 2.35619449},
                {{ 1,  1}, { 0, -1}, -2.35619449},
                {{ 1,  1}, {-1, -1}, 3.141592654},
                {{ 1,  1}, {-1, -2}, -2.819842099},
                {{ 1,  0}, { 1,  1}, 0.7853981634},
                // some more interesting vectors
                {{ 1, -4}, {12, -1}, 1.242676432},
                {{12, -1}, { 1, -4}, -1.242676432},
                {{-5,  6}, {-7,  7}, 0.0906598872},
                {{-7,  7}, {-5,  6}, -0.0906598872},
                {{ 0, 10}, { 1, 15}, -0.06656816378},
                {{ 1, 15}, { 0, 10}, 0.06656816378},
                {{-6, -1}, {16, -1}, 2.914025166},
                {{16, -1}, {-6, -1}, -2.914025166},
            }
        )
    );

    CAPTURE(
        std::get<0>(vectors).x,
        std::get<0>(vectors).y,
        std::get<1>(vectors).x,
        std::get<1>(vectors).y
    );

    CHECK(
        angle_between(std::get<0>(vectors), std::get<1>(vectors))
        == Approx(std::get<2>(vectors))
    );
}

TEST_CASE("are_intersecting", "[geometry]") {
    const Line A = {{ 2, 10}, { 9, 15}};
    const Line B = {{ 6, 14}, { 3,  9}};
    const Line C = {{ 6,  8}, {13, 10}};
    const Line D = {{11,  7}, {10,  9}};
    const Line E = {{ 2,  6}, { 3,  1}};
    const Line F = {{ 1,  5}, { 6,  2}};
    const Line G = {{15, 15}, {14,  0}};
    const Line H = {{14,  2}, {15, 13}};
    const Line J = {{ 8,  3}, {10,  5}};
    const Line K = {{ 8,  5}, {10,  3}};
    const Line M = {{15, 10}, {15,  1}};

    // using record = std::tuple<Line, Line, bool>;

    auto lines = GENERATE_COPY(
        table<Line, Line, bool>(
            {
                {A, B, true},
                {B, A, true},
                {C, D, false},
                {D, C, false},
                {E, F, true},
                {F, E, true},
                {G, H, true},
                {H, G, true},
                {J, K, true},
                {K, J, true},
                {A, M, false},
                {M, A, false},
                {E, C, false},
                {C, E, false},
                {H, B, false},
                {B, H, false},
            }
        )
    );

    CHECK(are_intersecting(std::get<0>(lines), std::get<1>(lines)) == std::get<2>(lines));
}

TEST_CASE("is_concave", "[geometry]") {
    const std::vector<Point> A = {
        {1, 12}, {2, 14}, {4, 13}, {2, 11},
    };
    const std::vector<Point> B = {
        {6, 14}, {8, 13}, {8, 11}, {6, 11}, {5, 12},
    };
    const std::vector<Point> C = {
        {2, 10}, {4, 9}, {2, 7}, {3, 6}, {1, 5},
    };
    const std::vector<Point> D = {
        {2, 4}, {2, 3}, {1, 3},
    };
    const std::vector<Point> E = {
        {5, 9}, {8, 10}, {7, 7}, {8, 4}, {5, 5}, {6, 7},
    };
    const std::vector<Point> F = {
        {8, 7}, {10, 14}, {10, 10}, {11, 12}, {11, 10}, {13, 12}, {10, 8}, {13, 8},
        {9, 6},
    };

    SECTION("convex shapes") {
        auto shape = GENERATE_COPY(values<std::vector<Point>>({A, B, D,}));

        CHECK_FALSE(is_concave(shape));

        std::reverse(shape.begin(), shape.end());

        CHECK_FALSE(is_concave(shape));
    }

    SECTION("concave shapes") {
        auto shape = GENERATE_COPY(values<std::vector<Point>>({C, E, F,}));

        CHECK(is_concave(shape));

        std::reverse(shape.begin(), shape.end());

        CHECK(is_concave(shape));
    }
}
