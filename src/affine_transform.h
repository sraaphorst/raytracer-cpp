/**
 * affine_transform.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>

#include "matrix.h"
#include "vector.h"

namespace raytracer {
    using Transformation = SquareMatrix<4>;

    constexpr Transformation translate(double x, double y, double z) {
        std::array<double, 3> point{x, y, z};

        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if (i == j)
                    m[i][j] = 1;
                else if (j == 3)
                    m[i][3] = point[i];
                else
                    m[i][j] = 0;
        return Transformation{m};
    }

    constexpr Transformation translate(const std::array<double, 3> &t) {
        return translate(t[0], t[1], t[2]);
    }

    constexpr Transformation translate(const Tuple &t) {
        return translate(t[0], t[1], t[2]);
    }
}
