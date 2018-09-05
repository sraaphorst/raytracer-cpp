/**
 * Matrix.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <array>

#include "transformers.h"
#include "vector.h"

namespace raytracer {
    using namespace transformers;

    template<typename T, size_t rows, size_t cols,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix final {
    public:
        using type = T;
        using row_type = std::array<T, cols>;
        using col_type = std::array<T, rows>;
        using matrix_type = std::array<row_type, rows>;

    private:
        const matrix_type contents;

    public:
        constexpr Matrix() noexcept {};
        constexpr Matrix(const matrix_type &contents) noexcept : contents{contents} {}
        constexpr Matrix(matrix_type&& contents) noexcept : contents{contents} {}
        constexpr Matrix(const Matrix&) noexcept = default;
        constexpr Matrix(Matrix&&) noexcept = default;
        constexpr Matrix(std::initializer_list<row_type> lst) : contents{initializer_list_to_array<row_type, rows>(lst)} {}

        ~Matrix() = default;

        constexpr Matrix &operator=(const Matrix&) noexcept = default;
        constexpr Matrix &operator=(Matrix&&) noexcept = default;

        constexpr const row_type &operator[](size_t idx) const {
            return contents[idx];
        }

        constexpr const Matrix<T, cols, rows> transpose() const {
            return transformers::transpose(contents);
        }

        constexpr Matrix operator+(const Matrix &other) const {
            return Matrix{contents + other.contents};
        };

        constexpr Matrix operator-(const Matrix &other) const {
            return Matrix{contents - other.contents};
        }

        template<size_t C2>
        constexpr Matrix<T, rows, C2> operator*(const Matrix<T, cols, C2> &other) const {
            return Matrix<T, rows, C2>{mat_mult<T,rows,cols,C2>(contents, other.contents)};
        }

        //template<typename S, size_t R1, size_t C1>
        template<typename, size_t, size_t, typename>
        friend class Matrix;

        /// This one, however, is constexpr, as checked by assigning the value to be returned to a constexpr variable.
        constexpr Vector<T, rows> operator*(const Vector<T, cols> &v) const {
            //return Vector<T, rows>{unitransform<T, row_type, rows>([&v] (const row_type &r) constexpr { return dot_product(r, v.contents); }, contents)};
            return Vector<T, rows>{contents * v.contents};
        }

        constexpr Matrix operator*(T factor) const {
            return Matrix{factor * contents};
        }

        constexpr Matrix operator/(T denom) const {
            return Matrix{contents / denom};
        };

        /// NOTE: iterators work as constexpr here, but working with indices do not.
        /// Tested for constexpr.
        constexpr bool operator==(const Matrix &other) const {
            for (auto it1 = contents.cbegin(), it2 = other.contents.cbegin(); it1 != contents.cend(); ++it1, ++it2)
                for (auto ot1 = it1->cbegin(), ot2 = it2->cbegin(); ot1 != it1->cend(); ++ot1, ++ot2)
                    if (!ALMOST_EQUALS(*ot1, *ot2)) return false;
            return true;
        }

        constexpr bool operator!=(const Matrix &other) const {
            return !(*this == other);
        }

        static constexpr size_t row_count() {
            return rows;
        }

        static constexpr size_t column_count() {
            return cols;
        }

        static constexpr typename std::enable_if_t<are_equal_v<rows,cols>, size_t>
        size() {
            static_assert(rows == cols, "Matrix::size() only for use with square matrices");
            return rows;
        }

//        template<size_t i>
//        constexpr T determinant_helper() const {
//            if constexpr (i == rows) return 0;
//            return cofactor<0,i>() + determinant_helper<i-1>();
//        }
//
//        template<>
//        constexpr T determinant_helper<cols>() const {
//            return 0;
//        }

//        template<size_t i, size_t j>
//        static constexpr Matrix<T, rows-1, cols-1> submatrix_helper(const std::array<std::array<T, cols>, rows> &contents) {
//            std::array<std::array<T, cols-1>, rows-1> newContents{};
//            for (size_t r = 0; r < rows-1; ++r) {
//                const size_t ridx = r >= i ? r + 1 : r;
//                for (size_t c = 0; c < cols-1; ++c) {
//                    const size_t cidx = c >= j ? c + 1 : c;
//                    newContents[r][c] = contents[ridx][cidx];
//                }
//            }
//            return newContents;
//        }

//        template<size_t i, size_t j>
//        static constexpr T minor_helper(const std::array<std::array<T, rows>, cols> &contents) {
//            return determinant_helper(submatrix_helper<i, j>(contents));
//        }

        /// Calculate the minor(i,j) of a matrix, i.e. the determinant of the submatrix(i,j).
        template<size_t i, size_t j>
        constexpr T minor() const {
            //static_assert(rows == cols, "Matrix::minor() only for use with square matrices");
            return array_minor<T, rows, cols, i, j>(contents);
            //return submatrix<i,j>().determinant();
        }

//        template<size_t i, size_t j>
//        static constexpr T cofactor_helper(const std::array<std::array<T, rows>, cols> &contents) {
//            return ((i + j) % 2 ? 1 : -1) * minor_helper<i, j>(contents);
//        }

        /// Calculate the cofactor(i,j) of a matrix, which is just (i+j)^(-1) * minor(i,j).
        template<size_t i, size_t j>
        constexpr T cofactor() const {
//            static_assert(rows == cols, "Matrix::cofactor() only for use with square matrices");
//            return (i + j % 2 ? 1 : -1) * minor<i,j>();
            return array_cofactor<T, rows, cols, i, j>(contents);
        }

//        template<size_t i>
//        struct determinant_helper {
//            static constexpr T value(const std::array<std::array<T, cols>, rows> &contents) {
//                return cofactor_helper<0,i>(contents) + determinant_helper<i-1>::value(contents);
//            }
//        };
//
//        template<>
//        struct determinant_helper<0> {
//            static constexpr T value(const std::array<std::array<T, cols>, rows>&) {
//                return 0;
//            }
//        };

        constexpr T determinant() const {
//            static_assert(rows == cols, "Matrix::determinant() only for use with square matrices");
//            if constexpr(rows == 1)
//                return contents[0][0];
//            if constexpr(rows == 2)
//                return contents[0][0] * contents[1][1] - contents[0][1] * contents[1][0];

//            T det = 0;
//            for (size_t col = 0; col < cols; ++col)
//                det += cofactor<0, col>();
//            else return determinant_helper<cols-1>();
            return array_determinant<T, rows, cols>(contents);
        }

        /// Omit row i and column j to get a submatrix of one dimension less in rows and cols.
        template<size_t i, size_t j>
        constexpr Matrix<T, rows-1, cols-1> submatrix() const {
//            std::array<std::array<T, cols-1>, rows-1> newContents{};
//            for (size_t r = 0; r < rows-1; ++r) {
//                const size_t ridx = r >= i ? r + 1 : r;
//                for (size_t c = 0; c < cols-1; ++c) {
//                    const size_t cidx = c >= j ? c + 1 : c;
//                    newContents[r][c] = contents[ridx][cidx];
//                }
//            }
//            return Matrix<T,rows-1,cols-1>{newContents};
            return Matrix<T, rows-1, cols-1>{{array_submatrix<T, rows, cols, i, j>(contents)}};
        }


        /// Multiply by factor on the left.
        friend constexpr Matrix operator*(T factor, const Matrix &m) {
            return m * factor;
        }

        /// Multiply by vector on the left.
        template<typename S, size_t m, size_t n>
        friend constexpr Vector<S,n> operator*(const Vector<S,m>&, const Matrix<S,m,n,S>&);
    };

    /// We use the property that (v * A)^T = A^t * v^t.
    template<typename S, size_t m, size_t n>
    constexpr Vector<S,n> operator*(const Vector<S,m> &v, const Matrix<S,m,n,S> &matrix) {
        return matrix.transpose() * v;
    }

    struct matrix_constants {
        /**
         * Unlike make_array, make_uniform_matrix and make_diagonal matrix allow us to be constexpr as they don't
         * use any std::function.
         */
        template<typename T, size_t R, size_t C>
        static constexpr Matrix<T, R, C> ones = make_uniform_matrix<T, R, C>(1);

        /// Identity matrix, only defined as a square matrix.
        template<typename T, size_t N>
        static constexpr Matrix<T, N, N> I = make_diagonal_matrix<T, N, N>(0, 1);
    };
}
