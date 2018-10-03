/**
 * TestIntersection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "transformers.h"
#include "tuple"

using namespace raytracer;
using namespace raytracer::transformers;

TEST_CASE("Intersection can be created and initialized", "[Intersection][constructor]") {
    Sphere s;
    Intersection it{0.5, s};
    REQUIRE(it.getObject() == s);
    REQUIRE(it.getT() == 0.5);
}

TEST_CASE("Intersections can be aggregated", "[Intersections][Intersection][aggregate]") {
    Sphere s;
    Intersection i1{1, s};
    Intersection i2{2, s};
    const auto is = Intersection::aggregate({i1, i2});
    REQUIRE(is.size() == 2);
    REQUIRE(is[0] == i1);
    REQUIRE(is[1] == i2);
}

SCENARIO("Intersect sets the object on the intersection") {
    GIVEN("r is a ray at point (0, 0, -5) with vector (0, 0, 1)") {
        const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
        AND_THEN("s is a sphere") {
            const Sphere s;
            WHEN("xs is the intersection of s and r") {
                const auto xs = s.intersect(r);
                THEN("size of intersection must be 2") {
                    REQUIRE(xs.size() == 2);
                    AND_THEN("xs shapes must be s") {
                        REQUIRE(xs[0].getObject() == s);
                        REQUIRE(xs[1].getObject() == s);
                    }
                }
            }
        }
    }
}

SCENARIO("The hit, when all intersections have positive t") {
    GIVEN("s is a sphere") {
        const Sphere s;
        GIVEN("i1 is the intersection (1, s)") {
            const Intersection i1{1, s};
            GIVEN("i2 is the intersection (2, s)") {
                const Intersection i2{2, s};
                THEN("hit is i1") {
                    const auto xs = Intersection::aggregate({i1, i2});
                    const auto hit = Intersection::hit(xs);
                    REQUIRE(hit.has_value());
                    REQUIRE(hit.value() == i1);
                }
            }
        }
    }
}

SCENARIO("The hit, when some intersections have negative t") {
    GIVEN("s is a sphere") {
        const Sphere s;
        GIVEN("i1 is the intersection (-1, s)") {
            const Intersection i1{-1, s};
            GIVEN("i2 is the intersection (1, s)") {
                const Intersection i2{1, s};
                THEN("hit is i2") {
                    const auto xs = Intersection::aggregate({i1, i2});
                    const auto hit = Intersection::hit(xs);
                    REQUIRE(hit.has_value());
                    REQUIRE(hit.value() == i2);
                }
            }
        }
    }
}

SCENARIO("The hit, when all intersections have negative t") {
    GIVEN("s is a sphere") {
        const Sphere s;
        GIVEN("i1 is the intersection (-2, s)") {
            const Intersection i1{-1, s};
            GIVEN("i2 is the intersection (-1, s)") {
                const Intersection i2{-2, s};
                THEN("hit is empty") {
                    const auto xs = Intersection::aggregate({i1, i2});
                    const auto hit = Intersection::hit(xs);
                    REQUIRE(!hit.has_value());
                }
            }
        }
    }
}

SCENARIO("The hit is always the lowest non-negative intersection") {
    GIVEN("s is a sphere") {
        const Sphere s;
        GIVEN("i1 is the intersection (5, s)") {
            const Intersection i1{5, s};
            GIVEN("i2 is the intersection (7, s)") {
                const Intersection i2{7, s};
                GIVEN("i3 is the intersection (-3, s)") {
                    const Intersection i3{-3, s};
                    GIVEN("i4 is the intersection (2, s)") {
                        const Intersection i4{2, s};
                        THEN("hit is i4") {
                            const auto xs = Intersection::aggregate({i1, i2, i3, i4});
                            const auto hit = Intersection::hit(xs);
                            REQUIRE(hit.has_value());
                            REQUIRE(hit.value() == i4);
                        }
                    }
                }
            }
        }
    }
}