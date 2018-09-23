/**
 * TestVariantOps.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <variant>

#include "variant_ops.h"

/**
 * These classes are a bare-bones simulation of what this exercise is trying to achieve, i.e. compile-time
 * pseudostatic polymorphism. We have a number of classes with no inheritance relation but with a method in
 * common. We can build them into an std:array of std::variant, and then invoke their common function across
 * all members of the array to obtain the results.
 *
 * Had we used inheritance, we would require the common method (val) to be virtual, in which case, we lose the
 * ability to use the results of this method during compile-time due to needing vtable lookups.
 *
 * This will be important with regards to intersections in the compile-time ray tracer, which need to store the shape
 * with which the intersection happens. Thus, Intersection will be a template based on shape.
 */
struct S1 {
    constexpr int val() const noexcept {
        return 1;
    }
};

struct S2 {
    constexpr int val() const noexcept {
        return 2;
    }
};

struct S3 {
    constexpr int val() const noexcept {
        return 3;
    }
};

struct S4 {
    constexpr int val() const noexcept {
        return 4;
    }
};

struct S5 {
    constexpr int val() const noexcept {
        return 5;
    }
};

struct S6 {
    constexpr int val() const noexcept {
        return 6;
    }
};

struct S7 {
    constexpr int val() const noexcept {
        return 7;
    }
};

struct S8 {
    constexpr int val() const noexcept {
        return 8;
    }
};

struct S0 {
    constexpr S0(int value) : value{value} {}
    int value;

    constexpr int val() const noexcept {
        return value;
    }
};

using namespace raytracer::variant_ops;

TEST_CASE("variant arrays can be built up from zero", "[variant_ops]") {
    constexpr std::array<std::variant<Variant_Monoid::zero>, 0> s0{};
    REQUIRE(s0.empty());

    constexpr std::array<std::variant<Variant_Monoid::zero, int>, 1> s1 = va_append_element(s0, 5);
    REQUIRE(s1.size() == 1);
    REQUIRE(s1[0] == std::variant<Variant_Monoid::zero, int>{5});

    constexpr std::array<std::variant<double, Variant_Monoid::zero, int>, 2> s2 = va_prepend_element(3.14159, s1);
    REQUIRE(s2.size() == 2);
    REQUIRE(s2[0] == std::variant<double, Variant_Monoid::zero, int>{3.14159});
    REQUIRE(s2[1] == std::variant<double, Variant_Monoid::zero, int>{5});
}

TEST_CASE("variant arrays can have the same type added to them multiple times", "[variant_ops]") {
    constexpr std::array<std::variant<int>, 2> s2{0, 1};
    constexpr std::array<std::variant<int>, 3> s3 = va_prepend_element(-1, s2);
    constexpr std::array<std::variant<int>, 4> s4 = va_append_element(s3, 2);
    REQUIRE(s4 == std::array<std::variant<int>, 4>{-1, 0, 1, 2,});
}

TEST_CASE("variant arrays can act pseudopolymorphically", "[variant_ops][pseudopolymorphism]") {
    using variants_all = std::variant<S1, S2, S3, S4, S5, S6, S7, S8, S0>;
    constexpr std::array<variants_all, 9> arr = {{S0{0}, S1{}, S2{}, S3{}, S4{}, S5{}, S6{}, S7{}, S8{}}};
    constexpr std::array<int, 9> intarr = va_map<int>(arr, [](auto &&a) { return a.val(); });
    REQUIRE(intarr == std::array<int, 9>{0, 1, 2, 3, 4, 5, 6, 7, 8});
}