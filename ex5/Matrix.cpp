/**
 * @file Matrix.cpp
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
 * class represents a black and white photos of 128*128 as matrix.
 */


// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include<iostream>
// ------------------------------ functions -----------------------------
/**
 * Constructor
 * Constructs matrix rows * cols (need to make sure rows,cols are non negative).
 * Initiates all elements to 0.
 * @param rows
 * @param cols
 */
Matrix::Matrix(int rows, int cols) : _rows(rows), _cols(cols)
{
    if (_rows < 0 || _cols < 0)
    {
        std::cerr << INVALID_MAT_DIMENSIONS;
        exit(EXIT_FAILURE);
    }
    _mat = new float[_rows * _cols]();


}

/**
 * copy constructor
 * Constructs matrix from another matrix
 * @param m
 */
Matrix::Matrix(const Matrix &m) : Matrix(m._rows, m._cols)
{

    this->_mat = new float[m._rows * m._cols]();
    for (int i = 0; i < m._rows * m._cols; i++)
    {
        this->_mat[i] = m._mat[i];
    }
}

/**
 * Default constructor
 * Constructs 1*1 matrix, where the single element is initiated to 0
 */
Matrix::Matrix() : Matrix(DEFAULD_ROWS, DEFAULT_COLS)
{

}

/**
 * Destructor
 * Destroys the matrix
 */
Matrix::~Matrix()
{
    delete[] this->_mat;
    this->_mat = nullptr;
}

/**
 * Returns the amount of rows (int).
 * @return
 */
int Matrix::getRows() const
{
    return this->_rows;
}

/**
 * Returns the amount of columns (int).
 * @return
 */
int Matrix::getCols() const
{
    return this->_cols;
}

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
Matrix Matrix::vectorize()
{
    // initailize new mat
    this->_rows = getRows() * getCols();
    this->_cols = 1;
    return *this;
}


/**
 * Prints matrix elements, no return value (void).
 * Prints space after each element (not including the last element in the row).
 * Prints new line after each row (not including the last row)
 */
void Matrix::print() const
{
    std::cout << *this;
}

/**
 * assignment
 * Matrix a, b;
 * a = b;
 * @param rhs
 * @return updated matrix
 */
Matrix &Matrix::operator=(const Matrix &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    this->_rows = rhs.getRows();
    this->_cols = rhs.getCols();
    delete[] this->_mat;
    this->_mat = new float[rhs._rows * rhs._cols]();
    for (int i = 0; i < rhs._rows * rhs._cols; i++)
    {
        this->_mat[i] = rhs._mat[i];
    }
    return *this;
}


/**
 * Matrix multiplication
 * Matrix a, b;
 * Matrix c = a * b
 * ** do not forget algebra rules for matrix multiplication.
 * Check dimensions valid for operation.
 * @param rhs
 * @return updated matrix
 */
Matrix Matrix::operator*(const Matrix &rhs) const
{
    if (this->getCols() != rhs.getRows())
    {
        std::cerr << INVALID_MAT_DIMENSIONS;
        exit(EXIT_FAILURE);
    }
    // can multiply matrix!!
    // create mat in valid size:
    Matrix res(this->getRows(), rhs.getCols());
    // multiply and fill res:
    int i, j, k;
    for (i = 0; i < this->getRows(); i++)
    {
        for (j = 0; j < rhs.getCols(); j++)
        {
            res._mat[i * this->_cols + j] = 0;
            float sum = 0;
            for (k = 0; k < this->getCols(); k++)
            {
                sum += this->_mat[i * this->_cols + k] * rhs._mat[k * rhs._cols + j];
            }
            res._mat[i * this->_cols + j] = sum;
        }
    }
    return res;
}

/**
 * Scalar mult. On the right
 * Matrix m;
 * float c;
 * Matrix m2 = m * c;
 * @param c
 * @return updated matrix
 */
Matrix Matrix::operator*(const float c) const
{
    Matrix res(*this);
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        res._mat[i] = c * this->_mat[i];
    }
    return res;
}


/**
 * Scalar mult. In the left.
 * Matrix m; float c;
 * Matrix m2 = c * m
 *
 * @param c
 * @param rhs
 * @return updated matrix
 */
Matrix operator*(float c, const Matrix &rhs)
{
    Matrix res(rhs.getRows(), rhs.getCols());
    for (int i = 0; i < rhs.getRows() * rhs.getCols(); ++i)
    {
        res._mat[i] = c * rhs._mat[i];
    }
    return res;
}


/**
 *  Matrix multiplication.
 *  Matrix a,b;
 *  a*=b;
 *  it is equivalent to  a= a*b
 * Check dimensions valid for operation
 * @param rhs
 * @return updated matrix
 */
Matrix &Matrix::operator*=(const Matrix &rhs)
{
    *this = *this * rhs;
    return *this;
}


/**
 * Scalar mult.accumulation.
 * Matrix a; float c;
 * a*=c
 * (note to self:  it is equivalent to a= a*c)
 * @param c
 * @return updated matrix
 */
Matrix &Matrix::operator*=(float c)
{
    Matrix res(this->getRows(), this->getCols());
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        this->_mat[i] = c * this->_mat[i];
    }
    return *this;
}


/**
 * Scalar division on the right
 * Matrix a; float c;
 * Matrix b = a / c;
 * check to scalar is not 0
 * @param c
 * @return updated matrix
 */
Matrix Matrix::operator/(float c)
{

    if (c == 0)
    {
        std::cerr << DEVISION_BY_ZERO;
        exit(EXIT_FAILURE);
    }

    Matrix res(this->getRows(), this->getCols());
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        res._mat[i] = this->_mat[i] / c;
    }
    return res;
}

/**
 * Scalar division
 * Matrix a;
 * float c;
 * a /= c;
 * ** check to scalar is not 0.
 * (note to self:  it is equivalent to a= a/c)
 * @param c
 * @return updated matrix
 */
Matrix &Matrix::operator/=(float c)
{
    if (c == 0)
    {
        std::cerr << DEVISION_BY_ZERO;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        this->_mat[i] = this->_mat[i] / c;
    }
    return *this;
}

/**
 * Matrix addition
 * Matrix a, b;
 * Matrix c = a + b;
 * Check dimensions valid for operation.
 * @param rhs
 * @return updated matrix
 */
Matrix Matrix::operator+(const Matrix &rhs) const
{

    if (this->getCols() != rhs.getCols() || this->getRows() != rhs.getRows())
    {
        std::cerr << INVALID_MAT_DIMENSIONS;
        exit(EXIT_FAILURE);
    }
    // mats are the same dimensions, so we can add them!
    Matrix res(this->getRows(), this->getCols());
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        res._mat[i] = this->_mat[i] + rhs._mat[i];
    }
    return res;

}

/**
 * Matrix addition accumulation
 * Matrix a, b;
 * a += b;
 * Check dimensions valid for operation.
 * (note to self:  it is equivalent to a= a+b)
 * @param c
 * @return updated matrix
 */
Matrix &Matrix::operator+=(const Matrix &rhs)
{
    *this = *this + rhs;
    return *this;
}

/**
 * Matrix scalar addition.
 * Matrix a; float c;
 * a += c;
 * (note to self:  it is equivalent to a= a+c)
 * @param c
 * @return updated matrix
 */
Matrix &Matrix::operator+=(float c)
{
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        this->_mat[i] = this->_mat[i] + c;
    }
    return *this;
}

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
 * @return float object for the given index
 */
float Matrix::operator()(int i, int j) const
{
    if (i < 0 || j < 0 || i > (this->getRows() - 1) || (j > this->getCols() - 1))
    {
        std::cerr << IDX_OUT_OF_RANGE;
        exit(EXIT_FAILURE);
    }
    return this->_mat[i * this->_cols + j];
}

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
 * @return float ref object for the given index
 */
float &Matrix::operator()(int i, int j)
{
    if (i < 0 || j < 0 || i > (this->getRows() - 1) || (j > this->getCols() - 1))
    {
        std::cerr << IDX_OUT_OF_RANGE;
        exit(EXIT_FAILURE);
    }
    return this->_mat[i * this->_cols + j];
}


/**
 * Brackets indexing
 * int i; Matrix m;
 * float val = m[i];
 * m[k] = 6.7;
 * read section 2.1.2
 *Check index is in valid range.
 * @param i
 * @return float object for the given index
 */
float Matrix::operator[](int i) const
{
    if (i < 0 || i > (this->getRows() * this->getCols() - 1))
    {
        std::cerr << IDX_OUT_OF_RANGE;
        exit(EXIT_FAILURE);
    }
    return this->_mat[i];
}


/**
 * Brackets indexing
 * int i; Matrix m;
 * float val = m[i];
 * m[k] = 6.7;
 * read section 2.1.2
 *Check index is in valid range.
 * @param i
 * @return float ref object for the given index
 */
float &Matrix::operator[](int i)
{

    if (i < 0 || i > (this->getRows() * this->getCols() - 1))
    {
        std::cerr << IDX_OUT_OF_RANGE;
        exit(EXIT_FAILURE);
    }
    return this->_mat[i];
}

/**
 * Equality
 * Matrix a, b;
 * bool t = (a == b);
 * @param rhs
 * @return true if matrices have the save values, false otherwise
 */
bool Matrix::operator==(const Matrix &rhs) const
{
    //check dimensions
    if (this->getCols() != rhs.getCols() || this->getRows() != rhs.getRows())
    {
        return false;
    }
    // check matrix vals one by one
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        if (this->_mat[i] != rhs._mat[i])
        {
            return false;
        }
    }
    return true;
}

/**
 * Not equal
 * Matrix a, b;
 * bool t = (a != b);
 * @param rhs
 * @return true if mat are different, false otherwise
 */
bool Matrix::operator!=(const Matrix &rhs) const
{
    if (this->getCols() != rhs.getCols() || this->getRows() != rhs.getRows())
    {
        return true;
    }
    // check matrix vals one by one
    for (int i = 0; i < this->getRows() * this->getCols(); ++i)
    {
        if (this->_mat[i] != rhs._mat[i])
        {
            return true;
        }
    }
    return false;
}

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
 * @return updated input stream
 */
std::istream &operator>>(std::istream &input, Matrix &rhs)
{
    // check what's the col and what's the row and update rhs!!!!
    if (!input.good())
    {
        std::cerr << INPUT_STREAM_INVALID;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rhs.getCols() * rhs.getRows(); ++i)
    {
        input >> rhs[i];
    }
    return input;
}

/**
 * Output stream
 * Matrix m;
 * std::cout << m << std::endl;
 * OR
 * file << m;
 * @param rhs
 * @return updated output stream
 */
std::ostream &operator<<(std::ostream &os, const Matrix &rhs)
{
    for (int i = 0; i < rhs._rows; i++)
    {
        for (int j = 0; j < rhs._cols; j++)
        {
            if (j != rhs._cols - 1)
            {
                os << rhs._mat[i * rhs._cols + j] << " ";
            }
            else
            {
                os << rhs._mat[i * rhs._cols + j];
            }
        }
        if (i != rhs._rows - 1)
        {
            os << "\n";
        }
    }
    return os;

}