#include <algorithm>
#include <vector>

#include <catch2/catch.hpp>

#include <triangberg_builder/types.hpp>
#include <triangberg_builder/geometry.hpp>

using namespace com::saxbophone::triangberg;

TEST_CASE("degrees_to_radians", "[geometry]") {
}

TEST_CASE("radians_to_degrees", "[geometry]") {
}

TEST_CASE("angle_between", "[geometry]") {
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
