/**
 * TestMatrix.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "common.h"
#include "matrix.h"

using namespace raytracer;

/// Matrices we will reuse multiple times.
const Matrix<double, 2, 3> m1 = {{0, 1, 2},
                                 {3, 4, 5}};
const Matrix<double, 3, 2> m2 = {{0, 3},
                                 {1, 4},
                                 {2, 5}};

TEST_CASE("Matrix should be constructible via initializer list and indexable", "[Matrix][initializer_list][index]") {
    Matrix<double,4,4> m = {{ 1,    2,    3,    4},
                            { 5.1,  6.1,  7.1,  8.1},
                            { 9,   10,   11,   12},
                            {13.5, 14.5, 15.5, 16.5}};

    REQUIRE(ALMOST_EQUALS(m[0][0],  1));
    REQUIRE(ALMOST_EQUALS(m[0][3],  4));
    REQUIRE(ALMOST_EQUALS(m[1][0],  5.1));
    REQUIRE(ALMOST_EQUALS(m[1][2],  7.1));
    REQUIRE(ALMOST_EQUALS(m[2][2], 11));
    REQUIRE(ALMOST_EQUALS(m[3][0], 13.5));
    REQUIRE(ALMOST_EQUALS(m[3][2], 15.5));
}

TEST_CASE("Matrix should know its size", "[Matrix][size]") {
    REQUIRE(m1.row_count() == 2);
    REQUIRE(m1.column_count() == 3);
    REQUIRE(m2.row_count() == 3);
    REQUIRE(m2.column_count() == 2);
}

TEST_CASE("Matrix should be able to determine equality", "[Matrix][equality]") {
    REQUIRE(m1 == m1);
    REQUIRE(m2 == m2);
    REQUIRE(m1 != m1 + m1);
    REQUIRE(m1 == (m1 + m1)/2);
}

TEST_CASE("Matrix comparison equality should be able to tolerate slight offsets", "[Matrix][equality]") {
    REQUIRE(m1 == m1 - EPSILON/2 * matrix_constants::ones<double, 2, 3>);
    REQUIRE(matrix_constants::I<double, 3, 3> == matrix_constants::I<double, 3, 3> + EPSILON/2 * matrix_constants::I<double, 3, 3>);
}

TEST_CASE("Matrix should be transposable", "[Matrix][transpose]") {
    REQUIRE(m1.transpose() == m2);
}

TEST_CASE("Matrix transpose should be self-inverting", "[Matrix][transpose]") {
    REQUIRE(m1.transpose().transpose() == m1);
}

TEST_CASE("Matrix multiplication should multiply compatibly sized matrices", "[Matrx][multiplication]") {
    Matrix<double, 2, 2> prod = {{ 5, 14},
                                 {14, 50}};
    REQUIRE(m1 * m2 == prod);
}

TEST_CASE("Matrix multiplication should multiply square matrices", "[Matrix][multiplication]") {
    Matrix<double, 4, 4> m1 = {{1, 2, 3, 4},
                               {2, 3, 4, 5},
                               {3, 4, 5, 6},
                               {4, 5, 6, 7}};

    Matrix<double, 4, 4> m2 = {{0, 1,  2,  4},
                               {1, 2,  4,  8},
                               {2, 4,  8, 16},
                               {4, 8, 16, 32}};

    Matrix<double, 4, 4> m3 = {{24, 49,  98, 196},
                               {31, 64, 128, 256},
                               {38, 79, 158, 316},
                               {45, 94, 188, 376}};

    REQUIRE(m1 * m2 == m3);
}

TEST_CASE("Matrix multiplied with a vector should give a vector", "[Matrix][vector_multiplication]") {
    Matrix<double, 3, 4> m = {{1, 2, 3, 4},
                              {2, 4, 4, 2},
                              {8, 6, 4, 1}};

    Vector<double, 4> v = {1, 2, 3, 1};
    Vector<double, 3> prod = {18, 24, 33};
    REQUIRE(m * v == prod);
}

TEST_CASE("Matrix multiplied on the left by a vector should give a vector", "[Matrix][vector_multipication]") {
    Vector<double, 3> v = {1, 2, 3};
    Vector<double, 2> prod = {8, 26};
    Vector<double, 2> vm2 = operator*(v,m2);
    REQUIRE(vm2 == prod);
}