/**
 * transformers.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <iomanip>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <utility>

#include "common.h"

namespace raytracer::transformers {
    namespace details {
        /// Reduce two arrays by applying a function for each index and combining the terms.
        template<class T, class R, size_t N, size_t k>
        struct ReducerAux {
            static constexpr inline R result(std::function<R(T, T)> f, std::function<R(R, R)> r, R defaultval, const std::array<T, N> &a, const std::array<T, N> &b) {
                return r(f(a[k-1], b[k-1]), ReducerAux<T, R, N, k - 1>::result(f, r, defaultval, a, b));
            }
        };

        template<class T, class R, size_t N>
        struct ReducerAux<T, R, N, 0> {
            static constexpr inline R result(std::function<R(T, T)> f, std::function<R(R, R)> r, R defaultval, const std::array<T, N>&, const std::array<T, N>&) {
                return defaultval;
            }
        };

        // TODO:  Not sure how we can pack these into an array?
        template<typename T, size_t N, T nondiag, T diag, size_t idx>
        struct DiagonalizerRow {
            static constexpr T value() {
                if constexpr (idx == N) return diag;
                else return nondiag;
            }
        };

        template<typename T, size_t N, T nondiag, T diag>
        struct DiagonalizerRow<T, N, nondiag, diag, N> {
            static constexpr T value() {
                return 0;
            }
        };

        /// Fill a row with values. Two parameters are provided: one for elements on the diagonal, and one for all others.
        template <typename T, unsigned long C, size_t... Indices>
        constexpr std::array<T, C> make_diagonal_matrix_helper(T nondiag, T diag, size_t row, std::index_sequence<Indices...>) {
            return {{ (Indices == row ? diag : nondiag)... }};
        }

        /// Fill a matrix with values. Two parameters are provided: one for elements on the diagonal, and one for all others.
        template <typename T, unsigned long int R, unsigned long int C, size_t... Indices>
        constexpr std::array<std::array<T, C>, R> make_diagonal_matrix_helper(T nondiag, T diag, std::index_sequence<Indices...>) {
            return {{ make_diagonal_matrix_helper<T, C>(nondiag, diag, Indices, std::make_index_sequence<C>{})... }};
        }

        template<class T, unsigned long int N, size_t... Indices>
        constexpr std::array<T, N> indextransform_helper(std::function<T(int)> f, std::index_sequence<Indices...>) {
            return {{ f(Indices)... }};
        }

        template<class R, class T, unsigned long int N, size_t... Indices>
        constexpr std::array<R, N> unitransform_helper(std::function<R(const T&)> f, const std::array<T, N> &a,
                                                       std::index_sequence<Indices...>) {
            return {{ f(a[Indices])... }};
        }

        template<class R, class T, unsigned long int N, size_t... Indices>
        constexpr std::array<T, N> bitransform_helper(std::function<R(const T&, const T&)> f, const std::array<T, N> &a1, const std::array<T, N> &a2,
                                                      std::index_sequence<Indices...>) {
            return {{ f(a1[Indices], a2[Indices])... }};
        }
    }

    template<class T, class R, size_t N>
    struct Reducer {
        static constexpr inline R result(std::function<R(T, T)> f, std::function<R(R, R)> r, R defaultval, const std::array<T, N> &a, const std::array<T, N> &b) {
            return details::ReducerAux<T, R, N, N>::result(f, r, defaultval, a, b);
        }
    };

    /// Create simple 2-D arrays where the value at pos (i,i) is one value, and the value at position (i,j), i != j, is another.
    template <typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> make_diagonal_matrix(T nondiag, T diag) {
        return details::make_diagonal_matrix_helper<T, R, C>(nondiag, diag, std::make_index_sequence<R>{});
    }

    template <typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> make_uniform_matrix(T fill) {
        return make_diagonal_matrix<T,R,C>(fill, fill);
    }

    /// Execute a transformation on a range of indices.
    template<class T, unsigned long int N>
    constexpr std::array<T, N> indextransform(std::function<T(int)> f) {
        return details::indextransform_helper<T, N>(f, std::make_index_sequence<N>{});
    }

    /// Execute a transformation on an array for each index.
    template<class R, class T, unsigned long int N>
    constexpr std::array<R, N> unitransform(std::function<R(const T&)> f, const std::array<T, N> &a) {
        return details::unitransform_helper(f, a, std::make_index_sequence<N>{});
    }

    /// Execute a transformation on a pair of arrays for each index.
    template<class R, class T, unsigned long int N>
    constexpr std::array<T, N> bitransform(std::function<R(const T&, const T&)> f, const std::array<T, N> &a1, const std::array<T, N> &a2) {
        return details::bitransform_helper(f, a1, a2, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opadd_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] + t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator+(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opadd_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opdiff_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] - t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator-(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        //return bitransform<T,T,N>([](const T &a, const T &b) { return a - b; }, t1, t2);
        return vector_opdiff_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opmult_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] * t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator*(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opmult_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename F, typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N>
    scalar_opmult_helper(F f, const std::array<T,N> &t, std::index_sequence<Indices...>) {
        return {{f * t[Indices]...}};
    }
    template<typename F, typename T, size_t N>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, std::array<T,N>>
    operator*(F f, const std::array<T,N> &t) {
        return scalar_opmult_helper<F,T,N>(f, t, std::make_index_sequence<N>{});

    }

    /// Checked for constexpr: ONLY WORKS IF T2 HAS NO ZEROES.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opdiv_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{(t1[Indices] / t2[Indices])...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator/(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opdiv_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename F, typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> scalar_opdiv_helper(const std::array<T,N> &t, F f, std::index_sequence<Indices...>) {
        return {{t[Indices] / f...}};
    }
    template<typename F, typename T, size_t N>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, std::array<T,N>>
    operator/(const std::array<T,N> &t, F f) {
        return scalar_opdiv_helper<F,T,N>(t, f, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_neg_helper(const std::array<T,N> &t, std::index_sequence<Indices...>) {
        return {{-t[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator-(const std::array<T,N> &t) {
        return vector_neg_helper<T,N>(t, std::make_index_sequence<N>{});
    }

    /// Array partial equality.
    /// If floating point, use ALMOST_EQUALS.
    template<typename T>
    constexpr typename std::enable_if_t<std::is_floating_point_v<T>, bool>
    equals(T t1, T t2) { return ALMOST_EQUALS(t1, t2); }

    /// If integral, just use ==.
    template<typename T>
    constexpr typename std::enable_if_t<std::is_integral_v<T>, bool>
    equals(T t1, T t2) { return t1 == t2; }

    /// If an array, iterate. This will allow us to check multidimensional arrays.
    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr bool equals_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return (ALMOST_EQUALS(t1[Indices], t2[Indices]) && ...);
    }
    template<typename T, size_t N>
    constexpr bool equals(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return equals_helper(t1, t2, std::make_index_sequence<N>{});
    }

    /// Create an array where the elements are determined by a supplied function invoked on index.
    template<class T, unsigned long int N>
    static constexpr std::array<T, N> make_array(const std::function<T(int)> f) {
        return indextransform<T,N>(f);
    }

    /// Test at compile time if two sizes are equal.
    template<size_t m, size_t n>
    struct are_equal final: std::false_type {};

    template<size_t m>
    struct are_equal<m,m> final: std::true_type {};

    /// Auxiliary tools to find submatrices.
//    template<typename T, size_t R, size_t C>
//    constexpr std::array<std::array<T, C-1>, R-1> array_submatrix(const std::array<std::array<T, C>, R> &m, size_t row, size_t col) {
//        static_assert(R > 1, "Submatrix requires that the matrix have more than one row");
//        static_assert(C > 1, "Submatrix requires that the matrix have more than one column");
//
//        if (row < 0 || row >= R)
//            throw std::invalid_argument("To calculate submatrix, row must be in bounds");
//        if (col < 0 || col >= C)
//            throw std::invalid_argument("To calculate submatrix, col must be in bounds");
//
//        return make_array<std::array<T, C-1>, R-1>([&m, row, col](int i) {
//            return make_array<T, C-1>([&m, row, col, i](int j) {
//                if (i < row  && j < col)  return m[i][j];
//                if (i >= row && j < col)  return m[i+1][j];
//                if (i < row && j >= col)  return m[i][j+1];
//                if (i >= row && j >= col) return m[i+1][j+1];
//            });
//        });

    template<typename T, size_t R, size_t C, size_t row, size_t col, size_t i, size_t j>
    constexpr std::array<std::array<T, C-1>, R-1> array_submatrix_helper(const std::array<std::array<T, C>, R> &m) {

    }

    template<typename T, size_t R, size_t C, size_t row, size_t col>
    constexpr std::array<std::array<T, C-1>, R-1> array_submatrix(const std::array<std::array<T, C>, R> &m) {
        static_assert(R > 1, "Submatrix requires that the matrix have more than one row");
        static_assert(C > 1, "Submatrix requires that the matrix have more than one column");

        if (row < 0 || row >= R)
            throw std::invalid_argument("To calculate submatrix, row must be in bounds");
        if (col < 0 || col >= C)
            throw std::invalid_argument("To calculate submatrix, col must be in bounds");

        return make_array<std::array<T, C-1>, R-1>([&m](int i) {
            return make_array<T, C-1>([&m, i](int j) {
                if (i < row  && j < col)  return m[i][j];
                if (i >= row && j < col)  return m[i+1][j];
                if (i < row && j >= col)  return m[i][j+1];
                if (i >= row && j >= col) return m[i+1][j+1];
            });
        });
    }

    /**
     * Auxiliary functions to find the determinant. Only for square matrices.
     * Different cases for N=1, 2, and > 2.
     * Annoyingly, I can't seem to put these in Matrix, where they would be more useful due to access to cofactor.
     * The compiler complains about overloads there.
     */

    // Forward declaration.
    template<typename T, size_t N, size_t i, size_t j>
    constexpr T array_cofactor(const std::array<std::array<T,N>,N> &m);

    // TODO: For this to be genuinely constexpr, we need to make array_submatrix constexpr, which means eliminating
    // TODO: the make_array calls and folding these up somehow into templates.
    template<typename T, size_t N, size_t k>
    struct ArrayDeterminantHelper {
        constexpr static T value(const std::array<std::array<T,N>,N> &m) {
            return m[0][k] * array_cofactor<T, N, 0, k>(m) + ArrayDeterminantHelper<T, N, k + 1>::value(m);
        }
    };
    template<typename T, size_t N>
    struct ArrayDeterminantHelper<T,N,N> {
        constexpr static T value(const std::array<std::array<T,N>,N> &m) {
            return T{};
        }
    };

    template<typename T, size_t N, size_t k>
    constexpr T array_determinant_helper(const std::array<std::array<T,N>,N> &m) {
        if constexpr (N == k) return T{};
        return m[0][k] * array_cofactor<T,N,0,k>(m) + array_determinant_helper<T,N,k+1>(m);
    }

    // TODO: Apparently, we don't need enable_if here either. I guess the compiler picks the most specific impl.
    template<typename T, size_t N> //typename = typename std::enable_if<(N > 2)>>
    constexpr T array_determinant(const std::array<std::array<T,N>,N> &m) {
        return ArrayDeterminantHelper<T,N,0>::value(m);
    }

    // TODO: We don't actually need the enable_ifs here, as the function parameters are unique.
    template<typename T, size_t N>//, typename = typename std::enable_if<N == 2>>
    constexpr T array_determinant(const std::array<std::array<T,2>,2> &m) {
        return m[0][0] * m[1][1] - m[1][0] * m[0][1];
    }

    template<typename T, size_t N>//, typename = typename std::enable_if<N == 1>>
    constexpr T array_determinant(const std::array<std::array<T,1>,1> &m) {
        return m[0][0];
    }

    /// Minor and cofactor so we can use them and don't have to redefine them in matrix.h
    template<typename T, size_t N, size_t i, size_t j>
    constexpr T array_minor(const std::array<std::array<T,N>,N> &m) {
        return array_determinant<T,N-1>(array_submatrix<T,N,N,i,j>(m));
    }

    template<typename T, size_t N, size_t i, size_t j>
    constexpr T array_cofactor(const std::array<std::array<T,N>,N> &m) {
        return ((i + j) % 2 ? -1 : 1) * array_minor<T,N,i,j>(m);
    }

    /// Checked for constexpr.
    template<typename T, size_t N>
    constexpr T dot_product(const std::array<T,N> &a1, const std::array<T,N> &a2) {
        T t{};
        for (int i=0; i < N; ++i)
            t += a1[i] * a2[i];
        return t;
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> initializer_list_to_array_helper(const std::initializer_list<T> lst, std::index_sequence<Indices...>) {
        return {{lst.begin()[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> initializer_list_to_array(const std::initializer_list<T> lst) {
        return initializer_list_to_array_helper<T,N>(lst, std::make_index_sequence<N>{});
    }
}
