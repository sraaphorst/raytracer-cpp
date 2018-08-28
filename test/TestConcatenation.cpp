/**
 * TestConcatenation.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <iterator>
#include <vector>

TEST_CASE("Concatenating two vectors should produce a new vector", "[vector]") {
    std::vector<int> v1{1, 2, 3};
    std::vector<int> v2{4, 5, 6};
    std::vector<int> v1v2{1, 2, 3, 4, 5, 6};

    v1.insert(v1.end(), std::make_move_iterator(v2.begin()), std::make_move_iterator(v2.end()));
    REQUIRE(v1 == v1v2);
}
