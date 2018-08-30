/**
 * Matrix.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>

#include "transformers.h"

namespace raytracer {
    template<typename T, size_t rows, size_t cols,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
    public:
        using type = T;
        using row_type = std::array<T, cols>;
        using matrix_type = std::array<row_type, rows>;

    protected:
        matrix_type contents;

    public:
        constexpr Matrix(const matrix_type &contents): contents{contents} {}
        constexpr Matrix(matrix_type&& contents): contents{contents} {}
        constexpr Matrix(const Matrix &other) = default;
        constexpr Matrix(Matrix&& other) = default;

        constexpr Matrix(std::initializer_list<row_type> r) {
            std::copy(r.begin(), r.end(), contents.begin());
        }

        ~Matrix() = default;

        Matrix operator+(const Matrix &other) const {
            return Matrix{details::bitransform(details::array_sum<T, cols>, contents, other.contents)};
        };
    };
}
