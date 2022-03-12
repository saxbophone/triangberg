#include <catch2/catch.hpp>

#include <triangberg_builder/Public.hpp>
#include <triangberg_builder/Drawing.hpp>

using namespace com::saxbophone::triangberg;
// test case to check network construction and train movement
TEST_CASE("Library works") {
    REQUIRE(library_works());
}
