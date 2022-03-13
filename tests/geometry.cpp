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
}
