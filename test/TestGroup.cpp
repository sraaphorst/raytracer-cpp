/**
 * TestGroup.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <numeric>
#include <vector>

#include <TestShape.h>

#include "cylinder.h"
#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "vec.h"

// A necessary evil to test the private members.
#define private public
#include "group.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Group: Creating a new group") {
    const auto g = Group::createGroup();
    REQUIRE(g->getShapes().empty());
    REQUIRE(g->getTransformation() == predefined_matrices::I<double, 4>);
}

TEST_CASE("Group: Adding a child to a group") {
    auto g = Group::createGroup();
    const auto s = TestShape::createTestShape();
    g->addAll(s);
    REQUIRE(g->getShapes().size() == 1);
    REQUIRE(g->getShapes()[0] == s);
    REQUIRE(s->getParent() == g);
    g->clearShapes();
}

TEST_CASE("Group: Intersecting a ray with an empty group") {
    const auto g = Group::createGroup();
    Ray ray{predefined_tuples::zero_point, predefined_tuples::z1};
    const auto xs = g->localIntersection(ray);
    REQUIRE(xs.empty());
}

TEST_CASE("Group: Intersecting a ray with a non-empty group") {
    auto g = Group::createGroup();
    const auto s1 = Sphere::createSphere();
    auto s2 = Sphere::createSphere();
    s2->setTransformation(translation(0, 0, -3));
    auto s3 = Sphere::createSphere();
    s3->setTransformation(translation(5, 0, 0));
    g->addAll(s1, s2, s3);

    const Ray ray{make_point(0, 0, -5), predefined_tuples::z1};
    const auto xs = g->localIntersection(ray);

    REQUIRE(xs.size() == 4);
    REQUIRE(xs[0].getObject() == s2);
    REQUIRE(xs[1].getObject() == s2);
    REQUIRE(xs[2].getObject() == s1);
    REQUIRE(xs[3].getObject() == s1);
}

TEST_CASE("Group: Intersecting a transformed group") {
    auto g = Group::createGroup();
    g->setTransformation(scale(2, 2, 2));
    auto s = Sphere::createSphere();
    s->setTransformation(translation(5, 0, 0));
    g->add(s);

    const Ray ray{make_point(10, 0, -10), predefined_tuples::z1};
    const auto xs = g->intersect(ray);
    REQUIRE(xs.size() == 2);
}

TEST_CASE("Group: localNormalAt not supported") {
    REQUIRE_THROWS(Group::createGroup()->localNormalAt(predefined_tuples::zero_point));
}

TEST_CASE("Group: A group has a bounding box that contains its children") {
    auto s = Sphere::createSphere();
    s->setTransformation(translation(2, 5, -3) * scale(2, 2, 2));

    auto c = Cylinder::createCylinder();
    c->setMinimumY(-2);
    c->setMaximumY(2);
    c->setTransformation(translation(-4, -1, 4) * scale(0.5, 1, 0.5));

    auto g = Group::createGroup();
    g->addAll(s, c);

    const auto box = g->bounds();
    REQUIRE(box.getMinPoint() == make_point(-4.5, -3, -5));
    REQUIRE(box.getMaxPoint() == make_point(4, 7, 4.5));
}