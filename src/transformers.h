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
    template <typename T, unsigned long int R, unsigned long C>
    constexpr std::array<std::array<T, C>, R> make_diagonal_matrix(T nondiag, T diag) {
        return details::make_diagonal_matrix_helper<T, R, C>(nondiag, diag, std::make_index_sequence<R>{});
    }

    template <typename T, unsigned long int R, unsigned long C>
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

    /// Simple type specifiers.
    template<typename T, size_t N> constexpr std::array<T,N> operator+(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a + b; }, t1, t2);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator-(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a - b; }, t1, t2);
    }

    template<typename T, size_t N>
    constexpr std::array<T,N> operator*(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a * b; }, t1, t2);
    }

    template<typename F, typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<F>::value, F>::type>
    constexpr std::array<T,N> operator*(F f, const std::array<T,N> &t) {
        return unitransform<T,T,N>([f](const T &a) { return f * a; }, t);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator/(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a / b; }, t1, t2);
    }

    template<typename F, typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<F>::value, F>::type>
    constexpr std::array<T,N> operator/(const std::array<T,N> &t, F f) {
        return unitransform<T,T,N>([f](const T &a) { return a / f; }, t);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator-(const std::array<T,N> &t) {
        return unitransform<T,T,N>([](const T &a) { return -a; }, t);
    }

    /// If floating point, use ALMOST_EQUALS.
    template<typename T>
    constexpr typename std::enable_if<std::is_floating_point<T>::value, bool>::type
    equals(T t1, T t2) { return ALMOST_EQUALS(t1, t2); }

    /// If integral, just use ==.
    template<typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    equals(T t1, T t2) { return t1 == t2; }

    /// If an array, iterate. This will allow us to check multidimensional arrays.
    template<typename T, size_t N>
    constexpr bool equals(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return Reducer<T, bool, N>::result([](const T &d1, const T &d2) { return equals(d1, d2); },
                [](bool b1, bool b2) { return b1 && b2; }, true, t1, t2);
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
    template<typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C-1>, R-1> array_submatrix(const std::array<std::array<T, C>, R> &m, size_t row, size_t col) {
        static_assert(R > 1, "Submatrix requires that the matrix have more than one row");
        static_assert(C > 1, "Submatrix requires that the matrix have more than one column");

        if (row < 0 || row >= R)
            throw std::invalid_argument("To calculate submatrix, row must be in bounds");
        if (col < 0 || col >= C)
            throw std::invalid_argument("To calculate submatrix, col must be in bounds");

        return make_array<std::array<T, C-1>, R-1>([&m, row, col](int i) {
            return make_array<T, C-1>([&m, row, col, i](int j) {
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
     * We also need a lot of forward declarations due to the code making circular calls.
     */
    template<typename T, size_t N>
    constexpr T array_cofactor(const std::array<std::array<T,N>,N> &m, size_t i, size_t j);

    template<typename T, size_t N>// size_t k, typename = std::enable_if<k == 0>>
    constexpr T array_determinant_helper(const std::array<std::array<T,N>,N> &m, int k) {
        return (k < N) ? (m[0][k] * array_cofactor<T,N>(m, 0, k) + array_determinant_helper<T,N>(m, k+1)) : T{};
    }

    template<typename T, size_t N, typename = std::enable_if<(N > 2)>>
    constexpr T array_determinant(const std::array<std::array<T,N>,N> &m) {
        return array_determinant_helper<T,N>(m, 0);
    }

    template<typename T, size_t N, typename = std::enable_if<N == 2>>
    constexpr T array_determinant(const std::array<std::array<T,2>,2> &m) {
        return m[0][0] * m[1][1] - m[1][0] * m[0][1];
    }

    template<typename T, size_t N, typename = std::enable_if<N == 1>>
    constexpr T array_determinant(const std::array<std::array<T,1>,1> &m) {
        return m[0][0];
    }

    /// Minor and cofactor so we can use them and don't have to redefine them in matrix.h
    template<typename T, size_t N>
    constexpr T array_minor(const std::array<std::array<T,N>,N> &m, size_t i, size_t j) {
        return array_determinant<T,N-1>(array_submatrix<T,N,N>(m, i, j));
    }

    template<typename T, size_t N>
    constexpr T array_cofactor(const std::array<std::array<T,N>,N> &m, size_t i, size_t j) {
        return ((i + j) % 2 ? -1 : 1) * array_minor<T,N>(m, i, j);
    }
}
