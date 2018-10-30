/**
 * TestMatrix.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "common.h"
#include "matrix.h"
#include "vec.h"
#include "transformers.h"

using namespace raytracer;
using namespace raytracer::transformers;

/// Matrices we will reuse multiple times.
constexpr Matrix<double, 2, 3> m1 = {{0, 1, 2},
                                     {3, 4, 5}};
constexpr Matrix<double, 3, 2> m2 = {{0, 3},
                                     {1, 4},
                                     {2, 5}};
constexpr SquareMatrix<3> m3 = {{1, 2, 3},
                                {4, 5, 6},
                                {7, 8, 9}};

TEST_CASE("Matrix: Matrix should be constructible via initializer list and indexable") {
    constexpr SquareMatrix<4> m = {{ 1,    2,    3,    4},
                                   { 5.1,  6.1,  7.1,  8.1},
                                   { 9,   10,   11,   12},
                                   {13.5, 14.5, 15.5, 16.5}};

    // Make sure indexing is constexpr.
    constexpr auto res = m[0][0];
    REQUIRE(ALMOST_EQUALS(m[0][0],  1));
    REQUIRE(ALMOST_EQUALS(m[0][3],  4));
    REQUIRE(ALMOST_EQUALS(m[1][0],  5.1));
    REQUIRE(ALMOST_EQUALS(m[1][2],  7.1));
    REQUIRE(ALMOST_EQUALS(m[2][2], 11));
    REQUIRE(ALMOST_EQUALS(m[3][0], 13.5));
    REQUIRE(ALMOST_EQUALS(m[3][2], 15.5));
}

TEST_CASE("Matrix: Matrix should know its size") {
    // Make sure size elements are constexpr.
    constexpr auto res1 = m1.row_count();
    constexpr auto res2 = m1.column_count();
    constexpr auto res3 = m3.size();
    REQUIRE(m1.row_count() == 2);
    REQUIRE(m1.column_count() == 3);
    REQUIRE(m2.row_count() == 3);
    REQUIRE(m2.column_count() == 2);

    // Only m3 has size as it is square.
    constexpr auto res4 = m3.size();
    REQUIRE(m3.size() == 3);
}

TEST_CASE("Matrix: Matrix should be able to determine equality") {
    // Make sure equality is constexpr.
    constexpr auto res1 = (m1 == m1);

    REQUIRE(m1 == m1);
    REQUIRE(m2 == m2);
    REQUIRE(m1 != m1 + m1);
    REQUIRE(m1 == (m1 + m1)/2);
}

TEST_CASE("Matrix: Matrix comparison equality should be able to tolerate slight offsets") {
    // Make sure expressions are constexpr.
    constexpr auto res1 = predefined_matrices::I<double, 3> + EPSILON/2 * predefined_matrices::I<double, 3>;
    constexpr auto res2 = m1 - EPSILON/2 * predefined_matrices::ones<double, 2, 3>;
    REQUIRE(m1 == m1 - EPSILON/2 * predefined_matrices::ones<double, 2, 3>);
    REQUIRE(predefined_matrices::I<double, 3> == predefined_matrices::I<double, 3> + EPSILON/2 * predefined_matrices::I<double, 3>);
}

TEST_CASE("Matrix: Matrix should be transposable") {
    // Make sure transpose is constexpr.
    constexpr auto res = m1.transpose();
    REQUIRE(m1.transpose() == m2);
}

TEST_CASE("Matrix: Matrix transpose should be self-inverting") {
    REQUIRE(m1.transpose().transpose() == m1);
}

TEST_CASE("Matrix: Matrix multiplication should multiply compatibly sized matrices") {
    constexpr SquareMatrix<2> prod = {{ 5, 14},
                                      {14, 50}};

    // Make sure multiplication is constexpr.
    constexpr auto res1 = m1 * m2;
    constexpr auto res2 = res1 == prod;
    REQUIRE(m1 * m2 == prod);
}

TEST_CASE("Matrix: Matrix multiplication should multiply square matrices") {
    constexpr SquareMatrix<4> m1 = {{1, 2, 3, 4},
                                    {2, 3, 4, 5},
                                    {3, 4, 5, 6},
                                    {4, 5, 6, 7}};

    constexpr SquareMatrix<4> m2 = {{0, 1,  2,  4},
                                    {1, 2,  4,  8},
                                    {2, 4,  8, 16},
                                    {4, 8, 16, 32}};

    constexpr SquareMatrix<4> m3 = {{24, 49,  98, 196},
                                    {31, 64, 128, 256},
                                    {38, 79, 158, 316},
                                    {45, 94, 188, 376}};

    // Make sure large multiplication is constexpr.
    constexpr auto res1 = m1 * m2;
    constexpr auto res2 = m1 * m2 == m3;
    REQUIRE(m1 * m2 == m3);
}

TEST_CASE("Matrix: Matrix multiplied with a vector should give a vector") {
    constexpr Matrix<double, 3, 4> m = {{1, 2, 3, 4},
                                        {2, 4, 4, 2},
                                        {8, 6, 4, 1}};
    constexpr Vector<double, 4> v = {1, 2, 3, 1};
    constexpr Vector<double, 3> prod = {18, 24, 33};

    // Make sure matrix-vector multiplication is constexpr.
    constexpr auto res = m * v;
    REQUIRE(m * v == prod);
}

TEST_CASE("Matrix: Matrix multiplied on the left by a vector should give a vector") {
    constexpr Vector<double, 3> v = {1, 2, 3};

    // Make sure vector-matrix multiplication is constexpr.
    constexpr auto res = v * m2;
    REQUIRE(v * m2 == Vector<double, 2>{8, 26});
}

TEST_CASE("Matrix: Identity matrix should not change the value of a matrix, whether on left or right") {
    constexpr SquareMatrix<4> m = {{0, 1,  2,  4},
                                   {1, 2,  4,  8},
                                   {2, 4,  8, 16},
                                   {4, 8, 16, 32}};
    REQUIRE(predefined_matrices::I<double, 4> * m == m);
    REQUIRE(m * predefined_matrices::I<double, 4> == m);
}

TEST_CASE("Identity matrix should not change the value of vectors, whether on left or right") {
    constexpr Vector<double, 4> v = {4, 3, 2, 1};
    REQUIRE(predefined_matrices::I<double, 4> * v == v);
    REQUIRE(v * predefined_matrices::I<double, 4> == v);
}

TEST_CASE("Matrix: Matrix of size 1x1 should be able to calculate its determinant") {
    constexpr Matrix<double, 1, 1> m = {{5}};

    // Check that determinant is constexpr.
    constexpr auto res = m.determinant();
    REQUIRE(m.determinant() == 5);
}

TEST_CASE("Matrix: Matrix of size 2x2 should be able to calculate its determinant") {
    constexpr SquareMatrix<2> m = {{ 1, 5},
                                   {-3, 2}};

    // Check that determinant is constexpr.
    constexpr auto res = m.determinant();
    REQUIRE(m.determinant() == 17);
}

TEST_CASE("Matrix: Matrix of size 2x2 should be able to get submatrix") {
    constexpr SquareMatrix<2> m = {{0, 1},
                                   {2, 3}};
    constexpr auto res = m.submatrix<0,0>();
    REQUIRE(m.submatrix<0,0>() == Matrix<double, 1, 1>{{3}});
}

TEST_CASE("Matrix: Matrix of size 3x3 should be able to get submatrix") {
    constexpr SquareMatrix<3> m = {{ 1, 5,  0},
                                   {-3, 2,  7},
                                   { 0, 6, -3}};

    constexpr SquareMatrix<2> sm = {{-3, 2},
                                    {0, 6}};

    constexpr auto res = m.submatrix<0,2>();
    REQUIRE(m.submatrix<0, 2>() == sm);
}

TEST_CASE("Matrix: Matrix of size 4x4 should be able to get submatrix") {
    constexpr SquareMatrix<4> m = {{-6, 1,  1, 6},
                                   {-8, 5,  8, 6},
                                   {-1, 0,  8, 2},
                                   {-7, 1, -1, 1}};

    constexpr SquareMatrix<3> sm = {{-6,  1, 6},
                                    {-8,  8, 6},
                                    {-7, -1, 1}};

    // Check that submatrix is constexpr.
    constexpr auto res = m.submatrix<2,1>();
    REQUIRE(m.submatrix<2, 1>() == sm);
}

TEST_CASE("Matrix: Matrix of non-square size should be able to get submatrix") {
    constexpr Matrix<double, 4, 3> m = {{0, 1,  2},
                                       {3,  4,  5},
                                       {6,  7,  8},
                                       {9, 10, 11}};

    constexpr Matrix<double, 3, 2> sm = {{0, 1},
                                         {3, 4},
                                         {6, 7}};

    // Check that submatrix is constexpr.
    constexpr auto res1 = m.submatrix<0,0>();
    constexpr auto res2 = m.submatrix<1,1>();
    constexpr auto res3 = m.submatrix<3,2>();
    REQUIRE(m.submatrix<3, 2>() == sm);
}

TEST_CASE("Matrix: Matrix of size 3x3 should be able to calculate its minors") {
    constexpr SquareMatrix<3> m = {{3,  5,  0},
                                   {2, -1, -7},
                                   {6, -1,  5}};

    // Check that minor is constexpr.
    constexpr auto res = m.minor<0,0>();
    REQUIRE(m.minor<0, 0>() == -12);
    REQUIRE(m.minor<0, 1>() ==  52);
    REQUIRE(m.minor<0, 2>() ==   4);
    REQUIRE(m.minor<1, 0>() ==  25);
    REQUIRE(m.minor<1, 1>() ==  15);
    REQUIRE(m.minor<1, 2>() == -33);
    REQUIRE(m.minor<2, 0>() == -35);
    REQUIRE(m.minor<2, 1>() == -21);
    REQUIRE(m.minor<2, 2>() == -13);
}

TEST_CASE("Matrix: Matrix of size 3x3 should be able to calculate its cofactors") {
    constexpr SquareMatrix<3> m = {{3,  5,  0},
                                   {2, -1, -7},
                                   {6, -1,  5}};

    // Check that cofactor is constexpr.
    constexpr auto res = m.cofactor<0, 0>();
    REQUIRE(m.cofactor<0, 0>() == -12);
    REQUIRE(m.cofactor<0, 1>() == -52);
    REQUIRE(m.cofactor<0, 2>() ==   4);
    REQUIRE(m.cofactor<1, 0>() == -25);
    REQUIRE(m.cofactor<1, 1>() ==  15);
    REQUIRE(m.cofactor<1, 2>() ==  33);
    REQUIRE(m.cofactor<2, 0>() == -35);
    REQUIRE(m.cofactor<2, 1>() ==  21);
    REQUIRE(m.cofactor<2, 2>() == -13);
}

TEST_CASE("Matrix: Matrix of size 3x3 should be able to calculate its determinant") {
    constexpr SquareMatrix<3> m = {{ 1, 2,  6},
                                   {-5, 8, -4},
                                   { 2, 6,  4}};

    // Check that determinant is constexpr.
    constexpr auto res1 = m.determinant();
    constexpr auto res2 = m.cofactor<0,0>();
    REQUIRE(m.cofactor<0, 0>() ==   56);
    REQUIRE(m.cofactor<0, 1>() ==   12);
    REQUIRE(m.cofactor<0, 2>() ==  -46);
    REQUIRE(m.determinant()  == -196);
}

TEST_CASE("Matrix: Matrix of size 4x4 should be able to calculate its determinant") {
    constexpr SquareMatrix<4> m = {{-2, -8,  3,  5},
                                   {-3,  1,  7,  3},
                                   { 1,  2, -9,  6},
                                   {-6,  7,  7, -9}};

    // Check that determinant is constexpr.
    constexpr auto res1 = m == m;
    constexpr auto res2 = m.determinant();
    constexpr auto res3 = m.cofactor<0,0>();
    REQUIRE(m.cofactor<0, 0>() ==  690);
    REQUIRE(m.cofactor<0, 1>() ==  447);
    REQUIRE(m.cofactor<0, 2>() ==  210);
    REQUIRE(m.cofactor<0, 3>() ==   51);
    REQUIRE(m.determinant()    == -4071);
}

TEST_CASE("Matrix: Matrix of size 2x2 should be able to calculate its inverse") {
    constexpr SquareMatrix<2> m = {{1, 2},
                                   {3, 4}};
    constexpr auto minv = m.invert();
    REQUIRE(m * minv == predefined_matrices::I<double, 2>);
    REQUIRE(minv * m == predefined_matrices::I<double, 2>);
}

TEST_CASE("Matrix: Matrix of size 3x3 should be able to calculate its inverse") {
    constexpr SquareMatrix<3> m = {{0, 1, 2},
                                   {-1, -5, 6},
                                   {7, 8, 9}};
    constexpr auto minv = m.invert();
    REQUIRE(m * minv == predefined_matrices::I<double, 3>);
    REQUIRE(minv * m == predefined_matrices::I<double, 3>);
}

TEST_CASE("Matrix: Matrix of size 4x4 should be able to calculate its inverse") {
    constexpr SquareMatrix<4> m1 = {{-5,  2,  6, -8},
                                    { 1, -5,  1,  8},
                                    { 7,  7, -6, -7},
                                    { 1, -3,  7,  4}};


    constexpr SquareMatrix<4> m1invchk = {{ 0.21805,  0.45113,  0.24060, -0.04511},
                                          {-0.80827, -1.45677, -0.44361,  0.52068},
                                          {-0.07895, -0.22368, -0.05263,  0.19737},
                                          {-0.52256, -0.81391, -0.30075,  0.30639}};

    constexpr auto m1inv = m1.invert();
    REQUIRE(m1inv == m1invchk);
    REQUIRE(m1 * m1inv == predefined_matrices::I<double, 4>);
    REQUIRE(m1inv * m1 == predefined_matrices::I<double, 4>);

    constexpr SquareMatrix<4> m2 = {{ 8, -5,  9,  2},
                                    { 7,  5,  6,  1},
                                    {-6,  0,  9,  6},
                                    {-3,  0, -9, -4}};
    constexpr SquareMatrix<4> m2invchk = {{-0.15385, -0.15385, -0.28205, -0.53846},
                                          {-0.07692,  0.12308,  0.02564,  0.03077},
                                          { 0.35897,  0.35897,  0.43590,  0.92308},
                                          {-0.69231, -0.69231, -0.76923, -1.92308}};
    constexpr auto m2inv = m2.invert();
    REQUIRE(m2inv == m2invchk);
    REQUIRE(m2 * m2inv == predefined_matrices::I<double, 4>);
    REQUIRE(m2inv * m2 == predefined_matrices::I<double, 4>);
    REQUIRE(m1inv * (m1 * m2) == m2);
    REQUIRE(m2inv * (m2 * m1) == m1);
}
