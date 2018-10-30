/**
 * shapefactory.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <vector>

#include "shape.h"

namespace raytracer::shapes {
    class Cube;
    class Plane;
    class Sphere;

    class ShapeFactory final {
        static std::vector<std::shared_ptr<Shape>> shapes;

    public:
        ShapeFactory() = delete;

        std::shared_ptr<Cube>   cube() const;
        std::shared_ptr<Plane>  plane() const;
        std::shared_ptr<Sphere> sphere() const;

    private:
        template<typename T>
        std::shared_ptr<T> makeShape() const {
            std::shared_ptr<T> s = std::make_shared<T>();
            shapes.emplace_back(s);
            return s;
        }
    };
}

