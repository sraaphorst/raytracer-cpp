/**
 * shapefactory.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <vector>

#include "shapes/cube.h"
#include "plane.h"
#include "shapefactory.h"
#include "sphere.h"


namespace raytracer::shapes {
    std::vector<std::shared_ptr<Shape>> ShapeFactory::shapes;

    std::shared_ptr<Cube> ShapeFactory::cube() const {
        return makeShape<Cube>();
    }

    std::shared_ptr<Plane> ShapeFactory::plane() const {
        return makeShape<Plane>();
    }
    std::shared_ptr<Sphere> ShapeFactory::sphere() const {
        return makeShape<Sphere>();
    }
}