/**
 * @file Matrix.h
 * @author  maayan.sharon
 * @version 1.0
 * @date Sep 2020
 *
 * @brief class represents a black and white photos of 128*128 as matrix.
 *
 * @section LICENSE
 * This program is not a free software; part of c++ course
 *
 * @section DESCRIPTION
 * class represents a black and white photos of 128*128 as matrix and it's functions
 */

#ifndef EX5_MATRIX_H

// ------------------------------ includes ------------------------------

#include <iostream>
#include <fstream>      // std::ifstream
#include <cmath>

// ------------------------------ const & macros -----------------------------

#define EX5_MATRIX_H
/**
 * default val rows
 */
#define DEFAULD_ROWS 1
/**
 * default val cols
 */
#define DEFAULT_COLS 1
/**
 * invalid matrix dimensions
 */
#define INVALID_MAT_DIMENSIONS "Invalid matrix dimensions.\n"
/**
 * error - dividing by 0
 */
#define DEVISION_BY_ZERO "Divesion by zero.\n"
/**
 * index out of range
 */
#define IDX_OUT_OF_RANGE "Index out of range.\n"
/**
 * input stream is invalid
 */
#define INPUT_STREAM_INVALID "Error loading from input stream.\n"


// ------------------------------ functions -----------------------------

/**
 *  class which represents a matrix
 */
class Matrix
{
private:

    int _rows, _cols;
    float *_mat;

public:
    /**
     * Constructor
     * Constructs matrix rows * cols (need to make sure rows,cols are non negative).
     * Initiates all elements to 0.
     * @param rows
     * @param cols
     */
    Matrix(int rows, int cols);

    /**
     * Default constructor
     * Constructs 1*1 matrix, where the single element is initiated to 0
     */
    Matrix();

    /**
     * copy constructor
     * Constructs matrix from another matrix
     * @param m
     */
    Matrix(const Matrix &m);

    /**
     * Destructor
     * Destroys the matrix
     */
    ~Matrix();

    /**
     * Returns the amount of rows (int).
     * @return
     */
    int getRows() const;

    /**
     * Returns the amount of columns (int).
     * @return
     */
    int getCols() const;

    /**
     * Transforms a matrix into a column vector.
     *  Supports function calling. I.E:
     *  Matrix m(5,4);
     *  m.vectorize();
     *  int r = m.getRows(); // suppose to return 20
     *  int c = m.getCols(); // suppose to return 1
        [ 1, 2, 3]
        [ 4, 5, 6] ⇒ transpose ( [1, 2, 3, 4, 5, 6, 7, 8, 9] )
        [ 7, 8, 9]
     *
     * @return
     */
    Matrix vectorize();

    /**
     * Prints matrix elements, no return value (void).
     * Prints space after each element (not including the last element in the row).
     * Prints new line after each row (not including the last row)
     */
    void print() const;

    /**
     * assignment
     * Matrix a, b;
     * a = b;
     * @param rhs
     * @return
     */
    Matrix &operator=(const Matrix &rhs);

    /**
     * Matrix multiplication
     * Matrix a, b;
     * Matrix c = a * b
     * ** do not forget algebra rules for matrix multiplication.
     * Check dimensions valid for operation.
     * @param rhs
     * @return
     */
    Matrix operator*(const Matrix &rhs) const;

    /**
     * Scalar mult. On the right
     * Matrix m;
     * float c;
     * Matrix m2 = m * c;
     * @param c
     * @return
     */
    Matrix operator*(float c) const;

    /**
     * Scalar mult. In the left.
     * Matrix m; float c;
     * Matrix m2 = c * m
     * @param c
     * @param rhs
     * @return
     */
    friend Matrix operator*(float c, const Matrix &rhs);

    /**
     *  Matrix multiplication.
     *  Matrix a,b;
     *  a*=b;
     *  it is equivalent to  a= a*b
     * Check dimensions valid for operation
     * @param rhs
     * @return
     */
    Matrix &operator*=(const Matrix &rhs);

    /**
     * Scalar mult.accumulation.
     * Matrix a; float c;
     * a*=c
     * (note to self:  it is equivalent to a= a*c)
     * @param c
     * @return
     */
    Matrix &operator*=(float c);

    /**
     * Scalar division on the right
     * Matrix a; float c;
     * Matrix b = a / c;
     * check to scalar is not 0
     * @param c
     * @return
     */
    Matrix operator/(float c);

    /**
     * Scalar division
     * Matrix a;
     * float c;
     * a /= c;
     * ** check to scalar is not 0.
     * (note to self:  it is equivalent to a= a/c)
     * @param c
     * @return
     */
    Matrix &operator/=(float c);

    /**
     * Matrix addition
     * Matrix a, b;
     * Matrix c = a + b;
     * Check dimensions valid for operation.
     * @param rhs
     * @return
     */
    Matrix operator+(const Matrix &rhs) const;

    /**
     * Matrix addition accumulation
     * Matrix a, b;
     * a += b;
     * Check dimensions valid for operation.
     * (note to self:  it is equivalent to a= a+b)
     * @param c
     * @return
     */
    Matrix &operator+=(const Matrix &rhs);

    /**
     * Matrix scalar addition.
     * Matrix a; float c;
     * a += c;
     * (note to self:  it is equivalent to a= a+c)
     * @param c
     * @return
     */
    Matrix &operator+=(float c);

    /**
     * Parenthesis indexing
     *int i, j; Matrix m;
     * float val = m(i, j);
     * m(i,j) = 5.6;
     * check the indexes are in the right range
     *
     * Check indexes are in valid ranges.

     * @param i
     * @param j
     * @return
     */
    float operator()(int i, int j) const;

    /**
     * Parenthesis indexing
     *int i, j; Matrix m;
     * float val = m(i, j);
     * m(i,j) = 5.6;
     * check the indexes are in the right range
     *
     * Check indexes are in valid ranges.

     * @param i
     * @param j
     * @return
     */
    float &operator()(int i, int j);

    /**
     * Brackets indexing
     * int i; Matrix m;
     * float val = m[i];
     * m[k] = 6.7;
     * read section 2.1.2
     * Check index is in valid range.
     * @param i
     * @param j
     * @return float with the relevant value
     */
    float operator[](int i) const;

    /**
     * Brackets indexing
     * int i; Matrix m;
     * float val = m[i];
     * m[k] = 6.7;
     * read section 2.1.2
     * Check index is in valid range.
     * @param i
     * @param j
     * @return ref float with the relevant value
     */
    float &operator[](int i);

    /**
     * Equality
     * Matrix a, b;
     * bool t = (a == b);
     * @param rhs
     * @return
     */
    bool operator==(const Matrix &rhs) const;

    /**
     * Not equal
     * Matrix a, b;
     * bool t = (a != b);
     * @param rhs
     * @return
     */
    bool operator!=(const Matrix &rhs) const;

    /**
     * Input stream
     * Check input stream validity.
     * Fills matrix elements. Reads from given input stream.
     * I.E:
     * ifstream is;
     * Matrix m(3, 5);
     * is >> m;
     * I.E:
     * The input is: 1 2 3 4 5 6
     * Is >> m;
     * m is [1 2 3]
     * [4 5 6]
     * @param rhs
     * @return
     */

    friend std::istream &
    operator>>(std::istream &input, Matrix &rhs);

    /**
     * Output stream
     * Matrix m;
     * std::cout << m << std::endl;
     * OR
     * file << m;
     * @param rhs
     * @return
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &rhs);
};

#endif //EX5_MATRIX_H
