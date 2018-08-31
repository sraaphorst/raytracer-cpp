/**
 * Matrix.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <array>

#include "transformers.h"
#include "tuple.h"
#include "vector.h"

namespace raytracer {
    using namespace transformers;

    template<typename T, size_t rows, size_t cols,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
    public:
        using type = T;
        using row_type = std::array<T, cols>;
        using matrix_type = std::array<row_type, rows>;

    protected:
        matrix_type contents;

        /// Used in matrix multiplication.
        static constexpr T dot_product(const row_type &r1, const row_type &r2) noexcept {
            return transformers::Reducer<T, T, cols>::result(
                    [](T t1, T t2) { return t1 * t2; },
                    [](const T &t1, const T &t2) { return t1 + t2; }, 0,
                    r1, r2);
        }

    public:
        constexpr Matrix() {};
        constexpr Matrix(const matrix_type &contents) noexcept : contents{contents} {}
        constexpr Matrix(matrix_type&& contents) noexcept : contents{contents} {}
        constexpr Matrix(const Matrix&) noexcept = default;
        constexpr Matrix(Matrix&&) noexcept = default;

        constexpr Matrix(std::initializer_list<row_type> r) {
            std::copy(r.begin(), r.end(), contents.begin());
        }

        ~Matrix() = default;

        constexpr Matrix &operator=(const Matrix&) noexcept = default;
        constexpr Matrix &operator=(Matrix&&) noexcept = default;

        constexpr row_type &operator[](size_t idx) {
            return contents[idx];
        }

        constexpr Matrix operator+(const Matrix &other) const {
            return Matrix{contents + other.contents};
        };

        constexpr Matrix operator-(const Matrix &other) const {
            return Matrix{contents - other.contents};
        }

        constexpr Matrix<T, rows, rows> operator*(const Matrix<T, cols, rows> &other) const {
            const Matrix otherT = other.transpose();

            Matrix<T, rows, rows> m;
            for (int i=0; i < rows; ++i)
                for (int j=0; j < rows; ++j)
                    m[i][j] = dot_product(contents[i], otherT.contents[j]);
            return m;
        }

        constexpr Vector<T, rows> operator*(const Vector<T, cols> &v) const {
            return unitransform([&v,this] (const row_type& r) { return dot_product(r, v); }, contents);
        }

        constexpr Matrix operator*(T factor) const {
            return Matrix{factor * contents};
        }

        constexpr Matrix operator/(T denom) const {
            return Matrix{contents / denom};
        };

        constexpr bool operator==(const Matrix &other) const {
            return equals(contents, other.contents);
        }

        constexpr bool operator!=(const Matrix &other) const {
            return !(*this == other);
        }

        constexpr Matrix<T, cols, rows> transpose() const {
            using column_type = std::array<T, rows>;

            return Matrix<T, cols, rows>{indextransform<column_type, cols>([this](int c) {
                return indextransform<T, rows>([this, c](int r){ return this->contents[r][c]; });
            })};
        }

        static constexpr size_t row_count() {
            return rows;
        }

        static constexpr size_t column_count() {
            return cols;
        }

        friend constexpr Matrix operator*(T factor, const Matrix &m) {
            return m * factor;
        }

        static Matrix ones() {
            Matrix m;
            for (auto i=0; i < rows; ++i)
                std::fill(m.contents[i].begin(), m.contents[i].end(), 1);
            return m;
        }
    };
}
